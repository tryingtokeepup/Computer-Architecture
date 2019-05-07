#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char *argv[])
{
  struct cpu cpu;

  cpu_init(&cpu);
  cpu_load(&cpu);
  cpu_run(&cpu);

  return 0;
}

// int main(int argc, char *argv[])
// {
//     FILE *fp;
//     char line[1024];

//     if (argc != 2)
//     {
//         fprintf(stderr, "usage: readfile filename\n");
//         return 1;
//     }

//     fp = fopen(argv[1], "r");

//     if (fp == NULL)
//     {
//         fprintf(stderr, "readfile: error opening file\n");
//         return 2;
//     }

//     while (fgets(line, 1024, fp) != NULL)
//     {
//         //printf("%s", line);

//         unsigned int val = strtoul(line, NULL, 10);

//         printf("%u\n", val);
//     }

//     fclose(fp);

//     return 0;
// }