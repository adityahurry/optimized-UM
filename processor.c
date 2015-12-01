/*
 * By: 	Alex Shankland and Aditya Hurry
 * Date:11/23/15
 * Project 6: Universal Machine
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/stat.h>
#include "bitpack.h"
#include "registers.h"
#include "segment.h"

/* Possible return results for execute_instructions() */
enum instruction_results {ISNEXT = 'c', ISHALT = 'h', ISLOADPROG = 'l'};

/* General magic number values used by the processor */
#define EMPTY_REGISTERS {0, 0, 0, 0, 0, 0, 0, 0}
#define OPCODE_LSB 28
#define OPCODE_WIDTH 4
#define REG_WIDTH 3
#define RA_LSB 6
#define RB_LSB 3
#define RC_LSB 0
#define BYTE_SIZE 8
#define NUM_BITS 32

/* Definitions of opcodes */
#define MOV 0
#define SEGLOAD 1
#define SEGSTORE 2
#define ADD 3
#define MULTIPLY 4
#define DIVIDE 5
#define NAND 6
#define HALT 7
#define SEGMAP 8
#define SEGUNMAP 9
#define OUTPUT 10
#define INPUT 11
#define LOADPROG 12
#define LOADVAL 13

/*
 * Executes one instruction at a time
 * Parameters: instruction - a 32 bit instruction
 *	       registers   - a pointer to the array of 8 registers
 * 	       sm	   - the segment manager struct
 */
char execute_instruction(uint32_t instruction, uint32_t *registers,
			 Segment_Manager sm);

/*
 * Reads the file provided on the command line and puts it into Segment zero
 * Parameters: fp 	    - the file pointer
 * 	       instructions - the UArray_T that represents Segment zero
 */
void read_file(FILE *fp, UArray_T instructions);

/*
 * Runs the program on the virtual machine
 * Parameter: path - the name of the file provided on the command line
 */
void run_program(char* path)
{
	UArray_T instructions;
       	int num_instructions;
	FILE *code;
	int program_counter;
	char result;
	uint32_t instruction;
	struct stat buf;
	int load_reg;
	Segment_Manager manager = new_Manager();
	uint32_t registers[NUM_REGISTERS] = EMPTY_REGISTERS;
	size_t size = sizeof(uint32_t);
	code = fopen(path, "r");
	if(code == NULL){
		fprintf(stderr, "INVALID FILE NAME\n");
		segment_Free(manager);
		exit(EXIT_FAILURE);
	}
	stat(path, &buf);
	num_instructions = buf.st_size/size;
	map_Segment(manager, num_instructions);
	instructions = get_Code(manager);
 	
	read_file(code, instructions);
	fclose(code);

	program_counter = 0;
	result = ISNEXT;
	while(result != ISHALT){
		instruction = *((uint32_t *)UArray_at(instructions,
						      program_counter));
		result = execute_instruction(instruction, registers, manager);
		switch(result){
		case ISNEXT:
			program_counter++;
			break;
		case ISLOADPROG:
			load_reg = Bitpack_getu(instruction, REG_WIDTH, 0);
			program_counter = registers[load_reg];
			instructions = get_Code(manager);
			break;
		default: break;		   
		}
	}
	segment_Free(manager);
}

char execute_instruction(uint32_t instruction, uint32_t *registers,
			 Segment_Manager sm)
{
	int opcode;
	int ra;
	int rb;
	int rc;
	int value;
	opcode = (int)Bitpack_getu((uint64_t)instruction, OPCODE_WIDTH,
				   OPCODE_LSB);
	if(opcode == LOADVAL){
		ra = (int)Bitpack_getu((uint64_t)instruction, 3, 25);
		value = (int)Bitpack_getu((uint64_t)instruction, 25, 0);
		registers[ra] = value;
		return ISNEXT;
	}
	ra = (int)Bitpack_getu((uint64_t)instruction, REG_WIDTH, RA_LSB);
	rb = (int)Bitpack_getu((uint64_t)instruction, REG_WIDTH, RB_LSB);
	rc = (int)Bitpack_getu((uint64_t)instruction, REG_WIDTH, RC_LSB);
	switch(opcode){
		case MOV:
			mov(registers+ra, registers[rb], registers[rc]);
			return ISNEXT;
		case SEGLOAD: 
			registers[ra] = segload(registers[rb], registers[rc],
						sm);
			return ISNEXT;
		case SEGSTORE:
			 segstore(registers[ra], registers[rb], registers[rc],
				  sm);
			return ISNEXT;
		case ADD:
			registers[ra] = add(registers[rb], registers[rc]);
			return ISNEXT;
		case MULTIPLY:
			registers[ra] = multiply(registers[rb], registers[rc]);
			return ISNEXT;
		case DIVIDE:
			registers[ra] = divide(registers[rb], registers[rc]);
			return ISNEXT;
		case NAND:
			registers[ra] = nand(registers[rb], registers[rc]);
			return ISNEXT;
		case HALT:
			return ISHALT;
		case SEGMAP:
			registers[rb] = segmap(registers[rc], sm);
			return ISNEXT;
		case SEGUNMAP:
			segunmap(registers[rc], sm);
			return ISNEXT;
		case OUTPUT:
			output(registers[rc]);
			return ISNEXT;
		case INPUT:
			registers[rc] = input();
			return ISNEXT;
		case LOADPROG:
			loadprog(registers[rb], sm);
			return ISLOADPROG;
		default:return ISHALT;
	}
}


void read_file(FILE *fp, UArray_T instructions)
{
	uint32_t word = 0;
	unsigned int c;
	int lsb = NUM_BITS - BYTE_SIZE;
	int count = 0;
	while(true){
		c = getc(fp);
		if((signed)c == EOF)
			break;
		word = (uint32_t)Bitpack_newu((uint32_t)word, 
					      BYTE_SIZE, lsb, c);
		lsb -= BYTE_SIZE;
		if(lsb < 0){
			uint32_t *inst = UArray_at(instructions, count);
			*inst = word;
			lsb = NUM_BITS-BYTE_SIZE;
			word = 0;
			count++;
		}
	}
}
