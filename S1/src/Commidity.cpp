#include "Commidity.h"

Commdity::Commdity(
    double pr,
    double pe,
    int q,
    std::string ct,
    std::string n)
    : price(pr), percent(pe), quantity(q), comType(ct), name(n)
{
}

bool Commdity::buy(int count)
{
    if (count <= quantity)
    {
        quantity -= count;
        return true;
    }
    return false;
}

bool Commdity::isType(const std::string &t)
{
    if (comType.compare(t) == 0)
        return true;
    return false;
}
