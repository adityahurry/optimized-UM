/*
 * By: 	Alex Shankland and Aditya Hurry
 * Date:11/23/15
 * Project 6: Universal Machine
 */

#ifndef REGISTERS_INCLUDED
#define REGISTERS_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "assert.h"
#include "seq.h"
#include "segment.h"

#define NUM_REGISTERS 8

typedef uint32_t Register;

/* 
 * Opcode 0: Move instruction
 * Function: If the value in Register C is not zero, move the value in Register
 *	     B into Register A
 */
static inline void mov(Register *A, Register B, Register C)
{
	if (C != 0) {
		*A = B;
	}
}

/* 
 * Opcode 1: Segment load instruction
 * Function: Return the value in segment[Register B] and offset[Register B]
 */
static inline uint32_t segload(Register B, 
			       Register C, 
			       Segment_Manager sm)
{
	return seg_Get(sm, B, C);
}

/* 
 * Opcode 2: Segment store instruction
 * Function: Stores the value in Register C into the segment[Register B],
 *	     offset[Register C]
 */
static inline void segstore(Register A, 
			    Register B, 
			    Register C, 
			    Segment_Manager sm)
{
	seg_Set(sm, A, B, C);
}

/* 
 * Opcode 3: Add instruction
 * Function: Returns the result of adding the value in Register B to the value
 * 	     in Register C
 */
static inline uint32_t add(Register B, Register C)
{
	return B + C;
}

/* 
 * Opcode 4: Multiply instruction
 * Function: Returns the result of multiplying the value in Register B to the
	     value in Register C
 */
static inline uint32_t multiply(Register B, Register C)
{
	return B * C;
}

/* 
 * Opcode 5: Divide instruction
 * Function: Returns the result of dividing the value in Register B by the
	     value in Register C
 */
static inline uint32_t divide(Register B, Register C)
{
	return B / C;
}

/* 
 * Opcode 6: Nand instruction
 * Function: Returns the result of nand'ing the value of Register B with
 * 	     the value in Register C
 */
static inline uint32_t  nand(Register B, Register C)
{
	return ~(B & C);
}

/* 
 * Opcode 7: Halt instruction
 * Function: There is no function as halt(), but it is an option for the user,
 * 	     thus its inclusion in this file as a comment
 */

/* 
 * Opcode 8: Segmap instruction
 * Function: A new segment of length equal to the value of Register C is
 *	     created and the id of the new segment is returned
 */
static inline uint32_t segmap(Register C, Segment_Manager sm)
{
	return map_Segment(sm, C);
}

/* 
 * Opcode 9: Segment unmap instruction
 * Function: The segment with the id specified in Register C is unmapped
 */
static inline void segunmap(Register C, Segment_Manager sm)
{
	unmap_Segment(sm, C);
}

/* 
 * Opcode 10: Output instruction
 * Function:  The value in Register C is outputted to stdout 
 */
static inline void output(Register C)
{
	assert(C < 256);
	printf("%c", (char)C);
}


/* 
 * Opcode 11: Input instruction
 * Function:  Returns one byte from stdin
 */
static inline uint32_t input()
{
	return (uint32_t)getchar();
}

/* 
 * Opcode 12: Input instruction
 * Function:  Loads the segment specified by Register B into Register 0
 * Note:      This function doesn't use Register C because that is the 
 * 	      the job of the processor function, the registers don't know
 *	      anything about the program counter.
 */
static inline void loadprog(Register B,  Segment_Manager sm)
{
	if (B == 0)
		return;
	unmap_Segment(sm, 0);
	copy_Segment(sm, B);
}

/* 
 * Opcode 13: Load value instruction
 * Function: There is no function as loadval(), but it is an option for the
 * 	     user, thus its inclusion in this file as a comment
 */

#endif
