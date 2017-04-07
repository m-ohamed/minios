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
void writeSector(char*, int);
void deleteFile(char* name);
void writeFile(char* name, char* buffer, int secNum);

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

	// char buffer[13312];
	// makeInterrupt21();
	// interrupt(0x21, 7, "messag\0", 0, 0); //delete messag
	// interrupt(0x21, 3, "messag\0", buffer, 0); // try to read messag
	// interrupt(0x21, 0, buffer, 0, 0); //print out the contents of buffer


	// int i=0;
	// char buffer1[13312];
	// char buffer2[13312];
	// buffer2[0]='h'; buffer2[1]='e'; buffer2[2]='l'; buffer2[3]='l';
	// buffer2[4]='o';
	// for(i=5; i<13312; i++) buffer2[i]=0x0;
	// makeInterrupt21();
	// interrupt(0x21,8, "testW\0", buffer2, 1); //write file testW
	// interrupt(0x21,3, "testW\0", buffer1, 0); //read file testW
	// interrupt(0x21,0, buffer1, 0, 0); // print out contents of testW

	makeInterrupt21();
	interrupt(0x21, 4, "shell\0", 0x2000, 0);


	// while(1){}

	return 0;

}

void writeFile(char* name, char* buffer, int secNum)
{
	int i;
	int j;
	int x;
	int y;
	char directory[512];
	char map[512];

	readSector(map, 1);
	readSector(directory, 2);

	i = 0;
	j = 0;
	x = 0;
	y = 0;

	for(i; i <= 512; i += 32)
	{
		if(directory[i] == 0x00)
		{
			break;
		}
	}

	for(j; j < 6; j++)
	{
		directory[i+j] = name[j];

		if(name[j] == '\0')
		{
			break;
		}
	}

	for(j; j < 6; j++)
		directory[i+j] = 0x00;


	i += 6;
	x = 0;
	y = 0;

	for(x; x < secNum; x++)
	{
		for(y; y <= 512; y++)
		{
			if(map[y] == 0x00)
			{
				map[y] = 0xFF;
				directory[i+x] = y;
				writeSector(buffer, y);
				break;
			}
		}
	}

	for(secNum; secNum < 26; secNum++)
	{
		directory[i+secNum] = 0x00;
	}

	writeSector(map, 1);
	writeSector(directory, 2);

	return;
}

void deleteFile(char* name)
{
	int i;
	int j;
	int found;
	char directory[512];
	char map[512];

	readSector(map, 1);
	readSector(directory, 2);

	found = 0;

	for(i = 0; found < 6 && i <= 512; i++)
	{
		if(name[found] == directory[i])
		{
			found++;
		}
		else
		{
			if(name[found] == '\0' || name[found] == 0x00)
			{
				i = i + (6 - found);
			}
			else
				found = 0;
		}
	}

	if(found == 0)
	{
		char er[18];
		er[0] = 'C';
		er[1] = 'a';
		er[2] = 'n';
		er[3] = 't';
		er[4] = ' ';
		er[5] = 'f';
		er[6] = 'i';
		er[7] = 'n';
		er[8] = 'd';
		er[9] = ' ';
		er[10] = 'f';
		er[11] = 'i';
		er[12] = 'l';
		er[13] = 'e';
		er[14] = '!';
		er[15] = '\n';
		er[16] = '\r';
		er[17] = '\0';
		interrupt(0x21, 0, er, 0, 0);
		return;
	}
	else
	{
		char er[29];
		er[0] = 'F';
		er[1] = 'i';
		er[2] = 'l';
		er[3] = 'e';
		er[4] = ' ';
		er[5] = 'd';
		er[6] = 'e';
		er[7] = 'l';
		er[8] = 'e';
		er[9] = 't';
		er[10] = 'e';
		er[11] = 'd';
		er[12] = ' ';
		er[13] = 's';
		er[14] = 'u';
		er[15] = 'c';
		er[16] = 'c';
		er[17] = 'e';
		er[18] = 's';
		er[19] = 's';
		er[20] = 'f';
		er[21] = 'u';
		er[22] = 'l';
		er[23] = 'l';
		er[24] = 'y';
		er[25] = '.';
		er[26] = '\n';
		er[27] = '\r';
		er[28] = '\0';
		interrupt(0x21, 0, er, 0, 0);
	}

	directory[i-6] = 0x00;

	for(j = 0; directory[i+j] != 0x00 && j < 26; j++)
	{
		map[directory[i+j]] = 0x00;
		directory[i+j] = 0x00;
	}

	writeSector(map, 1);
	writeSector(directory, 2);

	return;

}

void writeSector(char* buffer, int sector)
{
	int rs = (rem(sector,18)) + 1;
	int head = rem(div(sector,18),2);
	int track = div(sector,36);
	interrupt(0x13, 3*256+1, buffer, track*256+rs, head*256+0);
	return;
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
		char er[24];
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
		er[22] = '\r';
		er[23] = '\0';
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
	int j;
	int found;
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
			if(f[found] == '\0' || f[found] == 0x00)
			{
				i = i + (6 - found);
			}
			else
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

		if(c[i] == 0xA)
			interrupt(0x10, 0xE*256+'\r', 0, 0, 0);
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
			interrupt(0x10, 0xE*256+'\r', 0, 0, 0);
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
	switch(ax)
	{
		case 0: printString(bx); break;
		case 1: readString(bx); break;
		case 2: readSector(bx,cx); break;
		case 3: readFile(bx, cx); break;
		case 4: executeProgram(bx, cx); break;
		case 5: terminate(); break;
		case 6: writeSector(bx, cx); break;
		case 7: deleteFile(bx); break;
		case 8: writeFile(bx, cx, dx); break;
		default: break;
	}
}
