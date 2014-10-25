/*
 * soundConverter.c
 *
 *  Created on: Oct 23, 2014
 *      Author: Parker Ridd & Tyler Bellows
 */
#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>
#include "soundConverter.h"
#include <inttypes.h>

char* sndFileName;
char* soundName;
int* soundData;


int main(int argc, char *argv[]){
	if(argc != 2){
		printf("Error--wrong number of arguments. Expecting 2, received %d", argc);
	}

	sndFileName = argv[0];
	soundName = argv[1];
	return 0;
}

int getData(){
	return 0;

}

//writes the file
int writeFile(){
	FILE* fp;
	char buffer[FILENAME_BUFFER_SIZE];
	sprintf(buffer, "%s.c", sndFileName);
	fp = fopen(buffer, "w+");
	//before
	fprintf(fp, "int %s_soundData[] = {",soundName);



	return 0;
}


