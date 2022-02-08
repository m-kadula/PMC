
#include <iostream>
#include <fstream>
#include "PMC.h"
#include "parser.h"

int main (int argc, char ** argv)
{
    std::string file = argv[1];
    Cell mem[512];

    Parse( file, mem );

    for (int i = 0; i < 30; i++) {
        std::cout << mem[i].value << ' '
            << mem[i].get_command() << ' '
            << mem[i].get_adressing() << ' '
            << mem[i].get_argument() << std::endl;
    }

    return 0;
}
