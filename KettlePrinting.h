#ifndef KETTLE_PRINTING
#define KETTLE_PRINTING

#include <raylib.h>

#define KETTLE_WRAP_LIMIT 300

char* getGlobalKettleBuffer()
{
	static char global_kettle_printing_buffer[1000] = "";
	return global_kettle_printing_buffer;
}
int print_column, print_row, print_gap = 7;
Color text_colour;

#include <string.h>

void newKLine()
{
	print_column = 0;
	print_row++;
}

void printK(char* string)
{
	for(int i = 0; i != strlen(string); i++)
	{
		char input[2] = {string[i], '\0'};

		DrawText(input, ((print_gap << 1) * print_column), ((print_gap << 2) * print_row), 25, text_colour);
		print_column++;

		if(input[0] == '\n')
		{
			newKLine();
		}
	}
	// if(print_column == KETTLE_WRAP_LIMIT)
	// {
	// 	newKLine();
	// }
}

void printKFromBuffer()
{
	printK(getGlobalKettleBuffer());
}

void clearKScreen()
{
	print_column = print_row = 0;
	text_colour = WHITE;
	ClearBackground(BLACK);
}
#endif
