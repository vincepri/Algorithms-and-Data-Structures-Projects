//
//  Graph.h
//  Graphs
//
//  Created by Vincenzo on 5/26/13.
//  Copyright (c) 2013 whatever. All rights reserved.
//

#ifndef Graphs_Graph_h
#define Graphs_Graph_h
#include "List.h"
#include <string.h>
#include "BinaryHeap.h"

// ---------------------------------------------------- //
// Structures                                           //
// ---------------------------------------------------- //

struct TGraph {
    List *adj;
    char** name;
    int nodes_count;
};

typedef struct TGraph* Graph;


// ---------------------------------------------------- //
// Prototypes                                           //
// ---------------------------------------------------- //

Graph initGraph(int nodes_count);
void freeGraph(Graph G);
void addEdge(Graph G, int source, int target);
void removeEdge(Graph G, int source, int target);
void addNode(Graph G);
void removeNode(Graph G, int node_to_remove);
List checkListRemoval(List L, int node_to_remove);
void DFS(Graph G);
void DFS_helper(Graph G, List E, int i, int *vertex);
Graph makeReverse(Graph G);
void makeReverseList(Graph T, List E, int this_node);
void printGraph(Graph G);
void printGraphWithNames(Graph G);
Graph initGraphProject();
int setNodeData(Graph G, int source, int target, Transport *transport, int *weight, int *weight_alt, float *cost, int *time);
int unsetNodeData(Graph G, int source, int target, Transport *transport);
float randomCost(Transport transport, int weight);
int randomWeightAlt();
void dijkstra(Graph G, int source, int index, Transport transport, int *target, HeapNode *results, int *resultsIndex);
void showPath(Graph G, int prev[], int d[], int i);
void findPath(Graph G, int prev[], int d[], Transport trsp[], int i, HeapNode results[], int *resultsIndex);
void dijkstraWithTarget(Graph G, int source, int index, Transport transport, int target, int *max_cost_res, int *max_time_res, int X);
void dijkstraBlacklist(Graph G, int source, int index, int target, HeapNode results[], int *resultsIndex, int blacklist[][20], HeapNode old_resuts[], int old_results_index, int suggested, int old_cost, int old_time, float *old_weight_alt);
int** makeBlacklist(Graph G);

void findPathR(Graph G, int prev[], float d[], Transport trsp[], int i, HeapNode results[], int *resultsIndex);
void dijkstraR(Graph G, int source, int *target, HeapNode results[], int *resultsIndex, float old);

#endif
