#ifndef _CPU_H_
#define _CPU_H_
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define DATA_LEN 6
// Holds all information about the CPU
struct cpu
{
  // TODO
  unsigned int pc;
  unsigned int fl;
  unsigned char reg[8];
  unsigned char ram[256];
};

// ALU operations
enum alu_op
{
  ALU_MUL,
  ALU_ADD,
  ALU_CMP,
  ALU_AND,
  ALU_OR,
  ALU_XOR,
  ALU_NOT,
  ALU_SHL,
  ALU_SHR,
  ALU_MOD
  // Add more here
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.
#define LDI 0b10000010
#define HLT 0b00000001
#define PRN 0b01000111
#define MUL 0b10100010
#define POP 0b01000110
#define PUSH 0b01000101
#define CALL 0b01010000
#define RET 0b00010001
#define ADD 0b10100000
#define CMP 0b10100111
#define JMP 0b01010010
#define JEQ 0b01010101
#define JNE 0b01010110
//Add the ALU operations: `AND` `OR` `XOR` `NOT` `SHL` `SHR` `MOD`
#define AND 0b10101000
#define OR 0b10101010
#define XOR 0b10101011
#define NOT 0b01101001
#define SHL 0b10101100
#define SHR 0b10101101
#define MOD 0b10100100

// Function declarations

extern void cpu_load(struct cpu *cpu, char *filename);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
