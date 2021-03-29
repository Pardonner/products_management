#include <stdlib.h>
#include <errno.h>

#include <string.h>

extern int errno;

void freePointer(void **pointer)
{
	free(*pointer);
	*pointer = NULL;
}

void resizeStringPointer(char **stringPointer)
{
	errno = 0;
	*stringPointer = realloc(*stringPointer, sizeof(char) * (strlen(*stringPointer) + 1));

	if(*stringPointer == NULL)
		errno = -2;
}

char *createStringPointer(int size)
{
	errno = 0;
	
	char *stringPointer = malloc(size);

	if(stringPointer == NULL)
	{
		errno = -2;

		freePointer(&stringPointer);
	}

	return stringPointer;
}