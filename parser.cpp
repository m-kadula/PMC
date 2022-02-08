
#include <cstdlib>
#include <fstream>
#include "parser.h"
#include "PMC.h"

bool MatchCommand (
    const std::string& line,
    uint16_t& index,
    uint16_t& command,
    uint16_t& adressing,
    uint16_t& argument
) {
    size_t i;
    std::string ind;
    std::string cmd;
    std::string arg;
    bool scFlag = false;

    for ( i = 0; i < line.length(); i++ ) {

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

    scFlag = false;

    for ( i = i + 1 ; i < line.length(); i++ ) {

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

    while (
        i < line.length()
        && ( line[i] != '$' && line[i] != '@' && line[i] != '&' && line[i] != '+' )
    ) {
        if ( line[i] != ' ' )
            return false;

        i++;
    }

    switch ( line[i] ) {

        case '$':
            adressing = 0;
            break;
        
        case '@':
            adressing = 1;
            break;

        case '&':
            adressing = 2;
            break;

        case '+':
            adressing = 3;
            break;

        default:
            return false;

    }

    scFlag = false;

    for ( i = i + 1; i < line.length(); i++ ) {

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

    while ( i < line.length() && line[i] != ';' ) {
        if ( line[i] != ' ' )
            return false;

        i++;
    }

    index = (uint16_t) std::stoi( ind );
    argument = (uint16_t) std::stoi( arg );

    for ( i = 0; i < CMD_MAP_SIZE; i++ ) {

        if ( cmd == cmdMap[i] ) {
            command = i;
            return true;
        }

    }

    return false;
}

bool MatchStopCommand (const std::string& line, uint16_t& index)
{
    size_t i;
    std::string ind;
    std::string cmd;
    bool scFlag = false;

    for ( i = 0; i < line.length(); i++ ) {

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

    scFlag = false;

    for ( i = i + 1; i < line.length(); i++ ) {

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

    if ( !scFlag || cmd != cmdMap[0] )
        return false;

    index = (uint16_t) std::stoi( ind );

    while ( i < line.length() && line[i] != ';' ) {
        if ( line[i] != ' ' )
            return false;

        i++;
    }

    return true;
}

bool MatchAssign (const std::string& line, uint16_t& index, uint16_t& value)
{
    size_t i;
    std::string ind;
    std::string val;
    bool scFlag = false;

    for ( i = 0; i < line.length(); i++ ) {

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

    scFlag = false;

    for ( i = i + 1; i < line.length(); i++ ) {

        if ( line[i] >= '0' && line[i] <= '9' ) {

            while ( i < line.length() && line[i] >= '0' && line[i] <= '9' ) {
                val += line[i];
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

    index = (uint16_t) std::stoi( ind );
    value = (uint16_t) std::stoi( val );
    return true;
}

bool MatchEmptyLine (const std::string& line)
{
    size_t i;

    for ( i = 0; i < line.length() && line[i] != ';'; i++ ) {
        if ( line[i] != ' ' )
            return false;
    }

    return true;
}

void Parse (std::string fileName, Cell memory[MACHINE_MEM_SIZE])
{
    size_t i;
    std::fstream file;
    std::string line;

    uint16_t index;
    uint16_t command;
    uint16_t adressing;
    uint16_t argument;
    uint16_t value;

    file.open( fileName.c_str(), std::fstream::in );

    while ( true ) {

        std::getline(file, line);

        if ( file.fail() )
            break;

        if ( MatchCommand(line, index, command, adressing, argument) ) {
            memory[index] = Cell(adressing, command, argument);

        } else if ( MatchAssign(line, index, value) ) {
            memory[index] = Cell(value);

        } else if ( MatchStopCommand(line, index) ) {
            memory[index] = 0;

        } else if ( !MatchEmptyLine(line) ) {
            throw ParsingException();

        }
    }

    file.close();
}
