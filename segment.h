/* 
 * By: Alex Shankland and Aditya Hurry
 * Comp40 Project: UM
 * 11/15/2015
 */

#ifndef SEGMENT_INCLUDED
#define SEGMENT_INCLUDED

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "seq.h"
#include "uarray.h"

/*
 * A struct to manage the segment mapping
 * The segments are mapped using a Hanson Sequence
 * IDs are managed using a Hanson Sequence as a stack
 */
typedef struct Segment_Manager {
	Seq_T Segment_Table;
	Seq_T ID_Stack;
	unsigned ID_counter;
} *Segment_Manager;

typedef UArray_T Segment;

/* 
 * Effectively a constructor for the Segment_Manager struct
 * Return value: An initialized Segment_Manager
 */
Segment_Manager new_Manager();

/*
 * Creates a new segment of length given as a parameter
 * Parameters: sm     - Segment manager struct
 * 	       length - length of segment to create
 */
unsigned map_Segment(Segment_Manager sm, int length);

/*
 * Unmaps the segment at the specified ID, freeing the associated memory
 * Parameters: sm - Segment manager struct
 * 	       id - ID of segment to unmap 
 */
void unmap_Segment(Segment_Manager sm, unsigned id);

/*
 * Copies and inserts the segment at the given ID into the Segment_Table
 * Parameters: sm - Segment manager struct
 * 	       id - ID of segment to copy
 * Return value: The id of the newly inserted segment
 */
unsigned copy_Segment(Segment_Manager sm, unsigned id);

/*
 * Sets the value of segment[id][offset] to the given value
 * Parameters: sm 	- Segment manager struct
 * 	       id 	- ID of segment to set
 *	       offset	- Offset within the segment
 *	       value 	- Value to set
 * Return value: the old value of the specified segment[id][offset]
 */
uint32_t seg_Set(Segment_Manager sm, 
		 unsigned id, 
		 unsigned offset,
		 uint32_t value);

/*
 * Gets the value at segment[id][offset]
 * Parameters: sm 	- Segment manager struct
 * 	       id	- ID of segment to get from
 *	       offset	- Offset of segment[ID] to get from
 * Return Value: The value at segment[id][offset]
 */
uint32_t seg_Get(Segment_Manager sm, unsigned id, unsigned offset);

/*
 * Frees all the memory associated with the Segment Manager
 * Parameter: sm - The Segment Manager Struct
 */
void segment_Free(Segment_Manager sm);

/*
 * Returns the pointer to the code, which must live in segment[0]
 */
void *get_Code(Segment_Manager sm);

#endif
