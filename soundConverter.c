/*
 * soundConverter.c
 *
 *  Created on: Oct 23, 2014
 *      Author: Parker Ridd & Tyler Bellows
 */
#include <stdio.h>
#include "soundConverter.h"

char* fileName;
char* soundName;

int main(int argc, char *argv[]){
	if(argc != 2){
		printf("Error--wrong number of arguments. Expecting 2, received %d", argc);
	}

	fileName = argv[0];
	soundName = argv[1];

}

//writes the file
int writeFile(){
	FILE* fp;
	char buffer[FILENAME_BUFFER_SIZE];
	sprintf(buffer, "%s.c", fileName);
	fp = fopen("w+");
}


