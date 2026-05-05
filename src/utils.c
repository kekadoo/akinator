#include "utils.h"
#include <stdlib.h>
#include <string.h>

char* my_strdup(const char* str) {
	int len = strlen(str) + 1;
	char* copy = (char*)malloc(len * sizeof(char));
	strcpy(copy, str);
	return copy;
}
