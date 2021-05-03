#include "Application.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <strstream>

#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

Application::Application(const std::string &un, const std::string &pwd) : uname(un), password(pwd), logged(false)
{
    logged = false;
}

int Application::exec(const std::string &ip, const std::string &port)
{
    std::map<std::string, strValue> strMap;

    strMap["help"] = strValue::help;
    strMap["regis"] = strValue::regis;
    strMap["login"] = strValue::login;
    strMap["logout"] = strValue::logout;
    strMap["addcart"] = strValue::addcart;
    strMap["settle"] = strValue::settle;
    strMap["recharge"] = strValue::recharge;
    strMap["ls"] = strValue::ls;
    strMap["lsall"] = strValue::lsall;
    strMap["lsu"] = strValue::lsu;
    strMap["addcomm"] = strValue::addcomm;
    strMap["chquantity"] = strValue::chquantity;
    strMap["chpr"] = strValue::chpr;
    strMap["chpercent"] = strValue::chpercent;
    strMap["chtpercent"] = strValue::chtpercent;
    strMap["quit"] = strValue::quit;
    strMap["setpw"] = strValue::setpw;
    strMap["clcart"] = strValue::clcart;

    std::string oper;
    std::cout << "> ";

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(port.c_str()));
    inet_aton(ip.c_str(), &serverAddr.sin_addr);

    // char *buffRecv = (char *)malloc(MAXBUF);
    // char *buffSend = (char *)malloc(MAXBUF);
    char buffRecv[MAXBUF];
    char buffSend[MAXBUF];

    while (std::getline(std::cin, oper))
    {
        int clientFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        memset(buffRecv, 0, MAXBUF);
        memset(buffSend, 0, MAXBUF);

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
                              << "regis <name> <password> <0/1> : register a user\n"
                              << "login <name> <password> : login as a user\n"
                              << "logout : logout\n"
                              << "addcart <commdity name> <number>: add a commdity into cart\n"
                              << "settle : settle\n"
                              << "recharge <number> : recharge\n"
                              << "ls <commdity name> [commdity type] : list commdity\n"
                              << "lsall : list all commdity\n"
                              << "lsu <username> : list a user info\n"
                              << "addcomm <commdity name> <commdity type> <price>: add a commdity\n"
                              << "chquantity <commdity name> <number> : change quantity\n"
                              << "chpr <commdity name> <number> : change price\n"
                              << "chpercent <commdity name> <number> : change discount\n"
                              << "chtpercent <type> <number>\n"
                              << "quit : quit\n";
                    break;

                case addcart:
                    if (argv.size() < 3)
                    {
                        std::cout << "INVALID format\n";
                        break;
                    }
                    else
                    {
                        if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                        {
                            std::cout << "CANNOT connect server\n";
                            return 0;
                        }
                        std::istringstream iss(argv[2]);
                        int num;
                        iss >> num;

                        // std::string buff;
                        std::ostrstream oss(buffSend, MAXBUF);

                        oss << static_cast<char>(addcart)
                            << " " << token
                            << " " << argv[1]
                            << " " << num;

                        write(clientFd, buffSend, MAXBUF);

                        recv(clientFd, buffRecv, MAXBUF, 0);
                        std::istringstream recs(buffRecv);
                        char ret;
                        recs >> ret;
                        if (ret == '0')
                        {
                            std::cout << "Failed\n";
                        }
                        break;
                    }
                    break;

                case settle:
                {
                    if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                    {
                        std::cout << "CANNOT connect server\n";
                        return 0;
                    }

                    // std::string buff;
                    std::ostrstream oss(buffSend, MAXBUF);
                    oss << static_cast<char>(settle)
                        << " " << token;
                    write(clientFd, buffSend, MAXBUF);

                    int len = recv(clientFd, buffRecv, MAXBUF, 0);
                    // std::istringstream recs(buffRecv);
                    // char ret;
                    // recs >> ret;
                    /*
                    if (ret == '0')
                    {
                        std::cout << "Failed\n";
                    }
                    else
                    {
                        for (int i = 2; i < len; i++)
                        {
                            std::cout << buffRecv[i];
                        }
                    }
                    */

                    for (int i = 0; i < len; i++)
                    {
                        std::cout << buffRecv[i];
                    }
                }
                break;

                case recharge:
                    if (argv.size() < 2)
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }
                    else
                    {
                        if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                        {
                            std::cout << "CANNOT connect server\n";
                            return 0;
                        }

                        std::istringstream iss(argv[1]);
                        double b;
                        iss >> b;

                        // std::string buff;
                        std::ostrstream oss(buffSend, MAXBUF);
                        oss << static_cast<char>(recharge)
                            << " " << token
                            << " " << b;

                        write(clientFd, buffSend, MAXBUF);

                        recv(clientFd, buffRecv, MAXBUF, 0);
                        if (buffRecv[0] == '0')
                        {
                            std::cout << "Failed\n";
                        }
                    }
                    break;

                case regis:
                {
                    if (argv.size() < 4)
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }

                    if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                    {
                        std::cout << "CANNOT connect server\n";
                        return 0;
                    }

                    std::istringstream iss(argv[3]);
                    int t;
                    iss >> t;

                    // std::string buff;
                    std::ostrstream oss(buffSend, MAXBUF);
                    oss << static_cast<char>(regis)
                        << " " << argv[1]
                        << " " << argv[2]
                        << " " << t;

                    write(clientFd, buffSend, MAXBUF);

                    recv(clientFd, buffRecv, MAXBUF, 0);
                    if (buffRecv[0] == '0')
                    {
                        std::cout << "User [ " << argv[1] << " ] added\n"
                                  << "Please login again\n";
                    }
                    else
                    {
                        std::cout << "Failed\n";
                    }
                }
                break;

                case login:
                {
                    if (argv.size() < 3)
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }
                    if (isLogged())
                    {
                        std::cout << "Failed\n";
                        break;
                    }

                    if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                    {
                        std::cout << "CANNOT connect server\n";
                        return 0;
                    }

                    // std::string buff;
                    std::ostrstream oss(buffSend, MAXBUF);
                    oss << static_cast<char>(login)
                        << " " << argv[1]
                        << " " << argv[2];

                    write(clientFd, buffSend, MAXBUF);

                    recv(clientFd, buffRecv, MAXBUF, 0);

                    if (buffRecv[0] != '1')
                    {
                        std::cout << "Failed\n";
                    }
                    else
                    {
                        // token = buffRecv[2];
                        std::istringstream iss(buffRecv + 2);
                        iss >> token;
                        uname = argv[1];
                        password = argv[2];
                        logged = true;
                    }
                }
                break;

                case logout:
                {
                    logged = false;

                    if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                    {
                        std::cout << "CANNOT connect server\n";
                        return 0;
                    }

                    // std::string buff;
                    std::ostrstream oss(buffSend, MAXBUF);

                    oss << static_cast<char>(logout)
                        << " " << token;

                    write(clientFd, buffSend, MAXBUF);
                    recv(clientFd, buffRecv, MAXBUF, 0);

                    logged = false;
                }
                break;

                case ls:
                {
                    if (argv.size() < 3)
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }

                    if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                    {
                        std::cout << "CANNOT connect server\n";
                        return 0;
                    }

                    // std::string buff;
                    std::ostrstream oss(buffSend, MAXBUF);
                    oss << static_cast<char>(ls)
                        << " " << argv[1]
                        << " " << argv[2];

                    write(clientFd, buffSend, MAXBUF);

                    int len = recv(clientFd, buffRecv, MAXBUF, 0);
                    for (int i = 0; i < len; i++)
                    {
                        std::cout << buffRecv[i];
                    }
                }
                break;

                case lsall:
                {
                    if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                    {
                        std::cout << "CANNOT connect server\n";
                        return 0;
                    }

                    // std::string buff;
                    std::ostrstream oss(buffSend, MAXBUF);
                    oss << static_cast<char>(ls)
                        << " *"
                        << " *";

                    write(clientFd, buffSend, MAXBUF);

                    int len = recv(clientFd, buffRecv, MAXBUF, 0);
                    for (int i = 0; i < len; i++)
                    {
                        std::cout << buffRecv[i];
                    }
                }
                break;

                case addcomm:
                {
                    if (!isLogged())
                    {
                        std::cout << "NOT Logged\n";
                        break;
                    }
                    if (argv.size() < 4)
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }

                    if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                    {
                        std::cout << "CANNOT connect server\n";
                        return 0;
                    }

                    std::istringstream iss(argv[3]);
                    double p;
                    iss >> p;

                    // std::string buff;
                    std::ostrstream oss(buffSend, MAXBUF);

                    oss << static_cast<char>(addcomm)
                        << " " << argv[1]
                        << " " << token
                        << " " << argv[2]
                        << " " << p;

                    write(clientFd, buffSend, MAXBUF);
                    recv(clientFd, buffRecv, MAXBUF, 0);

                    if (buffRecv[0] == '0')
                    {
                        std::cout << "Failed\n";
                    }
                }
                break;

                case chquantity:
                {
                    if (!isLogged())
                    {
                        std::cout << "NOT Logged\n";
                        break;
                    }
                    if (argv.size() < 3)
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }

                    if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                    {
                        std::cout << "CANNOT connect server\n";
                        return 0;
                    }

                    std::istringstream iss(argv[2]);
                    int q;
                    iss >> q;

                    // std::string buff;
                    std::ostrstream oss(buffSend, MAXBUF);
                    oss << static_cast<char>(chquantity)
                        << " " << argv[1]
                        << " " << token
                        << " " << q;

                    write(clientFd, buffSend, MAXBUF);

                    recv(clientFd, buffRecv, MAXBUF, 0);

                    if (buffRecv[0] == '0')
                    {
                        std::cout << "Failed\n";
                    }
                }
                break;

                case chpr:
                {
                    if (!isLogged())
                    {
                        std::cout << "NOT Logged\n";
                        break;
                    }
                    if (argv.size() < 3)
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }
                    std::istringstream iss(argv[2]);
                    double p;
                    iss >> p;

                    if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                    {
                        std::cout << "CANNOT connect server\n";
                        return 0;
                    }

                    // std::string buff;
                    std::ostrstream oss(buffSend, MAXBUF);
                    oss << static_cast<char>(chpr)
                        << " " << argv[1]
                        << " " << token
                        << " " << p;

                    write(clientFd, buffSend, MAXBUF);

                    recv(clientFd, buffRecv, MAXBUF, 0);

                    if (buffRecv[0] == '0')
                    {
                        std::cout << "Failed\n";
                    }
                }
                break;

                case chpercent:
                {
                    if (!isLogged())
                    {
                        std::cout << "NOT Logged\n";
                        break;
                    }
                    if (argv.size() < 3)
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }
                    std::istringstream iss(argv[2]);
                    double p;
                    iss >> p;

                    if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                    {
                        std::cout << "CANNOT connect server\n";
                        return 0;
                    }

                    // std::string buff;
                    std::ostrstream oss(buffSend, MAXBUF);
                    oss << static_cast<char>(chpercent)
                        << " " << argv[1]
                        << " " << token
                        << " " << p;

                    write(clientFd, buffSend, MAXBUF);

                    recv(clientFd, buffRecv, MAXBUF, 0);
                    if (buffRecv[0] == '0')
                    {
                        std::cout << "Failed\n";
                    }
                }
                break;

                case chtpercent:
                {
                    if (!isLogged())
                    {
                        std::cout << "NOT Logged\n";
                        break;
                    }

                    if (argv.size() < 3)
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }
                    std::istringstream iss(argv[2]);
                    double p;
                    iss >> p;

                    if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                    {
                        std::cout << "CANNOT connect server\n";
                        return 0;
                    }

                    // std::string buff;
                    std::ostrstream oss(buffSend, MAXBUF);

                    oss << static_cast<char>(chtpercent)
                        << " " << argv[1]
                        << " " << token
                        << " " << p;

                    write(clientFd, buffSend, MAXBUF);

                    recv(clientFd, buffRecv, MAXBUF, 0);
                    if (buffRecv[0] == '0')
                    {
                        std::cout << "Failed\n";
                    }
                }
                break;

                case lsu:
                {
                    if (argv.size() < 2)
                    {
                        std::cout << "INVALID format\n";
                        break;
                    }

                    if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                    {
                        std::cout << "CANNOT connect server\n";
                        return 0;
                    }

                    // std::string buff;
                    std::ostrstream oss(buffSend, MAXBUF);
                    oss << static_cast<char>(lsu)
                        << " " << argv[1];

                    write(clientFd, buffSend, MAXBUF);

                    int len = recv(clientFd, buffRecv, MAXBUF, 0);
                    for (int i = 0; i < len; i++)
                    {
                        std::cout << buffRecv[i];
                    }
                }
                break;

                case quit:
                {
                    if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                    {
                        std::cout << "CANNOT connect server\n";
                        return 0;
                    }

                    if (isLogged())
                    {
                        // std::string buff;
                        std::ostrstream oss(buffSend, MAXBUF);
                        oss << static_cast<char>(quit)
                            << " " << token;

                        write(clientFd, buffSend, MAXBUF);
                        recv(clientFd, buffRecv, MAXBUF, 0);
                    }

                    std::cout << "Quit.\n";
                    close(clientFd);
                    return 0;
                }
                break;

                case setpw:
                {
                    if (!isLogged())
                    {
                        std::cout << "NOT Logged\n";
                        break;
                    }
                    if (argv.size() < 2)
                    {
                        std::cout << "INVALID Format\n";
                        break;
                    }

                    if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                    {
                        std::cout << "CANNOT connect server\n";
                        return 0;
                    }

                    std::ostrstream oss(buffSend, MAXBUF);
                    oss << static_cast<char>(setpw)
                        << " " << token
                        << " " << argv[1];

                    write(clientFd, buffSend, MAXBUF);

                    recv(clientFd, buffRecv, MAXBUF, 0);
                    if (buffRecv[0] == '0')
                    {
                        std::cout << "Failed\n";
                    }
                }
                break;

                case clcart:
                {
                    if (!isLogged())
                    {
                        std::cout << "NOT Logged\n";
                        break;
                    }

                    if (connect(clientFd, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                    {
                        std::cout << "CANNOT connect server\n";
                        return 0;
                    }

                    std::ostrstream oss(buffSend, MAXBUF);
                    oss << static_cast<char>(clcart)
                        << " " << token;
                    write(clientFd, buffSend, MAXBUF);

                    recv(clientFd, buffRecv, MAXBUF, 0);
                    if (buffRecv[0] == '0')
                    {
                        std::cout << "Failed\n";
                    }
                }
                break;

                default:
                    std::cout << "ILLEGAL arg : " << argv[0] << " . Type help for more info" << std::endl;
                    break;
                }
            }
            else
            {
                std::cout << "ILLEGAL arg : " << argv[0] << " . Type help for more info" << std::endl;
            }
        }
        std::string oper;
        if (isLogged())
        {
            std::cout << "[ ID : " << uname << " ]\n";
        }
        std::cout << "> ";
        close(clientFd);
    }
    return 0;
}

std::vector<std::string> Application::splitStr(const std::string &str, const char split)
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
