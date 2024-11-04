#include <stdio.h>
#include "structures.h"

void calculateFrequency(FILE* file, int frequency[]);
void printFrequencyTable(int frequency[]); // func for debugging purposes(func prints freq table)

int main(int argc, char* argv[]) {

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


