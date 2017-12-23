#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "lhades.hpp"

auto main(int argc, char **argv) -> int
{
    const std::string usage = "usage: lhades <lua-file-name>";

    if (argc <= 1) {
        std::cerr << usage << std::endl;
        std::exit(1);
    }

    LHades lhades(argv[1]);
    auto result = lhades.disassemble();

    std::cout << result << "\n" << std::endl;
    return 0;
}
