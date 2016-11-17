//============================================================================
// Name        : Lab10Morse.cpp
// Author      : zshikles
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <iostream>
#include <cstring>
#include <string>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

class MSG{
	protected:
		string mes;

	public:
		MSG();
		MSG(string);
		~MSG();
		virtual void printInfo();
};

MSG::~MSG(){
//no memory allocated so there is nothing to delete here
}

MSG::MSG(){ //default constructor

}

MSG::MSG(string x){
	mes=x;
}
void MSG::printInfo(){ //prints message
	cout << "Message: " << mes << endl;
}

class mcMSG : public MSG{
	public:
		string *tran_msg;
		int index;
		void printInfo();
		void translate();
		mcMSG();
		mcMSG(string);
		~mcMSG();
};

mcMSG::mcMSG(){ //default constructor
cout<<"Default mcMSG constructor called\nEnter a word:";
cin>>mes;
translate();
}

mcMSG::mcMSG(string y){ //constructs message
	mes=y;

	//sends the string to be translated to morse code
	translate();
}

mcMSG::~mcMSG(){
	//deconstructor to delete the allocated memory
	delete[] tran_msg;
}

void mcMSG::printInfo(){ //prints morse
	int fd;		// for the file descriptor of the special file we need to open.
	unsigned long *BasePtr;		// base pointer, for the beginning of the memory page (mmap)
	unsigned long *PBDR, *PBDDR;	// pointers for port B DR/DDR

	fd = open("/dev/mem", O_RDWR|O_SYNC);	// open the special file /dem/mem
	if(fd == -1){
		printf("\n error\n");
		//return();  // failed open
	}

	// We need to map Address 0x80840000 (beginning of the page)
	BasePtr = (unsigned long*)mmap(NULL,4096,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0x80840000);
	if(BasePtr == MAP_FAILED){
		printf("\n Unable to map memory space \n");
		//return();
	}  // failed mmap

	// To access other registers in the page, we need to offset the base pointer to reach the
	// corresponding addresses. Those can be found in the board's manual.
	PBDR = BasePtr + 1;		// Address of port B DR is 0x80840004
	PBDDR = BasePtr + 5;	// Address of port B DDR is 0x80840014

	*PBDDR |= 0x80;			// configures port B7 as output (Green LED)
	*PBDDR &= 0xFFFFFFFE;	// configures port B0 as input (first push button). You could use: &= ~0x01

	int i=0;
	int j=0;
/*
	if(index>26) //prevents seg fault
	{
		return;
	}
*/
	cout<<"Text: "<<mes<<endl;
	cout << "Morse: ";
	for(i=0; i<index; i++) //prints each letter in morse
	{
		cout << tran_msg[i] << " ";
		for(j=0; tran_msg[i][j]!='\0'; j++)
		{
			if(tran_msg[i][j]=='.'){
				*PBDR |= 0x20;	// ON: write a 1 to port B0. Mask all other bits.
				usleep(100000);// How can you sleep for less than a second?
				*PBDR &= ~0x20;	// OFF: write a 0 to port B0. Mask all other bits.
				usleep(100000);
			}
			else{
				*PBDR |= 0x40;
				usleep(100000);
				*PBDR &= ~0x40;
				usleep(100000);	
			}					
		}
		sleep(1);
	}
	*PBDR |= 0x80;	// ON: write a 1 to port B0. Mask all other bits.
	sleep(1);// How can you sleep for less than a second?
	*PBDR &= ~0x80;	// OFF: write a 0 to port B0. Mask all other bits.
	cout << endl;
}

void mcMSG::translate(){
	string letters="abcdefghijklmnopqrstuvwxyz"; //defining alphabet and morse code
	string morseCode[] = {".-","-...","-.-.","-..",".","..-.","--.","....","..",".---","-.-",".-..","--","-.","---",".--.","--.-",".-.","...","-","..-","...-",".--","-..-","-.--","--.."};
	int i=0;
	int j=0;
	string c;
	char u;

	index = mes.length(); //gets length of string
	tran_msg=new string[index];

	for(i=0; i<index; i++)
	{
		if(isupper(mes[i])) //changes uppercase letters to lowercase
		{
			u=mes[i];
			mes[i]=tolower(u);
		}

		for(j=0; j<26; j++){ //translates message into morse
			if(mes[i]==letters[j])
			{
				c=morseCode[j];
				tran_msg[i]=c;
			}
		}
	}
}



int main(void){
	string str;
	cout << "Enter a word to convert to morse code: "; //takes in word to convert
	cin >> str;
	mcMSG m1(str);
	m1.printInfo();

	return 0;
}
