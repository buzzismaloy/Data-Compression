#include <stdio.h>
#include <stdlib.h>

#define CHAR_COUNT 256

typedef struct HuffmanNode {
	char character;
	int frequency;
	struct HuffmanNode *left, *right;
} HuffmanNode;

typedef struct MinHeap {
	int size;
	int capacity;
	HuffmanNode** array;

} MinHeap;

void calculateFrequency(FILE* file, int frequency[]);
HuffmanNode* createMinHeap(int capacity);

int main() {

	return 0;
}

void calculateFrequency(FILE* file, int frequency[]) {
	char ch;
	while((ch = fgetc(file)) != EOF ) {
		frequency[ (unsigned char) ch ]++;
	}

}

HuffmanNode* createMinHeap(int capacity) {


}

