#include "eve_global.h"

#include <algorithm>
#include <iostream>

#include <ShlObj.h>

#include <argpase/argparse.hpp>

#include <comm/cvt.h>
#include <comm/std_io_path_util.h>
#include <comm/win_cls_logger.h>
#include <comm/win_evar_util.h>
#include <comm/win_shell_util.h>

using Parser   = argparse::ArgumentParser;
using Argument = argparse::Argument;

#define ENVVAR_PATH "PATH"
#define DEFAULT_ENVVAR_NAME ENVVAR_PATH

#define __CMD_GET__ "get"
#define __CMD_SET__ "set"
#define __CMD_DEL__ "del"
#define __CMD_EXPORT__ "export"
#define __CMD_IMPORT__ "import"

#define __ARG_HELP__ "--help"
#define __ARG_HELP2__ "-h"
#define __ARG_GLOBAL__ "--global"
#define __ARG_GLOBAL2__ "-g"
#define __ARG_NAME__ "--name"
#define __ARG_NAME2__ "-n"
#define __ARG_LIST__ "--list"
#define __ARG_LIST2__ "-l"

#define __ARG_SPLIT__ "--split"
#define __ARG_SPLIT2__ "-x"
#define __ARG_SLIM__ "--slim"
#define __ARG_SLIM2__ "-r"
#define __ARG_SORT_ASC__ "--sort-asc"
#define __ARG_SORT_ASC2__ "-sa"
#define __ARG_SORT_DESC__ "--sort-desc"
#define __ARG_SORT_DESC2__ "-sd"

#define __ARG_VALUE__ "--value"
#define __ARG_VALUE2__ "-v"
#define __ARG_VALUE_PLUS__ "--value+"
#define __ARG_VALUE_PLUS2__ "-v+"
#define __ARG_VALUE_MINUS__ "--value-"
#define __ARG_VALUE_MINUS2__ "-v-"
#define __ARG_SAVE__ "--save"
#define __ARG_SAVE2__ "-s"

#define __ARG_OUPUT__ "--output"
#define __ARG_OUPUT2__ "-o"

#define __ARG_INPUT__ "--input"
#define __ARG_INPUT2__ "-i"
#define __ARG_OVERRIDE__ "--override"

#define __SUCCESS__ 0

#define __ERR_UNKONW__ 0x1
#define __ERR_NAME_NOT_SPECIFIED__ 0x1000
#define __ERR_NAME_NOT_EXISTED__ 0x1001
#define __ERR_REG_READ_FAILD__ 0x1101
#define __ERR_REG_SET_FAILD__ 0x1102
#define __ERR_REG_DEL_FAILD__ 0x1103
#define __ERR_ADMIN_REQUIRED__ 0x1201
#define __ERR_PARAM_MISMATCH__ 0x1202

static Parser      cmd_root(__APPLICATION_NAME__);
static Parser      cmd_get(__CMD_GET__);
static Parser      cmd_set(__CMD_SET__);
static Parser      cmd_del(__CMD_DEL__);
static Parser      cmd_export(__CMD_EXPORT__);
static Parser      cmd_import(__CMD_IMPORT__);
static std::string err_msg_inner;

void                      def_common_args(Parser& cmd, bool def_name = true);
void                      def_format_args(Parser& cmd);
int                       get_common_args(Parser& parser, bool* global, std::string* name);
int                       get_format_args(Parser& parser, bool* split, bool* slim, bool* sort_asc, bool* sort_desc);
int                       get_env_val(const std::string& name, bool global, std::wstring& o_val);
void                      process_env_val(std::vector<std::wstring>& vals, bool slim, bool sort_asc, bool sort_desc);
std::vector<std::wstring> process_env_val(const std::wstring& val, bool slim, bool sort_asc, bool sort_desc);
int                       cmd_get_handler(Parser& cmd);
int                       cmd_set_handler(Parser& cmd);
int                       cmd_del_handler(Parser& cmd);
int                       cmd_export_handler(Parser& cmd);
int                       cmd_import_handler(Parser& cmd);
Parser&                   build_arg_parser();
const char*               err_desc(int code);

void def_common_args(Parser& cmd, bool def_name) {
    auto& arg_g = cmd.add_argument(__ARG_GLOBAL2__, __ARG_GLOBAL__)
                      .default_value(false)
                      .implicit_value(true)
                      .nargs(0)
                      .help("此开关控制相关操作作用到系统环境变量");
    auto& arg_n = cmd.add_argument(__ARG_NAME2__, __ARG_NAME__).required().nargs(1).help("环境变量名称");
    if (def_name) {
        arg_n.default_value(DEFAULT_ENVVAR_NAME);
    }
}

void def_format_args(Parser& cmd) {
    cmd.add_argument(__ARG_SPLIT2__, __ARG_SPLIT__)
        .default_value(false)
        .implicit_value(true)
        .nargs(0)
        .help("此开关控制是否换行显示环境变量的每个值");
    cmd.add_argument(__ARG_SLIM2__, __ARG_SLIM__)
        .default_value(false)
        .implicit_value(true)
        .nargs(0)
        .help("此开关控制是否去重");
    cmd.add_argument(__ARG_SORT_ASC2__, __ARG_SORT_ASC__)
        .default_value(false)
        .implicit_value(true)
        .nargs(0)
        .help("此开关控制是否升序排序环境变量");
    cmd.add_argument(__ARG_SORT_DESC2__, __ARG_SORT_DESC__)
        .default_value(false)
        .implicit_value(true)
        .nargs(0)
        .help("此开关控制是否降序排序环境变量");
}

int get_common_args(Parser& parser, bool* global, std::string* name) {
    if (parser.is_used(__ARG_GLOBAL__)) {
        *global = parser.get<bool>(__ARG_GLOBAL__);
    }
    if (parser.is_used(__ARG_NAME__)) {
        *name = parser.get<std::string>(__ARG_NAME__);
        if (name->empty()) {
            return __ERR_NAME_NOT_SPECIFIED__;
        }
    }
    else {
        *name = parser.get<std::string>(__ARG_NAME__);
    }
    return 0;
}

int get_format_args(Parser& cmd, bool* split, bool* slim, bool* sort_asc, bool* sort_desc) {
    if (cmd.is_used(__ARG_SPLIT__)) {
        *split = cmd.get<bool>(__ARG_SPLIT__);
    }
    if (cmd.is_used(__ARG_SLIM__)) {
        *slim = cmd.get<bool>(__ARG_SLIM__);
    }
    if (cmd.is_used(__ARG_SORT_ASC__)) {
        *sort_asc = cmd.get<bool>(__ARG_SORT_ASC__);
    }
    if (cmd.is_used(__ARG_SORT_DESC__)) {
        *sort_desc = cmd.get<bool>(__ARG_SORT_DESC__);
    }
    return 0;
}

int get_env_val(std::string& name, bool global, std::wstring& o_val) {
    try {
        o_val = evar_get(str_to_wstr(name), global);
    }
    catch (const std::exception& e) {
        err_msg_inner = e.what();
        return __ERR_REG_READ_FAILD__;
    }
    catch (...) {
        return __ERR_REG_READ_FAILD__;
    }
    return 0;
}

void process_env_val(std::vector<std::wstring>& vals, bool slim, bool sort_asc, bool sort_desc) {
    auto& items = vals;
    if (slim) {
        std::vector<std::wstring> tempPaths;
        for (auto& p : items) {
            if (std::find_if(tempPaths.begin(), tempPaths.end(), [&p](std::wstring& i) {
                    return path_compare(p, i) == 0;
                }) == tempPaths.end()) {
                tempPaths.push_back(p);
            }
        }
        items.assign(tempPaths.begin(), tempPaths.end());
    }
    if (sort_asc) {
        std::sort(items.begin(), items.end(), [](const std::wstring& left, const std::wstring& right) {
            return tstr_cmp_icase(left, right) < 0;
        });
    }
    else if (sort_desc) {
        std::sort(items.begin(), items.end(), [](const std::wstring& left, const std::wstring& right) {
            return tstr_cmp_icase(left, right) > 0;
        });
    }
}

std::vector<std::wstring> process_env_val(const std::wstring& val, bool slim, bool sort_asc, bool sort_desc) {
    std::vector<std::wstring> items = path_split(val);
    process_env_val(items, slim, sort_asc, sort_desc);
    return items;
}

int cmd_get_handler(Parser& cmd) {
    int         iret   = 0;
    bool        global = false, list = false, split = false, slim = false, sort_asc = false, sort_desc = false;
    std::string name;
    iret = get_common_args(cmd, &global, &name);
    if (iret) return iret;
    iret = get_format_args(cmd, &split, &slim, &sort_asc, &sort_desc);
    if (iret) return iret;
    if (cmd.is_used(__ARG_LIST__)) {
        list = cmd.get<bool>(__ARG_LIST__);
    }

    if (list) {
        return 0;
    }
    else {
        if (false == evar_exists(str_to_wstr(name), global)) {
            return __ERR_NAME_NOT_EXISTED__;
        }
        std::wstring val;
        iret = get_env_val(name, global, val);
        if (iret) return iret;
        if (split || slim || sort_asc || sort_desc) {
            std::vector<std::wstring> vals = process_env_val(val, slim, sort_asc, sort_desc);
            if (split) {
                for (auto& item : vals) {
                    clslog(item, MT_TEXT);
                }
            }
            else {
                clslog(path_join(vals), MT_TEXT);
            }
        }
        else {
            clslog(val, MT_TEXT);
        }
        return 0;
    }
}

int cmd_set_handler(Parser& cmd) {
    int         iret;
    auto        global = false, split = false, slim = false, sort_asc = false, sort_desc = false, save = false;
    std::string name, value, value_plus, value_minus;
    iret = get_common_args(cmd, &global, &name);
    if (iret) return iret;
    iret = get_format_args(cmd, &split, &slim, &sort_asc, &sort_desc);
    if (iret) return iret;

    if (cmd.is_used(__ARG_SAVE__)) {
        save = cmd.get<bool>(__ARG_SAVE__);
    }
    if (cmd.is_used(__ARG_VALUE__)) {
        value = cmd.get<std::string>(__ARG_VALUE__);
    }
    if (cmd.is_used(__ARG_VALUE_PLUS__)) {
        value_plus = cmd.get<std::string>(__ARG_VALUE_PLUS__);
    }
    if (cmd.is_used(__ARG_VALUE_MINUS__)) {
        value_minus = cmd.get<std::string>(__ARG_VALUE_MINUS__);
    }

    std::wstring val;
    std::wstring val_processed;

    if (false == value.empty()) {
        val = str_to_wstr(value);
    }
    else {
        iret = get_env_val(name, global, val);
        if (iret) return iret;
    }
    std::vector<std::wstring> vals = process_env_val(val, slim, sort_asc, sort_desc);
    if (false == value_plus.empty()) {
        vals.push_back(str_to_wstr(value_plus));
        process_env_val(vals, slim, sort_asc, sort_desc);
    }
    else {
        std::wstring value_minus_normalized = path_norm(str_to_wstr(value_minus));
        auto foundAt = std::find_if(vals.begin(), vals.end(), [&value_minus_normalized](const std::wstring& item) {
            return path_compare(value_minus_normalized, item) == 0;
        });
        if (foundAt != vals.end()) {
            vals.erase(foundAt);
        }
    }
    val_processed = path_join(vals);
    if (split) {
        for (auto& item : vals) {
            clslog(item, MT_TEXT);
        }
    }
    else {
        clslog(val_processed, MT_TEXT);
    }
    if (save) {
        try {
            if (global) {
                if (false == IsUserAnAdmin()) {
                    return __ERR_ADMIN_REQUIRED__;
                }
            }
            evar_set(str_to_wstr(name), val_processed, name == ENVVAR_PATH ? REG_EXPAND_SZ : REG_SZ, global);
            clslog("SUCCESS", MT_INFO);
            return __SUCCESS__;
        }
        catch (const std::exception& e) {
            err_msg_inner = e.what();
            return __ERR_REG_SET_FAILD__;
        }
        catch (...) {
            return __ERR_REG_SET_FAILD__;
        }
    }
    return __SUCCESS__;
}

int cmd_del_handler(Parser& cmd) {
    int         iret   = __SUCCESS__;
    bool        global = false;
    std::string name;
    iret = get_common_args(cmd, &global, &name);
    if (iret) return iret;

    if (false == evar_exists(str_to_wstr(name), global)) {
        return __ERR_NAME_NOT_EXISTED__;
    }

    try {
        evar_del(str_to_wstr(name), global);
        clslog("SUCCESS", MT_INFO);
    }
    catch (const std::exception& e) {
        err_msg_inner = e.what();
        return __ERR_REG_DEL_FAILD__;
    }
    catch (...) {
        return __ERR_REG_DEL_FAILD__;
    }
    return __SUCCESS__;
}

int cmd_export_handler(Parser& cmd) {
    std::string output;

    if (cmd.is_used(__ARG_OUPUT__)) {
        output = cmd.get<std::string>(__ARG_OUPUT__);
    }
    else {
    }
    // FILE *f = fopen(, );
    return 0;
}

int cmd_import_handler(Parser& cmd) {

    return 0;
}

Parser& build_arg_parser() {

    // app.add_argument(__ARG_HELP2__, __ARG_HELP__).help("显示命令行帮助");

    // GET
    def_common_args(cmd_get);
    def_format_args(cmd_get);
    cmd_get.add_argument(__ARG_LIST2__, __ARG_LIST__)
        .default_value(false)
        .implicit_value(true)
        .help("获取环境变量列表");
    // SET
    def_common_args(cmd_set);
    def_format_args(cmd_set);
    cmd_set.add_argument(__ARG_VALUE2__, __ARG_VALUE__).help("设置或修改环境变量值");
    cmd_set.add_argument(__ARG_VALUE_PLUS2__, __ARG_VALUE_PLUS__).help("追加环境变量值,指定了value选项时无效");
    cmd_set.add_argument(__ARG_VALUE_MINUS2__, __ARG_VALUE_MINUS__).help("移除环境变量值,指定了value选项时无效");
    cmd_set.add_argument(__ARG_SAVE2__, __ARG_SAVE__)
        .default_value(false)
        .implicit_value(true)
        .nargs(0)
        .help("此开关控制是否保存");

    // DEL
    def_common_args(cmd_del, false);
    // EXPORT
    cmd_export.add_argument(__ARG_OUPUT2__, __ARG_OUPUT__).required().nargs(1).help("环境变量保存路径");

    // IMPORT
    cmd_import.add_argument(__ARG_INPUT2__, __ARG_INPUT__).required().nargs(1).help("环境变量文件路径");
    cmd_import.add_argument(__ARG_OVERRIDE__)
        .default_value(false)
        .implicit_value(true)
        .nargs(0)
        .help("此开关控制是否覆盖已存在的环境变量值");

    cmd_root.add_subparser(cmd_get);
    cmd_root.add_subparser(cmd_set);
    cmd_root.add_subparser(cmd_del);
    cmd_root.add_subparser(cmd_export);
    cmd_root.add_subparser(cmd_import);

    return cmd_root;
}

const char* err_desc(int code) {
    if (code == __ERR_NAME_NOT_SPECIFIED__) {
        return "No environment variable name specified";
    }
    else if (code == __ERR_NAME_NOT_EXISTED__) {
        return "The specified name does not exist";
    }
    else if (code == __ERR_REG_READ_FAILD__) {
        return "Failed to read registry";
    }
    else if (code == __ERR_REG_SET_FAILD__) {
        return "Failed to write registry";
    }
    else if (code == __ERR_REG_DEL_FAILD__) {
        return "Failed to delete registry";
    }
    else if (code == __ERR_ADMIN_REQUIRED__) {
        return "Please start with administrator privileges";
    }
    else if (code == __ERR_PARAM_MISMATCH__) {
        return "Command line argument error";
    }
    return "Unknown error";
}

int main(int argc, char** argv) {
    char* locale = setlocale(LC_ALL, NULL);
    setlocale(LC_ALL, "chs");

    auto& parser = build_arg_parser();
    int   iret   = __SUCCESS__;
    if (argc == 1) {
        clslog(cmd_root.usage(), MT_TEXT);
        return -1;
    }
    try {
        parser.parse_args(argc, argv);
        try {
            if (parser.is_subcommand_used(__CMD_GET__)) {
                iret = cmd_get_handler(cmd_get);
            }
            else if (parser.is_subcommand_used(__CMD_SET__)) {
                iret = cmd_set_handler(cmd_set);
            }
            else if (parser.is_subcommand_used(__CMD_DEL__)) {
                iret = cmd_del_handler(cmd_del);
            }
            else if (parser.is_subcommand_used(__CMD_EXPORT__)) {
                iret = cmd_export_handler(cmd_export);
            }
            else if (parser.is_subcommand_used(__CMD_IMPORT__)) {
                iret = cmd_import_handler(cmd_import);
            }
        }
        catch (const std::exception& e) {
            err_msg_inner = e.what();
            iret          = __ERR_UNKONW__;
        }
        catch (...) {
            iret = __ERR_UNKONW__;
        }
    }
    catch (const std::exception& e) {
        err_msg_inner = e.what();
        iret          = __ERR_PARAM_MISMATCH__;
    }
    catch (...) {
        iret = __ERR_PARAM_MISMATCH__;
    }

    if (iret) {
        clserr(err_desc(iret), err_msg_inner.data());
        // clslog(err_desc(iret), MT_ERR);
        // // #ifdef DEBUG
        // if (err_msg_inner.length())
        // {
        //     clslog(err_msg_inner, MT_ERR);
        // }
        // // #endif
    }
    return iret;
}
