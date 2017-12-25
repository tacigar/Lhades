/*
** lhades
** Copyright (C) 2017 tacigar
*/

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "header.hpp"
#include "proto.hpp"
#include "reader.hpp"
#include "writer.hpp"

auto main(int argc, char **argv) -> int
{
    const std::string usage = "usage: lhades <lua-file-name>";
    if (argc <= 1) {
        std::cerr << usage << std::endl;
        std::exit(1);
    }

    std::ifstream ifs(argv[1], std::ios::in | std::ios::binary);
    if (ifs.fail()) {
        std::cerr << "cannot open the file : " << argv[1] << std::endl;
        std::exit(1);
    }

    lhades::Header::read(ifs);
    lhades::Reader::read<char>(ifs); // skip upvalue info
    auto proto = lhades::Proto::read(ifs, argv[1]);

    std::stringstream ss;
    lhades::Writer::write(ss, proto);
    std::cout << ss.str() << std::endl;

    return 0;
}
