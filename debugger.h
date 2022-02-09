
#ifndef _PMC_DEBUGGER_H_
#define _PMC_DEBUGGER_H_

#include <cstdlib>
#include <stdint.h>
#include <string>
#include "parser.h"
#include "PMC.h"

#define INDEX_LEN 3
#define CMD_LEN 5

std::string MakeTable (Machine& pmc);

#endif
