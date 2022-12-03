#ifndef FOUNDATIONS
#define FOUNDATIONS

#include <stdlib.h>
#include <stdio.h>

//registers.
typedef unsigned char byte;

int execution_cycles = 0;

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

//VRAM registers
#define REGISTER_X______    0x0d
#define REGISTER_Y______    0x0d
#define REGISTER_COL____    0x0e

/*No operation*/
#define NOP 0x00

/*
sets the program counter to the
specified value, sets SP to PC
(must end in 0, 2, 4, 6, 8, A, C, E)
used to be called JP
*/
#define JMP 0x10

/*set, sets the value of target
register to specified value.*/
#define SET 0x20

/*copies the value of the
target register to the address in
RAM pointed to the PTR
register.
(Copy To Ram from register)*/
#define CTR 0x30

/*Writes specified value To Ram.*/
#define WTR 0x40

/*copies the value in ram pointed to by
PTR into the specified register.
Read From Ram into register.*/
#define RFR 0x50

/*Adds the contents of register BUF and
register A and overwrites A with the result.*/
#define ADD 0x60

/*subtracts the contents of register BUF from register A
and overwrites A with the result.*/
#define SUB 0x70

/*
Move RAM[operand] to RAM[ptr]
copies the value in RAM at address equal to DAT into
the address in RAM pointed to by PTR.
RAM[register_DAT] = RAM[register_PTR];
*/
#define MRR 0x80

/*(Register To Buffer)
Copies the value in target register to the buffer register*/
#define RTB 0x90

/*(Register From Buffer)
Copies the value to target register from the buffer register*/
#define RFB 0xA0

/*Greater Than Branch
branch, if(register A > BUF)
then the program will jump to the opernad*/
#define GTB 0xB0

/*Less Than Branch
branch, if(register A < BUF)
then the program will jump
to the operand*/
#define LTB 0xC0

/*Equal Than Branch
branch, if(register A == BUF)
one in BUF (buffer)
then the program will jump to the operand*/
#define ETB 0xD0

/*Return,
This will put the value in the SP register into the PC
*/
#define RET 0xE0

/*
This switches RAM mode between VRAM and RAM (SRM = Ram Mode Switch).
*/
#define SRM 0xF0

/*
Yes, for this program I am using hex notation for ALL of the variable assignments.
this is:
	-it looks cool.
	-it's my program.
	-it's easier for the lower level bit violating.
*/

//Register A
byte register_A = 0x00;  bool register_A_updated;
//Register B
byte register_B = 0x00;  bool register_B_updated;
//Register C
byte register_C = 0x00;  bool register_C_updated;
//Register D
byte register_D = 0x00;  bool register_D_updated;
//PC
byte register_PC = 0x00;  bool register_PC_updated;
//IO register
byte register_IO = 0x00;  bool register_IO_updated;
//PTR register for RAM writes.
byte register_PTR = 0x00;  bool register_PTR_updated;
//The copy buffer register.
byte register_BUF = 0x00;  bool register_BUF_updated;
//The stack pointer (tracks the last stack position).
byte register_SP = 0x00;  bool register_SP_updated;

//255 bytes of ROM for program space.
byte ROM[0xff + 1];

//255 bytes of RAM for program space.
byte RAM[0xff + 1];

//254 bytes of RAM for program space.
#define VRAM_SIZE (0xff+1)
byte VRAM[VRAM_SIZE];

bool is_VRAM_mode;

/*
The first 4 bytes are an instruction.
The last 4 bytes are the target register.
*/
byte register_INS = 0x00;  bool INS_updated;

/*
This is the data for the instruction.
*/
byte register_DAT = 0x00;  bool register_DAT_updated;

//This is the register the instruction is targeted (first 4 bits).
byte register_INS_reg = 0x00; bool register_INS_reg_updated;
//This is the operation/instruction (last 4 bits).
byte register_INS_opr = 0x00; bool register_INS_opr_updated;

byte stack[0x10]; bool updated_stack[0x10];
byte colour_register = 0x00; bool colour_register_updated;

#endif