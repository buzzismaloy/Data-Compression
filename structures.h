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

HuffmanNode* createNode(char character, int frequency) {
	HuffmanNode* newNode = (HuffmanNode*) malloc(sizeof(HuffmanNode));
	newNode->character = character;
	newNode->frequency = frequency;
	newNode->left = newNode->right = NULL;

	return newNode;
}

MinHeap* createMinHeap(int capacity) {
	MinHeap* minHeap = (MinHeap*) malloc(sizeof(MinHeap));
	minHeap->size = 0;
	minHeap->capacity = capacity;
	minHeap->array = (HuffmanNode**) malloc(capacity * sizeof(HuffmanNode*));

	return minHeap;
}

void swapNodes(HuffmanNode** a, HuffmanNode** b) {
	HuffmanNode *temp = *a;
	*a = *b;
	*b = temp;
}

void insertMinHeap(MinHeap* minHeap, HuffmanNode* node) {

}

void minHeapify(MinHeap* minHeap, int index) {

}

HuffmanNode* extractMin(MinHeap* minHeap) {

}




