#ifndef __USER_H__
#define __USER_H__

/*
 * Copyright © 2021 Navi.HuangXing. All rights reserved.
 * @author      : Navi.HuangXing 
 * @date        : 2021-04-24
 * @file        : User.h
 * @description : 用户类
*/

#include <string>
#include <vector>
#include <utility>

class User
{
private:
    std::string name;
    std::string password;
    int balance;

public:
    enum Type
    {
        consumer,
        merchant,
    };
    virtual int getUserType() = 0; // 获得用户类型

    int getBalance();        // 获得余额
    void setBalance() const; // 设置余额

    std::string getName() const;             // 获得用户名
    std::string getPassword() const;         // 获得密码
    void setPassword(const std::string &pw); // 设置密码

    User(const std::string &n, const std::string &pw); // 构造函数
};

class Consumer : public User
{
private:
    std::vector<std::pair<int, int>> cart;

public:
    virtual int getUserType() override { return Type::consumer; }
    int addCart(int comId, int num); // 加入购物车
    bool settle();                   // 结算
};

class Merchant : public User
{
private:
public:
    virtual int getUserType() override { return Type::merchant; }

};

#endif
