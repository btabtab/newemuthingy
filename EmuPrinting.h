#ifndef EMUPRINTING
#define EMUPRINTING
#include <stdlib.h>
#include <stdio.h>
// (4 flat 81) - 19:39 - oven broken.
#include "old_emu_code.h"
#include "KettlePrinting.h"
#include "Foundations.h"

void kettlePrintROMRowColour(int print_row)
{
	char label[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
	sprintf(getGlobalKettleBuffer(), "|%c: ", label[print_row]);
	printKFromBuffer(WHITE);
	
	for(byte col = 0; col != 16; col++)
	{
		byte access_index = (byte)ROM[col + (16 * print_row)];

		if(col + (16 * print_row) == register_PC)
		{
			sprintf(getGlobalKettleBuffer(), "[%02X]", access_index);
			printKFromBuffer(GREEN);
			col++;
			sprintf(getGlobalKettleBuffer(), "{%02X}", access_index);
			printKFromBuffer(BLUE);
		}
		else
		{
			if(col + (16 * print_row) == register_PC - 1)
			{
				sprintf(getGlobalKettleBuffer(), " %02X$", access_index);
				printKFromBuffer(WHITE);
			}
			else
			{
				sprintf(getGlobalKettleBuffer(), " %02X ", access_index);
				printKFromBuffer(WHITE);
			}
		}
	}
}
char ins_list[16][4] =
{
	"   ",
	"JMP",
	"SET",
	"CTR",
	"WTR",
	"RFR",
	"ADD",
	"SUB",
	"MRR",
	"RTB",
	"RFB",
	"GTB",
	"LTB",
	"ETB",
	"RET",
	"SRM",
};
char reg_list[0x10][5] =
{
	"    ",
	"RG A",
	"RG B",
	"RG C",
	"RG D",
	" PC ",
	" IO ",
	"PNTR",
	"PROC",//process register
	"DATA",
	"INST",//instruction
	"OPER",//operand
	" SP ",
	"ERR1",
	"ERR2",
	"ERR3",
};


char* getInstructionAsString(byte instruction)
{
	return ins_list[instruction >> 4];
}
bool doesInsUseRegister(byte reg)
{
	return (reg == ADD || reg == SUB || reg == SET || reg == CTR || reg == RFR || reg == RTB || reg == RFB || reg == GTB || reg == LTB || reg == ETB);
}
char* getRegisterAsString(byte reg)
{
	return reg_list[(byte)(reg << 4) >> 4];
}
void kettlePrintROMFormattedRowColour(int print_row)
{
	char label[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
	sprintf(getGlobalKettleBuffer(), "|%c:", label[print_row]);
	printKFromBuffer(WHITE);
	
	for(byte col = 0; col != 16; col++)
	{
		byte access_index = (byte)col + (16 * print_row);

		if(col + (16 * print_row) == register_PC)
		{
			sprintf(getGlobalKettleBuffer(), "$%s", getInstructionAsString(ROM[access_index]));
			printKFromBuffer(GREEN);
			col++;
			sprintf(getGlobalKettleBuffer(), "{%02X}", ROM[access_index+1]);
			printKFromBuffer(BLUE);
		}
		else
		{
			sprintf(getGlobalKettleBuffer(), " %s", getInstructionAsString(ROM[access_index]));
			printKFromBuffer(WHITE);
			col++;
			if(ROM[access_index] || (byte)ROM[(col - 1) + (16 * print_row)])
			{
				sprintf(getGlobalKettleBuffer(), "{%02X}", ROM[access_index+1]);
			}
			else
			{
				sprintf(getGlobalKettleBuffer(), "    ");
			}
			printKFromBuffer(LIGHTGRAY);
		}
	}
}
void kettlePrintRAMRow(int print_row)
{
	char label[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
	sprintf(getGlobalKettleBuffer(), "|%c: ", label[print_row]);
	printKFromBuffer(WHITE);
	for(byte col = 0; col != 16; col++)
	{
		if(col + (16 * print_row) == register_PTR)
		{
			if(is_VRAM_mode){sprintf(getGlobalKettleBuffer(), ">%02X<", (byte)VRAM[col + (16 * print_row)]);}
			else{sprintf(getGlobalKettleBuffer(), ">%02X<", (byte)RAM[col + (16 * print_row)]);}

			printKFromBuffer(VRAM_colours[is_VRAM_mode + 1 + 2]);
		}
		else
		{
			if(is_VRAM_mode){sprintf(getGlobalKettleBuffer(), " %02X ", (byte)VRAM[col + (16 * print_row)]);}
			else{sprintf(getGlobalKettleBuffer(), " %02X ", (byte)RAM[col + (16 * print_row)]);}

			printKFromBuffer(VRAM_colours[is_VRAM_mode + 1]);
		}
	}
	sprintf(getGlobalKettleBuffer(), "\n");
	printKFromBuffer(WHITE);
}
void resetUpdatedRegisters()
{
	register_A_updated = register_B_updated = register_C_updated = register_D_updated = register_PC_updated = register_IO_updated = register_PTR_updated = register_BUF_updated = register_SP_updated = INS_updated = register_DAT_updated = register_INS_reg_updated = register_INS_opr_updated = colour_register_updated = false;
	for(int i = 0; i != 0x10; i++)
	{
		updated_stack[i] = false;
	}
}

Color update_display_colours[] = {WHITE, PURPLE, GRAY, DARKPURPLE};

int n_frame_rate = 2;

void kettlePrintRegistersRAW()
{
	if(IsKeyPressed(KEY_UP))
	{
		n_frame_rate += (1 + IsKeyDown(KEY_LEFT_SHIFT));
		SetTargetFPS(n_frame_rate);
	}
	if(IsKeyPressed(KEY_DOWN))
	{
		n_frame_rate -= (1 + IsKeyDown(KEY_LEFT_SHIFT));
		SetTargetFPS(n_frame_rate);
	}
	if(IsKeyPressed(KEY_LEFT))
	{
		n_frame_rate = 30 + (n_frame_rate * IsKeyDown(KEY_LEFT_SHIFT));
	}
	if(IsKeyPressed(KEY_RIGHT))
	{
		n_frame_rate = (n_frame_rate * IsKeyDown(KEY_LEFT_SHIFT)) - 3;
	}

	if(n_frame_rate < 0)
	{
		n_frame_rate = -(n_frame_rate);
	}
	SetTargetFPS(n_frame_rate);

	sprintf(getGlobalKettleBuffer(), "[A   =\t%02X] ", (unsigned)register_A			); printKFromBuffer(update_display_colours[register_A_updated]			); kettlePrintROMRowColour(0x00), kettlePrintRAMRow(0x00);
	sprintf(getGlobalKettleBuffer(), "[B   =\t%02X] ", (unsigned)register_B			); printKFromBuffer(update_display_colours[register_B_updated]			); kettlePrintROMRowColour(0x01), kettlePrintRAMRow(0x01);
	sprintf(getGlobalKettleBuffer(), "[C   =\t%02X] ", (unsigned)register_C			); printKFromBuffer(update_display_colours[register_C_updated]			); kettlePrintROMRowColour(0x02), kettlePrintRAMRow(0x02);
	sprintf(getGlobalKettleBuffer(), "[D   =\t%02X] ", (unsigned)register_D			); printKFromBuffer(update_display_colours[register_D_updated]			); kettlePrintROMRowColour(0x03), kettlePrintRAMRow(0x03);
	sprintf(getGlobalKettleBuffer(), "[PC  =\t%02X] ", (unsigned)register_PC 		); printKFromBuffer(update_display_colours[register_PC_updated]			); kettlePrintROMRowColour(0x04), kettlePrintRAMRow(0x04);
	sprintf(getGlobalKettleBuffer(), "[PTR =\t%02X] ", (unsigned)register_PTR		); printKFromBuffer(update_display_colours[register_PTR_updated]		); kettlePrintROMRowColour(0x05), kettlePrintRAMRow(0x05);
	sprintf(getGlobalKettleBuffer(), "[INS =\t%02X] ", (unsigned)register_INS 		); printKFromBuffer(update_display_colours[register_INS_reg_updated]	); kettlePrintROMRowColour(0x06), kettlePrintRAMRow(0x06);
	sprintf(getGlobalKettleBuffer(), "[DAT =\t%02X] ", (unsigned)register_DAT 		); printKFromBuffer(update_display_colours[register_DAT_updated]		); kettlePrintROMRowColour(0x07), kettlePrintRAMRow(0x07);
	sprintf(getGlobalKettleBuffer(), "[opr =\t%02X] ", (unsigned)register_INS_opr	); printKFromBuffer(update_display_colours[register_INS_opr_updated]	); kettlePrintROMRowColour(0x08), kettlePrintRAMRow(0x08);
	sprintf(getGlobalKettleBuffer(), "[reg =\t%02X] ", (unsigned)register_INS_reg	); printKFromBuffer(update_display_colours[register_INS_reg_updated]	); kettlePrintROMRowColour(0x09), kettlePrintRAMRow(0x09);
	sprintf(getGlobalKettleBuffer(), "[IO  =\t%02X] ", (unsigned)register_IO 		); printKFromBuffer(update_display_colours[register_IO_updated]			); kettlePrintROMRowColour(0x0a), kettlePrintRAMRow(0x0a);
	sprintf(getGlobalKettleBuffer(), "[BUF =\t%02X] ", (unsigned)register_BUF		); printKFromBuffer(update_display_colours[register_BUF_updated]		); kettlePrintROMRowColour(0x0b), kettlePrintRAMRow(0x0b);
	sprintf(getGlobalKettleBuffer(), "[SP  =\t%02X] ", (unsigned)register_SP 		); printKFromBuffer(update_display_colours[register_SP_updated]			); kettlePrintROMRowColour(0x0c), kettlePrintRAMRow(0x0c);
	sprintf(getGlobalKettleBuffer(), "['X  =\t%02X] ", (unsigned)0					); printKFromBuffer(update_display_colours[0]							); kettlePrintROMRowColour(0x0d), kettlePrintRAMRow(0x0d);
	sprintf(getGlobalKettleBuffer(), "['Y  =\t%02X] ", (unsigned)0					); printKFromBuffer(update_display_colours[0]							); kettlePrintROMRowColour(0x0e), kettlePrintRAMRow(0x0e);
	sprintf(getGlobalKettleBuffer(), "[COL =\t%02X] ", (unsigned)colour_register	); printKFromBuffer(update_display_colours[colour_register_updated]		); kettlePrintROMRowColour(0x0f), kettlePrintRAMRow(0x0f);
	char* ram_modes[2] = {"RAM", "V-RAM"};
	sprintf(getGlobalKettleBuffer(), "execution cycles: %d (This is how many instructions have been executed).\n[Current mode for RAM: %s]\n frame rate / target frame rate = %d / %d\n", execution_cycles, ram_modes[is_VRAM_mode], GetFPS(), n_frame_rate);
	printKFromBuffer(WHITE);
	// drawScreenSmall((2500 - VRAM_SIZE) - 3, (1000 - VRAM_SIZE) - 3);
	drawScreenBig((2500 - VRAM_SIZE) - 3, (1000 - VRAM_SIZE) - 3, VRAM_SIZE >> 4);

	resetUpdatedRegisters();
}
void printStack()
{
	for(int i = 0; i != 0x08; i++)
	{
		sprintf(getGlobalKettleBuffer(), "%X: [0x%02X]", i, stack[i]);
		printKFromBuffer(update_display_colours[updated_stack[i]]);
	}
	sprintf(getGlobalKettleBuffer(), "\n");
	printKFromBuffer(WHITE);
	for(int i = 0x08; i != 0x10; i++)
	{
		sprintf(getGlobalKettleBuffer(), "%X: [0x%02X]", i, stack[i]);
		printKFromBuffer(update_display_colours[updated_stack[i]]);
	}
}
void kettlePrintRegistersModern()
{
	if(IsKeyPressed(KEY_UP))
	{
		n_frame_rate += (1 + IsKeyDown(KEY_LEFT_SHIFT));
		SetTargetFPS(n_frame_rate);
	}
	if(IsKeyPressed(KEY_DOWN))
	{
		n_frame_rate -= (1 + IsKeyDown(KEY_LEFT_SHIFT));
		SetTargetFPS(n_frame_rate);
	}
	if(IsKeyPressed(KEY_LEFT))
	{
		n_frame_rate = 30 + (n_frame_rate * IsKeyDown(KEY_LEFT_SHIFT));
	}
	if(IsKeyPressed(KEY_RIGHT))
	{
		n_frame_rate = (n_frame_rate * IsKeyDown(KEY_LEFT_SHIFT)) - 3;
	}

	if(n_frame_rate < 0)
	{
		n_frame_rate = -(n_frame_rate);
	}

	if(IsKeyPressed(KEY_Z))
	{
		clearAllMemory();
	}
	SetTargetFPS(n_frame_rate);

	sprintf(getGlobalKettleBuffer(), "[A   =\t%02X] ", (unsigned)register_A			); printKFromBuffer(update_display_colours[register_A_updated]			); kettlePrintROMFormattedRowColour(0x00), kettlePrintRAMRow(0x00);
	sprintf(getGlobalKettleBuffer(), "[B   =\t%02X] ", (unsigned)register_B			); printKFromBuffer(update_display_colours[register_B_updated + 2]		); kettlePrintROMFormattedRowColour(0x01), kettlePrintRAMRow(0x01);
	sprintf(getGlobalKettleBuffer(), "[C   =\t%02X] ", (unsigned)register_C			); printKFromBuffer(update_display_colours[register_C_updated]			); kettlePrintROMFormattedRowColour(0x02), kettlePrintRAMRow(0x02);
	sprintf(getGlobalKettleBuffer(), "[D   =\t%02X] ", (unsigned)register_D			); printKFromBuffer(update_display_colours[register_D_updated + 2]		); kettlePrintROMFormattedRowColour(0x03), kettlePrintRAMRow(0x03);
	sprintf(getGlobalKettleBuffer(), "[PC  =\t%02X] ", (unsigned)register_PC 		); printKFromBuffer(update_display_colours[register_PC_updated]			); kettlePrintROMFormattedRowColour(0x04), kettlePrintRAMRow(0x04);
	sprintf(getGlobalKettleBuffer(), "[PTR =\t%02X] ", (unsigned)register_PTR		); printKFromBuffer(update_display_colours[register_PTR_updated + 2]	); kettlePrintROMFormattedRowColour(0x05), kettlePrintRAMRow(0x05);
	sprintf(getGlobalKettleBuffer(), "[INS =\t%02X] ", (unsigned)register_INS 		); printKFromBuffer(update_display_colours[register_INS_reg_updated]	); kettlePrintROMFormattedRowColour(0x06), kettlePrintRAMRow(0x06);
	sprintf(getGlobalKettleBuffer(), "[DAT =\t%02X] ", (unsigned)register_DAT 		); printKFromBuffer(update_display_colours[register_DAT_updated + 2]	); kettlePrintROMFormattedRowColour(0x07), kettlePrintRAMRow(0x07);
	sprintf(getGlobalKettleBuffer(), "[opr =\t%02X] ", (unsigned)register_INS_opr	); printKFromBuffer(update_display_colours[register_INS_opr_updated]	); kettlePrintROMFormattedRowColour(0x08), kettlePrintRAMRow(0x08);
	sprintf(getGlobalKettleBuffer(), "[reg =\t%02X] ", (unsigned)register_INS_reg	); printKFromBuffer(update_display_colours[register_INS_reg_updated + 2]); kettlePrintROMFormattedRowColour(0x09), kettlePrintRAMRow(0x09);
	sprintf(getGlobalKettleBuffer(), "[IO  =\t%02X] ", (unsigned)register_IO 		); printKFromBuffer(update_display_colours[register_IO_updated]			); kettlePrintROMFormattedRowColour(0x0a), kettlePrintRAMRow(0x0a);
	sprintf(getGlobalKettleBuffer(), "[BUF =\t%02X] ", (unsigned)register_BUF		); printKFromBuffer(update_display_colours[register_BUF_updated + 2]	); kettlePrintROMFormattedRowColour(0x0b), kettlePrintRAMRow(0x0b);
	sprintf(getGlobalKettleBuffer(), "[SP  =\t%02X] ", (unsigned)register_SP 		); printKFromBuffer(update_display_colours[register_SP_updated]			); kettlePrintROMFormattedRowColour(0x0c), kettlePrintRAMRow(0x0c);
	sprintf(getGlobalKettleBuffer(), "[    =\t%02X] ", (unsigned)0					); printKFromBuffer(update_display_colours[0 + 2]						); kettlePrintROMFormattedRowColour(0x0d), kettlePrintRAMRow(0x0d);
	sprintf(getGlobalKettleBuffer(), "[    =\t%02X] ", (unsigned)0					); printKFromBuffer(update_display_colours[0]							); kettlePrintROMFormattedRowColour(0x0e), kettlePrintRAMRow(0x0e);
	sprintf(getGlobalKettleBuffer(), "[    =\t%02X] ", (unsigned)0					); printKFromBuffer(update_display_colours[colour_register_updated + 2]	); kettlePrintROMFormattedRowColour(0x0f), kettlePrintRAMRow(0x0f);
	printStack();
	printKFromBuffer(WHITE);
}
void printInformationText()
{
	char* ram_modes[2] = {"RAM", "V-RAM"};
	sprintf(getGlobalKettleBuffer(), "\nexecution cycles: %d (This is how many instructions have been executed).\n[Current mode for RAM: %s]\n frame rate / target frame rate = %d / %d\n", execution_cycles, ram_modes[is_VRAM_mode], GetFPS(), n_frame_rate);
	sprintf(getGlobalKettleBuffer(), "%s\n, Z resets the program, G hides the internals, S stops the program\n", getGlobalKettleBuffer());
	printKFromBuffer(WHITE);

	sprintf(getGlobalKettleBuffer(), "(formatted)	\ncurrent instruction: ( %s | %s ) 0x%02X\n", getInstructionAsString(ROM[register_PC] >> 4), getRegisterAsString(ROM[register_PC]), ROM[register_PC + 1]);
	printKFromBuffer(ORANGE);
	sprintf(getGlobalKettleBuffer(), "(raw)			\ncurrent instruction: ( 0x%02X | 0x%02X ) 0x%02X\n", (ROM[register_PC]), ((ROM[register_PC] << 4) >> 4), ROM[register_PC + 1]);
	printKFromBuffer(RED);

	resetUpdatedRegisters();
}
/*
Q steps,
W increments the instruction at the PC's address,
E decrements the instruction at the PC's address,
R increments the value at the PC's address,
T decrements the value at the PC's address,
*/
void printSystemControls()
{
	sprintf(getGlobalKettleBuffer(), "Press:\n");
	sprintf(getGlobalKettleBuffer(), "%sQ to step\n", getGlobalKettleBuffer());
	sprintf(getGlobalKettleBuffer(), "%sW increments the instruction at the PC's address,\n", getGlobalKettleBuffer());
	sprintf(getGlobalKettleBuffer(), "%sE decrements the instruction at the PC's address,\n", getGlobalKettleBuffer());
	sprintf(getGlobalKettleBuffer(), "%sR decrements the register at the PC's address,\n", getGlobalKettleBuffer());
	sprintf(getGlobalKettleBuffer(), "%sT decrements the register at the PC's address,\n", getGlobalKettleBuffer());
	sprintf(getGlobalKettleBuffer(), "%sY increments the data at the PC's address ( +1),\n", getGlobalKettleBuffer());
	sprintf(getGlobalKettleBuffer(), "%sTU decrements the data at the PC's address ( +1),\n", getGlobalKettleBuffer());
	printKFromBuffer(WHITE);
}

#endif