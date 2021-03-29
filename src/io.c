#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include "./headers/io.h"

extern int errno;

char *readLine()
{
    errno = 0;

    char *result = NULL;
    
    // If the function wasn't called often, the static keyword should not be used.
    static size_t size = sizeof(char) * STRING_INPUT_DEFAULT_SIZE;
    
    const ssize_t lineSize = getline(&result, &size, stdin);

    if(lineSize < 0 || errno != 0) 
    {
    	errno = -1;

        free(result);
    	result = NULL;
    }
    else if(result[lineSize - 1] == '\n')
    {
        result[lineSize - 1] = '\0';
        
    	result = realloc (result, sizeof(char) * lineSize);
    }

    return result;
}

int readInteger()
{
    errno = 0;

    char *line = readLine();

    size_t lineSize = strlen(line);

    int result = 0;

    if(line != NULL && errno == 0)
    {
        char current = '\0';
        for(int i = 0; i < lineSize; i++)
        {
            current = line[i];

            if(!((current == '-' && i == 0) || (current <= '9' && current >= '0')))
            {
                errno = 1;
                break;
            }
        }

        if(errno == 0)
        {
           result = (int) strtol(line, NULL, 10); 
        }
    }

    free(line);
    return result;
}

double readDouble()
{
    errno = 0;

    char *line = readLine();

    size_t lineSize = strlen(line);

    double result = 0.0;

    if(line != NULL && errno == 0)
    {
        bool hasFractionalPart = false;

        char current = '\0';
        for(int i = 0; i < lineSize; i++)
        {
            current = line[i];

            if(current == NUMBER_FRACTIONAL_SEPARATOR)
            {
                if(hasFractionalPart)
                {
                    errno = 2;
                    break;
                }

                hasFractionalPart = true;

                if(NUMBER_FRACTIONAL_SEPARATOR != '.')
                    line[i] = '.';
            }
            else if(!((current == '-' && i == 0) || (current <= '9' && current >= '0')))
            {
                errno = 1;
                break;
            }
        }

        if(errno == 0)
        {
            if(!hasFractionalPart)
                result = (double) strtol(line, NULL, 10);
            else
                result = (double) strtod(line, NULL); 
        }
    }

    free(line);
    return result;
}