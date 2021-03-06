#include <iostream>
#include <fstream>
#include "Trade.h"

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

bool Trade::getUserInfo(const std::string &username) const
{
    bool ret = false;
    if (username == "*" || haveUser(username))
    {
        for (auto it : userList)
        {
            if (username == "*" || username.compare(it->getName()) == 0)
            {
                // ??????????????????

                std::cout << "User Info [ " << it->getName() << " ]" << std::endl;
                std::cout << "Type : " << (int)it->getUserType() << std::endl;
                std::cout << "Balance : " << it->getBalance() << std::endl;
                ret = true;
            }
        }
    }
    if (!ret)
    {
        std::cout << "CANNOT find user : " << username << std::endl;
    }
    return ret;
}

void Trade::listComm() const
{
    for (const auto &it : commList)
    {
        std::cout << "Name : " << it.getName() << " "
                  << "Price : " << it.getPrice() << " "
                  << "Type : " << it.getComType() << " "
                  << "Quantity : " << it.getQuantity() << std::endl;
    }
}

void Trade::listComm(const std::string &name, const std::string &comType, const std::string &uname) const
{
    for (const auto &it : commList)
    {
        if ((name == "*" || it.getName().find(name) != std::string::npos) && (comType == "*" || comType.compare(it.getComType()) == 0) && (uname == "" || uname.compare(it.getOwner()) == 0))
        {
            std::cout << "Name : " << it.getName() << " "
                      << "Price : " << it.getPrice() << " "
                      << "Type : " << it.getComType() << " "
                      << "Quantity : " << it.getQuantity() << std::endl;
        }
    }
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

bool Trade::setPassword(const std::string &username, const std::string &password)
{
    for (auto uit : userList)
    {
        if (username == uit->getName())
        {
            uit->setPassword(password);
            return true;
        }
    }
    return false;
}
