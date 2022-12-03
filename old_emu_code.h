#ifndef OLD_EMU_CODE
#define OLD_EMU_CODE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <KettlePrinting.h>
#include <Foundations.h>

void kettlePrintRegistersModern();
void kettlePrintRegistersRAW();

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
byte getRegData(byte target);

void loadProgramIntoROM(byte* program, byte offset, int length)
{
	for(byte a = 0; a != length; a++)
	{
		ROM[offset + a] = program[a];
	}
}
void loadFunctionIntoROMSmart(byte* program, byte length, byte* int_to_write_auto_offset_to)
{
	static byte auto_offset = 0;
	auto_offset += length;
	if(int_to_write_auto_offset_to)
	{
		(*int_to_write_auto_offset_to) = auto_offset;
	}
	loadProgramIntoROM(program, (auto_offset - length), length);
}

void clearAllMemory()
{
	register_A = register_B = register_C = register_D = register_DAT = register_INS = register_IO = register_PC = register_PTR = register_SP = 0;
	for(int i = 0; i != 0xff; i++)
	{
		ROM[i] = RAM[i] = VRAM[i] = 0x00;
	}
	for(int i = 0; i != 0x10; i++)
	{
		stack[i] = updated_stack[i] = 0x00;
	}
}

Color VRAM_colours[] =
{
	WHITE,
	LIGHTGRAY,
	GRAY,
	DARKGRAY,
	YELLOW,
	GOLD,
	ORANGE,
	PINK,
	RED,
	MAROON,
	GREEN,
	LIME,
	DARKGREEN,
	SKYBLUE,
	BLUE,
	DARKBLUE,
	PURPLE,
	VIOLET,
	DARKPURPLE,
	BEIGE,
	BROWN,
	DARKBROWN,
	BLACK
};

void drawScreenSmall(int x, int y)
{
	for(int xi = -3; xi != (16 + 3); xi++)
	{
		for(int yi = -3; yi != (16 + 3); yi++)
		{
			DrawPixel(x + xi, y + yi, ORANGE);
		}
	}
	for(int xi = 0; xi != 16; xi++)
	{
		for(int yi = 0; yi != 16; yi++)
		{
			DrawPixel((x + xi), (y + yi), VRAM_colours[(int)VRAM[xi + (16 * yi)]]);
		}
	}
}
int fixRange(int num, int max)
{
	if (max < num)
	{
		if (max < num)
		{
			return max;
		}
		return max - num;
	}
	return num;
}
void drawScreenBig(int x, int y, int size)
{

	for(int xi = 0; xi != 16; xi++)
	{
		for(int yi = 0; yi != 16; yi++)
		{
			for(int xii = 0; xii != size; xii++)
			{
				for(int yii = 0; yii != size; yii++)
				{
					DrawPixel(x + (xi * size) + xii, y + (yi * size) + yii, VRAM_colours[fixRange((int)VRAM[xi + (16 * yi)], 0x0f)]);
				}
			}
			// DrawPixel(x + (xi * size), y + (yi * size), VRAM_colours[(int)VRAM[xi + (16 * yi)]]);
		}
	}
}
void printInformationText();
void initialiseProgram()
{
	/*
	This increments the values
	in registers A and B in
	the fibbonachi sequence.
	*/
	/*
	byte fibbonachi_progam[] =
	{
		(SET | REGISTER_A______), 0x01,//set register A
		(NOP | NOREG___________), 0x00,// |-
		(RTB | REGISTER_A______), 0x00,// o = a;  //0x00 <- reg b
		(ADD | NOREG___________), 0x00,// a += b; //add a + b
		(RFB | REGISTER_B______), 0x00,// b = o;  //reg a <- 0x00
		(JMP | NOREG___________), 0x02,//  \<--Jump to address 0x04.
	};
	*/
	byte main_program[] =
	{
		(SET | REGISTER_C______), 0x01,// |-
		(RTB | REGISTER_C______), 0x00,// |-
		(JMP | NOREG___________), 0x20,// |-
		(SET | REGISTER_SP_____), 0x00,// |-
		(JMP | NOREG___________), 0x00,// |-
	};
	byte ptr_increment[] =
	{
		(SET | REGISTER_A______), 0x01,// |-
		(JMP | NOREG___________), 0xE0,// |-
		(ADD | REGISTER_PTR____), 0x00,// |-
		(RFB | REGISTER_PTR____), 0x00,// |-
		(RET | NOREG___________), 0x00,// |-
	};
	byte updateVRAM[] =
	{
		(SRM | NOREG___________), 0x00,// |-
		(CTR | REGISTER_C______), 0x00,// |-
		(SRM | NOREG___________), 0x00,// |-
		(RET | NOREG___________), 0x00,// |-
	};

	//this is just in case stuff breaks and the PC becoms a non multiple of 2
	byte prog_reset[] =
	{
		(RET | NOREG___________), RET,//
	};

	clearAllMemory();

	loadFunctionIntoROMSmart(main_program, 	sizeof(main_program), 	&main_program[5]);
	loadFunctionIntoROMSmart(ptr_increment, sizeof(ptr_increment), 	&ptr_increment[3]);
	loadFunctionIntoROMSmart(updateVRAM, 	sizeof(updateVRAM), 	NULL);

	// loadProgramIntoROM(main_program,	0x00, sizeof(main_program));
	// loadProgramIntoROM(ptr_increment,	0x20, sizeof(ptr_increment));
	// loadProgramIntoROM(updateVRAM,		0xE0, sizeof(updateVRAM));
	loadProgramIntoROM(prog_reset,		0xFE, sizeof(prog_reset));
}

void printSystemControls();
void stepProgram()
{
	loadInstructionIntoINSDATRegisters();
	splitINSRegisterIntoRegAndOp();
	register_PC += 2;
	executeInstruction();
}
void controlSystem()
{
	if(IsKeyPressed(KEY_Q))
	{
		stepProgram();
	}
	if(IsKeyPressed(KEY_W))
	{
		ROM[register_PC]++;
	}
	if(IsKeyPressed(KEY_E))
	{
		ROM[register_PC]--;
	}
	if(IsKeyPressed(KEY_R))
	{
		ROM[register_PC+1]++;
	}
	if(IsKeyPressed(KEY_T))
	{
		ROM[register_PC+1]--;
	}
}
int runEmu()
{
	initialiseProgram();
	bool render_internals = true, run_program = true;
	while(!WindowShouldClose())
	{
		{
			BeginDrawing();
			{
				clearKScreen();
				if(render_internals)
				{
					if(IsKeyDown(KEY_O))
					{
						kettlePrintRegistersRAW();
					}
					else
					{
						kettlePrintRegistersModern();
						if(!run_program)
						{
							printSystemControls();
						}
					}
				}
				printInformationText();
				drawScreenBig((2500 - VRAM_SIZE) - 3, (1000 - VRAM_SIZE) - 3, VRAM_SIZE >> 4);
			}
			EndDrawing();
		}
		if(run_program)
		{
			stepProgram();
		}

		render_internals = (!IsKeyDown(KEY_G));
		if(IsKeyPressed(KEY_S))
		{
			run_program = !run_program;
			render_internals = true;
		}
		controlSystem();
	}
	return 0;
}

void pushPCToStack()
{
	if(register_SP == 0x10)
	{
		register_SP = 0x00;
	}

	stack[register_SP] = register_PC;

	updated_stack[register_SP] = true;
	
	register_SP++;
}
void popStack()
{
	if(register_SP)
	{
		register_SP--;
	}
	register_PC = stack[register_SP];
	stack[register_SP] = 0x00;
	updated_stack[register_SP] = register_PC_updated = register_SP_updated = true;
}
void executeJump()
{
	//advances stack counter, updates stack at value.
	pushPCToStack();
	//updates PC value.
	register_PC = register_DAT;
}
void executeADD(byte register_to_use)
{
	register_BUF = getRegData(register_to_use) + register_BUF;
	register_BUF_updated = true;
}
void executeSUB(byte register_to_use)
{
	register_BUF = getRegData(register_to_use) - register_BUF;
	register_BUF_updated = true;
}
void executeRET()
{
	popStack();
}
void executeInstruction()
{
	execution_cycles++;
	switch (register_INS_opr)
	{
		case NOP:
			sprintf(getGlobalKettleBuffer(), "NOP\n");
			printKFromBuffer(WHITE);
		return;
		break;

		case JMP:
			sprintf(getGlobalKettleBuffer(), "JP, sleeping for 1\n");
			printKFromBuffer(WHITE);
			executeJump();
		return;
		break;

		case SET:
			sprintf(getGlobalKettleBuffer(), "SET\n");
			printKFromBuffer(WHITE);
			writeToRegister(register_INS_reg, register_DAT);
		return;
		break;

		case CTR:
			sprintf(getGlobalKettleBuffer(), "copying val %02x, into RAM\n", getRegData(register_INS_reg));
			printKFromBuffer(WHITE);
			if(is_VRAM_mode)
			{
				VRAM[register_PTR] = getRegData(register_INS_reg);
			}
			else
			{
				RAM[register_PTR] = getRegData(register_INS_reg);
			}
		return;
		break;

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
			executeADD(register_INS_reg);
			return;
		break;
		case SUB:
			sprintf(getGlobalKettleBuffer(), "SUB\n");
			printKFromBuffer(WHITE);
			executeSUB(register_INS_reg);
		break;
		case MRR:
			sprintf(getGlobalKettleBuffer(), "Move ram to ram (MRR)\n");
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
		case RTB:
			register_BUF = getRegData(register_INS_reg);
			register_BUF_updated = true;
		break;
		case RFB:
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
				executeJump();
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
				pushPCToStack();
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
				pushPCToStack();
				register_PC = register_DAT;
				sprintf(getGlobalKettleBuffer(), "true condition, executing jump.\n");
				printKFromBuffer(WHITE);
			}
		break;

		case RET:
			sprintf(getGlobalKettleBuffer(), "Return\n");
			printKFromBuffer(WHITE);
			executeRET();
		break;

		case SRM:
			is_VRAM_mode = !is_VRAM_mode;
		break;

		//If this happens something has gone horribly wrong.
		default:
		sprintf(getGlobalKettleBuffer(), "\n");
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

byte getRegData(byte target)
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
