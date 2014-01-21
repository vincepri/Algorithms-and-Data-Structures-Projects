//
//  BinaryHeap.h
//  Graphs
//
//  Created by Vincenzo on 6/4/13.
//  Copyright (c) 2013 whatever. All rights reserved.
//

#ifndef Graphs_BinaryHeap_h
#define Graphs_BinaryHeap_h
#include "NodeData.h"

struct THeapNode {
    int target;
    int distance;
    Transport transport;
};

typedef struct THeapNode* HeapNode;


struct TBinaryHeap {
    int capacity;
    int size;
    HeapNode* nodes;
};

typedef struct TBinaryHeap* BinaryHeap;


BinaryHeap initHeap(int nodes);
void makeHeapEmpty(BinaryHeap H);
int isHeapEmpty(BinaryHeap H);
int isHeapFull(BinaryHeap H) ;
void freeHeap(BinaryHeap H);
void insertHeap(BinaryHeap H, HeapNode N);
HeapNode deleteMinHeap(BinaryHeap H);

#endif
