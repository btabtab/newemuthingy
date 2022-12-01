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

void loadProgramIntoROM(byte* program, byte offset, int length)
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
	byte fibbonachi_progam[] = {
		(SET | REGISTER_A______), 0x01,//set register A
		(NOP | NOREG___________), 0x00,// |-
		(CTB | REGISTER_A______), 0x00,// o = a;  //0x00 <- reg b
		(ADD | NOREG___________), 0x00,// a += b; //add a + b
		(CFB | REGISTER_B______), 0x00,// b = o;  //reg a <- 0x00
		(JP_ | NOREG___________), 0x02,//  \<--Jump to address 0x04.
	};
	byte RAMFILL_program[] =
	{
		(SET | REGISTER_B______), 0x01,//set register B to 1,
		(ADD | NOREG___________), 0x00,//increments the value of reg A,
		(CTB | REGISTER_B______), 0x00,//copies the value of reg B to BUF,
		(CTR | REGISTER_A______), 0x00,//copies the value of reg A to RAM[PTR],
		(RFR | REGISTER_PTR____), 0x00,//copies the value of RAM[PTR] to PTR,
		(GTB | NOREG___________), 0x0E,//asks if register A is bigger than BUF, if so then it jumps to 0x0E,
		(JP_ | NOREG___________), 0x02,//jumps to 0x02 if A is not bigger than BUF,
		(CTB | REGISTER_A______), 0x00,//copies the value of register A to the buffer,
		(CFB | REGISTER_PTR____), 0x00,//copies the value of BUF to PTR,
		(RMS | NOREG___________), 0x00,//switches RAM mode,
		(JP_ | NOREG___________), 0x00,//jumps to program start.,
	};

	for(int i = 0; i != 0xff; i++)
	{
		ROM[i] = RAM[i] = VRAM[i] = 0x00;
	}
	for(byte a = 1; a != 0xff; a+=2)
	{
		ROM[a-1] = 0x00;
		ROM[a-0] = 0xb0;
	}
	byte offset = 0x00;
	loadProgramIntoROM(RAMFILL_program, offset, sizeof(RAMFILL_program));

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
			sprintf(getGlobalKettleBuffer(), "NOP\n");
			printKFromBuffer(WHITE);
		return;
		break;

		case JP_:
			sprintf(getGlobalKettleBuffer(), "JP, sleeping for 1\n");
			printKFromBuffer(WHITE);
			//fist modification to this function in god knows how long!@!@:
			register_SP = register_PC;
			register_PC = register_DAT;
			register_SP_updated = register_PC_updated = true;
		return;
		break;

		case SET:
			sprintf(getGlobalKettleBuffer(), "SET\n");
			printKFromBuffer(WHITE);
			writeToRegister(register_INS_reg, register_DAT);
		return;
		break;

		case CTR:
			sprintf(getGlobalKettleBuffer(), "copying val %02x, into RAM\n", getRegADDR(register_INS_reg));
			printKFromBuffer(WHITE);
			if(is_VRAM_mode)
			{
				VRAM[register_PTR] = getRegADDR(register_INS_reg);
			}
			else
			{
				RAM[register_PTR] = getRegADDR(register_INS_reg);
			}
		return;
		break;

		// case PTR:
		// 	sprintf(getGlobalKettleBuffer(), "PTR\n");
		printKFromBuffer(WHITE);
		// 	register_PTR = register_DAT;
		// return;
		// break;

		case WTR:
			sprintf(getGlobalKettleBuffer(), "WTR\n");
			printKFromBuffer(WHITE);
			if(is_VRAM_mode)
			{
				VRAM[register_PTR] = register_DAT;
			}
			else
			{
				RAM[register_PTR] = register_DAT;
			}
		return;
		break;

		case RFR:
			if(is_VRAM_mode)
			{
				sprintf(getGlobalKettleBuffer(), "RFR, %02x\n", VRAM[register_PTR]);
				printKFromBuffer(WHITE);
				writeToRegister(register_INS_reg, VRAM[register_PTR]);
			}
			else
			{
				sprintf(getGlobalKettleBuffer(), "RFR, %02x\n", RAM[register_PTR]);
				printKFromBuffer(WHITE);
				writeToRegister(register_INS_reg, RAM[register_PTR]);
			}
		return;
		break;

		case ADD:
			sprintf(getGlobalKettleBuffer(), "ADD\n");
			printKFromBuffer(WHITE);
			register_A = register_A + register_B;
			register_A_updated = true;
			return;
		break;
		case SUB:
			sprintf(getGlobalKettleBuffer(), "SUB\n");
			printKFromBuffer(WHITE);
			register_A = register_A - register_B;
			register_A_updated = true;
		case MRR:
			sprintf(getGlobalKettleBuffer(), "Move ram ro ram (MRR)\n");
			printKFromBuffer(WHITE);
			if(is_VRAM_mode)
			{
				VRAM[register_DAT] = VRAM[register_PTR];
			}
			else
			{
				RAM[register_DAT] = RAM[register_PTR];
			}
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
			sprintf(getGlobalKettleBuffer(), "GTB\n");
			printKFromBuffer(WHITE);
			sprintf(getGlobalKettleBuffer(), "(%02x(A) > %02x(BUF))\n", (byte)register_A, (byte)register_BUF);
			printKFromBuffer(WHITE);
			if(register_A > register_BUF)
			{
				register_SP = register_PC;
				register_PC = register_DAT;
				register_SP_updated = register_PC_updated = true;
				sprintf(getGlobalKettleBuffer(), "true condition, executing jump.\n");
				printKFromBuffer(WHITE);
			}
		break;
		case LTB:
			sprintf(getGlobalKettleBuffer(), "LTB\n");
			printKFromBuffer(WHITE);
			sprintf(getGlobalKettleBuffer(), "(%02x(A) < %02x(BUF))\n", (byte)register_A, (byte)register_BUF);
			printKFromBuffer(WHITE);
			if(register_A < register_BUF)
			{
				register_SP = register_PC;
				register_PC = register_DAT;
				register_SP_updated = register_PC_updated = true;
				sprintf(getGlobalKettleBuffer(), "true condition, executing jump.\n");
				printKFromBuffer(WHITE);
			}
		break;
		case ETB:
			sprintf(getGlobalKettleBuffer(), "ETB\n");
			printKFromBuffer(WHITE);
			sprintf(getGlobalKettleBuffer(), "(%02x(A) == %02x(BUF)))\n", (byte)register_A, (byte)register_BUF);
			printKFromBuffer(WHITE);
			if(register_A == register_BUF)
			{
				register_SP = register_PC;
				register_PC = register_DAT;
				register_SP_updated = register_PC_updated = true;
				sprintf(getGlobalKettleBuffer(), "true condition, executing jump.\n");
				printKFromBuffer(WHITE);
			}
		break;

		case RET:
			sprintf(getGlobalKettleBuffer(), "Return\n");
			printKFromBuffer(WHITE);
			register_PC = register_SP;
			register_PC_updated = true;
		break;

		case RMS:
			is_VRAM_mode = !is_VRAM_mode;
		break;

		//If this happens something has gone horribly wrong.
		default:
		sprintf(getGlobalKettleBuffer(), "klaopk\n");
		printKFromBuffer(WHITE);
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
			sprintf(getGlobalKettleBuffer(), "\nREGISTER_A set to: %02x\n", value);
			printKFromBuffer(WHITE);
			printKFromBuffer(WHITE);

			register_A = value;
			register_A_updated = true;
		break;

		case REGISTER_B______:
			sprintf(getGlobalKettleBuffer(), "\nREGISTER_B set to: %02x\n", value);
			printKFromBuffer(WHITE);
			register_B = value;
			register_B_updated = true;
		break;

		case REGISTER_C______:
			sprintf(getGlobalKettleBuffer(), "\nREGISTER_C set to: %02x\n", value);
			printKFromBuffer(WHITE);
			register_C = value;
			register_C_updated = true;
		break;

		case REGISTER_D______:
			sprintf(getGlobalKettleBuffer(), "\nREGISTER_D set to: %02x\n", value);
			printKFromBuffer(WHITE);
			register_D = value;
			register_D_updated = true;
		break;

		case REGISTER_PC_____:
			sprintf(getGlobalKettleBuffer(), "\nREGISTER_PC set to: %02x\n", value);
			printKFromBuffer(WHITE);
			register_PC = value;
			register_PC_updated = true;
		break;

		case REGISTER_IO_____:
			sprintf(getGlobalKettleBuffer(), "\nREGISTER_I set to: %02x\n", value);
			printKFromBuffer(WHITE);
			register_IO = value;
			register_IO_updated = true;
		break;

		case REGISTER_PTR____:
			sprintf(getGlobalKettleBuffer(), "\nREGISTER_PTR set to: %02x\n", value);
			printKFromBuffer(WHITE);
			register_PTR = value;
			register_PTR_updated = true;
		break;

		case REGISTER_INS____:
			sprintf(getGlobalKettleBuffer(), "\nREGISTER_I set to: %02x\n", value);
			printKFromBuffer(WHITE);
			register_INS = value;
			INS_updated = true;
		break;

		case REGISTER_DAT____:
			sprintf(getGlobalKettleBuffer(), "\nREGISTER_D set to: %02x\n", value);
			printKFromBuffer(WHITE);
			register_DAT = value;
			register_DAT_updated = true;
		break;

		case REGISTER_INS_REG:
			sprintf(getGlobalKettleBuffer(), "\nREGISTER_I set to: %02x\n", value);
			printKFromBuffer(WHITE);
			register_INS_reg = value;
			register_INS_reg_updated = true;
		break;

		case REGISTER_INS_OPR:
			sprintf(getGlobalKettleBuffer(), "\nREGISTER_I set to: %02x\n", value);
			printKFromBuffer(WHITE);
			register_INS_opr = value;
			register_INS_opr_updated= true;
		break;

		case REGISTER_SP_____:
			sprintf(getGlobalKettleBuffer(), "\nREGISTER_SP set to: %02x\n", value);
			printKFromBuffer(WHITE);
			register_SP = value;
			register_SP_updated = true;
		break;

	default:
	sprintf(getGlobalKettleBuffer(), "\ntarget %02x\n", target);
	printKFromBuffer(WHITE);
	sprintf(getGlobalKettleBuffer(), "value %02x\n", value);
	printKFromBuffer(WHITE);
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
			sprintf(getGlobalKettleBuffer(), "\nreturning REGISTER_A\n");
			printKFromBuffer(WHITE);
			return register_A;
		break;

		case REGISTER_B______:
			sprintf(getGlobalKettleBuffer(), "\nreturning REGISTER_B\n");
			printKFromBuffer(WHITE);
			return register_B;
		break;

		case REGISTER_C______:
			sprintf(getGlobalKettleBuffer(), "\nreturning REGISTER_C\n");
			printKFromBuffer(WHITE);
			return register_C;
		break;

		case REGISTER_D______:
			sprintf(getGlobalKettleBuffer(), "\nreturning REGISTER_D\n");
			printKFromBuffer(WHITE);
			return register_D;
		break;

		case REGISTER_PC_____:
			sprintf(getGlobalKettleBuffer(), "\nreturning REGISTER_PC\n");
			printKFromBuffer(WHITE);
			return register_PC;
		break;

		case REGISTER_IO_____:
			sprintf(getGlobalKettleBuffer(), "\nreturning REGISTER_I\n");
			printKFromBuffer(WHITE);
			return register_IO;
		break;

		case REGISTER_PTR____:
			sprintf(getGlobalKettleBuffer(), "\nreturning REGISTER_PTR\n");
			printKFromBuffer(WHITE);
			return register_PTR;

		break;

		case REGISTER_INS____:
			sprintf(getGlobalKettleBuffer(), "\nreturning REGISTER_I\n");
			printKFromBuffer(WHITE);
			return register_INS;
		break;

		case REGISTER_DAT____:
			sprintf(getGlobalKettleBuffer(), "\nreturning REGISTER_D\n");
			printKFromBuffer(WHITE);
			return register_DAT;
		break;

		case REGISTER_INS_REG:
			sprintf(getGlobalKettleBuffer(), "\nreturning REGISTER_I\n");
			printKFromBuffer(WHITE);
			return register_INS_reg;
		break;

		case REGISTER_INS_OPR:
			sprintf(getGlobalKettleBuffer(), "\nreturning REGISTER_I\n");
			printKFromBuffer(WHITE);
			return register_INS_opr;
		break;

		case REGISTER_SP_____:
			sprintf(getGlobalKettleBuffer(), "\nreturning REGISTER_SP\n");
			printKFromBuffer(WHITE);
			return register_SP;
		break;

	default:
	sprintf(getGlobalKettleBuffer(), "\nUH OH BROKED\n");
	printKFromBuffer(WHITE);
	return 'g';
		break;
	}
}
#endif
