#ifndef FOUNDATIONS
#define FOUNDATIONS

#include <stdlib.h>
#include <stdio.h>

//registers.
typedef unsigned char byte;

#define NOREG___________	0x00
#define REGISTER_A______	0x01
#define REGISTER_B______	0x02
#define REGISTER_C______	0x03
#define REGISTER_D______	0x04
#define REGISTER_PC_____	0x05
#define REGISTER_IO_____	0x06
#define REGISTER_PTR____	0x07
#define REGISTER_INS____	0x08
#define REGISTER_DAT____	0x09
#define REGISTER_INS_REG	0x0a
#define REGISTER_INS_OPR	0x0b
#define REGISTER_SP_____	0x0c

/*No operation*/
#define NOP 0x00

/*sets the program counter to the
specified value
(must end in 0, 2, 4, 6, 8, a, c, e)*/
#define JP_ 0x10

/*set, sets the value of target
register to specified value.*/
#define SET 0x20

/*copies the value of the
target register to the address in
RAM pointer to by the PTR
register.*/
#define CPY 0x30

/*Writes the contents of the targeted register to
the address in RAM pointed to by PTR.*/
#define WTR 0x40

/*copies the value in ram pointed to by
PTR into the specified register.*/
#define RFR 0x50

/*Adds the contents of register B and
register A and overwrites A with the result.*/
#define ADD 0x60

/*subtracts the contents of register B from register A
and overwrites A with the result.*/
#define SUB 0x70

/*
copies the value in RAM at address equal to PTR into
the address in RAM equal to PTR.
RAM[register_DAT] = RAM[register_PTR];
*/
#define MRR 0x80

/*(copy to buffer)
Copies the value in target register to the buffer register*/
#define CTB 0x90

/*(copy from buffer)
Copies the value to target register from the buffer register*/
#define CFB 0xA0

/*Bigger than branch
branch, if the value in register A is bigger
than the one in CPY then the program will jump
to the target address*/
#define GTB 0xB0

/*Less than branch
branch, if the value in register A is smaller
than the one in CPY then the program will jump
to the target address*/
#define LTB 0xC0

/*Less than branch
branch, if the value in register A is equal to
one in CPY then the program will jump to the
target address*/
#define ETB 0xD0

/*Return,
This will put the value in the SP register into the PC*/
#define RET 0xE0
/*
This is going to be an emulator for a
system based around generating sounds
and making music.
*/

/*
Yes, for this program I am using hex notation for ALL of the variable assignments.
this is:
	-it looks cool.
	-it's my program.
	-it's easier for the lower level bit violating.
*/

//Register A
byte register_A = 0x00;
//Register B
byte register_B = 0x00;
//Register C
byte register_C = 0x00;
//Register D
byte register_D = 0x00;
//PC
byte register_PC = 0x00;
//IO register
byte register_IO = 0x00;
//PTR register for RAM writes.
byte register_PTR = 0x00;
//The copy buffer register.
byte register_BUF = 0x00;
//The stack pointer (the value of the PC befor a jump).
byte register_SP = 0x00;

//255 bytes of ROM for program space.
char ROM[0xff];

//255 bytes of RAM for program space.
char RAM[0xff];

/*
The first 4 bytes are an instruction.
The last 4 bytes are the target register.
*/
byte register_INS = 0x00;

/*
This is the data for the instruction.
*/
byte register_DAT = 0x00;

//This is the register the instruction is targeted (first 4 bits).
byte register_INS_reg = 0x00;
//This is the operation/instruction (last 4 bits).
byte register_INS_opr = 0x00;


#endif