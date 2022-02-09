
#include <cstdlib>
#include <stdint.h>
#include <exception>
#include "PMC.h"

#define MACHINE_MEM_SIZE 512

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

// commands

void Machine::stop ()
{
    isWorking = false;
}

void Machine::load ()
{
    AC = this->GetOR();
    PC++;
}

void Machine::store ()
{
    int16_t OR = this->GetOR();

    (*this)[OR] = AC;
    PC++;
}

void Machine::jump ()
{
    PC = this->GetOR();
}

void Machine::jneg ()
{
    if ( AC < 0 )
        PC = this->GetOR();
    else
        PC++;
}

void Machine::jzero ()
{
    if ( AC == 0 )
        PC = this->GetOR();
    else
        PC++;
}

void Machine::add ()
{
    AC = AC + this->GetOR();
    PC++;
}

void Machine::sub ()
{
    AC = AC - this->GetOR();
    PC++;
}

void Machine::mult ()
{
    AC = AC * this->GetOR();
    PC++;
}

void Machine::div ()
{
    AC = AC / this->GetOR();
    PC++;
}

void Machine::and_ ()
{
    AC = AC & this->GetOR();
    PC++;
}

void Machine::or_ ()
{
    AC = AC | this->GetOR();
    PC++;
}

void Machine::not_ ()
{
    AC = ~( this->GetOR() );
    PC++;
}

void Machine::cmp ()
{
    if ( AC == this->GetOR() )
        AC = -1;
    else
        AC = 0;

    PC++;
}

void Machine::shz ()
{
    int16_t OR = this->GetOR();

    if (OR < 0)
        AC >>= -OR;
    else
        AC <<= OR;

    PC++;
}

void Machine::step ()
{
    switch ( (*this)[PC].get_command() ) {

        case 0:
            this->stop();
            break;

        case 1:
            this->load();
            break;

        case 2:
            this->store();
            break;
        
        case 3:
            this->jump();
            break;

        case 4:
            this->jneg();
            break;

        case 5:
            this->jzero();
            break;

        case 6:
            this->add();
            break;

        case 7:
            this->sub();
            break;

        case 8:
            this->mult();
            break;

        case 9:
            this->div();
            break;

        case 10:
            this->and_();
            break;

        case 11:
            this->or_();
            break;

        case 12:
            this->not_();
            break;

        case 13:
            this->cmp();
            break;

        case 14:
            this->shz();
            break;

        default:
            throw std::exception();

    }
}
