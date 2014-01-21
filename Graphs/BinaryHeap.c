//
//  BinaryHeap.c
//  Graphs
//
//  Created by Vincenzo on 6/4/13.
//  Copyright (c) 2013 whatever. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include "BinaryHeap.h"


BinaryHeap initHeap(int nodes) {
    BinaryHeap H = malloc(sizeof(struct TBinaryHeap));
    H->size = 0;
    H->capacity = nodes*nodes;
    H->nodes = calloc(nodes*nodes, sizeof(struct THeapNode));
    H->nodes[0] = malloc(sizeof(struct THeapNode));
    H->nodes[0]->distance = INT_MIN;
    H->nodes[0]->target = INT_MIN;
    return H;
}


void makeHeapEmpty(BinaryHeap H) {
    H->size = 0;
}


int isHeapEmpty(BinaryHeap H) {  // Ritorna 1 se e' vuoto, ovvero se size = 0
    return H->size == 0;
}


int isHeapFull(BinaryHeap H) {
    return H->size == H->capacity;
}


void freeHeap(BinaryHeap H) {
    free(H->nodes);
    free(H);
}


void insertHeap(BinaryHeap H, HeapNode N) {
    assert(!isHeapFull(H));
    int i = 0;
    for (i = ++H->size; H->nodes[i/2]->distance > N->distance; i /= 2) {
        H->nodes[i] = H->nodes[i/2];
    }
    H->nodes[i] = N;
}


HeapNode deleteMinHeap(BinaryHeap H) {
    assert(!isHeapEmpty(H));
    int i = 0;
    int child = 0;
    HeapNode min = H->nodes[1];
    HeapNode last = H->nodes[H->size];
    H->size -= 1;
    
    for (i = 1; i*2 <= H->size; i = child) {
        child = i*2;
        if (child != H->size && H->nodes[child+1]->distance < H->nodes[child]->distance) {
            child++;
        }
        if (last->distance > H->nodes[child]->distance) {
            H->nodes[i] = H->nodes[child];
        } else {
            break;
        }
    }
    H->nodes[i] = last;
    return min;
    
}