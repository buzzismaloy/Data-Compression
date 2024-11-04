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
	if(minHeap->size == minHeap->capacity) {
		fprintf(stderr, "MinHeap is full, unable to insert new node\n");
		return;
	}

	int i = minHeap->size;
	minHeap->array[i] = node;
	minHeap->size++;

	while(i && minHeap->array[i]->frequency < minHeap->array[(i - 1) / 2]->frequency) {
		swapNodes(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);
		i = (i - 1) / 2;
	}
}

void minHeapify(MinHeap* minHeap, int index) {
	int smallest = index;
	int left = 2 * index + 1;
	int right = 2 * index + 2;

	if(left < minHeap->size && minHeap->array[left]->frequency < minHeap->array[smallest]->frequency)
       		smallest = left;

	if(right < minHeap->size && minHeap->array[right]->frequency < minHeap->array[smallest]->frequency)
        	smallest = right;

	if(smallest != index) {
        	swapNodes(&minHeap->array[smallest], &minHeap->array[index]);
        	minHeapify(minHeap, smallest);
    	}
}

HuffmanNode* extractMin(MinHeap* minHeap) {
	if(minHeap->size == 0) 
		return NULL;

	HuffmanNode* minNode = minHeap->array[0];
	minHeap->array[0] = minHeap->array[minHeap->size - 1];
	minHeap->size--;
	minHeapify(minHeap, 0);

	return minNode;
}




