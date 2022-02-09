
#include <iostream>
#include <fstream>
#include "PMC.h"
#include "parser.h"
#include "debugger.h"

int main (int argc, char ** argv)
{
    size_t i;
    size_t start = 0;
    std::string file;
    std::string tmp;
    Cell mem[512];
    bool debug = false;

    if ( argc > 1 ) {
        file = argv[1];
    } else {
        std::cout << "Error: no file" << std::endl;
        return 1;
    }

    for ( i = 2; i < argc; i++ ) {

        tmp = argv[i];

        if ( tmp == "-p" ) {

            if ( i == argc - 1 ) {
                std::cout << "Error: no argument for -p flag." << std::endl;
                return 2;
            }

            try {
                start = std::stoi( argv[i + 1] );
            } catch ( std::invalid_argument() ) {
                std::cout << "Error: no argument for -p flag." << std::endl;
                return 2;
            }

            i++;

        } else if ( tmp == "-d" ) {
            debug = true;

        }

    }

    size_t x = Parse( file, mem );

    if ( x == -1 ) {
        std::cout << "Error: no file" << std::endl;
        return 1;

    } else if ( x != 0 ) {
        std::cout << "Error: line " << x << std::endl;
        return 1;
    }

    Machine mn (mem, start);

    if ( debug ) {
        i = 0;

        while ( mn.isWorking ) {
            std::cout << "Step " << i << ':' << std::endl;
            std::cout << MakeTable(mn) << std::endl << std::endl;
            mn.step();
            i++;
        }

    } else {

        while ( mn.isWorking )
            mn.step();

        std::cout << MakeTable(mn) << std::endl;

    }

    return 0;
}
