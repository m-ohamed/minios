#include <stdio.h>

int interrupt (int number, int AX, int BX, int CX, int DX);
void putInMemory (int segment, int address, char character);
void printString(char*);
char* readString(char*);
void readSector(char*, int);
int div(int, int);
int rem(int, int);
void handleInterrupt21(int, int, int, int);
void readFile(char*, char*);
void executeProgram(char*, int);
void launchProgram(int segment);
void terminate();

int main()
{

	/*interrupt(0x10, 0xE*256+'H', 0, 0, 0);
	interrupt(0x10, 0xE*256+'e', 0, 0, 0);
	interrupt(0x10, 0xE*256+'l', 0, 0, 0);
	interrupt(0x10, 0xE*256+'l', 0, 0, 0);
	interrupt(0x10, 0xE*256+'o', 0, 0, 0);
	interrupt(0x10, 0xE*256+' ', 0, 0, 0);
	interrupt(0x10, 0xE*256+'W', 0, 0, 0);
	interrupt(0x10, 0xE*256+'o', 0, 0, 0);
	interrupt(0x10, 0xE*256+'r', 0, 0, 0);
	interrupt(0x10, 0xE*256+'l', 0, 0, 0);
	interrupt(0x10, 0xE*256+'d', 0, 0, 0);
	interrupt(0x10, 0xE*256+'!', 0, 0, 0);*/


	//printString("Hello World\0");

	/*char line[80];
	printString("Enter a line: \0");
	readString(line);
	printString(line);*/


	/*char buffer[512];
	readSector(buffer, 30);
	printString(buffer);*/

	/*makeInterrupt21();
	interrupt(0x21,0,0,0,0);*/

	// char line[80];
	// makeInterrupt21();
	// interrupt(0x21,1,line,0,0);
	// interrupt(0x21,3,line,0,0);

	// char buffer[13312]; /*this is the maximum size of a file*/
	// makeInterrupt21();
	// interrupt(0x21, 3, "messag\0", buffer, 0); /*read the file into buffer*/
	// interrupt(0x21, 0, buffer, 0, 0); /*print out the file*/
	// while(1); /*hang up*/

	// makeInterrupt21();
	// interrupt(0x21, 4, "tstprg\0", 0x2000, 0);
	// while(1);

	// makeInterrupt21();
	// interrupt(0x21, 4, "tstpr2\0", 0x2000, 0);
	// while(1);

	makeInterrupt21();
	interrupt(0x21, 4, "shell\0", 0x2000, 0);


	//while(1){}

	return 0;

}

void terminate()
{
	char shell[6];
	shell[0] = 's';
	shell[1] = 'h';
	shell[2] = 'e';
	shell[3] = 'l';
	shell[4] = 'l';
	shell[5] = '\0';

	interrupt(0x21, 4, shell, 0x2000, 0);
}

void executeProgram(char* name, int segment)
{
	int i;
	int j;
	char program[13312];

	for(i = 0; i < 13312; i++)
	{
		program[i] = 0x00;
	}

	readFile(name, program);

	if(program[0] == 0x00)
	{
		char er[44];
		er[0] = 'C';
		er[1] = 'a';
		er[2] = 'n';
		er[3] = 't';
		er[4] = ' ';
		er[5] = 'e';
		er[6] = 'x';
		er[7] = 'e';
		er[8] = 'c';
		er[9] = 'u';
		er[10] = 't';
		er[11] = 'e';
		er[12] = ' ';
		er[13] = 'p';
		er[14] = 'r';
		er[15] = 'o';
		er[16] = 'g';
		er[17] = 'r';
		er[18] = 'a';
		er[19] = 'm';
		er[20] = '!';
		er[21] = '\n';
		er[22] = '\b';
		er[23] = '\b';
		er[24] = '\b';
		er[25] = '\b';
		er[26] = '\b';
		er[27] = '\b';
		er[28] = '\b';
		er[29] = '\b';
		er[30] = '\b';
		er[31] = '\b';
		er[32] = '\b';
		er[33] = '\b';
		er[34] = '\b';
		er[35] = '\b';
		er[36] = '\b';
		er[37] = '\b';
		er[38] = '\b';
		er[39] = '\b';
		er[40] = '\b';
		er[41] = '\b';
		er[42] = '\b';
		er[43] = '\0';
		interrupt(0x21, 0, er, 0, 0);
		return;
	}

	for(j = 0; j < 13312; j++)
	{
		putInMemory(segment, j, program[j]);
	}

	launchProgram(segment);

}

void readFile(char* f, char* b)
{
	int i;
	int found;
	int j;
	char charArray[512];

	readSector(charArray,2);
	found = 0;



	for(i = 0; found < 6 && i <= 512; i++)
	{
		if(f[found] == charArray[i])
		{
			found++;
		}
		else
		{
			found = 0;
		}
	}

	for(j = 0; charArray[i+j] != 0x00; j++)
	{
		readSector(b,charArray[i+j]);
		b += 512;
	}

	return;
}

void printString(char* c)
{
	int i;

	for(i = 0; c[i] != '\0'; i = i + 1)
	{
		interrupt(0x10, 0xE*256+c[i], 0, 0, 0);
	}

	return;
}

char* readString(char* c)
{
	int i = 0;

	while(1)
	{
		c[i] = interrupt(0x16, 0, 0, 0, 0);
		if(c[i] == 0xd)
		{
			interrupt(0x10, 0xE*256+0xd, 0, 0, 0);
			c[i] = 0xa;
			interrupt(0x10, 0xE*256+c[i], 0, 0, 0);
			i = i + 1;
			c[i] = 0x0;
			interrupt(0x10, 0xE*256+c[i], 0, 0, 0);
			interrupt(0x10, 0xE*256+'\b', 0, 0, 0);
			return c;
		}
		else
		{
			if(c[i] == 0x8)
			{
				if(i != 0)
				{
					interrupt(0x10, 0xE*256+'\b', 0, 0, 0);
					interrupt(0x10, 0xE*256+' ', 0, 0, 0);
					interrupt(0x10, 0xE*256+'\b', 0, 0, 0);
					i = i - 1;
				}
			}
			else
			{
				interrupt(0x10, 0xE*256+c[i], 0, 0, 0);
				i = i + 1;
			}
		}
	}

	return c;
}

void readSector(char* buffer, int sector)
{
	int rs = (rem(sector,18)) + 1;
	int head = rem(div(sector,18),2);
	int track = div(sector,36);
	interrupt(0x13, 2*256+1, buffer, track*256+rs, head*256+0);
	return;
}

int div(int nu, int de)
{
	int temp = 1;
	int quotient = 0;
	while (de <= nu)
	{
		de <<= 1;
		temp <<= 1;
	}
	while (temp > 1)
	{
		de >>= 1;
		temp >>= 1;
		if (nu >= de)
		{
			nu -= de;
			quotient += temp;
		}
	}
	return quotient;
}

int rem(int num, int divisor)
{
	int i = 1;
    int product = 0;

    if (divisor == 0)
    {
        return -1;
    }

    if (divisor < 0)
    	divisor = -divisor;


    if (num < 0)
    	num = -num;



    while (product <= num)
    {
        product = divisor * i;
        i++;
    }

    return num - (product - divisor);
}

void handleInterrupt21(int ax, int bx, int cx, int dx)
{
	if(ax == 0)
	{
		printString(bx);
	}
	else
	{
		if(ax == 1)
		{
			readString(bx);
		}
		else
		{
			if(ax == 2)
			{
				readSector(bx,cx);
			}
			else
			{
				if(ax == 3)
				{
					readFile(bx, cx);
				}
				else
				{
					if(ax == 4)
					{
						executeProgram(bx, cx);
					}
					else
					{
						if(ax == 5)
						{
							terminate();
						}
					}
				}
			}
		}
	}
}
