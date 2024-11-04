#include <stdio.h>
#include "HuffmanCode.h"

void calculateFrequency(FILE* file, int frequency[]);
void printFrequencyTable(int frequency[]); // func for debugging purposes(func prints freq table)
void freeTree(HuffmanNode* root);

void printCodes(char codes[CHAR_COUNT][CHAR_COUNT], int frequency[]);
void encodeFile(FILE *inFile, FILE *outFile, char codes[CHAR_COUNT][CHAR_COUNT]);
void decodeFile(FILE* encodedFile, FILE* decodedFile, HuffmanNode* root);

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
	fclose(inputFile);

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

inputFile = fopen(argv[1], "r");
if (inputFile == NULL) {
    perror("Error reopening input file for encoding");
    freeTree(root);
    fclose(outputFile);
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

	// Decoding section for debugging purposes
    	FILE *encodedFile = fopen(argv[2], "rb");
   	FILE *decodedFile = fopen("decoded_output.txt", "w");	

	if(!encodedFile || !decodedFile) {
		perror("Error opening file for decoding");
        	freeTree(root);
        	return 1;
	}
	decodeFile(encodedFile, decodedFile, root);	
	printf("File successfully decoded to decoded_output.txt\n");	

	fclose(encodedFile);
    	fclose(decodedFile);
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
    unsigned char buffer = 0; // Buffer for bits
    int bufferIndex = 0; // Number of bits currently in the buffer

    while ((ch = fgetc(inFile)) != EOF) {
        char *code = codes[(unsigned char)ch]; // Get the Huffman code for the character
        for (int i = 0; code[i] != '\0'; i++) {
            // Add the bit to the buffer
            buffer = (buffer << 1) | (code[i] - '0'); // '0' -> 0, '1' -> 1
            bufferIndex++;

            // If the buffer is full (8 bits), write it to the file
            if (bufferIndex == 8) {
                fputc(buffer, outFile);
                buffer = 0; // Reset buffer
                bufferIndex = 0;
            }
        }
    }

    // Handle any remaining bits in the buffer
    if (bufferIndex > 0) {
        buffer <<= (8 - bufferIndex); // Pad with zeros
        fputc(buffer, outFile);
    }
}

// Function to decode a file using the Huffman tree
void decodeFile(FILE *encodedFile, FILE *decodedFile, HuffmanNode *root) {
HuffmanNode *current = root;
    int byte;

    while ((byte = fgetc(encodedFile)) != EOF) {
        for (int i = 7; i >= 0; i--) {
            int bit = (byte >> i) & 1;

            if (bit == 0) {
                current = current->left;
            } else {
                current = current->right;
            }

            // If at a leaf node, write the character
            if (!current->left && !current->right) {
                fputc(current->character, decodedFile);
                current = root; // Reset for next character
            }
        }
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


