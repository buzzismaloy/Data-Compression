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

HuffmanNode* createNode(int capacity, int frequency) {

}

MinHeap* createMinHeap(int capacity) {

}

void swapNodes(HuffmanNode** a, HuffmanNode** b) {

}

void insertMinHeap(MinHeap* minHeap, HuffmanNode* node) {

}

void minHeapify(MinHeap* minHeap, int index) {

}

HuffmanNode* extractMin(MinHeap* minHeap) {

}




