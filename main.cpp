
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "PMC.h"
#include "parser.h"

#define NUM_LEN 4
#define CMD_LEN 5

std::string MakeTable (Machine& pmc)
{
    size_t i, j;
    std::string index;
    std::string cmd;
    std::string arg;
    std::stringstream line;
    char addressing;

    for ( i = 0; i < MACHINE_MEM_SIZE; i++ ) {

        if ( pmc[i].value == 0 && pmc.PC != i && !(pmc[i].isCommand) )
            continue;

        switch ( pmc[i].get_adressing() ) {

            case 0:
                addressing = '$';
                break;

            case 1:
                addressing = '@';
                break;

            case 2:
                addressing = '&';
                break;

            case 3:
                addressing = '+';
                break;

        };

        index = std::to_string(i) + ' ';
        for ( j = index.length(); j < NUM_LEN; j++ )
            index = ' ' + index;

        cmd = cmdMap[ pmc[i].get_command() ];
        for ( j = cmd.length(); j < CMD_LEN; j++ )
            cmd += ' ';

        arg = std::to_string( pmc[i].get_argument() );
        for ( j = arg.length(); j < NUM_LEN; j++ )
            arg += ' ';

        if ( pmc[i].isCommand ) {
            line << index << ":  " << cmd << "  "
                << addressing << "   " << arg << ";  "
                << pmc[i].value;
        } else {
            line << index << ":  " << pmc[i].value;
        }

        if ( pmc.PC == i )
            line << "\t<--- " << pmc.AC;

        line << std::endl;
    }

    return line.str();
}

int main (int argc, char ** argv)
{
    size_t i;
    size_t start;
    std::string file;
    std::string tmp;
    Cell mem[MACHINE_MEM_SIZE];
    bool debug = false;
    long parseCode;

    if ( argc > 1 ) {
        file = argv[1];
    } else {
        std::cout << "Error: no file." << std::endl;
        return 1;
    }

    parseCode = Parse( file, mem );

    if ( parseCode == -1 ) {
        std::cout << "Error: file not found." << std::endl;
        return 1;

    } else if ( parseCode != 0 ) {
        std::cout << "Error: on line " << parseCode << std::endl;
        return 1;
    }

    start = 0;
    while ( start < MACHINE_MEM_SIZE && !(mem[start].isCommand) )
        start++;

    for ( i = 2; i < argc; i++ ) {

        tmp = argv[i];

        if ( tmp == "-p" ) {

            if ( i == argc - 1 ) {
                std::cout << "Error: no argument for -p flag." << std::endl;
                return 2;
            }

            try {
                start = std::stoi( argv[i + 1] );
            } catch ( std::invalid_argument ) {
                std::cout << "Error: no argument for -p flag." << std::endl;
                return 2;
            }

            i++;

        } else if ( tmp == "-d" ) {
            debug = true;

        } else {
            std::cout << "Error: flag '" << tmp << "' not recognised." << std::endl;
            return 2;
        }

    }

    Machine mn (mem, start);

    try {

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

    } catch ( MemoryException ) {
        std::cout << "Error: tried to access memory outside of bounds." << std::endl;
    }

    return 0;
}
