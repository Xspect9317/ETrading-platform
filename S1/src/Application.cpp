#include "Application.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <map>

Application::Application(const std::string &un, const std::string &pwd) : uname(un), password(pwd), logged(false)
{
    trade = new Trade;
    logged = false;
}

int Application::exec()
{
    std::map<std::string, strValue> strMap;

    strMap["help"] = strValue::help;
    strMap["regis"] = strValue::regis;
    strMap["login"] = strValue::login;
    // strMap["logout"]=strValue::logout;
    // strMap["addcart"]=strValue::addcart;
    // strMap["settle"]=strValue::settle;
    strMap["recharge"] = strValue::recharge;
    strMap["ls"] = strValue::ls;
    strMap["lsall"] = strValue::lsall;
    strMap["addcomm"] = strValue::addcomm;
    strMap["chquantity"] = strValue::chquantity;
    strMap["chpr"] = strValue::chpr;
    strMap["chpercent"] = strValue::chpercent;
    strMap["quit"] = strValue::quit;

    std::string oper;
    std::cout << "> ";
    while (std::cin >> oper)
    {
        auto argv = splitStr(oper);
        if (argv.size() > 0)
        {
            auto it = strMap.find(argv[0]);
            if (it != strMap.end())
            {
                switch (strMap[argv[0]])
                {
                case help:
                    std::cout << "help : help info\n"
                              << "regis <name> <password> : register a user\n"
                              << "login <name> <password> : login as a user\n"
                              << "logout : logout\n"
                              //   << "addcart <commdity name> : add a commdity into cart\n"
                              //   << "settle : settle\n"
                              //   << "recharge <number> : recharge\n"
                              << "ls <commdity name> [commdity type] : list commdity\n"
                              << "lsall : list all commdity\n"
                              << "addcomm <commdity name> <commdity type> <price>: add a commdity\n"
                              << "chquantity <commdity name> <number> : change quantity\n"
                              << "chpr <commdity name> <number> : change price\n"
                              << "chpercent <commdity name> <number> : change discount\n"
                              << "quit : quit\n";
                    break;
                case regis:
                    if (argv.size() < 3)
                    {
                        std::cout << "INVALID Format\n";
                        continue;
                    }
                    if (trade->addUser(argv[1], argv[2]))
                    {
                        std::cout << "User [ " << argv[1] << " ] added\n"
                                  << "Please login again\n";
                    }
                    else
                    {
                        std::cout << "Failed\n";
                    }
                    break;
                case login:
                    if (argv.size() < 3)
                    {
                        std::cout << "INVALID Format\n";
                        continue;
                    }
                    if (trade->checkPassword(argv[1], argv[2]))
                    {
                        uname = argv[1];
                        password = argv[2];
                        logged = true;
                        std::cout << "Logged in\n";
                    }
                    else
                    {
                        std::cout << "Failed\n";
                    }
                    break;
                case ls:
                    if (argv.size() < 3)
                    {
                        std::cout << "INVALID Format\n";
                        continue;
                    }
                    trade->listComm(argv[1], argv[2]);
                    break;
                case lsall:
                    trade->listComm("*", "*");
                    break;
                case addcomm:
                    if (argv.size() < 4)
                    {
                        std::cout << "INVALID Format\n";
                        continue;
                    }
                    std::istringstream iss(argv[3]);
                    double p;
                    iss >> p;
                    if (!trade->addComm(argv[1], uname, argv[2], p))
                    {
                        std::cout << "Failed\n";
                    }
                    break;
                case chquantity:
                    // Todo
                    break;
                }
            }
            else
            {
                std::cout << "ILLEGAL arg : " << argv[0] << " . Type help for more info" << std::endl;
            }
        }

        std::cout << "> ";
    }
}

std::vector<std::string> Application::splitStr(const std::string &str, const char split = ' ')
{
    std::vector<std::string> ret;
    std::istringstream iss(str);
    std::string buf;
    while (std::getline(iss, buf, split))
    {
        ret.push_back(buf);
    }
    return ret;
}