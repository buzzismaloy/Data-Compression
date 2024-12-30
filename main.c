#include <stdio.h>
#include "HuffmanCode.h"

const short int BUFFER_SIZE = 4096;

void calculateFrequency(FILE* file, int frequency[]);
void printFrequencyTable(int frequency[]); // func for debugging purposes(func prints freq table)
void freeTree(HuffmanNode* root);

void printCodes(char codes[CHAR_COUNT][CHAR_COUNT], int frequency[]);
void encodeFile(FILE *inFile, FILE *outFile, char codes[CHAR_COUNT][CHAR_COUNT]);
void decodeFile(FILE* encodedFile, FILE* decodedFile, HuffmanNode* root);
void writeTree(FILE* outfile, HuffmanNode* root);
HuffmanNode* readTree(FILE* inFile);

int main(int argc, char* argv[]) {
	if (argc < 4) {
		fprintf(stderr, "Usage: %s <-c/-d>  <input file> <output file>\n", argv[0]);
		return 1;
	}
	
	FILE *inputFile = fopen(argv[2], "rb");
	if (!inputFile) {
		perror("Error opening input file!");
		return 1;
	}

	FILE* outputFile = NULL;

	if (strcmp(argv[1], "-c") == 0) {
		int frequency[CHAR_COUNT] = {0};
		calculateFrequency(inputFile, frequency);
		rewind(inputFile);

		HuffmanNode* root = buildHuffmanTree(frequency);
		if (!root) {
            		fprintf(stderr, "Error: Failed to build Huffman tree.\n");
            		fclose(inputFile);
            		return 1;
        	}

		char codes[CHAR_COUNT][CHAR_COUNT] = {{0}};
		char code[CHAR_COUNT];
		generateCodes(root, code, 0, codes);
		
		outputFile = fopen(argv[3], "wb");
		if (!outputFile) {
			perror("Error opening output file!");
			freeTree(root);
			fclose(inputFile);
			return 1;
		}

		writeTree(outputFile, root);//write the tree to the encrypted file
		encodeFile(inputFile, outputFile, codes);
		printf("File successfully compressed to %s\n", argv[3]);

        	freeTree(root);
	} else if (strcmp(argv[1], "-d") == 0) {
		HuffmanNode* root = readTree(inputFile);
		if (!root) {
			fprintf(stderr, "Error: No Huffman Tree found in the input file. Decompression aborted.\n");
            		fclose(inputFile);
            		return 1;
		}

		outputFile = fopen(argv[3], "w");
		if (!outputFile) {
			perror("Error opening output file!");
			freeTree(root);
			fclose(inputFile);
			return 1;
		}

		decodeFile(inputFile, outputFile, root);
		printf("File successfully decompressed to %s\n", argv[3]);

		freeTree(root);
	} else {
        	fprintf(stderr, "Invalid option. Use -c to compress or -d to decompress.\n");
        	fclose(inputFile);
        	fclose(outputFile);
        	return 1;
	}

	fclose(inputFile);
	fclose(outputFile);
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
	char buffer[BUFFER_SIZE]; // Buffer to hold chunks of data
	size_t bytesRead;
	unsigned char byteBuffer = 0; // Buffer for bits
	int bitCount = 0; // Number of bits currently in the buffer

   	while ((bytesRead = fread(buffer, 1, sizeof(buffer), inFile)) > 0) {
		for (size_t i = 0; i < bytesRead; i++) {
            		char *code = codes[(unsigned char)buffer[i]]; // Get the Huffman code for the character
            		for (int j = 0; code[j] != '\0'; j++) {
                		// Add the bit to the buffer
                		byteBuffer = (byteBuffer << 1) | (code[j] - '0'); // '0' -> 0, '1' -> 1
                		bitCount++;

                		// If the buffer is full (8 bits), write it to the file
                		if (bitCount == 8) {
                    			fputc(byteBuffer, outFile);
                    			byteBuffer = 0; // Reset buffer
                    			bitCount = 0;
                		}
            		}
        	}
	}

    	// Handle any remaining bits in the buffer
	if (bitCount > 0) {
		byteBuffer <<= (8 - bitCount); // Pad with zeros
        	fputc(byteBuffer, outFile);
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

void writeTree(FILE* outFile, HuffmanNode* root) {
	if (!root) {
		fputc('0', outFile);
		return;
	}

	fputc('1', outFile);
	fputc(root->character, outFile);
    	writeTree(outFile, root->left);
    	writeTree(outFile, root->right);
}

HuffmanNode* readTree(FILE *inFile) {
int flag = fgetc(inFile);
    if (flag == EOF) {
        fprintf(stderr, "Error: Unexpected end of file while reading Huffman tree.\n");
        return NULL;
    }

    if (flag == '0') {
        return NULL;
    } else if (flag != '1') {
        fprintf(stderr, "Error: Invalid flag '%c' encountered in Huffman tree.\n", flag);
        return NULL;
    }

    HuffmanNode *node = (HuffmanNode *)malloc(sizeof(HuffmanNode));
    if (!node) {
        perror("Error allocating memory for Huffman node");
        return NULL;
    }

    int character = fgetc(inFile);
    if (character == EOF) {
        fprintf(stderr, "Error: Unexpected end of file while reading Huffman node character.\n");
        free(node);
        return NULL;
    }

    node->character = (unsigned char)character;
    node->left = readTree(inFile);
    node->right = readTree(inFile);

    return node;
}

// Free the Huffman Tree
void freeTree(struct HuffmanNode* root) {
	if (root) {
        	freeTree(root->left);
        	freeTree(root->right);
        	free(root);
	}
}


