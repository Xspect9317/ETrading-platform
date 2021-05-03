#include "Application.h"
#include <iostream>
#include <string>

int main(int argc, char **argv)
{
    std::cout << "Client V3\n"
              << "Usage : V3-client [ip-address] [port]\n";
    Application app("", "");
    if (argc > 2)
        return app.exec(std::string(argv[1]), std::string(argv[2]));
}
