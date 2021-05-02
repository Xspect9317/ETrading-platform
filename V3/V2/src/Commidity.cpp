#include "Commidity.h"

Commdity::Commdity(
    double pr,
    double pe,
    int q,
    const std::string& ct,
    const std::string& n,
    const std::string& own)
    : price(pr), percent(pe), quantity(q), comType(ct), name(n), owner(own)
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
