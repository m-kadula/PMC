
#ifndef _PMC_PMC_H_
#define _PMC_PMC_H_

#include <cstdlib>
#include <stdint.h>
#include <exception>

#define MACHINE_MEM_SIZE 512

class MemoryException: public std::exception {};

struct Cell {
    int16_t value;  // might cause problems in uint16_t methods
    bool isCommand;

    Cell ();
    Cell (int16_t val);
    Cell (uint16_t adressing, uint16_t command, uint16_t argument);

    uint16_t get_command ();
    uint16_t get_adressing ();
    uint16_t get_argument ();
};

class Machine {
    private:
    Cell defMem[MACHINE_MEM_SIZE];
    uint16_t defPC;
    Cell mem[MACHINE_MEM_SIZE];
    uint16_t PC;

    // commands
    void stop ();
    void load ();
    void store ();
    void jump ();
    void jneg ();
    void jzero ();
    void add ();
    void sub ();
    void mult ();
    void div ();
    void and_ ();
    void or_ ();
    void not_ ();
    void cmp ();
    void shz ();

    public:
    int16_t AC;
    bool isWorking;

    Machine (Cell memory[MACHINE_MEM_SIZE], uint16_t PCstart);
    Cell& operator [] (size_t index);
    void reset ();

    int16_t GetOR ();
    void step ();
};

#endif
