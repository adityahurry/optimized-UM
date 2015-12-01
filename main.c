/* 
 * By: Alex Shankland and Aditya Hurry
 * Comp40 Project: UM
 * 11/15/2015
 */

#include <stdlib.h>
#include <assert.h>
#include "processor.h"

int main(int argc, char *argv[])
{
	assert(argc == 2);
	run_program((char*)argv[1]);
	return 0;
}
