
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

bool MatchIndex (const std::string& line, uint16_t& index, size_t& pntr);
bool MatchCommand (const std::string& line, uint16_t& command, size_t& pntr);
bool MatchAddress (const std::string& line, uint16_t& addressing, size_t& pntr);
bool MatchAgrument (const std::string& line, uint16_t& argument, size_t& pntr);
bool MatchAnnotation (const std::string& line, size_t& pntr);
long Parse (std::string fileName, Cell memory[MACHINE_MEM_SIZE]);

#endif
