#include <stdio.h>
#include "structures.h"

void calculateFrequency(FILE* file, int frequency[]);
void printFrequencyTable(int frequency[]); // func for debugging purposes(func prints freq table)

int main(int argc, char* argv[]) {
	if(argc < 3) {
		fprintf(stderr, "Usage: <input file> <output file>\n");
		return 1;
	}
	
	FILE *inputFile = fopen(argv[1], "r");
	if(inputFile == NULL) {
		perror("Error opening file");
		return 1;
	}

	int frequency[CHAR_COUNT] = {0};
	calculateFrequency(inputFile, frequency);
	fclose(inputFile);

	printFrequencyTable(frequency);

	return 0;
}

void calculateFrequency(FILE* file, int frequency[]) {
	char ch;
	while((ch = fgetc(file)) != EOF ) {
		frequency[ (unsigned char) ch ]++;
	}

}

void printFrequencyTable(int frequency[]) {
	for(int i = 0; i < CHAR_COUNT; ++i){
		if (frequency[i] > 0)
			printf("Character '%c' (ASCII %d) has frequency %d\n", i, i, frequency[i]);
	}
}


