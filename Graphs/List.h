//
//  List.h
//  Graphs
//
//  Created by Vincenzo on 5/26/13.
//  Copyright (c) 2013 whatever. All rights reserved.
//

#ifndef Graphs_List_h
#define Graphs_List_h
#include "NodeData.h"

// ---------------------------------------------------- //
// Structures                                           //
// ---------------------------------------------------- //


struct TList {
    int target;
    int* weight;
    int* weight_alt;
    float* cost;
    int* time;
    void* awsm[6];
    struct TList* next;
    int min[4];
};

typedef struct TList* List;


struct TFakeList {
    struct TList* getReal;
    struct TFakeList* next;
};

typedef struct TFakeList* FakeList;


// ---------------------------------------------------- //
// Prototypes                                           //
// ---------------------------------------------------- //

List initNode(int target);
void freeList(List E);
List addNodeList(List L, int target, int *removed);
List removeNodeList(List L, int target, int *removed);
void printList(List L);
List findNode(List L, int target);
#endif
