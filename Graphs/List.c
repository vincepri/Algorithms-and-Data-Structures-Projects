//
//  List.c
//  Graphs
//
//  Created by Vincenzo on 5/26/13.
//  Copyright (c) 2013 whatever. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "List.h"


// ---------------------------------------------------- //
// Constructors and Deconstructors                      //
// ---------------------------------------------------- //

List initNode(int target)
{
    List E = malloc(sizeof(struct TList));
    E->target = target;
    E->weight = malloc(5*sizeof(int));
    E->weight_alt = malloc(5*sizeof(int));
    E->cost = malloc(5*sizeof(float));
    E->time = malloc(5*sizeof(int));
    E->awsm[0] = E->weight;
    E->awsm[1] = E->weight_alt;
    E->awsm[2] = E->cost;
    E->awsm[3] = E->time;
    E->awsm[4] = NULL;
    E->awsm[5] = NULL;
    E->min[0] = -1;
    E->min[1] = -1;
    E->min[2] = -1;
    E->min[3] = -1;
    E->next = NULL;
    int i = 0;
    for (i = 0; i < 5; i++) {
        E->weight[i] = INT_MAX;
        E->weight_alt[i] = INT_MAX;
        E->cost[i] = INT_MAX;
        E->time[i] = INT_MAX;
    }
    return E;
}


void freeList(List E)
{
    if (E != NULL) {
        freeList(E->next);
        free(E);
    }
}

// ---------------------------------------------------- //
// Access functions                                     //
// ---------------------------------------------------- //

void printList(List L) {
    if (L != NULL) {
        printf(" %d ", L->target);
        printList(L->next);
    }
}


List findNode(List L, int target) {
    if (L != NULL) {
        if (L->target == target) {
            return L;
        }
        return findNode(L->next, target);
    }
    return NULL;
}


// ---------------------------------------------------- //
// Manipulation functions                               //
// ---------------------------------------------------- //

List addNodeList(List L, int target, int *added) {
    if (L != NULL) {
        if (L->target != target) {
            L->next = addNodeList(L->next, target, added);
        }
    } else {
        L = initNode(target);
    }
    return L;
}


List removeNodeList(List L, int target, int *removed) {
    if (L != NULL) {
        if (L->target == target) {
            List tmp = L->next;
            free(L);
            return tmp;
        }
        L->next = removeNodeList(L->next, target, removed);
    }
    return L;
}

// ---------------------------------------------------- //
// Other functions                                      //
// ---------------------------------------------------- //

