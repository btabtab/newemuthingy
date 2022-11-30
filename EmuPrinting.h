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
	printKFromBuffer();
	
	for(byte col = 0; col != 16; col++)
	{
		if(col + (16 * print_row) == register_PC)
		{
			text_colour = GREEN;
			sprintf(getGlobalKettleBuffer(), "[%02x]", (unsigned char)ROM[col + (16 * print_row)]);
			printKFromBuffer();
			col++;
			text_colour = BLUE;
			sprintf(getGlobalKettleBuffer(), "{%02x}", (unsigned char)ROM[col + (16 * print_row)]);
			printKFromBuffer();
		}
		else
		{
			if(col + (16 * print_row) == register_PC - 1)
			{
				sprintf(getGlobalKettleBuffer(), " %02x$", (unsigned char)ROM[col + (16 * print_row)]);
				printKFromBuffer();
			}
			else
			{
				sprintf(getGlobalKettleBuffer(), " %02x ", (unsigned char)ROM[col + (16 * print_row)]);
				printKFromBuffer();
	
			}
		}
		text_colour = WHITE;
	}
}
void kettlePrintRAMRow(int print_row)
{
	char label[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
	sprintf(getGlobalKettleBuffer(), "|%c: ", label[print_row]);
	printKFromBuffer();
	for(byte col = 0; col != 16; col++)
	{
		if(col + (16 * print_row) == register_PTR)
		{
			sprintf(getGlobalKettleBuffer(), ">%02x<", (unsigned)RAM[col + (16 * print_row)]);
			printKFromBuffer();
		}
		else
		{
			sprintf(getGlobalKettleBuffer(), " %02x ", (unsigned)RAM[col + (16 * print_row)]);
			printKFromBuffer();
		}
	}
	sprintf(getGlobalKettleBuffer(), "\n");
	printKFromBuffer();
}
void kettlePrintRegisters()
{
	if(IsKeyPressed(KEY_UP))
	{
		print_gap++;
	}
	if(IsKeyPressed(KEY_DOWN))
	{
		print_gap--;
	}

	// printKFromBuffer();
	// return;
	sprintf(getGlobalKettleBuffer(), "[A   =\t%02x] ", (unsigned)register_A			);  printKFromBuffer(); kettlePrintROMRowColour(0x00), kettlePrintRAMRow(0x00);
	sprintf(getGlobalKettleBuffer(), "[B   =\t%02x] ", (unsigned)register_B			);  printKFromBuffer(); kettlePrintROMRowColour(0x01), kettlePrintRAMRow(0x01);
	sprintf(getGlobalKettleBuffer(), "[C   =\t%02x] ", (unsigned)register_C			);  printKFromBuffer(); kettlePrintROMRowColour(0x02), kettlePrintRAMRow(0x02);
	sprintf(getGlobalKettleBuffer(), "[D   =\t%02x] ", (unsigned)register_D			);  printKFromBuffer(); kettlePrintROMRowColour(0x03), kettlePrintRAMRow(0x03);
	sprintf(getGlobalKettleBuffer(), "[PC  =\t%02x] ", (unsigned)register_PC 		);  printKFromBuffer(); kettlePrintROMRowColour(0x04), kettlePrintRAMRow(0x04);
	sprintf(getGlobalKettleBuffer(), "[PTR =\t%02x] ", (unsigned)register_PTR		);  printKFromBuffer(); kettlePrintROMRowColour(0x05), kettlePrintRAMRow(0x05);
	sprintf(getGlobalKettleBuffer(), "[INS =\t%02x] ", (unsigned)register_INS 		);  printKFromBuffer(); kettlePrintROMRowColour(0x06), kettlePrintRAMRow(0x06);
	sprintf(getGlobalKettleBuffer(), "[DAT =\t%02x] ", (unsigned)register_DAT 		);  printKFromBuffer(); kettlePrintROMRowColour(0x07), kettlePrintRAMRow(0x07);
	sprintf(getGlobalKettleBuffer(), "[opr =\t%02x] ", (unsigned)register_INS_opr	);  printKFromBuffer(); kettlePrintROMRowColour(0x08), kettlePrintRAMRow(0x08);
	sprintf(getGlobalKettleBuffer(), "[reg =\t%02x] ", (unsigned)register_INS_reg	);  printKFromBuffer(); kettlePrintROMRowColour(0x09), kettlePrintRAMRow(0x09);
	sprintf(getGlobalKettleBuffer(), "[IO  =\t%02x] ", (unsigned)register_IO 		);  printKFromBuffer(); kettlePrintROMRowColour(0x0a), kettlePrintRAMRow(0x0a);
	sprintf(getGlobalKettleBuffer(), "[BUF =\t%02x] ",(unsigned)register_BUF		);	printKFromBuffer(); kettlePrintROMRowColour(0x0b), kettlePrintRAMRow(0x0b);
	sprintf(getGlobalKettleBuffer(), "[SP  =\t%02x] ", (unsigned)register_SP 		);  printKFromBuffer(); kettlePrintROMRowColour(0x0c), kettlePrintRAMRow(0x0c);
	sprintf(getGlobalKettleBuffer(), "['?  =\t%02x] ", 0x00 						);	printKFromBuffer(); kettlePrintROMRowColour(0x0d), kettlePrintRAMRow(0x0d);
	sprintf(getGlobalKettleBuffer(), "['?  =\t%02x] ", 0x00 						);	printKFromBuffer(); kettlePrintROMRowColour(0x0e), kettlePrintRAMRow(0x0e);
	sprintf(getGlobalKettleBuffer(), "['?  =\t%02x] ", 0x00 						);	printKFromBuffer(); kettlePrintROMRowColour(0x0f), kettlePrintRAMRow(0x0f);
}
#endif