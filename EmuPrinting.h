#ifndef EMUPRINTING
#define EMUPRINTING
#include <stdlib.h>
#include <stdio.h>
// (4 flat 81) - 19:39 - oven broken.
#include "KettlePrinting.h"
#include "Foundations.h"

void kettlePrintROMRowColour(int print_row)
{
	char label[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
	sprintf(getGlobalKettleBuffer(), "|%c: ", label[print_row]);
	printKFromBuffer(WHITE);
	
	for(byte col = 0; col != 16; col++)
	{
		if(col + (16 * print_row) == register_PC)
		{
			sprintf(getGlobalKettleBuffer(), "[%02x]", (unsigned char)ROM[col + (16 * print_row)]);
			printKFromBuffer(GREEN);
			col++;
			sprintf(getGlobalKettleBuffer(), "{%02x}", (unsigned char)ROM[col + (16 * print_row)]);
			printKFromBuffer(BLUE);
		}
		else
		{
			if(col + (16 * print_row) == register_PC - 1)
			{
				sprintf(getGlobalKettleBuffer(), " %02x$", (unsigned char)ROM[col + (16 * print_row)]);
				printKFromBuffer(WHITE);
			}
			else
			{
				sprintf(getGlobalKettleBuffer(), " %02x ", (unsigned char)ROM[col + (16 * print_row)]);
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
			sprintf(getGlobalKettleBuffer(), ">%02x<", (unsigned)RAM[col + (16 * print_row)]);
			printKFromBuffer(WHITE);
		}
		else
		{
			sprintf(getGlobalKettleBuffer(), " %02x ", (unsigned)RAM[col + (16 * print_row)]);
			printKFromBuffer(WHITE);
		}
	}
	sprintf(getGlobalKettleBuffer(), "\n");
	printKFromBuffer(WHITE);
}
void resetUpdatedRegisters()
{
	register_A_updated = register_B_updated = register_C_updated = register_D_updated = register_PC_updated = register_IO_updated = register_PTR_updated = register_BUF_updated = register_SP_updated = INS_updated = register_DAT_updated = register_INS_reg_updated = register_INS_opr_updated = x_register_updated = y_register_updated = colour_register_updated = false;
}
void kettlePrintRegistersRAW()
{
	if(IsKeyPressed(KEY_UP))
	{
		print_gap++;
	}
	if(IsKeyPressed(KEY_DOWN))
	{
		print_gap--;
	}
	Color update_display_colours[] = {WHITE, PURPLE};
	// printKFromBuffer();
	// return;
	sprintf(getGlobalKettleBuffer(), "[A   =\t%02x] ", (unsigned)register_A			); printKFromBuffer(update_display_colours[register_A_updated]			); kettlePrintROMRowColour(0x00), kettlePrintRAMRow(0x00);
	sprintf(getGlobalKettleBuffer(), "[B   =\t%02x] ", (unsigned)register_B			); printKFromBuffer(update_display_colours[register_B_updated]			); kettlePrintROMRowColour(0x01), kettlePrintRAMRow(0x01);
	sprintf(getGlobalKettleBuffer(), "[C   =\t%02x] ", (unsigned)register_C			); printKFromBuffer(update_display_colours[register_C_updated]			); kettlePrintROMRowColour(0x02), kettlePrintRAMRow(0x02);
	sprintf(getGlobalKettleBuffer(), "[D   =\t%02x] ", (unsigned)register_D			); printKFromBuffer(update_display_colours[register_D_updated]			); kettlePrintROMRowColour(0x03), kettlePrintRAMRow(0x03);
	sprintf(getGlobalKettleBuffer(), "[PC  =\t%02x] ", (unsigned)register_PC 		); printKFromBuffer(update_display_colours[register_PC_updated]			); kettlePrintROMRowColour(0x04), kettlePrintRAMRow(0x04);
	sprintf(getGlobalKettleBuffer(), "[PTR =\t%02x] ", (unsigned)register_PTR		); printKFromBuffer(update_display_colours[register_PTR_updated]		); kettlePrintROMRowColour(0x05), kettlePrintRAMRow(0x05);
	sprintf(getGlobalKettleBuffer(), "[INS =\t%02x] ", (unsigned)register_INS 		); printKFromBuffer(update_display_colours[register_INS_reg_updated]	); kettlePrintROMRowColour(0x06), kettlePrintRAMRow(0x06);
	sprintf(getGlobalKettleBuffer(), "[DAT =\t%02x] ", (unsigned)register_DAT 		); printKFromBuffer(update_display_colours[register_DAT_updated]		); kettlePrintROMRowColour(0x07), kettlePrintRAMRow(0x07);
	sprintf(getGlobalKettleBuffer(), "[opr =\t%02x] ", (unsigned)register_INS_opr	); printKFromBuffer(update_display_colours[register_INS_opr_updated]	); kettlePrintROMRowColour(0x08), kettlePrintRAMRow(0x08);
	sprintf(getGlobalKettleBuffer(), "[reg =\t%02x] ", (unsigned)register_INS_reg	); printKFromBuffer(update_display_colours[register_INS_reg_updated]	); kettlePrintROMRowColour(0x09), kettlePrintRAMRow(0x09);
	sprintf(getGlobalKettleBuffer(), "[IO  =\t%02x] ", (unsigned)register_IO 		); printKFromBuffer(update_display_colours[register_IO_updated]			); kettlePrintROMRowColour(0x0a), kettlePrintRAMRow(0x0a);
	sprintf(getGlobalKettleBuffer(), "[BUF =\t%02x] ", (unsigned)register_BUF		); printKFromBuffer(update_display_colours[register_BUF_updated]		); kettlePrintROMRowColour(0x0b), kettlePrintRAMRow(0x0b);
	sprintf(getGlobalKettleBuffer(), "[SP  =\t%02x] ", (unsigned)register_SP 		); printKFromBuffer(update_display_colours[register_SP_updated]			); kettlePrintROMRowColour(0x0c), kettlePrintRAMRow(0x0c);
	sprintf(getGlobalKettleBuffer(), "['X  =\t%02x] ", (unsigned)x_register			); printKFromBuffer(update_display_colours[x_register_updated]			); kettlePrintROMRowColour(0x0d), kettlePrintRAMRow(0x0d);
	sprintf(getGlobalKettleBuffer(), "['Y  =\t%02x] ", (unsigned)y_register			); printKFromBuffer(update_display_colours[y_register_updated]			); kettlePrintROMRowColour(0x0e), kettlePrintRAMRow(0x0e);
	sprintf(getGlobalKettleBuffer(), "[COL =\t%02x] ", (unsigned)colour_register	); printKFromBuffer(update_display_colours[colour_register_updated]		); kettlePrintROMRowColour(0x0f), kettlePrintRAMRow(0x0f);
	resetUpdatedRegisters();
}
#endif