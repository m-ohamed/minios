#include <stdio.h>

char* split_string(char*);

void exec(char*, int);

int comparestr(char*, char*);

char buffer[13312];

int main()
{
  char input[20];

  while(1)
  {
    interrupt(0x21, 0, "SHELL>\0", 0, 0);
    interrupt(0x21, 1, input, 0, 0);

    if(comparestr(split_string(input), "view\0") == 1)
    {
      exec(input, 0);
    }
    else
    {
      if(comparestr(split_string(input), "execute\0") == 1)
      {
        exec(input, 1);
      }
      else
      {
        interrupt(0x21, 0, "Bad command!\n\b\b\b\b\b\b\b\b\b\b\b\b\0", 0, 0);
      }
    }
  }
}

char* split_string(char* input)
{
  int i;
  char firstoutput[15];

  for(i = 0; input[i] != '\0'; i++)
  {
    if(input[i] == ' ')
    {
      return firstoutput;
    }

    firstoutput[i] = input[i];
  }
}

void exec(char* input, int op)
{
  int c;
  int i;
  int counter;
  char secoutput[6];
  counter = 0;

  for(c = 0; c < 13312; c++)
  {
    buffer[c] = 0x00;
  }

  if(op == 0)
  {
    for(i = 5; counter < 6; i++)
    {
      secoutput[i-5] = input[i];
      counter++;
    }

    for(counter; counter < 6; counter++)
    {
      secoutput[counter] = 0x00;
    }

    interrupt(0x21, 3, secoutput, buffer, 0);

    if(buffer[0] != 0x00)
    {
      interrupt(0x21, 0, buffer, 0, 0);
    }
    else
    {
      interrupt(0x21, 0, "File not found!\n\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\0", 0, 0);
    }
  }
  else
  {
    for(i = 8; counter < 6; i++)
    {
      secoutput[i-8] = input[i];
      counter++;
    }

    for(counter; counter < 6; counter++)
    {
      secoutput[counter] = 0x00;
    }

    interrupt(0x21, 4, secoutput, 0x2000, 0);
  }
}

int comparestr(char* first, char* sec)
{
  int i;
  int flag = 1;

  for(i = 0; sec[i] != '\0' && flag == 1; i++)
  {
    if(first[i] != sec[i])
    {
      flag = 0;
    }
  }

  return flag;
}
