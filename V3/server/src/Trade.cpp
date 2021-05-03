#include <iostream>
#include <fstream>
#include "Trade.h"

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <sstream>

bool Trade::readUserFile(const std::string &fp)
{
    std::cout << "Reading user data file : " << fp << " ... ";
    std::ifstream f(fp, std::ios::in);
    if (!f.good())
    {
        std::cout << fp << ": not exist\n";
        return false;
    }
    std::string tempname, temppassword;
    int temptype, tempbal;
    f >> tempname;
    while (!f.eof())
    {
        f >> temppassword >> temptype >> tempbal;
        User *u = nullptr;
        switch (temptype)
        {
        case 0:
            u = new Consumer(tempname, temppassword, tempbal);
            break;
        case 1:
            u = new Merchant(tempname, temppassword, tempbal);
            break;
        }
        if (u != nullptr)
        {
            userList.push_back(u);
        }
        f >> tempname;
    }
    f.close();
    std::cout << "Done\n";
    return true;
}

bool Trade::saveUserFile(const std::string &fp) const
{
    std::cout << "Saving user data file : " << fp << " ... ";
    std::ofstream f(fp, std::ios::out);
    for (auto it : userList)
    {
        User::Type t = it->getUserType();
        int type;
        if (it->getName().compare(adminName) == 0)
        {
            continue;
        }
        switch (t)
        {
        case User::Type::consumer:
            type = 0;
            break;
        case User::Type::merchant:
            type = 1;
            break;
        }
        f << it->getName()
          << " " << it->getPassword()
          << " " << type
          << " " << it->getBalance() << std::endl;
    }
    f.close();
    std::cout << "Done\n";
    return true;
}

bool Trade::readCommFile(const std::string &fp)
{
    std::cout << "Reading commdity data file : " << fp << " ... ";
    std::ifstream f(fp, std::ios::in);
    if (!f.good())
    {
        std::cout << fp << ": not exist\n";
        return false;
    }
    std::string tempname, temptype, tempowner;
    double tempprice, temppercent;
    int quantity;
    f >> tempname;
    while (!f.eof())
    {
        f >> temptype >> tempowner >> tempprice >> temppercent >> quantity;
        Commdity c(tempprice, temppercent, quantity, temptype, tempname, tempowner);
        commList.push_back(c);
        f >> tempname;
    }
    f.close();
    std::cout << "Done\n";
    return true;
}

bool Trade::saveCommFile(const std::string &fp) const
{
    std::cout << "Saving commdity data file : " << fp << " ... ";
    std::ofstream f(fp, std::ios::out);
    for (const auto &it : commList)
    {
        f << it.getName()
          << " " << it.getComType()
          << " " << it.getOwner()
          << " " << it.getOriginPrice()
          << " " << it.getPercent()
          << " " << it.getQuantity() << std::endl;
    }
    f.close();
    std::cout << "Done\n";
    return true;
}

bool Trade::haveUser(const std::string &username) const
{
    for (auto it : userList)
    {
        if (username.compare(it->getName()) == 0)
        {
            return true;
        }
    }
    return false;
}

bool Trade::checkPassword(const std::string &username,
                          const std::string &password) const
{
    for (auto it : userList)
    {
        if (username.compare(it->getName()) == 0)
        {
            if (password.compare(it->getPassword()) == 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    return false;
}

bool Trade::addUser(const std::string &username, const std::string &password, int type)
{
    if (haveUser(username))
    {
        return false;
    }

    User *u = nullptr;
    switch (type)
    {
    case 0:
        u = new Consumer(username, password, 0);
        break;
    case 1:
        u = new Merchant(username, password, 0);
        break;
    }
    if (u != nullptr)
    {
        userList.push_back(u);
        return true;
    }
    return false;
}

bool Trade::delUser(const std::string &username)
{
    for (auto it = userList.begin(); it != userList.end(); it++)
    {
        if (username.compare((*it)->getName()) == 0)
        {
            User *temp = *it;
            userList.erase(it);
            delete temp;
            return true;
        }
    }
    return false;
}

std::string Trade::getUserInfo(const std::string &username) const
{
    std::string ret;
    std::ostringstream oss(ret);
    if (username == "*" || haveUser(username))
    {
        for (auto it : userList)
        {
            if (username == "*" || username.compare(it->getName()) == 0)
            {
                // 输出用户信息

                oss << "User Info [ " << it->getName() << " ]" << std::endl;
                oss << "Type : " << (int)it->getUserType() << std::endl;
                oss << "Balance : " << it->getBalance() << std::endl;
                ret = true;
            }
        }
    }
    if (ret.size() == 0)
    {
        oss << "CANNOT find user : " << username << std::endl;
    }
    return ret;
}

std::string Trade::listComm() const
{
    std::string ret;
    std::ostringstream oss(ret);
    for (const auto &it : commList)
    {
        oss << "Name : " << it.getName() << " "
            << "Price : " << it.getPrice() << " "
            << "Type : " << it.getComType() << " "
            << "Quantity : " << it.getQuantity() << std::endl;
    }
    return ret;
}

std::string Trade::listComm(const std::string &name, const std::string &comType, const std::string &uname) const
{
    std::string ret;
    std::ostringstream oss(ret);
    for (const auto &it : commList)
    {
        if ((name == "*" || it.getName().find(name) != std::string::npos) && (comType == "*" || comType.compare(it.getComType()) == 0) && (uname == "" || uname.compare(it.getOwner()) == 0))
        {
            oss << "Name : " << it.getName() << " "
                << "Price : " << it.getPrice() << " "
                << "Type : " << it.getComType() << " "
                << "Quantity : " << it.getQuantity() << std::endl;
        }
    }
    return ret;
}

bool Trade::addComm(const std::string &name, const std::string &uname, const std::string &comType, double price)
{
    for (auto it : commList)
    {
        if (name.compare(it.getName()) == 0)
            return false;
    }
    Commdity c(price, 1, 0, comType, name, uname);
    commList.push_back(c);
    return true;
}

bool Trade::delComm(const std::string &name, const std::string &uname)
{
    for (auto it = commList.begin(); it != commList.end(); it++)
    {
        if (name.compare(it->getName()) == 0 && uname.compare(it->getOwner()) == 0)
        {
            commList.erase(it);
            return true;
        }
    }
    return false;
}

bool Trade::changeQuantity(const std::string &name, const std::string &uname, int q)
{
    for (auto it = commList.begin(); it != commList.end(); it++)
    {
        if (name.compare(it->getName()) == 0 && (uname == adminName || uname.compare(it->getOwner()) == 0))
        {
            it->setQuantity(q);
            return true;
        }
    }
    return false;
}

bool Trade::setPrice(const std::string &name, const std::string &uname, double p)
{
    for (auto it = commList.begin(); it != commList.end(); it++)
    {
        if (name.compare(it->getName()) == 0 && uname.compare(it->getOwner()) == 0)
        {
            it->setOriginPrice(p);
            return true;
        }
    }
    return false;
}

bool Trade::setPercent(const std::string &name, const std::string &uname, double p)
{
    for (auto it = commList.begin(); it != commList.end(); it++)
    {
        if (name.compare(it->getName()) == 0 && (uname.compare(it->getOwner()) == 0 || uname == adminName))
        {
            it->setPercent(p);
            return true;
        }
    }
    return false;
}

bool Trade::setPercent(double p, const std::string &type, const std::string &uname)
{
    for (auto it = commList.begin(); it != commList.end(); it++)
    {
        if (type.compare(it->getComType()) == 0 && (uname.compare(it->getOwner()) == 0 || uname == adminName))
        {
            it->setPercent(p);
        }
    }
    return true;
}

bool Trade::buy(const std::string &uname)
{
    for (auto uit : userList)
    {
        if (uname.compare(uit->getName()) == 0 && uit->getUserType() == User::Type::consumer)
        {
            double sum = 0.0;
            Consumer *cuit = dynamic_cast<Consumer *>(uit);
            auto cart = cuit->getCart();
            dynamic_cast<Merchant *>(uit);
            for (auto cit : cart)
            {
                double price = getPrice(cit.first);
                if (price == -1 && cit.second <= getQuantity(cit.first))
                {
                    return false;
                }
                changeQuantity(cit.first, uit->getName(), getQuantity(cit.first) - cit.second);
                sum += price * cit.second;
            }
            if (sum > uit->getBalance())
            {
                for (auto cit : cart)
                {
                    // double price = getPrice(cit.first);
                    changeQuantity(cit.first, uit->getName(), getQuantity(cit.first) + cit.second);
                }
                return false;
            }
            std::cout << "Buy : \n";
            for (auto cit : cart)
            {
                addbal(getOwner(cit.first), cit.second * getPrice(cit.first));
                std::cout << cit.first << " : " << cit.second << std::endl;
            }
            uit->setBalance(uit->getBalance() - sum);
            clearCart(uit->getName());
            return true;
        }
    }
    return false;
}

int Trade::getQuantity(const std::string &name) const
{
    int ret = -1;
    for (auto it = commList.begin(); it != commList.end(); it++)
    {
        if (name.compare(it->getName()) == 0)
        {
            ret = it->getQuantity();
            return ret;
        }
    }
    return ret;
}

double Trade::getPrice(const std::string &name) const
{
    int ret = -1;
    for (auto it = commList.begin(); it != commList.end(); it++)
    {
        if (name.compare(it->getName()) == 0)
        {
            ret = it->getPrice();
            return ret;
        }
    }
    return ret;
}

bool Trade::clearCart(const std::string &uname)
{
    for (auto uit : userList)
    {
        if (uname.compare(uit->getName()) == 0 && uit->getUserType() == User::Type::consumer)
        {
            Consumer *it = dynamic_cast<Consumer *>(uit);
            it->cart.erase(it->cart.begin(), it->cart.end());
            return true;
        }
    }
    return false;
}

bool Trade::haveComm(const std::string &name) const
{
    for (auto it : commList)
    {
        if (name.compare(it.getName()) == 0)
        {
            return true;
        }
    }
    return false;
}

bool Trade::addCart(const std::string &uname, const std::string &name, int q)
{
    if (haveComm(name) && haveUser(uname))
    {
        for (auto uit : userList)
        {
            if (uname.compare(uit->getName()) == 0 && uit->getUserType() == User::Type::consumer)
            {
                auto it = dynamic_cast<Consumer *>(uit);
                it->addCart(name, q);
                return true;
            }
        }
    }
    return false;
}

Trade::Trade(const Trade &t)
{
}

Trade::Trade()
{
    readCommFile();
    readUserFile();

    Merchant *upt = new Merchant(adminName, adminPwd, 0);
    userList.push_back(upt);
}

Trade::~Trade()
{
    for (auto uit : userList)
    {
        delete uit;
    }
}

double Trade::getbal(const std::string &uname) const
{
    for (auto uit : userList)
    {
        if (uname.compare(uit->getName()) == 0)
        {
            return uit->getBalance();
        }
    }
    return -1;
}

bool Trade::setbal(const std::string &uname, double b)
{
    for (auto uit : userList)
    {
        if (uname.compare(uit->getName()) == 0)
        {
            uit->setBalance(b);
            return true;
        }
    }
    return false;
}

bool Trade::addbal(const std::string &uname, double b)
{
    for (auto uit : userList)
    {
        if (uname.compare(uit->getName()) == 0)
        {
            uit->setBalance(b + uit->getBalance());
            return true;
        }
    }
    return false;
}

std::string Trade::getOwner(const std::string &name)
{
    for (auto cit : commList)
    {
        if (name.compare(cit.getName()) == 0)
        {
            return cit.getOwner();
        }
    }
    return "";
}

char Trade::keyGen(const std::string &name)
{
    int ret = -1;
    if (tokenMap.size() == MAXMAP)
    {
        return ret;
    }
    for (int i = 0; i < MAXMAP; i++)
    {
        auto it = tokenMap.find(i);
        if (it != tokenMap.end())
        {
            ret = i;
            tokenMap[i] = name;
        }
    }
    return ret;
}

int Trade::exec(const std::string &port)
{
    int serverFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in myAddr;
    myAddr.sin_family = AF_INET;
    myAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myAddr.sin_port = htons(atoi(port.c_str()));

    bind(serverFd, (sockaddr *)&myAddr, sizeof(myAddr));

    listen(serverFd, 5);

    char *buffRecv, *buffSend;
    buffRecv = (char *)malloc(MAXBUF);
    buffSend = (char *)malloc(MAXBUF);
    unsigned int len = 0;

    sockaddr_in clientAddr;
    int addrLen = sizeof(clientAddr);

    int clientFd;
    while (1)
    {
        memset(buffRecv, 0, MAXBUF);
        memset(buffSend, 0, MAXBUF);
        len = 0;

        clientFd = accept(serverFd, (sockaddr *)&clientAddr, (socklen_t *)&addrLen);
        int ret = read(clientFd, buffRecv, MAXBUF);

        std::istringstream iss(buffRecv);

        int oper;
        iss >> oper;

        std::string name, pwd, type, t, cname;
        char token;
        int num;
        double mon;

        switch (oper)
        {
        // regis
        case 1:
            iss >> name >> pwd >> type;
            if (!addUser(name, pwd, atoi(type.c_str())))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            else
            {
                buffSend[0] = '1';
                len++;
                break;
            }
            break;

        // login
        case 2:
            iss >> name >> pwd;
            if (!checkPassword(name, pwd))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            else
            {
                token = keyGen(name);
                if (token != -1)
                {
                    buffSend[0] = '1';
                    buffSend[1] = '0';
                    buffSend[2] = token;
                    len += 2;
                }
                else
                {
                    buffSend[0] = '2';
                    len++;
                }
                break;
            }
            break;

        // logout
        case 3:
            iss >> t;
            token = atoi(t.c_str());
            tokenMap.erase(token);

        // addcart
        case 4:
            iss >> t >> cname >> num;
            token = atoi(t.c_str());
            name = tokenMap[token];
            if (!haveComm(name))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            if (!addCart(name, cname, num))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            else
            {
                buffSend[0] = '1';
                len++;
                break;
            }
            break;

        // settle
        case 5:
            iss >> t;
            token = atoi(t.c_str());
            name = tokenMap[token];
            if (!buy(name))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            else
            {
                buffSend[0] = '1';
                buffSend[1] = ' ';
                len += 2;
                double b = getbal(name);
                memcpy(buffSend + len, &b, sizeof(b));
                len += sizeof(b);
                break;
            }
            break;

        // recharge
        case 6:
            iss >> t >> mon;
            token = atoi(t.c_str());
            name = tokenMap[token];
            if (!addbal(name, mon))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            else
            {
                buffSend[0] = '1';
                buffSend[1] = ' ';
                len += 2;
                double b = getbal(name);
                memcpy(buffSend + len, &b, sizeof(b));
                len += sizeof(b);
                break;
            }
            break;

        // ls
        case 7:
            iss >> cname >> type;
            {
                std::string ret = listComm(cname, type);
                memcpy(buffSend, ret.c_str(), ret.size());
                len += ret.size();
            }
            break;

        // lsall
        case 8:
            // use wildcard
            {
                std::string ret = listComm("*", "*");
                memcpy(buffSend, ret.c_str(), ret.size());
                len += ret.size();
            }
            break;

        // lsu
        case 9:
            iss >> name;
            {
                std::string ret = getUserInfo(name);
                memcpy(buffSend, ret.c_str(), ret.size());
                len += ret.size();
            }
            break;

        // addcomm
        case 10:
            iss >> cname >> t >> type >> mon;
            token = atoi(t.c_str());
            name = tokenMap[token];
            if (!addComm(cname, name, type, mon))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            else
            {
                buffSend[0] = '1';
                len++;
                break;
            }
            break;

        // chquantity
        case 11:
            iss >> cname >> t >> num;
            token = atoi(t.c_str());
            name = tokenMap[token];
            if (!changeQuantity(cname, name, num))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            else
            {
                buffSend[0] = '1';
                len++;
                break;
            }
            break;

        // chpr
        case 12:
            iss >> cname >> t >> mon;
            token = atoi(t.c_str());
            name = tokenMap[token];
            if (!setPrice(cname, name, mon))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            else
            {
                buffSend[0] = '1';
                len++;
                break;
            }
            break;

        // chpercent
        case 13:
            iss >> cname >> t >> mon;
            token = atoi(t.c_str());
            name = tokenMap[token];
            if (!setPercent(cname, name, mon))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            else
            {
                buffSend[0] = '1';
                len++;
                break;
            }
            break;

        // chtpercent
        case 14:
            iss >> type >> t >> mon;
            token = atoi(t.c_str());
            name = tokenMap[token];
            if (!setPercent(mon, type, name))
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            else
            {
                buffSend[0] = '1';
                len++;
                break;
            }
            break;

        // quit
        case 15:
            iss >> t;
            token = atoi(t.c_str());
            name = tokenMap[token];
            if (name == adminName)
            {
                buffSend[0] = '1';
                len++;

                saveCommFile();
                saveUserFile();

                break;
            }
            else
            {
                buffSend[0] = '0';
                len++;
                break;
            }
            break;
        }
        send(clientFd, buffSend, len, 0);
        close(clientFd);
    }
}