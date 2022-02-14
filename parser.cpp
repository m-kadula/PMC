
#include <cstdlib>
#include <fstream>
#include "parser.h"
#include "PMC.h"

bool MatchIndex (const std::string& line, uint16_t& index, size_t& pntr)
{
    size_t i;
    std::string ind;
    bool scFlag = false;

    for ( i = pntr; i < line.length(); i++ ) {

        if ( line[i] >= '0' && line[i] <= '9' ) {

            while ( i < line.length() && line[i] >= '0' && line[i] <= '9' ) {
                ind += line[i];
                i++;
            }

            scFlag = true;
            break;

        } else if ( line[i] != ' ' ) {
            return false;

        }

    }

    if ( !scFlag )
        return false;

    while ( i < line.length() && line[i] != ':' ) {
        if ( line[i] != ' ' )
            return false;

        i++;
    }

    if ( line[i] != ':' )
        return false;

    index = (uint16_t) std::stoi( ind );

    if ( index >= MACHINE_MEM_SIZE )
        return false;

    pntr = i + 1;

    return true;
}

bool MatchCommand (const std::string& line, uint16_t& command, size_t& pntr)
{
    size_t i, j;
    std::string cmd;
    bool scFlag = false;

    for ( i = pntr; i < line.length(); i++ ) {

        if ( line[i] >= 'A' && line[i] <= 'Z' ) {

            while ( i < line.length() && line[i] >= 'A' && line[i] <= 'Z' ) {
                cmd += line[i];
                i++;
            }

            scFlag = true;
            break;

        } else if ( line[i] != ' ' ) {
            return false;

        }

    }

    if ( !scFlag )
        return false;

    for ( j = 0; j < CMD_MAP_SIZE; j++ ) {

        if ( cmd == cmdMap[j] ) {
            command = j;
            pntr = i;
            return true;
        }

    }

    return false;
}

bool MatchAddress (const std::string& line, uint16_t& addressing, size_t& pntr)
{
    size_t i = pntr;

    while (
        i < line.length() && line[i] != '$'
        && line[i] != '@' && line[i] != '&' && line[i] != '+'
    ) {
        if ( line[i] != ' ' )
            return false;

        i++;
    }

    switch ( line[i] ) {

        case '$':
            addressing = 0;
            break;

        case '@':
            addressing = 1;
            break;

        case '&':
            addressing = 2;
            break;

        case '+':
            addressing = 3;
            break;

        default:
            return false;

    }

    pntr = i + 1;

    return true;
}

bool MatchAgrument (const std::string& line, uint16_t& argument, size_t& pntr)
{
    size_t i;
    std::string arg;
    bool scFlag = false;

    for ( i = pntr; i < line.length(); i++ ) {

        if ( line[i] >= '0' && line[i] <= '9' ) {

            while ( i < line.length() && line[i] >= '0' && line[i] <= '9' ) {
                arg += line[i];
                i++;
            }

            scFlag = true;
            break;

        } else if ( line[i] != ' ' ) {
            return false;

        }

    }

    if ( !scFlag )
        return false;

    argument = (uint16_t) std::stoi( arg );
    pntr = i;

    return true;
}

bool MatchAnnotation (const std::string& line, size_t& pntr)
{
    size_t i;

    for ( i = pntr; i < line.length() && line[i] != ';'; i++ ) {
        if ( line[i] != ' ' )
            return false;
    }

    pntr = i + 1;
    return true;
}

long Parse (std::string fileName, Cell memory[MACHINE_MEM_SIZE])
{
    size_t pntr;
    size_t lineIndex;
    std::fstream file;
    std::string line;

    uint16_t index;
    uint16_t command;
    uint16_t adressing;
    uint16_t argument;

    bool errorFlag;

    file.open( fileName.c_str(), std::fstream::in );

    if ( file.fail() ) {
        file.close();
        return -1;
    }

    errorFlag = false;
    lineIndex = 1;

    while ( true ) {

        std::getline(file, line);

        if ( file.fail() )
            break;

        pntr = 0;

        if ( MatchIndex(line, index, pntr) ) {

            if ( MatchCommand(line, command, pntr) ) {

                if ( MatchAddress(line, adressing, pntr) ) {

                    if ( MatchAgrument(line, argument, pntr) ) {

                        if ( MatchAnnotation(line, pntr) ) {
                            memory[index] = Cell(adressing, command, argument);

                        } else {
                            errorFlag = true;
                        }

                    } else {
                        errorFlag = true;
                    }

                } else if ( MatchAnnotation(line, pntr) ) {
                    memory[index] = Cell(0, command, 0);

                } else {
                    errorFlag = true;
                }

            } else if ( MatchAgrument(line, argument, pntr) ) {

                if ( MatchAnnotation(line, pntr) ) {
                    memory[index] = Cell(argument);

                } else {
                    errorFlag = true;
                }

            } else {
                errorFlag = true;
            }

        } else if ( !MatchAnnotation(line, pntr) ) {
            errorFlag = true;
        }

        if ( errorFlag )
            break;

        lineIndex++;
    }

    file.close();

    if ( errorFlag )
        return lineIndex;

    return 0;
}
