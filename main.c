#include <stdio.h>
#include "HuffmanCode.h"

void calculateFrequency(FILE* file, int frequency[]);
void printFrequencyTable(int frequency[]); // func for debugging purposes(func prints freq table)
void freeTree(HuffmanNode* root);

void printCodes(char codes[CHAR_COUNT][CHAR_COUNT], int frequency[]);
void encodeFile(FILE *inFile, FILE *outFile, char codes[CHAR_COUNT][CHAR_COUNT]);

int main(int argc, char* argv[]) {
	if(argc < 3) {
		fprintf(stderr, "Usage: ./%s  <input file> <output file>\n", argv[0]);
		return 1;
	}
	
	FILE *inputFile = fopen(argv[1], "r");
	if(inputFile == NULL) {
		perror("Error opening file");
		return 1;
	}

	int frequency[CHAR_COUNT] = {0};
	calculateFrequency(inputFile, frequency);
	rewind(inputFile);
	//fclose(inputFile);

	printFrequencyTable(frequency);
	
	HuffmanNode* root = buildHuffmanTree(frequency);

 	char codes[256][256] = {{0}};
    	char code[256];
    	generateCodes(root, code, 0, codes);

	printCodes(codes, frequency);


    	// Open output file and encode input text
    	FILE *outputFile = fopen(argv[2], "wb");
    	if (!outputFile) {
        	perror("Error opening output file");
        	fclose(inputFile);
        	freeTree(root);
        	return 1;
    	}

    	encodeFile(inputFile, outputFile, codes);

    	printf("File successfully encoded to %s\n", argv[2]);	

	/*MinHeap* minHeap = createMinHeap(CHAR_COUNT);

	for(int i = 0; i < CHAR_COUNT; ++i) {
		if(frequency[i] > 0)
			insertMinHeap(minHeap, createNode((char)i, frequency[i]));
	}

	//testing heap functionality
	printf("\n\nExtracting nodes:\n");
	while(minHeap->size > 0) {
		HuffmanNode* minNode = extractMin(minHeap);
		printf("Character '%c' with frequency %d\n", minNode->character, minNode->frequency);
		free(minNode);
	}

	free(minHeap->array);
	free(minHeap);*/

	fclose(inputFile);
	fclose(outputFile);
    	freeTree(root);	

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
		if(frequency[i] > 0)
			printf("Character '%c' (ASCII %d) has frequency %d\n", i, i, frequency[i]);
	}
}

// Print Huffman Codes (optional, for verification)
void printCodes(char codes[256][256], int frequency[]) {
    printf("Character   Frequency   Huffman Code\n");
    for (int i = 0; i < 256; i++) {
        if (frequency[i]) {
            printf("'%c'         %d         %s\n", i, frequency[i], codes[i]);
        }
    }
}

// Function to encode a text file using Huffman Codes
void encodeFile(FILE *inFile, FILE *outFile, char codes[256][256]) {
    char ch;
    while ((ch = fgetc(inFile)) != EOF) {
        fprintf(outFile, "%s", codes[(unsigned char)ch]);
    }
}

// Free the Huffman Tree
void freeTree(struct HuffmanNode* root) {
    if (root) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}


