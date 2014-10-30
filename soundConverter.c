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
#define FORMAT_HEX 0x20746D66
#define DATA_HEX 0x61746164
#define DB_ON 1

char* sndFileName;
char* soundName;
int* soundData;


int main(int argc, char *argv[]){
	if(argc != 3){
		printf("Error--wrong number of arguments. Expecting 3, received %d\n", argc);
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
	FILE *wavFile;

	wavFile = fopen(sndFileName, "r");
	if(wavFile == NULL){
		printf("There was an error when opening the file %s!\n" , sndFileName);
		return -1;
	}
	unsigned int intBuf;
	unsigned short shortBuf;

	int numCt = 0;
	int foundFormat = 0;
	while(!foundFormat){
		fread(&intBuf, sizeof(int), 1, wavFile);
		if(intBuf == FORMAT_HEX){
			foundFormat = 1;
			numCt++;
			printf("Found format tag at int count %d!\n", numCt);
		}
	}

	fread(&intBuf, sizeof(int), 1, wavFile);
	int subchunkSize = intBuf;
	fread(&shortBuf, sizeof(short), 1, wavFile);
	unsigned short audioFormat = shortBuf;
	fread(&shortBuf, sizeof(short), 1, wavFile);
	unsigned short numChannels = shortBuf;
	if(numChannels > 1){
		printf("Unsupported number of channels : %hu!\n", numChannels );
		return -1;
	}
	else if(DB_ON){
		printf("Number of channels: %hu\n", numChannels);
	}

	fread(&intBuf, sizeof(int), 1, wavFile);
	int sampleRate = intBuf;
	printf("Sample Rate: %d\n", sampleRate);
	fread(&intBuf, sizeof(int), 1, wavFile);
	int byteRate = intBuf;
	fread(&shortBuf, sizeof(short), 1, wavFile);
	int blockAlign = shortBuf;
	fread(&shortBuf, sizeof(short), 1, wavFile);
	int bitsPerSample = shortBuf;

	int foundData = 0;
	numCt = 0;
	while(!foundData){
		fread(&intBuf, sizeof(int), 1, wavFile);
		if(intBuf == DATA_HEX){
			foundData = 1;
			numCt++;
			printf("Found data tag at int count %d!\n", numCt);
		}
	}

	fread(&intBuf, sizeof(int), 1, wavFile);
	int subchunk2Size = intBuf;
	int numSamples = (subchunk2Size * 8) / (numChannels * bitsPerSample);
	printf("Number of samples: %d\n", numSamples);

	unsigned char charBuf;
	fprintf(ofp, "int %s_soundData[] = {",soundName);
	//print array data
	int i;
	for(i = 0; i < numSamples; i++){
		//print a line after every few ints
		if(i != 0 && i % INTS_PER_LINE == 0 ){
			fprintf(ofp, "\n\t");
		}
		fread(&charBuf, sizeof(char), 1, wavFile);
		fprintf(ofp, "%hu, ", charBuf);
	}
	//to print after the actual data
	fprintf(ofp, "};\n");

	//----------------printing number of samples----------------//
	fprintf(ofp,"int %s_numberOfSamples = %d;\n", soundName, numSamples);
	//----------------printing sample rate----------------//
	fprintf(ofp,"int %s_sampleRate = %d;\n", soundName, sampleRate);



	fclose(ofp);
	//release everything that has been malloc'd
	free(wavFile);


	return 0;
}


