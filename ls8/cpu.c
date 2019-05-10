#include "cpu.h"

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

void cpu_load(struct cpu *cpu, char *filename)
{

  FILE *fp;
  char line[1024];
  int address = 0;
  char *endpointer;
  fp = fopen(filename, "r");
  if (fp == NULL)
  {
    fprintf(stderr, "readfile: error opening file\n");
    exit(1);
  }

  while (fgets(line, 1024, fp) != NULL)
  {
    //printf("%s", line);

    unsigned char val = strtoul(line, &endpointer, 2);
    // FOUR FUCKING HOURS
    //printf("%u\n", val);
    if (endpointer == line)
    {
      printf("hey, this line is ... not an instruction. %s \n", endpointer);
      continue;
    }
    cpu->ram[address++] = val;
  }

  fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
  case ALU_MUL:
    cpu->reg[regA] = cpu->reg[regA] * cpu->reg[regB];
    break;
  case ALU_ADD:
    cpu->reg[regA] = cpu->reg[regA] + cpu->reg[regB];
    printf("Add worked! %d \n", cpu->reg[regA]);
    break;
  default:
    exit(1);
  }
}

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
    printf("PC is currently: %d\n", cpu->pc);
    // 2. Figure out how many operands this next instruction requires

    operandA = cpu_ram_read(cpu, (cpu->pc + 1)) & 0xff; // masking with and mask
    operandB = cpu_ram_read(cpu, (cpu->pc + 2)) & 0xff; // if pc > 0xff, we mask it

    // 4. switch() over it to decide on a course of action.
    switch (IR)
    {
    case CALL:
      //next_addr = pc + 2;
      // decrement the stack pointer by one
      cpu->reg[7]--;
      printf("yo, i'm still alive. 1 \n");
      // cpu_ram_write -> i need to write the current PC location into ram
      // where is the pc right now? cpu->pc i hate
      // cpu->reg[7] == 243? probably
      cpu_ram_write(cpu, cpu->reg[7], cpu->pc + add_to_pc);
      // call the process pointer? program counter
      cpu->pc = cpu->reg[operandA] - add_to_pc;
      printf("yo, i'm still alive. 2 \n");

      break;
    case RET:
      //pop the value from the top of the stack and store it in the PC
      cpu->pc = cpu_ram_read(cpu, cpu->reg[7]) - add_to_pc;
      printf("yo, i'm still alive. 3 \n");
      // this points to the address on the stack that i have to pop
      cpu->reg[7]++; // emulate the pop by interating upwards by 1
      break;
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
      //cpu->reg[operandA] = cpu->reg[operandA] * cpu->reg[operandB];
      alu(cpu, ALU_MUL, operandA, operandB);
      break;
    case ADD:
      printf("Case ADD successfully hit! \n");
      alu(cpu, ALU_ADD, operandA, operandB);
      break;
    case POP:
      //cpu->reg[operandA]
      cpu->reg[operandA] = cpu_ram_read(cpu, cpu->reg[7]);

      cpu->reg[7]++;
      //printf("popped value: %d\n", cpu->ram[operandA]);
      break;
    case PUSH:
      // decrement the sp by 1
      cpu->reg[7]--;
      cpu_ram_write(cpu, cpu->reg[7], cpu->reg[operandA]);
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
  // TODO: Initialize the PC and other special registers

  cpu->pc = 0;
  cpu->fl = 0;
  // clear RAM to 0
  memset(cpu->ram, 0, 256 * sizeof(cpu->ram[0])); //R0-R6 are cleared to 0.
  memset(cpu->reg, 0, sizeof(cpu->reg) - 1);
  // 7th register
  cpu->reg[7] = 0xF4;
}
