//
//  NodeData.c
//  Graphs
//
//  Created by Vincenzo on 5/26/13.
//  Copyright (c) 2013 whatever. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "NodeData.h"


// ---------------------------------------------------- //
// Constructors and Deconstructors                      //
// ---------------------------------------------------- //

//NodeData initNodeData()
//{
//    NodeData ND = malloc(sizeof(struct TNodeData));
//    ND->cost = 0;
//    ND->time = 0;
//    return ND;
//}


// ---------------------------------------------------- //
// Access functions                                     //
// ---------------------------------------------------- //
//
//float getCost(NodeData E)
//{
//    assert(E != NULL);
//    return E->cost;
//}


//unsigned long getTime(NodeData E)
//{
//    assert(E != NULL);
//    return E->time;
//}


// ---------------------------------------------------- //
// Manipulation functions                               //
// ---------------------------------------------------- //

//void setCost(NodeData E, float cost)
//{
//    assert(E != NULL);
//    E->cost = cost;
//}
//
//
//void setTime(NodeData E, unsigned long time)
//{
//    assert(E != NULL);
//    E->time = time;
//}



// ---------------------------------------------------- //
// Other functions                                      //
// ---------------------------------------------------- //

void printTransports() {
    printf("(0) Pullman - (1) Treno - (2) Aereo - (3) Nave - (4) Taxi - (5) Tutti\n");
}