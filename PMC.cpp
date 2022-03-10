
#include <cstdlib>
#include <stdint.h>
#include <exception>
#include "PMC.h"

// Cell contents

Cell::Cell ()
{
    value = 0;
    isCommand = false;
}

Cell::Cell (int16_t val)
{
    value = val;
    isCommand = false;
}

Cell::Cell (uint16_t adressing, uint16_t command, uint16_t argument)
{
    argument &= 511;
    adressing = ( adressing << 9 ) & 1536;
    command = ( command << 11 ) & 30720;

    value = command | adressing | argument;
    isCommand = true;
}

uint16_t Cell::get_command ()
{
    return ( uint16_t(value) >> 11 ) & 15;
}

uint16_t Cell::get_adressing ()
{
    return ( uint16_t(value) >> 9 ) & 3;
}

uint16_t Cell::get_argument ()
{
    return uint16_t(value) & 511;
}

// Machine contents

Machine::Machine (Cell memory[MACHINE_MEM_SIZE], uint16_t PCstart)
{
    size_t i;

    for ( i = 0; i < MACHINE_MEM_SIZE; i++ ) {
        defMem[i] = memory[i];
        mem[i] = memory[i];
    }

    defPC = PCstart;
    PC = PCstart;
    AC = 0;
    isWorking = true;
}

Cell& Machine::operator [] (size_t index)
{
    if ( index >= MACHINE_MEM_SIZE )
        throw MemoryException();

    return mem[index];
}

void Machine::reset ()
{
    size_t i;

    for ( i = 0; i < MACHINE_MEM_SIZE; i++ )
        mem[i] = defMem[i];

    PC = defPC;
    AC = 0;
    isWorking = true;
}

int16_t Machine::GetOR ()
{
    switch ( (*this)[PC].get_adressing() ) {

        case 0b0:
            return (*this)[PC].get_argument();

        case 0b1:
            return (*this)[ (*this)[PC].get_argument() ].value;

        case 0b10:
            return (*this)[ (*this)[ (*this)[PC].get_argument() ].value ].value;

        case 0b11:
            return (*this)[ (*this)[PC].get_argument() + AC ].value;

    }

    return 0;
}

void Machine::step ()
{
    int16_t OR;

    switch ( (*this)[PC].get_command() ) {

        case 0:  // STOP
            isWorking = false;
            break;

        case 1:  // LOAD
            AC = this->GetOR();
            PC++;
            break;

        case 2:  // STORE
            OR = this->GetOR();
            (*this)[OR] = AC;
            PC++;
            break;

        case 3:  // JUMP
            PC = this->GetOR();
            break;

        case 4:  // JNEG
            if ( AC < 0 )
                PC = this->GetOR();
            else
                PC++;
            break;

        case 5:  // JZERO
            if ( AC == 0 )
                PC = this->GetOR();
            else
                PC++;
            break;

        case 6:  // ADD
            AC = AC + this->GetOR();
            PC++;
            break;

        case 7:  // SUB
            AC = AC - this->GetOR();
            PC++;
            break;

        case 8:  // MULT
            AC = AC * this->GetOR();
            PC++;
            break;

        case 9:  // DIV
            AC = AC / this->GetOR();
            PC++;
            break;

        case 10: // AND
            AC = AC & this->GetOR();
            PC++;
            break;

        case 11: // OR
            AC = AC | this->GetOR();
            PC++;
            break;

        case 12: // NOT
            AC = ~( this->GetOR() );
            PC++;
            break;

        case 13: // CMP
            if ( AC == this->GetOR() )
                AC = -1;
            else
                AC = 0;
            PC++;
            break;

        case 14: // ZSH
            OR = this->GetOR();
            if (OR < 0)
                AC >>= -OR;
            else
                AC <<= OR;

            PC++;
            break;

        default:
            throw std::exception();

    }
}
