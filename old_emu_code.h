#ifndef OLD_EMU_CODE
#define OLD_EMU_CODE

#include <stdlib.h>
#include <stdio.h>

#include <EmuPrinting.h>
#include <KettlePrinting.h>
#include <Foundations.h>

void loadInstructionIntoINSDATRegisters()
{
	register_INS = ROM[register_PC];
	register_DAT = ROM[register_PC+1];
}
void splitINSRegisterIntoRegAndOp()
{
	register_INS_opr = (byte)((register_INS >> 4) << 4);
	register_INS_reg = (byte)((register_INS << 4));
}

void executeInstruction();
void writeToRegister(byte target, byte value);
byte getRegADDR(byte target);

void loadProgramIntoROM(char* program, char offset, int length)
{
	for(byte a = 0; a != length+1; a++)
	{
		ROM[a + offset] = program[a];
	}
}

int runEmu()
{
	/*
	This increments the values
	in registers A and B in
	the fibbonachi sequence.
	*/
	char fibbonachi_progam[] = {
		(SET | REGISTER_A______), 0x01,//set register A
		(NOP | NOREG___________), 0x00,// |-
		(CTB | REGISTER_A______), 0x00,// o = a;  //0x00 <- reg b
		(ADD | NOREG___________), 0x00,// a += b; //add a + b
		(CFB | REGISTER_B______), 0x00,// b = o;  //reg a <- 0x00
		(JP_ | NOREG___________), 0x02,//  \<--Jump to address 0x04.
	};
	for(int i = 0; i != 0xff; i++)
	{
		ROM[i] = RAM[i] = 0x00;
	}
	for(byte a = 1; a != 0xff; a+=2)
	{
		ROM[a-1] = 0x00;
		ROM[a-0] = 0xb0;
	}
	char offset = 0x00;
	loadProgramIntoROM(fibbonachi_progam, offset, sizeof(fibbonachi_progam));

	while(!WindowShouldClose())
	{
		{
			BeginDrawing();
			{
				clearKScreen();
				kettlePrintRegistersRAW();
			}
			EndDrawing();
		}

		loadInstructionIntoINSDATRegisters();
		splitINSRegisterIntoRegAndOp();
		register_PC += 2;
		executeInstruction();
	}
	return 0;
}

void executeInstruction()
{
	switch (register_INS_opr)
	{
		case NOP:
			printf("NOP\n");
		return;
		break;

		case JP_:
			printf("JP, sleeping for 1\n");
			//fist modification to this function in god knows how long!@!@:
			register_SP = register_PC;
			register_PC = register_DAT;
			register_SP_updated = register_PC_updated = true;
		return;
		break;

		case SET:
			printf("SET\n");
			writeToRegister(register_INS_reg, register_DAT);
		return;
		break;

		case CPY:
			printf("copying val %02x, into RAM\n", getRegADDR(register_INS_reg));
			RAM[register_PTR] = getRegADDR(register_INS_reg);
		return;
		break;

		// case PTR:
		// 	printf("PTR\n");
		// 	register_PTR = register_DAT;
		// return;
		// break;

		case WTR:
			printf("WTR\n");
			RAM[register_PTR] = register_DAT;
		return;
		break;

		case RFR:
			printf("RFR, %02x\n", RAM[register_PTR]);
			writeToRegister(register_INS_reg, RAM[register_PTR]);
		return;
		break;

		case ADD:
			printf("ADD\n");
			register_A = register_A + register_B;
			register_A_updated = true;
			return;
		break;
		case SUB:
			printf("SUB\n");
			register_A = register_A - register_B;
			register_A_updated = true;
		case MRR:
			printf("Move ram ro ram (MRR)\n");
			RAM[register_DAT] = RAM[register_PTR];
		break;
		case CTB:
			register_BUF = getRegADDR(register_INS_reg);
			register_BUF_updated = true;
		break;
		case CFB:
			writeToRegister(register_INS_reg, register_BUF);
		break;
		//TODO: test BTB, LTB, ETB!!
		case GTB:
			printf("GTB\n");
			printf("(%02x(A) > %02x(BUF))\n", (unsigned char)register_A, (unsigned char)register_BUF);
			if(register_A > register_BUF)
			{
				register_SP = register_PC;
				register_PC = register_DAT;
				register_SP_updated = register_PC_updated = true;
				printf("true condition, executing jump.\n");
			}
		break;
		case LTB:
			printf("LTB\n");
			printf("(%02x(A) < %02x(BUF))\n", (unsigned char)register_A, (unsigned char)register_BUF);
			if(register_A < register_BUF)
			{
				register_SP = register_PC;
				register_PC = register_DAT;
				register_SP_updated = register_PC_updated = true;
				printf("true condition, executing jump.\n");
			}
		break;
		case ETB:
			printf("ETB\n");
			printf("(%02x(A) == %02x(BUF)))\n", (unsigned char)register_A, (unsigned char)register_BUF);
			if(register_A == register_BUF)
			{
				register_SP = register_PC;
				register_PC = register_DAT;
				register_SP_updated = register_PC_updated = true;
				printf("true condition, executing jump.\n");
			}
		break;

		case RET:
			printf("Return\n");
			register_PC = register_SP;
			register_PC_updated = true;
		break;

		//If this happens something has gone horribly wrong.
		default:
		printf("klaopk\n");
		break;
	}
}

void writeToRegister(byte target, byte value)
{
	switch (target >> 4)
	{
		case NOREG___________:
			return;
		break;

		case REGISTER_A______:
			printf("\nREGISTER_A set to: %02x\n", value);
			register_A = value;
			register_A_updated = true;
		break;

		case REGISTER_B______:
			printf("\nREGISTER_B set to: %02x\n", value);
			register_B = value;
			register_B_updated = true;
		break;

		case REGISTER_C______:
			printf("\nREGISTER_C set to: %02x\n", value);
			register_C = value;
			register_C_updated = true;
		break;

		case REGISTER_D______:
			printf("\nREGISTER_D set to: %02x\n", value);
			register_D = value;
			register_D_updated = true;
		break;

		case REGISTER_PC_____:
			printf("\nREGISTER_PC set to: %02x\n", value);
			register_PC = value;
			register_PC_updated = true;
		break;

		case REGISTER_IO_____:
			printf("\nREGISTER_I set to: %02x\n", value);
			register_IO = value;
			register_IO_updated = true;
		break;

		case REGISTER_PTR____:
			printf("\nREGISTER_PTR set to: %02x\n", value);
			register_PTR = value;
			register_PTR_updated = true;
		break;

		case REGISTER_INS____:
			printf("\nREGISTER_I set to: %02x\n", value);
			register_INS = value;
			INS_updated = true;
		break;

		case REGISTER_DAT____:
			printf("\nREGISTER_D set to: %02x\n", value);
			register_DAT = value;
			register_DAT_updated = true;
		break;

		case REGISTER_INS_REG:
			printf("\nREGISTER_I set to: %02x\n", value);
			register_INS_reg = value;
			register_INS_reg_updated = true;
		break;

		case REGISTER_INS_OPR:
			printf("\nREGISTER_I set to: %02x\n", value);
			register_INS_opr = value;
			register_INS_opr_updated= true;
		break;

		case REGISTER_SP_____:
			printf("\nREGISTER_SP set to: %02x\n", value);
			register_SP = value;
			register_SP_updated = true;
		break;

	default:
	printf("\ntarget %02x\n", target);
	printf("value %02x\n", value);
		break;
	}
}

byte getRegADDR(byte target)
{
	switch (target >> 4)
	{
		case NOREG___________:
			return 'z';
		break;

		case REGISTER_A______:
			printf("\nreturning REGISTER_A\n");
			return register_A;
		break;

		case REGISTER_B______:
			printf("\nreturning REGISTER_B\n");
			return register_B;
		break;

		case REGISTER_C______:
			printf("\nreturning REGISTER_C\n");
			return register_C;
		break;

		case REGISTER_D______:
			printf("\nreturning REGISTER_D\n");
			return register_D;
		break;

		case REGISTER_PC_____:
			printf("\nreturning REGISTER_PC\n");
			return register_PC;
		break;

		case REGISTER_IO_____:
			printf("\nreturning REGISTER_I\n");
			return register_IO;
		break;

		case REGISTER_PTR____:
			printf("\nreturning REGISTER_PTR\n");
			return register_PTR;

		break;

		case REGISTER_INS____:
			printf("\nreturning REGISTER_I\n");
			return register_INS;
		break;

		case REGISTER_DAT____:
			printf("\nreturning REGISTER_D\n");
			return register_DAT;
		break;

		case REGISTER_INS_REG:
			printf("\nreturning REGISTER_I\n");
			return register_INS_reg;
		break;

		case REGISTER_INS_OPR:
			printf("\nreturning REGISTER_I\n");
			return register_INS_opr;
		break;

		case REGISTER_SP_____:
			printf("\nreturning REGISTER_SP\n");
			return register_SP;
		break;

	default:
	printf("\nUH OH BROKED\n");
	return 'g';
		break;
	}
}
#endif
