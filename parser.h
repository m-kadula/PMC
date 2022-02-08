
#ifndef _PMC_PARSER_H_
#define  _PMC_PARSER_H_

#include <cstdlib>
#include <fstream>
#include "PMC.h"

#define CMD_MAP_SIZE 15

const std::string cmdMap[CMD_MAP_SIZE] {
    "STOP", "LOAD", "STORE", "JUMP", "JNEG",
    "JZERO", "ADD", "SUB", "MULT", "DIV",
    "AND", "OR", "NOT", "CMP", "SHZ"
};

class ParsingException: public std::exception {};

bool MatchCommand (
    const std::string& line,
    uint16_t& index,
    uint16_t& command,
    uint16_t& adressing,
    uint16_t& argument
);

bool MatchStopCommand (const std::string& line, uint16_t& index);
bool MatchAssign (const std::string& line, uint16_t& index, uint16_t& value);
bool MatchEmptyLine (const std::string& line);

void Parse (std::string fileName, Cell memory[MACHINE_MEM_SIZE]);

#endif
