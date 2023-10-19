#include <stdio.h>

#include "nekosbest.h"

int main(void)
{
    puts("Hello, World!");
    puts(nbGetApiInfo()->pApiBaseUrl);
    return 0;
}