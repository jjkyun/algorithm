#include "huffman.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Character size is 256
#define CHAR_SIZE 1 << 8

/*
 *  Two way of huffman coding
 *  1. Build a huffman tree from input characters
 *  2. Encode: Traverse the huffman tree and assign codes to characters
 *  3. Decode: Traverse the huffman tree and assign characters to codes
 */

// Global variables
// Frequency of each ASCII character 
int ascii_table[CHAR_SIZE];
// Bitcode of each ASCII character
char *huffman_code_table[CHAR_SIZE];
h_node *h_nodes;
int size_h_nodes;
int size_heap;


/* Count frequency of characters in source file */
void count_char_frequency(char *src, int length)
{
    for (int i = 0; i < length; i++) {
        ascii_table[src[i]]++;
    }
}

/* Initiate huffman nodes */
void init_h_nodes()
{
    // Count characters which the frequency is not 0 
    for (int i = 0; i < CHAR_SIZE; i++) {
        if (ascii_table[i] != 0) {
            size_h_nodes++;
        }
    }
    // Initalize huffman nodes with character and frequency
    h_nodes = (h_node *)malloc(sizeof(h_node) * size_h_nodes);
    int j = 0;
    for (int i = 0; i < CHAR_SIZE; i++) {
        if (ascii_table[i] != 0) {
            h_nodes[j].character = i;
            h_nodes[j].freq = ascii_table[i];
            h_nodes[j].left = NULL;
            h_nodes[j].right = NULL;
            j++;
        }
    }
}

/* Create internal node 
 * Internal node: Only frequency, no character */
h_node *create_internal_node(int freq, h_node *left, h_node *right)
{
    h_node *internal = (h_node *)malloc(sizeof(h_node));
    internal->freq = freq;
    internal->left = left;
    internal->right = right;
    return internal;
}

/* Heapify for building heap tree */
void heapify(h_node **heap, int index)
{
    int parent = index;
    int left = 2 * index;
    int right = left + 1;
    int minimum = parent;

    if (left <= size_heap && heap[left]->freq < heap[minimum]->freq) {
        minimum = left;
    }

    if (right <= size_heap && heap[right]->freq < heap[minimum]->freq) {
        minimum = right;
    }

    if (minimum != parent) {
        h_node *temp = heap[parent];
        heap[parent] = heap[minimum];
        heap[minimum] = temp;
        heapify(heap, minimum);
    }
}

/* Build min heap tree */
h_node **build_min_heap()
{
    size_heap = size_h_nodes;
    h_node **heap = (h_node **)malloc(sizeof(h_node *) * size_heap + 1);
    heap[0] = NULL;
    for (int i = 1; i <= size_heap; i++) {
        heap[i] = &h_nodes[i - 1];
    }
    for (int i = size_heap / 2; i > 0; i--) {
        heapify(heap, i);
    }
    return heap;
}

/* Extract minimum from min heap to build huffman tree */
h_node *extract_minimum(h_node **heap)
{
    if (size_heap > 0) {
        h_node *minimum = heap[1];
        heap[1] = heap[size_heap];
        heap[size_heap] = minimum;
        size_heap--;
        heapify(heap, 1);
        return minimum;
    }
    return NULL;
}

/* Insert new node(internal) to min heap tree */
void insert(h_node **heap, h_node *node)
{
    size_heap++;
    heap[size_heap] = node;

    int parent = size_heap / 2;
    int me = size_heap;
    h_node *temp = NULL;
    // Do heapify upward
    while (parent > 0 && heap[me]->freq < heap[parent]->freq) {
        temp = heap[me];
        heap[me] = heap[parent];
        heap[parent] = temp;
        me = parent;
        parent = me / 2;
    }
}

/* Build huffman tree */
h_node *build_huffman_tree()
{
    // Initialize huffman nodes
    init_h_nodes();
    // Build min heap tree
    h_node **heap = build_min_heap();
    // While heap tree has only roote node extract minimum frequency
    while (size_heap > 1) {
        h_node *first = extract_minimum(heap);
        h_node *second = extract_minimum(heap);
        h_node *internal = create_internal_node(first->freq + second->freq, first, second);
        insert(heap, internal);
    }

    // return root
    return extract_minimum(heap);
}

/* Extend huffman code */
char *extend_huffman_code(char *code, char zero_or_one)
{
    char *extended = NULL;
    if (code == NULL) {
        extended = (char *)malloc(sizeof(char) + 1);
        extended[0] = zero_or_one;
        extended[1] = '\0';
    } else {
        extended = (char *)malloc(sizeof(char) * (strlen(code) + 1 + 1));
        strncpy(extended, code, strlen(code));
        extended[strlen(code)] = zero_or_one;
        extended[strlen(code) + 1] = '\0';
    }
    return extended;
}

/* Map huffman code
 * Becareful to make 0 and 1 string type
 * Or 000 and 0 can't be distinguished */
void map_huffman_code(h_node *root, char *code)
{
    // If the root is left, then insert 0
    if (root->left) {
        map_huffman_code(root->left, extend_huffman_code(code, '0'));
    } 

    // If the root is right, then insert 1
    if (root->right) {
        map_huffman_code(root->right, extend_huffman_code(code, '1'));
    }

    if (root->left == NULL && root->right == NULL) {
        huffman_code_table[root->character] = code;
    }
}

/* Convert source file characters to huffman code and isnert it to bit array */ 
int string_to_bit_array(char *src, int length, char *dst)
{
    // Check the size of source file characters
    int bit_array_length = 0;
    for (int i = 0; i < CHAR_SIZE; i++) {
        if (ascii_table[i] != 0) {
            bit_array_length += ascii_table[i] * strlen(huffman_code_table[i]);
        }
    } 
    // Make buf array according to the size of source file
    char *buf = (char*)malloc(sizeof(char) * bit_array_length);
    int k = 0;
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < strlen(huffman_code_table[src[i]]); j++) {
            // Insert huffman code to buf array 
            buf[k++] = huffman_code_table[src[i]][j];
        }
    }
    // Convert buf array to bit type
    for (int i = 0; i < bit_array_length; i++) {
        if (buf[i] == '1') {
            SET_BIT(dst, i);
        } else {
            RESET_BIT(dst, i);
        }
    }
    free(buf);
    
    return bit_array_length;
}

/* Check if the node is leaf or internal */
int is_leaf(h_node *position)
{
    return position->left == NULL && position->right == NULL;
}


/**
  Encode string to bit array using huffman coding algorithm

  @param[in]	src				Source raw string
  @param[in]	src_byte_size	Length of source string, in bytes
  @param[out]	dst_tree		Root node of huffman tree
  @param[out]	dst				Destination encoded bit array
  @return Length of encoded bit array, in bits (e.g. 0100 => 4)
*/
int encode_huffman(char *src, int src_byte_size, 
        h_node** dst_tree, char *dst){
    // Check the frequency
    count_char_frequency(src, src_byte_size);
    // Build huffman tree using min heap tree
    *dst_tree = build_huffman_tree();
    // Make huffman code from each characters
    map_huffman_code(*dst_tree, NULL);
    // Return the encoded code
    return string_to_bit_array(src, src_byte_size, dst);
}

/**
  Decode bit array to string using huffman coding algorithm

  @param[in]	src				Source bit array
  @param[in]	src_bit_size	Length of bit array, in bits
  @param[in]	dst_tree		Root node of huffman tree
  @param[out]	dst				Destination string
  @return Length of decoded string, in bytes
*/
int decode_huffman(char *src, int src_bit_size,
		h_node* dst_tree, char *dst){
    // Point to root node
    h_node *current = dst_tree;
    // LEFT or RIGHT
    int direction = 0;
    int dst_index = 0;
    // Traverse
    for (int i = 0; i < src_bit_size; i++) {
        
        direction = GET_BIT(src, i);

#define LEFT 0
#define RIGHT 1
        switch (direction) {
            case LEFT:
                current = current->left;
                break;
            case RIGHT:
                current = current->right;
                break;
        }
        // If leaf node then get the character
        if (is_leaf(current)) {
            dst[dst_index++] = current->character;
            current = dst_tree;
        }
    }
    // Insert NULL at the end
    dst[dst_index] = '\0';

    return strlen(dst);
}

