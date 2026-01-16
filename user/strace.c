#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "user.h"

int main(int argc, char *argv[])
{
    char command[256];
    strcpy(command, argv[2]);

    int mask_var = atoi(argv[1]);

    trace(mask_var);
    exec(command, &argv[2]);

    exit(0);
}