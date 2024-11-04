#include "structures.h"
#include <string.h>


HuffmanNode* buildHuffmanTree(int frequency[]) {
	MinHeap* minHeap = createMinHeap(CHAR_COUNT);

	for(int i = 0; i < CHAR_COUNT; ++i) {
		if(frequency[i] > 0)
			insertMinHeap(minHeap, createNode((char)i, frequency[i]));
	}

	while(minHeap->size != 1) {
		HuffmanNode* left = extractMin(minHeap);
		HuffmanNode* right = extractMin(minHeap);

		HuffmanNode* merged = createNode('\0', left->frequency +right->frequency);
		merged->left = left;
		merged->right = right;

		insertMinHeap(minHeap, merged);
	}

	return extractMin(minHeap);
}

void generateCodes(HuffmanNode* root, char* code, int top, char codes[CHAR_COUNT][CHAR_COUNT]) {
	if(root->left) {
		code[top] = '0';
		generateCodes(root->left, code, top + 1, codes);
	}

	if(root->right) {
		code[top] = '1';
		generateCodes(root->right, code, top + 1, codes);
	}

	if(!root->left && !root->right) {
		code[top] = '\0';
		strcpy(codes[(unsigned char) root->character], code);
	}
}

/*void buildHuffmanCode(HuffmanNode* root, char* codes[]) {}*/
