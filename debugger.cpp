
#include <cstdlib>
#include <stdint.h>
#include <string>
#include <sstream>
#include <iostream>
#include "parser.h"
#include "PMC.h"
#include "debugger.h"

std::string MakeTable (Machine& pmc)
{
    size_t i, j;
    std::string index;
    std::string cmd;
    std::stringstream line;
    char addressing;

    for ( i = 0; i < MACHINE_MEM_SIZE; i++ ) {

        if ( pmc[i].value == 0 && pmc.PC != i )
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

        index = std::to_string(i);
        for ( j = index.length(); j < INDEX_LEN; j++ )
            index += ' ';

        cmd = cmdMap[ pmc[i].get_command() ];
        for ( j = cmd.length(); j < CMD_LEN; j++ )
            cmd += ' ';

        line << index << ":\t" << cmd << '\t'
            << addressing << '\t' << pmc[i].get_argument() << '\t'
            << pmc[i].value;

        if ( pmc.PC == i )
            line << "\t<--- " << pmc.AC;

        line << std::endl;
    }

    return line.str();
}
