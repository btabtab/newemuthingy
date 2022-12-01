#ifndef EMUPRINTING
#define EMUPRINTING
#include <stdlib.h>
#include <stdio.h>
// (4 flat 81) - 19:39 - oven broken.
#include "KettlePrinting.h"
#include "Foundations.h"

Color VRAM_colours[0x0f] =
{
	BLACK,
	WHITE,
	BLUE,
	GREEN,
	RED,
};

void drawScreen(int x, int y)
{
	for(int xi = -3; xi != (16 + 3); xi++)
	{
		for(int yi = -3; yi != (16 + 3); yi++)
		{
			DrawRectangle((x + xi) * 4, (y + yi) * 4, 4, 4, ORANGE);
		}
	}
	for(int xi = 16; xi != 0; xi--)
	{
		for(int yi = 16; yi != 0; yi--)
		{
			int access_index = (int)VRAM[xi + (16 * yi)];
			DrawRectangle((x - xi) * (-4), (y - yi) * (-4), 4, 4, VRAM_colours[!access_index]);
		}
	}
}

void kettlePrintROMRowColour(int print_row)
{
	char label[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
	sprintf(getGlobalKettleBuffer(), "|%c: ", label[print_row]);
	printKFromBuffer(WHITE);
	
	for(byte col = 0; col != 16; col++)
	{
		if(col + (16 * print_row) == register_PC)
		{
			sprintf(getGlobalKettleBuffer(), "[%02X]", (byte)ROM[col + (16 * print_row)]);
			printKFromBuffer(GREEN);
			col++;
			sprintf(getGlobalKettleBuffer(), "{%02X}", (byte)ROM[col + (16 * print_row)]);
			printKFromBuffer(BLUE);
		}
		else
		{
			if(col + (16 * print_row) == register_PC - 1)
			{
				sprintf(getGlobalKettleBuffer(), " %02X$", (byte)ROM[col + (16 * print_row)]);
				printKFromBuffer(WHITE);
			}
			else
			{
				sprintf(getGlobalKettleBuffer(), " %02X ", (byte)ROM[col + (16 * print_row)]);
				printKFromBuffer(WHITE);
			}
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
			if(is_VRAM_mode){sprintf(getGlobalKettleBuffer(), ">%02X<", (unsigned)VRAM[col + (16 * print_row)]);}
			else{sprintf(getGlobalKettleBuffer(), ">%02X<", (unsigned)RAM[col + (16 * print_row)]);}

			printKFromBuffer(VRAM_colours[is_VRAM_mode + 1 + 2]);
		}
		else
		{
			if(is_VRAM_mode){sprintf(getGlobalKettleBuffer(), " %02X ", (unsigned)VRAM[col + (16 * print_row)]);}
			else{sprintf(getGlobalKettleBuffer(), " %02X ", (unsigned)RAM[col + (16 * print_row)]);}

			printKFromBuffer(VRAM_colours[is_VRAM_mode + 1]);
		}
	}
	sprintf(getGlobalKettleBuffer(), "\n");
	printKFromBuffer(WHITE);
}
void resetUpdatedRegisters()
{
	register_A_updated = register_B_updated = register_C_updated = register_D_updated = register_PC_updated = register_IO_updated = register_PTR_updated = register_BUF_updated = register_SP_updated = INS_updated = register_DAT_updated = register_INS_reg_updated = register_INS_opr_updated = x_register_updated = y_register_updated = colour_register_updated = false;
}

Color update_display_colours[] = {WHITE, PURPLE};

int n_frame_rate = 2;

void kettlePrintRegistersRAW()
{
	if(IsKeyPressed(KEY_UP))
	{
		n_frame_rate += (1 + IsKeyDown(KEY_LEFT_SHIFT));
	}
	if(IsKeyPressed(KEY_DOWN))
	{
		n_frame_rate -= (1 + IsKeyDown(KEY_LEFT_SHIFT));
	}
	if(IsKeyPressed(KEY_LEFT))
	{
		n_frame_rate = 30;
	}
	if(IsKeyPressed(KEY_RIGHT))
	{
		n_frame_rate = 3;
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
	sprintf(getGlobalKettleBuffer(), "['X  =\t%02X] ", (unsigned)x_register			); printKFromBuffer(update_display_colours[x_register_updated]			); kettlePrintROMRowColour(0x0d), kettlePrintRAMRow(0x0d);
	sprintf(getGlobalKettleBuffer(), "['Y  =\t%02X] ", (unsigned)y_register			); printKFromBuffer(update_display_colours[y_register_updated]			); kettlePrintROMRowColour(0x0e), kettlePrintRAMRow(0x0e);
	sprintf(getGlobalKettleBuffer(), "[COL =\t%02X] ", (unsigned)colour_register	); printKFromBuffer(update_display_colours[colour_register_updated]		); kettlePrintROMRowColour(0x0f), kettlePrintRAMRow(0x0f);
	char* ram_modes[2] = {"RAM", "V-RAM"};
	sprintf(getGlobalKettleBuffer(), "[Current mode for RAM: %s]\n frame rate = %d\n", ram_modes[is_VRAM_mode], n_frame_rate);
	printKFromBuffer(WHITE);

	drawScreen(2500, 1000);

	resetUpdatedRegisters();
}
#endif