/*
 * By: 	Alex Shankland and Aditya Hurry
 * Date:11/23/15
 * Project 6: Universal Machine
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "seq.h"
#include "uarray.h"
#include "segment.h"

/* 
 * Helper function: Takes in a segment manager and a segment to insert, grabs
 * 		    an id to associate with that segment, and puts it in the
 *		    table. 
 * Parameters: sm 	- Segment Manager struct
 *	       segment 	- Segment to insert
 * Return value: The id of the newly inserted segment
 */
unsigned insert_Segment(Segment_Manager sm, Segment segment)
{
	unsigned id;
	if (Seq_length(sm->ID_Stack) != 0) {
		id = (unsigned)(uintptr_t)Seq_remhi(sm->ID_Stack);
		Seq_put(sm->Segment_Table, id, segment);
	}
	else {
		Seq_addhi(sm->Segment_Table, segment);
		id = sm->ID_counter;
		sm->ID_counter++;
	}
	return id;
}

/* 
 * Effectively a constructor for the Segment_Manager struct
 * Return value: An initialized Segment_Manager
 */
Segment_Manager new_Manager()
{
	Seq_T Segment_Table = Seq_new(8);
	Seq_T ID_Stack = Seq_new(8);
	unsigned ID_counter = 0;
	Segment_Manager manager = (Segment_Manager)malloc(
				  sizeof(struct Segment_Manager));
	assert(manager);
	manager->Segment_Table = Segment_Table;
	manager->ID_Stack = ID_Stack;
	manager->ID_counter = ID_counter;
	return manager;
}

/*
 * Creates a new segment of length given as a parameter
 * Parameters: sm     - Segment manager struct
 * 	       length - length of segment to create
 */
unsigned map_Segment(Segment_Manager sm, int length)
{
	Segment segment = (Segment)UArray_new(length, sizeof(uint32_t));
	for (int i=0; i<length; i++)
	{
		*((uint32_t *)UArray_at(segment, i)) = 0;
	}
	unsigned id = insert_Segment(sm, segment);
	return id;
}

/*
 * Unmaps the segment at the specified ID, freeing the associated memory
 * Parameters: sm - Segment manager struct
 * 	       id - ID of segment to unmap 
 */
void unmap_Segment(Segment_Manager sm, unsigned id)
{
	Segment segment = Seq_put(sm->Segment_Table, id, NULL);
	UArray_free(&segment);
	Seq_addhi(sm->ID_Stack, (void*)(uintptr_t)id);
}

/*
 * Copies and inserts the segment at the given ID into the Segment_Table
 * Parameters: sm - Segment manager struct
 * 	       id - ID of segment to copy
 * Return value: The id of the newly inserted segment
 */
unsigned copy_Segment(Segment_Manager sm, unsigned id)
{
	Segment original = Seq_get(sm->Segment_Table, id);
	unsigned length = UArray_length(original);
	Segment copy = UArray_copy(original, length);
	return insert_Segment(sm, copy);
}

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
		 uint32_t value)
{
	uint32_t old_value;
	Segment segment = Seq_get(sm->Segment_Table, id);
	uint32_t *location = UArray_at(segment, offset);
	old_value = *location;
	*location = value;
	return old_value;
}

/*
 * Gets the value at segment[id][offset]
 * Parameters: sm 	- Segment manager struct
 * 	       id	- ID of segment to get from
 *	       offset	- Offset of segment[ID] to get from
 * Return Value: The value at segment[id][offset]
 */
uint32_t seg_Get(Segment_Manager sm, unsigned id, unsigned offset)
{
	Segment segment = Seq_get(sm->Segment_Table, id);
       	return *(uint32_t*)UArray_at(segment, offset);
}

/*
 * Frees all the memory associated with the Segment Manager
 * Parameter: sm - The Segment Manager Struct
 */
void segment_Free(Segment_Manager sm)
{
	Segment nextsegment;
	while (Seq_length(sm->Segment_Table) != 0) {
		nextsegment = (UArray_T)Seq_remhi(sm->Segment_Table);
		if (nextsegment != NULL)
			UArray_free(&nextsegment);
	};
	Seq_free(&(sm->Segment_Table));
	Seq_free(&(sm->ID_Stack));
	free(sm);
	return;
}

/*
 * Returns the pointer to the code, which must live in segment[0]
 */
void *get_Code(Segment_Manager sm)
{
	return Seq_get(sm->Segment_Table, 0);
}
