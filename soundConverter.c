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

	runConversion();
	return 0;
}

//writes the file
int runConversion(){
	//create and open the output file
	FILE* ofp;
	char buffer[FILENAME_BUFFER_SIZE];
	sprintf(buffer, "%s.c", sndFileName);
	ofp = fopen(buffer, "w+");

	//create and open the input file]
	SNDFILE *wavFile;
	SF_INFO info;
	int *buf;
	wavFile = sf_open(sndFileName, SFM_READ, &info);
	int sampleRate = info.samplerate;
	int channels = info.channels;
	int samples = info.frames;
	int num_items = samples * channels;
	buf = (int*) malloc(num_items * sizeof(int));

	int numRead = sf_read_int(wavFile, buf, num_items);

	sf_close(wavFile);


	//to print before the actual data
	fprintf(ofp, "int %s_soundData[] = {",soundName);
	//print array data
	int i;
	for(i = 0; i < numRead; i++){
		//print a line after every few ints
		if(i != 0 && i % INTS_PER_LINE == 0 ){
			fprintf(ofp, "\n");
		}
		fprintf(ofp, "%d, ", buf[i]);
	}
	//to print after the actual data
	fprintf(ofp, "};\n");

	//----------------printing number of samples----------------//
	fprintf(ofp,"int %s_numberOfSamples = %d\n", soundName, numRead);
	//----------------printing sample rate----------------//
	fprintf(ofp,"int %s_sampleRate = %d\n", soundName, sampleRate);


	fclose(ofp);
	//release everything that has been malloc'd
	free(buf);



	return 0;
}


