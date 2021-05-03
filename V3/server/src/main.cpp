#include "Trade.h"
#include <iostream>

int main(int argc, char **argv)
{
    std::cout << "Server V3\n"
              << "Usage : V3-server [port]\n";
    Trade trade;
    if (argc > 1)
        trade.exec(argv[1]);
}
