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
#include <string.h>

char* sndFileName;
char* soundName;
int* soundData;


int main(int argc, char *argv[]){
	if(argc != 3){
		printf("Error--wrong number of arguments. Expecting 2, received %d\n", argc);
		return -1;
	}

	sndFileName = argv[1];
	soundName = argv[2];

	int err = runConversion();
	if(err == -1){
		printf("Conversion Failed!\n");
		return -1;
	}
	else{
		printf("Successful!");
	}
	return 0;
}

//writes the file
int runConversion(){
	//create and open the output file
	FILE* ofp;
	char buffer[strlen(soundName) + 1 + 2];
	sprintf(buffer, "%s.c", soundName);
	ofp = fopen(buffer, "w+");

	//create and open the input file]
	SNDFILE *wavFile;
	SF_INFO info;
	unsigned short *buf;
	wavFile = sf_open(sndFileName, SFM_READ, &info);
	if(wavFile == NULL){
		printf("There was an error when opening the file %s!\n" , sndFileName);
		return -1;
	}
	int sampleRate = info.samplerate;
	int channels = info.channels;
	int samples = info.frames;
	int soundfmt = info.format;
	int num_items = samples * channels;


	//print out file stats:
	printf("SOUND FILE STATS\nFormat: %x\nSample Rate: %d\nSamples: %d\nChannels: %d\n",soundfmt, sampleRate, samples, channels);

	buf = (unsigned short*) malloc(num_items * sizeof(unsigned short));

	int numRead = sf_read_short(wavFile, buf, num_items);

	sf_close(wavFile);


	//----------------print array data----------------//
	fprintf(ofp, "int %s_soundData[] = {",soundName);
	//print array data
	int i;
	for(i = 0; i < numRead; i++){
		//print a line after every few ints
		if(i != 0 && i % INTS_PER_LINE == 0 ){
			fprintf(ofp, "\n\t");
		}
		fprintf(ofp, "%hu, ", (buf[i] >> 8) & 0x00FF);
	}
	//to print after the actual data
	fprintf(ofp, "};\n");

	//----------------printing number of samples----------------//
	fprintf(ofp,"int %s_numberOfSamples = %d;\n", soundName, numRead);
	//----------------printing sample rate----------------//
	fprintf(ofp,"int %s_sampleRate = %d;\n", soundName, sampleRate);



	fclose(ofp);
	//release everything that has been malloc'd
	free(buf);



	return 0;
}


