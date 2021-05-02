#ifndef __COMMODITY_H__
#define __COMMODITY_H__

/*
 * Copyright © 2021 Navi.HuangXing. All rights reserved.
 * @author      : Navi.HuangXing 
 * @date        : 2021-04-24
 * @file        : Commidity.h
 * @description : 商品模块
*/

#include <string>

class Commdity
{
private:
    double price;
    double percent;
    int quantity;
    std::string comType;
    std::string name;

    std::string owner;

public:
    double getPrice() const { return price * percent; }
    double getOriginPrice() const { return price; }
    void setOriginPrice(double p) { price = p; }

    int getQuantity() const { return quantity; }
    void setQuantity(int q) { quantity = q; }

    double getPercent() const { return percent; }
    void setPercent(double p) { percent = p; }

    std::string getComType() const { return comType; }
    void setComType(const std::string &t) { comType = t; }

    std::string getName() const { return name; }
    void setName(const std::string &n) { name = n; }

    std::string getOwner() const { return owner; }

    Commdity(double pr, double pe, int q, const std::string &ct, const std::string &n, const std::string &own);

    bool buy(int count);
    bool isType(const std::string &t);
};

#endif
