#include "cpu.h"

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

void cpu_load(struct cpu *cpu, char *filename)
{
  // char data[DATA_LEN] = {
  //     // From print8.ls8
  //     0b10000010, // LDI R0,8
  //     0b00000000,
  //     0b00001000,
  //     0b01000111, // PRN R0
  //     0b00000000,
  //     0b00000001 // HLT
  // };

  // int address = 0;

  // for (int i = 0; i < DATA_LEN; i++)
  // {
  //   cpu->ram[address++] = data[i];
  // }
  FILE *fp;
  char line[1024];
  int address = 0;
  fp = fopen(filename, "r");
  if (fp == NULL)
  {
    fprintf(stderr, "readfile: error opening file\n");
    exit(1);
  }

  while (fgets(line, 1024, fp) != NULL)
  {
    //printf("%s", line);

    unsigned char val = strtoul(line, NULL, 2);

    printf("%u\n", val);
    cpu->ram[address++] = val;
  }

  fclose(fp);
}

/**
 * ALU
 */
// void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
// {
//   switch (op)
//   {
//   case ALU_MUL:
//     // TODO
//     break;

//     // TODO: implement more ALU ops
//   }
// }

/**
 * Run the CPU
 */
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

void trace(struct cpu *cpu)
{
  printf("%02X | ", cpu->pc);

  printf("%02X %02X %02X |",
         cpu_ram_read(cpu, cpu->pc),
         cpu_ram_read(cpu, cpu->pc + 1),
         cpu_ram_read(cpu, cpu->pc + 2));

  for (int i = 0; i < 8; i++)
  {
    printf(" %02X", cpu->reg[i]);
  }

  printf("\n");
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  // do i need to use pointers here? yikes.
  cpu->ram[address] = value;
}
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  unsigned char IR, operandA, operandB;

  while (running)
  {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    IR = cpu_ram_read(cpu, cpu->pc); // only works if cpu_ram_read is unsigned char, wonder why?
    int add_to_pc = (IR >> 6) + 1;

    // 2. Figure out how many operands this next instruction requires

    operandA = cpu_ram_read(cpu, (cpu->pc + 1)) & 0xff; // masking with and mask
    operandB = cpu_ram_read(cpu, (cpu->pc + 2)) & 0xff; // if pc > 0xff, we mask it

    // 4. switch() over it to decide on a course of action.
    switch (IR)
    {
    case LDI:
      cpu->reg[operandA] = operandB;
      //cpu->pc += 3;
      break;
    case PRN:
      printf("%d\n", cpu->reg[operandA]);
      // printf("8\n");
      //cpu->pc += 2;
      break;
    case HLT:
      printf("Quitting out. \n");
      running = 0;
      break;
    case MUL:
      cpu->reg[operandA] = cpu->reg[operandA] * cpu->reg[operandB];
      break;
    default:
      printf("Unknown instruction at %d: %d\n", cpu->pc, IR);
      exit(1);
    }
    // 6. Move the PC to the next instruction.
    cpu->pc += add_to_pc;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // make PC zero
  cpu->pc = 0;
  // TODO: Initialize the PC and other special registers
  memset(cpu->ram, 0, sizeof(cpu->ram));
  memset(cpu->reg, 0, sizeof(cpu->reg));
  // 7th register
}
