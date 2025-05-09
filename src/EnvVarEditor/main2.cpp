//#include <iostream>
//#include <algorithm>
//
//#include <comm/win_cls_logger.h>
//#include <comm/win_evar_util.h>
//
//#include "eve_global.h"
//
//#ifdef __BOOST_ENABLED__
//#include <boost/program_options.hpp>
//namespace bpo = boost::program_options;
//#endif
//
//
//#define ENVVAR_PATH "PATH"
//#define DEFAULT_ENVVAR_NAME ENVVAR_PATH
//    
//#ifdef __BOOST_ENABLED__
//int main(int argc, char **argv)
//{
//    char *locale = setlocale(LC_ALL, NULL);
//    setlocale(LC_ALL, "chs");
//    std::string
//        name,       // 环境变量名
//        val,        // 设置的环境变量值
//        val_append, // 追加的环境变量值
//        val_remove; // 移除的环境变量值
//    bool
//        global = 0,    // 1:系统环境
//        sort_asc = 0,  // 1:升序
//        sort_desc = 0, // 1:降序
//        split = 0,     // 1:分割换行显示
//        slim = 0,      // 1:去重
//        save = 0,      // 1:保存
//        del = 0;       // 1:移除
//
//    bpo::variables_map vm;
//    bpo::options_description opts_desc("Usage");
//    opts_desc.add_options()("help,h", "显示命令行帮助")("name,n", bpo::value(&name)->default_value(DEFAULT_ENVVAR_NAME), "环境变量名称")("val", bpo::value(&val), "环境变量值")("val+", bpo::value(&val_append), "追加环境变量值,指定了value选项时无效")("val-", bpo::value(&val_remove), "移除环境变量值,指定了value选项时无效")("del", bpo::bool_switch(&del)->default_value(false), "移除指定名称的环境变量")("global,g", bpo::bool_switch(&global)->default_value(false), "此开关控制相关操作作用到系统环境变量")("sort-asc,a", bpo::bool_switch(&sort_asc)->default_value(false), "此开关控制是否升序排序环境变量")("sort-desc,d", bpo::bool_switch(&sort_desc)->default_value(false), "此开关控制是否降序排序环境变量")("split,x", bpo::bool_switch(&split)->default_value(false), "此开关控制是否换行显示环境变量的每个值")("slim,r", bpo::bool_switch(&slim)->default_value(false), "此开关控制是否去重")("save,s", bpo::bool_switch(&save)->default_value(false), "此开关控制是否保存");
//
//    try
//    {
//        bpo::store(bpo::command_line_parser(argc, argv).options(opts_desc).run(), vm);
//        bpo::notify(vm);
//    }
//    catch (const std::exception &ex)
//    {
//        std::cout << ex.what();
//        return -1;
//    }
//    if (vm.count("name"))
//    {
//        if (name.empty())
//        {
//            return -2;
//        }
//    }
//    else
//    {
//        if (name.empty())
//        {
//            name.assign(ENVVAR_PATH);
//        }
//    }
//    if (vm.count("help"))
//    {
//        std::cout << opts_desc << std::endl;
//        return 0;
//    }
//
//    std::wstring path, path_result;
//    std::vector<std::wstring> paths;
//    if (vm.count("value"))
//    {
//        path = str_to_wstr(val);
//        if (path.length() == 0)
//        {
//            clslog("the env val inputted is empty", MT_WARN);
//        }
//    }
//    else
//    {
//        try
//        {
//            path = evar_get(str_to_wstr(name), global);
//        }
//        catch (const std::exception &ex)
//        {
//            clslog(ex.what(), MT_ERR);
//            return -999;
//        }
//    }
//
//    if (split || slim || sort_asc || sort_desc)
//    {
//        paths = path_split(path);
//        if (slim)
//        {
//            std::vector<std::wstring> tempPaths;
//            for (auto &p : paths)
//            {
//                if (std::find_if(tempPaths.begin(), tempPaths.end(), [&p](std::wstring &i)
//                                 { return path_compare(p, i) == 0; }) == tempPaths.end())
//                {
//                    tempPaths.push_back(p);
//                }
//            }
//            paths.assign(tempPaths.begin(), tempPaths.end());
//        }
//        if (sort_asc)
//        {
//            std::sort(paths.begin(), paths.end(), [](const std::wstring &left, const std::wstring &right)
//                      { return tstr_cmp_icase(left, right) < 0; });
//        }
//        else if (sort_desc)
//        {
//            std::sort(paths.begin(), paths.end(), [](const std::wstring &left, const std::wstring &right)
//                      { return tstr_cmp_icase(left, right) > 0; });
//        }
//        path_result = path_join(paths);
//    }
//    else
//    {
//        path_result = path;
//    }
//    if (split)
//    {
//        for (auto &p : paths)
//        {
//            std::wcout << p << std::endl;
//        }
//    }
//    else
//    {
//        std::wcout << path_result;
//    }
//    if (save)
//    {
//        try
//        {
//            if (global)
//            {
//                if (!IsUserAnAdmin())
//                {
//                    clslog("\ncan not save the value, please start as administrator", MT_ERR);
//                    return -1;
//                }
//            }
//            evar_set(str_to_wstr(name), path_result, name == ENVVAR_PATH ? REG_EXPAND_SZ : REG_SZ, global);
//            clslog("SUCCESS", MT_INFO);
//            return 0;
//        }
//        catch (const std::exception &ex)
//        {
//            clslog(ex.what(), MT_ERR);
//            return 9;
//        }
//    }
//    return 0;
//}
//#endif