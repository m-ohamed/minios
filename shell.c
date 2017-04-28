#include <stdio.h>

char* split_string(char*);

void exec(char*, int);

int comparestr(char*, char*);

int rem(int, int);

int div(int, int);

int getNum(char*, int);

char buffer[13312];

int main()
{
  char input[30];

  while(1)
  {
    interrupt(0x21, 0, "\r", 0, 0);
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
        if(comparestr(split_string(input), "delete\0") == 1)
        {
          exec(input, 2);
        }
        else
        {
          if(comparestr(split_string(input), "copy\0") == 1)
          {
            exec(input, 3);
          }
          else
          {
            if(comparestr(split_string(input), "dir\0") == 1)
            {
              exec(input, 4);
            }
            else
            {
              if(comparestr(split_string(input), "create\0") == 1)
              {
                exec(input, 5);
              }
              else
              {
                if(comparestr(split_string(input), "kill\0") == 1)
                {
                  exec(input, 6);
                }
                else
                {
                  interrupt(0x21, 0, "Bad command!\n\r\0", 0, 0);
                }
              }
            }
          }
        }
      }
    }

    interrupt(0x21, 0, "\n\r\0", 0, 0);

  }
}

void exec(char* input, int op)
{
  int c;
  int i;
  int j;
  int counter;
  char secoutput[6];
  char thirdfile[7];
  char directory[512];
  char line[100];

  counter = 0;
  i = 0;
  j = 0;

  for(c = 0; c < 13312; c++)
  {
    buffer[c] = 0x00;
  }

  switch(op)
  {
    case 0:
      for(i = 5; counter < 6 && input[i] != '\0'; i++)
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
        interrupt(0x21, 0, "File not found!\n\r\0", 0, 0);
      }

      break;

    case 1:
      for(i = 8; counter < 6 && input[i] != '\0'; i++)
      {
        secoutput[i-8] = input[i];
        counter++;
      }

      for(counter; counter < 6; counter++)
      {
        secoutput[counter] = 0x00;
      }

      interrupt(0x21, 4, secoutput, 0x2000, 0);

      break;

    case 2:
      for(i = 7; counter < 6 && input[i] != '\0'; i++)
      {
        secoutput[i-7] = input[i];
        counter++;
      }

      for(counter; counter < 6; counter++)
      {
        secoutput[counter] = 0x00;
      }

      interrupt(0x21, 7, secoutput, 0, 0);

      break;

    case 3:
      for(i = 5; input[i] != ' '; i++)
      {
        if(counter < 6)
        {
          secoutput[i-5] = input[i];
          counter++;
        }
      }

      i = i + 1;
      j = 0;

      for(i; input[i] != '\0'; i++)
      {
        if(j < 6)
        {
          thirdfile[j] = input[i];
          j++;
        }
      }


      for(counter; counter < 6; counter++)
      {
        secoutput[counter] = 0x00;
      }

      for(j; j < 6; j++)
      {
        thirdfile[j] = 0x00;
      }

      interrupt(0x21, 3, secoutput, buffer, 0);

      i = 0;
      j = 0;

      for(i; i < 26; i++)
      {
        for(j = 0; j < 512; j++)
        {
          if(buffer[(i*512) + j] != 0x00)
            break;
        }
        if(j == 512)
          break;
      }

      if(buffer[0] != 0x00)
      {
        interrupt(0x21, 8, thirdfile, buffer, i);
        interrupt(0x21, 0, "File copied successfully!\n\r\0", 0, 0);
      }
      else
      {
        interrupt(0x21, 0, "File not found!\n\r\0", 0, 0);
      }

      break;

    case 4:

      interrupt(0x21, 2, directory, 2, 0);

      i = 0;
      j = 0;

      for(i = 0; i < 512; i = i + 32)
      {
        while(directory[i] == 0x00)
          i += 32;

        for(j = 0; j < 6; j++)
          thirdfile[j] = 0x00;

        for(j = 0; j < 6; j++)
        {
          if(directory[i+j] == 0x00)
          {
            break;
          }

          thirdfile[j] = directory[i+j];
        }

        thirdfile[6] = 0x00;

        interrupt(0x21, 0, "name: ", 0, 0);
        interrupt(0x21, 0, thirdfile, 0, 0);
        interrupt(0x21, 0, "\n\r\0", 0, 0);
        interrupt(0x21, 0, "size: ", 0, 0);

        counter = 0;

        for(j = 6; j < 32; j++)
        {
          if(directory[i+j] == 0x00)
            break;

          counter++;
        }

        line[0] = div(counter, 10) + 48;
        line[1] = rem(counter, 10) + 48;
        line[2] = '\0';

        interrupt(0x21, 0, line, 0, 0);
        interrupt(0x21, 0, "\n\n\r\0", 0, 0);

      }

      break;

    case 5:
      for(i = 7; counter < 6 && input[i] != '\0'; i++)
      {
        secoutput[i-7] = input[i];
        counter++;
      }

      for(counter; counter < 6; counter++)
      {
        secoutput[counter] = 0x00;
      }

      i = 0;

      while(1)
      {
        for(j = 0; j < 100; j++)
          line[j] = 0x00;

        interrupt(0x21, 0, ":>\0", 0, 0);

        interrupt(0x21, 1, line, 0, 0);


        if(line[0] == 0xA)
          break;

        for(j = 0; line[j] != 0x00; j++)
          buffer[j+i] = line[j];

        i = i + j;

      }

      j = 0;

      j = div(i,512);

      if(rem(i,512) != 0)
        j += 1;

      interrupt(0x21, 8, secoutput, buffer, j);
      interrupt(0x21, 0, "File write successful!\n\r\0", 0, 0);

      break;

    case 6:
      for(i = 5; counter < 6 && input[i] != '\0'; i++)
      {
        secoutput[i-5] = input[i];
        counter++;
      }

      switch(secoutput[0])
      {
        case '0': c = 0; break;
        case '1': c = 1; break;
        case '2': c = 2; break;
        case '3': c = 3; break;
        case '4': c = 4; break;
        case '5': c = 5; break;
        case '6': c = 6; break;
        case '7': c = 7; break;
        case '8': c = 8; break;
        case '9': c = 9; break;
        default: break;
      }

      // secoutput[0] = c;
      // interrupt(0x21, 0, c + " ", 0, 0);
      // c = getNum(secoutput, counter);

      interrupt(0x21, 9, c, 0, 0);


      break;

    default: break;
  }

}

int getNum(char* num, int size)
{
  int i;
  int j;
  int sum;
  int fin;

  switch(size)
  {
    // case '0': i = 0; break;
    case '1': fin = 1; break;
    case '2': fin = 10; break;
    case '3': fin = 100; break;
    case '4': fin = 1000; break;
    case '5': fin = 10000; break;
    default: break;
  }

  for(i = 0; i < size; i++)
  {
    switch(num[i])
    {
      case '0': j = 0; break;
      case '1': j = 1; break;
      case '2': j = 2; break;
      case '3': j = 3; break;
      case '4': j = 4; break;
      case '5': j = 5; break;
      case '6': j = 6; break;
      case '7': j = 7; break;
      case '8': j = 8; break;
      case '9': j = 9; break;
      default: break;
    }

    sum += (j*fin);

    fin = div(fin, 10);
  }

  return sum;
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

  return firstoutput;
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
