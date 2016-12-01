//============================================================================
// Name        : Lab10part2.cpp
// Author      : zshikles
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>


int main(int argc, char **argv){
	int fd;		// for the file descriptor of the special file we need to open.
	unsigned long *BasePtr;		// base pointer, for the beginning of the memory page (mmap)
	unsigned long *PBDR, *PBDDR;	// pointers for port B DR/DDR

    fd = open("/dev/mem", O_RDWR|O_SYNC);	// open the special file /dem/mem
	if(fd == -1){
		printf("\n error\n");
	    return(-1);  // failed open
	}

	// We need to map Address 0x80840000 (beginning of the page)
	BasePtr = (unsigned long*)mmap(NULL,4096,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0x80840000);
    if(BasePtr == MAP_FAILED){
    	printf("\n Unable to map memory space \n");
    	return(-2);
    }  // failed mmap

	// To access other registers in the page, we need to offset the base pointer to reach the
	// corresponding addresses. Those can be found in the board's manual.
	PBDR = BasePtr + 1;		// Address of port B DR is 0x80840004
    PBDDR = BasePtr + 5;	// Address of port B DDR is 0x80840014

    *PBDDR |= 0x80;			// configures port B7 as output (Green LED)
	*PBDDR &= 0xFFFFFFFE;	// configures port B0 as input (first push button). You could use: &= ~0x01

	// The program will turn on the green LED, sleep for a while, then off, sleep, then on again, then off.
	// You could use loops, if you wanted/needed.
   	
	//flash the RED light
	*PBDR |= 0x20;	//turns on the RED light
	usleep(100000);// makes the program wait for less than a second
    	*PBDR &= ~0x20;	//turns of the RED light
    	usleep(100000);// makes the program wait for less than a second
   
	//flash the YELLOW light
	*PBDR |= 0x40;//turns on the YELLOW light
	usleep(100000);// makes the program wait for less than a second
    	*PBDR &= ~0x40;//turns off the YELLOW light
    	usleep(100000);// makes the program wait for less than a second
   	
	//flash the GREEN light
	*PBDR |= 0x80;	//turns on the GREEN light
	usleep(100000);// makes the program wait for less than a second
   	 *PBDR &= ~0x80;//turns off the GREEN light
    	 usleep(100000);// makes the program wait for less than a second
	
	//flash the GREEN light
	*PBDR |= 0x80;	//turns on the GREEN light
	usleep(100000);// makes the program wait for less than a second
	*PBDR &= ~0x80;	//turns off the GREEN light
	usleep(100000);// makes the program wait for less than a second
	
	//flash the YELLOW light
	*PBDR |= 0x40;//turns on the YELLOW light
	usleep(100000);// makes the program wait for less than a second
	*PBDR &= ~0x40;//turns off the YELLOW light
	usleep(100000);// makes the program wait for less than a second
	
	//flash the RED light
	*PBDR |= 0x20;	//turns on the RED light
	usleep(100000);// makes the program wait for less than a second
	*PBDR &= ~0x20;	//turns off the RED light

	// If you wanted to read the status of Port B0, how could you do it?

    close(fd);	// close the special file
    return 0;
}
