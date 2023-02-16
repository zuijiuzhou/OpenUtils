#include <iostream>
#include <string>
#include <regex>
#include <type_traits>
#include <Windows.h>
#include <ShlObj.h>

#include <boost/program_options.hpp>

namespace bpo = boost::program_options;

int main(int argc, char **argv)
{

    char *locale = setlocale(LC_ALL, NULL);
    setlocale(LC_ALL, "chs");
    std::string name, val;
    bool sort_asc = 0, sort_desc = 0, split = 0, slim = 0, save = 0;

    bpo::variables_map vm;
    bpo::options_description opts_desc("Usage");

    opts_desc.add_options()
    ("help,h", "显示命令行帮助")
    ("name,n", bpo::value(&name)->default_value("PATH"), "环境变量名称")
    ("value,v",  bpo::value(&val), "环境变量值")
    ("sort-asc,a", bpo::bool_switch(&sort_asc)->default_value(false), "此开关控制是否升序排序环境变量")
    ("sort-desc,d", bpo::bool_switch(&sort_desc)->default_value(false), "此开关控制是否降序排序环境变量")
    ("split", bpo::bool_switch(&split)->default_value(false), "此开关控制是否换行显示环境变量的每个值")
    ("slim,x", bpo::bool_switch(&slim)->default_value(false), "此开关控制是否去重")
    ("save,s", bpo::bool_switch(&save)->default_value(false), "此开关控制是否保存");
    
    try{
        bpo::store(bpo::command_line_parser(argc, argv).options(opts_desc).run(), vm);
        bpo::notify(vm);
    }
    catch(const std::exception& ex){
        std::cout << ex.what();
        return -1;    
    }
    if(vm.count("name")){
        if(name.empty()){
            return -2;
        }
    }

    if(vm.count("help"))
    {
        std::cout << opts_desc <<std::endl;
    }
    return 0;
}