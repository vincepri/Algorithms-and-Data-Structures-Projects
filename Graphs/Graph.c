//
//  Graph.c
//  Graphs
//
//  Created by Vincenzo on 5/26/13.
//  Copyright (c) 2013 whatever. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include "Graph.h"

// ---------------------------------------------------- //
// Constructors and Deconstructors                      //
// ---------------------------------------------------- //

Graph initGraph(int nodes_count) {
    Graph G = malloc(sizeof(struct TGraph));
    G->adj = calloc(nodes_count+1, sizeof(List));
    G->name = calloc(nodes_count+1, sizeof(char*));
    G->nodes_count = nodes_count;
    return G;
}

void freeGraph(Graph G) {
    if (G != NULL) {
        if (G->nodes_count > 0) {
            int i = 0;
            for (i = 0; i < G->nodes_count; i++) {
                freeList(G->adj[i]);
            }
        }
        free(G);
    }
}


// ---------------------------------------------------- //
// Access functions                                     //
// ---------------------------------------------------- //

int getNodesCount(Graph G) {
    assert(G != NULL);
    return G->nodes_count;
}


int getEdgesCount(Graph G){
    assert(G != NULL);
    int i = 0;
    int x = 0;
    for (i = 0; i < G->nodes_count; i++) {
        List E = G->adj[i];
        while (E != NULL) {
            x++;
            E = E->next;
        }
    }
    return x;
}


void printGraph(Graph G) {
    if (G != NULL) {
        int x = 0;
        for (x = 0; x < G->nodes_count; x++) {
            printf("%d -> ", x);
            printList(G->adj[x]);
            printf("\n");
        }
    }
}

void printGraphWithNames(Graph G) {
    if (G != NULL) {
        int x = 0;
        for (x = 0; x < G->nodes_count; x++) {
            printf("%d %s -> ", x, G->name[x]);
            printList(G->adj[x]);
            printf("\n");
        }
    }
}


void DFS(Graph G) {
    if (G != NULL) {
        int *vertex = calloc(G->nodes_count+1, sizeof(int));
        int x = 0;
        for (x = 0; x < G->nodes_count; x++) {
            if (vertex[x] == 0) {
                printf("\n%d ->", x);
                DFS_helper(G, G->adj[x], x, vertex);
            }
        }
        free(vertex);
    }
}


void DFS_helper(Graph G, List E, int i, int *vertex) {
    vertex[i] = 1;
    if (E != NULL) {
        if (vertex[E->target] == 0) {
            printf(" %d ", E->target);
            DFS_helper(G, G->adj[E->target], E->target, vertex);
        }
        if (E->next != NULL && vertex[E->next->target] == 0) {
            printf(" - ");
        }
        DFS_helper(G, E->next, E->target, vertex);
    }
}


// ---------------------------------------------------- //
// Manipulation functions                               //
// ---------------------------------------------------- //


void addEdge(Graph G, int source, int target)
{
    assert(G != NULL);
    assert(source < G->nodes_count);
    assert(target < G->nodes_count);
    int added = 0;
    G->adj[source] = addNodeList(G->adj[source], target, &added);
}


void removeEdge(Graph G, int source, int target)
{
    assert(G != NULL);
    assert(source < G->nodes_count);
    assert(target < G->nodes_count);
    int removed = 0;
    G->adj[source] = removeNodeList(G->adj[source], target, &removed);
    if (removed == 0) {
        printf("Si e' verificato un errore. \n");
    }
}


void addNode(Graph G)
{
    if (G != NULL) {
        G->adj = realloc(G->adj, (G->nodes_count+1) * sizeof(List));
        G->name = (char**)realloc(G->name, sizeof(char*) * (G->nodes_count+1));
        G->nodes_count += 1;
        G->adj[G->nodes_count] = NULL;
    }
}


void removeNode(Graph G, int node_to_remove)
{
    if (G != NULL) {
        int i = 0;
        int x = 0;
        List *tmp = G->adj;
        G->adj = calloc(G->nodes_count, sizeof(List));
        for (i = 0; i <= G->nodes_count; i++) {
            if (i != node_to_remove) {
                G->adj[x] = checkListRemoval(tmp[i], node_to_remove);
                G->name[x] = G->name[i];
                x++;
            } else {
                freeList(G->adj[x]);
            }
        }
        G->name = (char**)realloc(G->name, sizeof(char*) * (G->nodes_count));
        G->nodes_count -= 1;
        free(*tmp);
    }
}


List checkListRemoval(List L, int node_to_remove) {
    if (L != NULL) {
        L->next = checkListRemoval(L->next, node_to_remove);
        if (L->target == node_to_remove) {
            List tmp = L->next;
            free(L);
            return tmp;
        } else if (L->target > node_to_remove) {
            L->target -= 1;
        }
    }
    return L;
}

// ---------------------------------------------------- //
// Other functions                                      //
// ---------------------------------------------------- //


// Calcola il grafo trasposto
Graph makeReverse(Graph G) {
    if (G != NULL) {
        Graph T = initGraph(G->nodes_count);
        int x = 0;
        for (x = 0; x < G->nodes_count; x++) {
            makeReverseList(T, G->adj[x], x);
        }
        return T;
    }
    return NULL;
}


// Helper per la creazione delle liste del grafo trasposto
void makeReverseList(Graph T, List E, int this_node) {
    assert(T != NULL);
    if (E != NULL) {
        addEdge(T, E->target, this_node);
        makeReverseList(T, E->next, this_node);
    }
}


// Setta i dati del nodo
int setNodeData(Graph G, int source, int target, Transport *transport, int *weight, int *weight_alt, float *cost, int *time) {
    assert(G != NULL);
    List L = findNode(G->adj[source], target);
    if (L == NULL) {
        printf("L'arco specificato non esiste. \n");
        return 0;
    } else {
        if (weight != NULL) {
            L->weight[*transport] = *weight;
            printf("%d VS %d", *weight, L->weight[L->min[0]]);
            if (L->min[0] == -1 || *weight < L->weight[L->min[0]]) {
                printf(" -- SET");
                L->min[0] = *transport;
            }
            printf("\n");
        }
        if (weight_alt != NULL) {
            L->weight_alt[*transport] = *weight_alt;
            if (L->min[1] == -1 || *weight_alt < L->weight_alt[L->min[1]]) {
                L->min[1] = *transport;
            }
        }
        if (cost != NULL) {
            L->cost[*transport] = *cost;
            if (L->min[2] == -1 || *cost < L->cost[L->min[2]]) {
                L->min[2] = *transport;
            }
        }
        if (time != NULL) {
            L->time[*transport] = *time;
            if (L->min[3] == -1 || *time < L->time[L->min[3]]) {
                L->min[3] = *transport;
            }
            
        }
    }
    return 1;
}


int unsetNodeData(Graph G, int source, int target, Transport *transport) {
    assert(G != NULL);
    assert(transport != NULL);
    List L = findNode(G->adj[source], target);
    if (L == NULL) {
        printf("L'arco specificato non esiste. \n");
        return 0;
    }
    L->weight[*transport] = 0;
    L->weight_alt[*transport] = 0;
    L->time[*transport] = 0;
    L->cost[*transport] = 0;
    return 1;
}


int randomWeightAlt() {
    srand((unsigned int)time(NULL));
    return rand() % 3;
}


float randomCost(Transport transport, int weight) {
    srand((unsigned int)time(NULL));
    float cost_per_meter = 0;
    if (transport == pullman || transport == nave) {
        cost_per_meter = 0.000445;
    } else if (transport == taxi) {
        cost_per_meter = 0.000659;
    } else if (transport == treno) {
        cost_per_meter = 0.000088;
    } else if (transport == aereo) {
        cost_per_meter = 0.000188;
    }
    return (weight - (weight/100)*(rand() % 40)) * cost_per_meter / (rand() % 10);
}


void dijkstra(Graph G, int source, int index, Transport transport, int *target, HeapNode results[], int *resultsIndex) {
    BinaryHeap Q = initHeap(G->nodes_count);
    HeapNode N = malloc(sizeof(struct THeapNode));
    int i = 0;
    int d[G->nodes_count];
    int prev[G->nodes_count];
    Transport trsp[G->nodes_count];
    
    // Inizializzo
    for (i = 0; i < G->nodes_count; i++) {
        d[i] = INT_MAX;
        prev[i] = -1;
        trsp[i] = NaN;
    }
    N->distance = 0;
    N->target = source;
    d[source] = 0;
    
    // Inserisci nella coda
    insertHeap(Q, N);
    
    // Start!
    while (!isHeapEmpty(Q)) {
        N = deleteMinHeap(Q);  // Cancello ed estraggo il minimo
        List L = G->adj[N->target];
        while (L != NULL) {
            float dist = 0;
            if (transport != NaN) {
                if (index != 2) {
                    dist = d[N->target] + ((int*)L->awsm[index])[transport];
                } else {
                    dist = d[N->target] + ((float*)L->awsm[index])[transport];
                }
            } else if (L->min[index] != -1) {
                if (index != 2) {
                    dist = d[N->target] + ((int*)L->awsm[index])[L->min[index]];
                } else {
                    dist = d[N->target] + ((float*)L->awsm[index])[L->min[index]];
                }
            } else {
                dist = INT_MAX;
            }
            printf("dist: %f ", dist);
            if (dist < d[L->target]) {
                HeapNode v = malloc(sizeof(struct THeapNode));
                d[L->target] = dist;
                prev[L->target] = N->target;
                if (transport != NaN) {
                    trsp[L->target] = transport;
                } else {
                    trsp[L->target] = L->min[index];
                }
                v->target = L->target;
                v->distance = dist;
                insertHeap(Q, v);
            }
            L = L->next;
        }
    }
    
    if (target == NULL || results == NULL || resultsIndex == NULL) {
        for (i = 0; i < G->nodes_count; i++) {
            showPath(G, prev, d, i);
            printf("\n");
        }
    } else {
        *resultsIndex = 0;
        
        findPath(G, prev, d, trsp, *target, results, resultsIndex);
        printf("%d /n", d[*resultsIndex-1]);
    }

}

void dijkstraR(Graph G, int source, int *target, HeapNode results[], int *resultsIndex, float old) {
    BinaryHeap Q = initHeap(G->nodes_count);
    HeapNode N = malloc(sizeof(struct THeapNode));
    int i = 0;
    float d[G->nodes_count];
    int prev[G->nodes_count];
    Transport trsp[G->nodes_count];
    Transport tmp_trsp;
    
    // Inizializzo
    for (i = 0; i < G->nodes_count; i++) {
        d[i] = INT_MAX;
        prev[i] = -1;
        trsp[i] = NaN;
    }
    N->distance = 0;
    N->target = source;
    d[source] = 0;
    
    // Inserisci nella coda
    insertHeap(Q, N);
    
    // Start!
    while (!isHeapEmpty(Q)) {
        N = deleteMinHeap(Q);  // Cancello ed estraggo il minimo
        List L = G->adj[N->target];
        while (L != NULL) {
            float dist = 0;
            if (L->min[2] != -1 && L->min[3] != -1 && L->min[2] == L->min[3]) {
                dist = d[N->target] + (((float*)L->awsm[2])[L->min[2]] * ((int*)L->awsm[3])[L->min[3]]);
                tmp_trsp = L->min[2];
            } else if (L->min[2] != -1 && L->min[3] != -1 && L->min[2] != L->min[3]) {
                float first_pos = (((float*)L->awsm[2])[L->min[2]] * ((int*)L->awsm[3])[L->min[2]]);
                float second_pos = (((float*)L->awsm[2])[L->min[3]] * ((int*)L->awsm[3])[L->min[3]]);
//                printf("first_pos: %f \n", first_pos);
//                printf("second_pos: %f \n", second_pos);
                if (first_pos <= second_pos) {
                    dist = d[N->target] + first_pos;
                    tmp_trsp = L->min[2];
                } else {
                    dist = d[N->target] + second_pos;
                    tmp_trsp = L->min[3];
                }
            } else {
                dist = INT_MAX;
            }
//            printf("dist: %f \n", dist);
            if (dist < d[L->target]) {
                HeapNode v = malloc(sizeof(struct THeapNode));
                d[L->target] = dist;
                prev[L->target] = N->target;
                trsp[L->target] = tmp_trsp;
                v->target = L->target;
                v->distance = dist;
                insertHeap(Q, v);
            }
            L = L->next;
        }
    }
    *resultsIndex = 0;
    findPathR(G, prev, d, trsp, *target, results, resultsIndex);
    float best = 0;
    if (*resultsIndex > 0) {
        best = results[*resultsIndex-1]->distance;
    }
    if (best <= old) {
        return;
    }
    *resultsIndex = 0;
}

void findPathR(Graph G, int prev[], float d[], Transport trsp[], int i, HeapNode results[], int *resultsIndex) {
    if (prev[i] == i) {
        results[*resultsIndex] = malloc(sizeof(struct THeapNode));
        results[*resultsIndex]->target = i;
        results[*resultsIndex]->distance = d[i];
        results[*resultsIndex]->transport = trsp[i];
    } else if (prev[i] == -1) {
        results[*resultsIndex] = malloc(sizeof(struct THeapNode));
        results[*resultsIndex]->target = -1;
        results[*resultsIndex]->distance = -1;
        results[*resultsIndex]->transport = trsp[i];
    } else {
        findPathR(G, prev, d, trsp, prev[i], results, resultsIndex);
        results[*resultsIndex] = malloc(sizeof(struct THeapNode));
        results[*resultsIndex]->target = i;
        results[*resultsIndex]->distance = d[i];
        results[*resultsIndex]->transport = trsp[i];
        *resultsIndex += 1;
    }
}


//void dijkstraBlacklist(Graph G, int source, int index, int target, HeapNode results[], int *resultsIndex, int blacklist[][20], HeapNode old_results[], int old_results_index, int suggested, int old_cost, int old_time, float *old_weight_alt) {
//
//    // Override nel caso in cui stiamo cercando il percorso turistico
//    // ma l'index non e' quello del peso turistico
//    if (suggested == 0 && index != 1) {
//        index = 1;
//    }
//    
//    // Inizio a mettere sempre l'ultimo arco del precedente percorso in blacklist (settando ad 1)
//    if (old_results_index == 1) {
//        blacklist[source][old_results[old_results_index-1]->target] = 1;
//    } else if (old_results_index != 0){
//        blacklist[old_results[old_results_index-2]->target][old_results[old_results_index-1]->target] = 1;
//    }
//    
//    assert(results != NULL);
//    BinaryHeap Q = initHeap(G->nodes_count);
//    HeapNode N = malloc(sizeof(struct THeapNode));
//    int i = 0;
//    int d[G->nodes_count];
//    int prev[G->nodes_count];
//    int old_target = -1;
//    Transport trsp[G->nodes_count];
//    
//    // Inizializzo
//    for (i = 0; i < G->nodes_count; i++) {
//        d[i] = INT_MAX;
//        prev[i] = -1;
//        trsp[i] = NaN;
//    }
//    N->distance = 0;
//    N->target = source;
//    d[source] = 0;
//    
//    // Inserisci nella coda
//    insertHeap(Q, N);
//    
//    // Start!
//    while (!isHeapEmpty(Q)) {
//        N = deleteMinHeap(Q);  // Cancello ed estraggo il minimo
//        old_target = N->target;
//        List L = G->adj[N->target];
//        while (L != NULL) {
//            float dist = 0;
////            printf("BLACK:%d  TARG:%d \n", blacklist[blacklistIndex-2]->target, old_target);
//            if (L->min[index] != -1) {
//                if (index != 2) {
//                    dist = d[N->target] + ((int*)L->awsm[index])[L->min[index]];
//                } else {
//                    dist = d[N->target] + ((float*)L->awsm[index])[L->min[index]];
//                }
//            } else {
//                dist = INT_MAX;
//            }
//            if (blacklist[old_target][L->target] == 1) {
//                dist = INT_MAX;  // Esclusione
//            }
//            if (dist < d[L->target]) {
//                HeapNode v = malloc(sizeof(struct THeapNode));
//                d[L->target] = dist;
//                prev[L->target] = N->target;
//                trsp[L->target] = L->min[index];
//                v->target = L->target;
//                v->distance = dist;
//                insertHeap(Q, v);
//            }
//            L = L->next;
//        }
//    }
//    
//    i = 0;
//    *resultsIndex = 0;
//    int x = source;
//    int max_cost = 0;
//    int max_time = 0;
//    int weight_alt = 0;
//    findPath(G, prev, d, trsp, target, results, resultsIndex);
//    if (*resultsIndex > 0) {
//        for (i = 0; i < *resultsIndex; i++) {
//            List this_node = findNode(G->adj[x], results[i]->target);
//            max_cost += this_node->cost[results[i]->transport];
//            max_time += this_node->time[results[i]->transport];
//            weight_alt += this_node->weight_alt[results[i]->transport];
//            x = results[i]->target;
//        }
//        weight_alt /= *resultsIndex;
//    
//        if (suggested == 1) {
//            // Percorso consigliato: un percorso è consigliato se costa fino al 10% in più rispetto al percorso
//            // di costo minimo, ma consente un guadagno di tempo del 40% rispetto a quest’ultimo
////            printf("max_cost:%d - old_cost:%d | max_time:%d - old_time:%d \n ", max_cost, old_cost, max_time, old_time);
//            if ((max_cost > old_cost + ((old_cost/100)*10) || max_time > old_time - ((old_time/100)*40))){
//                return dijkstraBlacklist(G, source, index, target, results, resultsIndex, blacklist, results, *resultsIndex, suggested, old_cost, old_time, NULL);
//            }
//        } else if (suggested == 0) {
//            // Percorso alternativo turistico: percorso che ha un grado turistico maggiore
//            // (calcolato come la media dei gradi turistici delle città visitate). Il tempo di percorrenza ed il
//            // costo di tale percorso non devono comunque superare il doppio del tempo di percorrenza e del costo dei migliori
//            // percorsi per tempo di percorrenza e per costo, rispettivamente.
//            if (weight_alt >= *old_weight_alt || max_time > old_time*2 || max_cost > old_cost*2){
//                return dijkstraBlacklist(G, source, index, target, results, resultsIndex, blacklist, results, *resultsIndex, suggested, old_cost, old_time, old_weight_alt);
//            }
//        }
//    }
//}



void dijkstraWithTarget(Graph G, int source, int index, Transport transport, int target, int *cost_res, int *time_res, int X) {
    HeapNode results[G->nodes_count];
    int resultsIndex = 0;
    dijkstra(G, source, index, transport, &target, results, &resultsIndex);
    int i = 0;
    int x = source;
    int max_cost = 0;
    int max_time = 0;
    float requested_best = 0;
    float weight_alt = 0;
    printf("\n ## Miglior percorso calcolato ## \n%s", G->name[source]);
    for (i = 0; i < resultsIndex; i++) {
        if (index == 0) {
            printf("-> {%d} %s(%d km) ", results[i]->transport, G->name[results[i]->target], (results[i]->distance/1000));
        } else if (index == 2){
            printf("-> {%d} %s(%d) Euro ", results[i]->transport, G->name[results[i]->target], results[i]->distance);
        } else if (index == 3) {
            printf("-> {%d} %s(%d ore) ", results[i]->transport, G->name[results[i]->target], results[i]->distance/3600);
        } else {
            printf("-> {%d} %s(%d) ", results[i]->transport, G->name[results[i]->target], results[i]->distance);
        }
        List this_node = findNode(G->adj[x], results[i]->target);
        max_cost += this_node->cost[results[i]->transport];
        max_time += this_node->time[results[i]->transport];
        requested_best += ((this_node->cost[results[i]->transport] + this_node->cost[results[i]->transport]*0.1) * (this_node->time[results[i]->transport] - this_node->time[results[i]->transport]*0.4));
        weight_alt += (float)this_node->weight_alt[results[i]->transport];
        x = results[i]->target;
    }
    weight_alt /= resultsIndex;
    printf(" [%d Euro - %d ore - %d g.t.]", max_cost, max_time/3600, (int)weight_alt);
    if (cost_res != NULL && time_res != NULL) {
        *cost_res = max_cost;
        *time_res = max_time;
    }
    printf("\n");
    
    // Cerca percorso consigliato
    if (X == 1 || X == 2) {
        HeapNode new_results[20];
        int new_resultsIndex = 0;
        int x = source;
        
        dijkstraR(G, source, &target, new_results, &new_resultsIndex, requested_best);
    
        int i = 0;
        int new_max_cost = 0;
        int new_max_time = 0;
        float new_weight_alt = 0;
//        index = 5;
        index = 0;
        int new_max_weight = 0;
        if (new_resultsIndex != 0) {
            printf("\n ## Percorso consigliato ## \n%s", G->name[source]);
            for (i = 0; i < new_resultsIndex; i++) {
                List this_node = findNode(G->adj[x], new_results[i]->target);
                if (index == 0) {
                    new_max_weight += this_node->weight[new_results[i]->transport];
                    printf("-> {%d} %s(%d km) ", new_results[i]->transport, G->name[new_results[i]->target], (new_max_weight/1000));
                } else if (index == 2){
                    printf("-> {%d} %s(%d) Euro ", new_results[i]->transport, G->name[new_results[i]->target], new_results[i]->distance);
                } else if (index == 3) {
                    printf("-> {%d} %s(%d ore) ", new_results[i]->transport, G->name[new_results[i]->target], new_results[i]->distance/3600);
                } else {
                    printf("-> {%d} %s(%d) ", new_results[i]->transport, G->name[new_results[i]->target], new_results[i]->distance);
                }
                
                new_max_cost += this_node->cost[new_results[i]->transport];
                new_max_time += this_node->time[new_results[i]->transport];
                new_weight_alt += (float)this_node->weight_alt[new_results[i]->transport];
                x = new_results[i]->target;
            }
            new_weight_alt /= new_resultsIndex;
            printf(" [%d Euro - %d ore - %d g.t.]", new_max_cost, new_max_time/3600, (int)new_weight_alt);
        } else {
            printf("Il percorso trovato e' anche il percorso consigliato! \n");
        }
    }
//    if (X == 0 || X == 2) {
//        HeapNode new_results[20];
//        int new_resultsIndex = 0;
//        int x = source;
//        index = 1;
//        int blacklist[20][20];
//        dijkstraBlacklist(G, source, index, target, new_results, &new_resultsIndex, blacklist, results, resultsIndex, 0, max_cost, max_time, &weight_alt);
//        int i = 0;
//        int new_max_cost = 0;
//        int new_max_time = 0;
//        float new_weight_alt = 0;
//        if (new_resultsIndex != 0) {
//            printf("\n ## Percorso turistico ## \n%s", G->name[source]);
//            for (i = 0; i < new_resultsIndex; i++) {
//                if (index == 0) {
//                    printf("-> {%d} %s(%d km) ", new_results[i]->transport, G->name[new_results[i]->target], (new_results[i]->distance/1000));
//                } else if (index == 2){
//                    printf("-> {%d} %s(%d) Euro ", new_results[i]->transport, G->name[new_results[i]->target], new_results[i]->distance);
//                } else if (index == 3) {
//                    printf("-> {%d} %s(%d ore) ", new_results[i]->transport, G->name[new_results[i]->target], new_results[i]->distance/3600);
//                } else {
//                    printf("-> {%d} %s(%d) ", new_results[i]->transport, G->name[new_results[i]->target], new_results[i]->distance);
//                }
//                List this_node = findNode(G->adj[x], new_results[i]->target);
//                new_max_cost += this_node->cost[new_results[i]->transport];
//                new_max_time += this_node->time[new_results[i]->transport];
//                new_weight_alt += (float)this_node->weight_alt[new_results[i]->transport];
//                x = new_results[i]->target;
//            }
//            new_weight_alt /= new_resultsIndex;
//            printf(" [%d Euro - %d ore - %d g.t.]", new_max_cost, new_max_time/3600, (int)new_weight_alt);
//        }
//    }
}


//int** makeBlacklist(Graph G) {
//    int **blacklist = calloc(G->nodes_count+1, sizeof(int));
//    int i = 0;
//    for (i = 0; i < G->nodes_count; i++) {
//        blacklist[i] = calloc(G->nodes_count+1, sizeof(int));
//    }
////    printf("AAAAAAAAAA: %d", blacklist[13][2]);
//    return blacklist;
//}


void showPath(Graph G, int prev[], int d[], int i) {
    if (prev[i] == i) {
        printf("%s", G->name[i]);
    } else if (prev[i] == -1) {
        printf("%s()", G->name[i]);
    } else {
        showPath(G, prev, d, prev[i]);
        printf("-> %s(%d) ", G->name[i], d[i]);
    }
}


void findPath(Graph G, int prev[], int d[], Transport trsp[], int i, HeapNode results[], int *resultsIndex) {
    if (prev[i] == i) {
        results[*resultsIndex] = malloc(sizeof(struct THeapNode));
        results[*resultsIndex]->target = i;
        results[*resultsIndex]->distance = d[i];
        results[*resultsIndex]->transport = trsp[i];
    } else if (prev[i] == -1) {
        results[*resultsIndex] = malloc(sizeof(struct THeapNode));
        results[*resultsIndex]->target = -1;
        results[*resultsIndex]->distance = -1;
        results[*resultsIndex]->transport = trsp[i];
    } else {
        findPath(G, prev, d, trsp, prev[i], results, resultsIndex);
        results[*resultsIndex] = malloc(sizeof(struct THeapNode));
        results[*resultsIndex]->target = i;
        results[*resultsIndex]->distance = d[i];
        results[*resultsIndex]->transport = trsp[i];
        *resultsIndex += 1;
    }
}



// ---------------------------------------------------- //
// Sample project Init                                  //
// ---------------------------------------------------- //


Graph initGraphProject() {
    int x = 0;
    int Londra[60] = {1, 1093624, 38208, 2, 1712844, 58340, 3, 3199223, 109175, 4, 1861357, 62166, 5, 456802, 18510, 6, 2583700, 93224, 7, 922389, 32725, 13, 1272255, 43498, 14, 577122, 22380, 15, 2054404, 67484, 16, 1826419, 60419, 17, 1891246, 68691, 18, 2369875, 110879, 19, 583747, 27003, -1 };
    int Berlino[60] = {8, 569235, 18350, 9, 872235, 29292, 10, 678237, 24243, 11, 1330886, 43756, 12, 1037505, 34843, 13, 349429, 12851, 14, 573367, 18554, 15, 1703861, 53565, 16, 2204812, 68636, 17, 1081569, 43022, 18, 1618990, 83544, 19, 1690038, 63270, -1 };
    int Madrid[60] = {14, 1749365, 57039, 15, 2152124, 67548, 16, 357271, 12000, 17, 3135668, 105466, 18, 3614299, 147654, 19, 2311741, 83450, -1 };
    int Atene[60] = {0, 3193163, 107738, 7, 2637637, 87268, 8, 2346555, 81356, 9, 1476040, 51016, 10, 1716015, 58351, 11, 2412250, 80016, 12, 2123561, 71189, 13, 1997945, 66915, 18, 3419503, 146946, 19, 3791959, 133205, -1 };
    int Roma[60] = {0, 1862099, 62117, 1, 1512149, 48258, 2, 1959993, 62460, 3, 1312507, 74047, 5, 1419534, 45900, 6, 1882903, 69880, 7, 1668300, 54077, 8, 1804732, 61624, 9, 1226361, 39621, 10, 1131060, 36189, 11, 687112, 22844, 12, 582395, 19232, 13, 1311838, 42143, 14, 1407991, 47138, 15, 226269, 7876, 16, 1703946, 53896, 17, 2552636, 88580, 18, 2877686, 127214, 19, 2460893, 87584, -1 };
    int Parigi[60] = {0, 454418, 18336, 6, 2347143, 82757, 7, 900421, 29225, 14, 482971, 16763, 15, 1610334, 51037, 16, 1376152, 43070, 17, 1869278, 65190, 18, 2347907, 107378, 19, 1053214, 43803, -1 };
    int Bucarest[60] = { 15, 2074714, 75337, 16, 2921737, 101442, 17, 2773941, 106892, 18, 2376688, 129692, 19, 3141209, 118763, -1};
    int Amburgo[60] = {3, 2645748, 89200, 4, 1701349, 53986, 8, 848628, 26605, 9, 1167583, 37739, 10, 973585, 32690, 11, 1404743, 45851, 12, 1111362, 36938, 13, 644775, 21298, 14, 424755, 13826, 18, 1456075, 79229, 19, 1518758, 58008, -1 };
    int Varsavia[60] = {14, 1112968, 34043, 15, 1998989, 66736, 16, 2744414, 84125, 17, 1647075, 59676, 18, 1056029, 66252, 19, 2229636, 78759, -1 };
    int Budapest[60] = {5, 1485793, 46938, 6, 865680, 36507, 7, 1164864, 37166, 8, 873781, 31254, 10, 243241, 8249, 11, 1250738, 39937, 12, 962049, 31110, 13, 525170, 16813, -1 };
    int Vienna[60] = {0, 1471015, 50112, 15, 1323658, 41429, 16, 2170679, 67534, 17, 1757853, 66263, 18, 1757373, 92024, 19, 2069813, 75579, -1 };
    int Monaco[60] = {0, 1414031, 46740, 1, 1332613, 44134, 2, 1280709, 40849, 3, 2413484, 80663, 4, 685026, 22933, 5, 954618, 30031, 6, 1903811, 70569, 12, 304024, 11084, 19, 2012830, 72207, -1 };
    int Milano[60] = {0, 1293019, 44263, 1, 1035046, 34572, 2, 1576269, 50460, 3, 2127788, 71797, 4, 582373, 19396, 5, 850453, 28046, 6, 1618117, 61703, 7, 1108087, 36652, 8, 1567846, 50019, 9, 961576, 31444, 10, 866274, 28012, 11, 303385, 10844, 13, 869837, 29612, 14, 825294, 29065, 15, 775422, 24714, 19, 1891817, 69730, -1 };
    int Praga[60] = { 1, 350369, 12360, 2, 2311327, 72502, 4, 1308198, 42240, 5, 1030735, 32580, 6, 1387427, 52467, 8, 681885, 27241, 15, 1501245, 47558, 16, 2055278, 63938, 17, 1427606, 54725, 18, 1731640, 92435, 19, 1864485, 68809, -1 };
    int Colonia[60] = {13, 696900, 22334, 15, 1600228, 51842, 16, 1721117, 54114, 17, 1393956, 49892, 18, 1872583, 92080, 19, 1176519, 47658, -1 };
    int Napoli[60] = {7, 1861304, 59480, 8, 1997739, 67027, 9, 1419366, 45024, 10, 1324067, 41592, 11, 880117, 28246, 12, 775401, 24635, 17, 2745643, 93983, 18, 3070689, 132617, 19, 2653900, 92986, -1 };
    int Valencia[60] = {0, 1827238, 60119,  2, 354961, 12163, 3, 3430729, 111747, 5, 1377041, 42992, 7, 2116906, 66855, 8, 2739529, 84374, 9, 2264505, 71394, 10, 2169208, 67962, 11, 1024326, 32463, 14, 1729250, 54253, 15, 1895315, 59335, 19, 2426032, 85586, -1 };
    int Stoccolma[60] = {0, 1890804, 68599, 8, 1643832, 60198, 9, 1951911, 71904, 10, 1757913, 66855, 11, 2375580, 81909, 12, 2082204, 72996, 15, 2747523, 94406, 16, 3126194, 103097, 19, 2489598, 94066, -1 };
    int Helsinki[60] = {7, 1457071, 79124, 8, 1055549, 66111, 9, 1943791, 96446, 10, 1754335, 91547, 11, 2854212, 124017, 12, 2560835, 115104, 15, 3068548, 132043, 16, 3604819, 145205, 17, 479910, 42782, 19, 2968230, 136174, -1 };
    int Dublino[60] = {0, 586395, 27078, 7, 1522658, 58085, 8, 2226691, 79015, 9, 2321323, 83074, 10, 2072698, 75619, 11, 2014207, 72583, 12, 1898066, 70079, 13, 1872522, 68858, 14, 1177390, 47740, 15, 2654669, 92844, 16, 2426688, 85779, 17, 2491516, 94051, 18, 2970147, 136239, -1 };
    
    Graph G = initGraph(20);
    G->name[0] = "Londra";
    x = 0;
    srand((unsigned int)(time(NULL)));
    while (Londra[x] != -1) {
        addEdge(G, 0, Londra[x]);
        int j = 0;
        for (j = 0; j < 5; j++) {
            Transport transport = j;
            int target = Londra[x];
            int weight = Londra[x+1];
            int time = (int)Londra[x+2];
            float cost = randomCost(transport, weight);
			int weight_alt = randomWeightAlt();
			if (transport == pullman || transport == taxi) {
                setNodeData(G, 0, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == treno){
                // abbassa del 20% il tempo
                time -= (time/100)*(rand() % 20);
                setNodeData(G, 0, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == aereo){
                //abbassa del 60% il tempo
                time -= (time/100)*(rand() % 70);
                setNodeData(G, 0, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == nave){
                // alza il tempo del 2% e abbassa il km del 5%
                time += (time/100)*2;
                weight -= (weight/100)*5;
                setNodeData(G, 0, target, &transport, &weight, &weight_alt, &cost, &time);
            }
        }
        x += 3;
    }
    G->name[1] = "Berlino";
    x = 0;
    while (Berlino[x] != -1) {
        addEdge(G, 1, Berlino[x]);
        int j = 0;
        for (j = 0; j < 5; j++) {
            Transport transport = j;
            int target = Berlino[x];
            int weight = Berlino[x+1];
            int time = (int)Berlino[x+2];
            float cost = randomCost(transport, weight);
			int weight_alt = randomWeightAlt();
			if (transport == pullman || transport == taxi) {
                setNodeData(G, 1, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == treno){
                // abbassa del 20% il tempo
                time -= (time/100)*(rand() % 20);
                setNodeData(G, 1, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == aereo){
                //abbassa del 60% il tempo
                time -= (time/100)*(rand() % 70);
                setNodeData(G, 1, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == nave){
                // alza il tempo del 2% e abbassa il km del 5%
                time += (time/100)*2;
                setNodeData(G, 1, target, &transport, &weight, &weight_alt, &cost, &time);
            }
        }
        x += 3;
    }
    G->name[2] = "Madrid";
    x = 0;
    while (Madrid[x] != -1) {
        addEdge(G, 2, Madrid[x]);
        int j = 0;
        for (j = 0; j < 5; j++) {
            Transport transport = j;
            int target = Madrid[x];
            int weight = Madrid[x+1];
            int time = (int)Madrid[x+2];
            float cost = randomCost(transport, weight);
			int weight_alt = randomWeightAlt();
			if (transport == pullman || transport == taxi) {
                setNodeData(G, 2, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == treno){
                // abbassa del 20% il tempo
                time -= (time/100)*(rand() % 20);
                setNodeData(G, 2, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == aereo){
                //abbassa del 60% il tempo
                time -= (time/100)*(rand() % 70);
                setNodeData(G, 2, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == nave){
                // alza il tempo del 2% e abbassa il km del 5%
                time += (time/100)*2;
                weight -= (weight/100)*5;
                setNodeData(G, 2, target, &transport, &weight, &weight_alt, &cost, &time);
            }
        }
        x += 3;
    }
    G->name[3] = "Atene";
    x = 0;
    while (Atene[x] != -1) {
        addEdge(G, 3, Atene[x]);
        int j = 0;
        for (j = 0; j < 5; j++) {
            Transport transport = j;
            int target = Atene[x];
            int weight = Atene[x+1];
            int time = (int)Atene[x+2];
            float cost = randomCost(transport, weight);
			int weight_alt = randomWeightAlt();
			if (transport == pullman || transport == taxi) {
                setNodeData(G, 3, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == treno){
                // abbassa del 20% il tempo
                time -= (time/100)*(rand() % 20);
                setNodeData(G, 3, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == aereo){
                //abbassa del 60% il tempo
                time -= (time/100)*(rand() % 70);
                setNodeData(G, 3, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == nave){
                // alza il tempo del 2% e abbassa il km del 5%
                time += (time/100)*2;
                weight -= (weight/100)*5;
                setNodeData(G, 3, target, &transport, &weight, &weight_alt, &cost, &time);
            }
        }
        x += 3;
    }
    G->name[4] = "Roma";
    x = 0;
    while (Roma[x] != -1) {
        addEdge(G, 4, Roma[x]);
        int j = 0;
        for (j = 0; j < 5; j++) {
            Transport transport = j;
            int target = Roma[x];
            int weight = Roma[x+1];
            int time = (int)Roma[x+2];
            float cost = randomCost(transport, weight);
			int weight_alt = randomWeightAlt();
			if (transport == pullman || transport == taxi) {
                setNodeData(G, 4, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == treno){
                // abbassa del 20% il tempo
                time -= (time/100)*(rand() % 20);
                setNodeData(G, 4, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == aereo){
                //abbassa del 60% il tempo
                time -= (time/100)*(rand() % 70);
                setNodeData(G, 4, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == nave){
                // alza il tempo del 2% e abbassa il km del 5%
                time += (time/100)*2;
                weight -= (weight/100)*5;
                setNodeData(G, 4, target, &transport, &weight, &weight_alt, &cost, &time);
            }
        }
        x += 3;
    }
    G->name[5] = "Parigi";
    x = 0;
    while (Parigi[x] != -1) {
        addEdge(G, 5, Parigi[x]);
        int j = 0;
        for (j = 0; j < 5; j++) {
            Transport transport = j;
            int target = Parigi[x];
            int weight = Parigi[x+1];
            int time = (int)Parigi[x+2];
            float cost = randomCost(transport, weight);
			int weight_alt = randomWeightAlt();
			if (transport == pullman || transport == taxi) {
                setNodeData(G, 5, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == treno){
                // abbassa del 20% il tempo
                time -= (time/100)*(rand() % 20);
                setNodeData(G, 5, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == aereo){
                //abbassa del 60% il tempo
                time -= (time/100)*(rand() % 70);
                setNodeData(G, 5, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == nave){
                // alza il tempo del 2% e abbassa il km del 5%
                time += (time/100)*2;
                weight -= (weight/100)*5;
                setNodeData(G, 5, target, &transport, &weight, &weight_alt, &cost, &time);
            }
        }
        x += 3;
    }
    G->name[6] = "Bucarest";
    x = 0;
    while (Bucarest[x] != -1) {
        addEdge(G, 6, Bucarest[x]);
        int j = 0;
        for (j = 0; j < 5; j++) {
            Transport transport = j;
            int target = Bucarest[x];
            int weight = Bucarest[x+1];
            int time = (int)Bucarest[x+2];
            float cost = randomCost(transport, weight);
			int weight_alt = randomWeightAlt();
			if (transport == pullman || transport == taxi) {
                setNodeData(G, 6, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == treno){
                // abbassa del 20% il tempo
                time -= (time/100)*(rand() % 20);
                setNodeData(G, 6, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == aereo){
                //abbassa del 60% il tempo
                time -= (time/100)*(rand() % 70);
                setNodeData(G, 6, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == nave){
                // alza il tempo del 2% e abbassa il km del 5%
                time += (time/100)*2;
                weight -= (weight/100)*5;
                setNodeData(G, 6, target, &transport, &weight, &weight_alt, &cost, &time);
            }
        }
        x += 3;
    }
    G->name[7] = "Amburgo";
    x = 0;
    while (Amburgo[x] != -1) {
        addEdge(G, 7, Amburgo[x]);
        int j = 0;
        for (j = 0; j < 5; j++) {
            Transport transport = j;
            int target = Amburgo[x];
            int weight = Amburgo[x+1];
            int time = (int)Amburgo[x+2];
            float cost = randomCost(transport, weight);
			int weight_alt = randomWeightAlt();
			if (transport == pullman || transport == taxi) {
                setNodeData(G, 7, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == treno){
                // abbassa del 20% il tempo
                time -= (time/100)*(rand() % 20);
                setNodeData(G, 7, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == aereo){
                //abbassa del 60% il tempo
                time -= (time/100)*(rand() % 70);
                setNodeData(G, 7, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == nave){
                // alza il tempo del 2% e abbassa il km del 5%
                time += (time/100)*2;
                weight -= (weight/100)*5;
                setNodeData(G, 7, target, &transport, &weight, &weight_alt, &cost, &time);
            }
        }
        x += 3;
    }
    G->name[8] = "Varsavia";
    x = 0;
    while (Varsavia[x] != -1) {
        addEdge(G, 8, Varsavia[x]);
        int j = 0;
        for (j = 0; j < 5; j++) {
            Transport transport = j;
            int target = Varsavia[x];
            int weight = Varsavia[x+1];
            int time = (int)Varsavia[x+2];
            float cost = randomCost(transport, weight);
			int weight_alt = randomWeightAlt();
			if (transport == pullman || transport == taxi) {
                setNodeData(G, 8, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == treno){
                // abbassa del 20% il tempo
                time -= (time/100)*(rand() % 20);
                setNodeData(G, 8, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == aereo){
                //abbassa del 60% il tempo
                time -= (time/100)*(rand() % 70);
                setNodeData(G, 8, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == nave){
                // alza il tempo del 2% e abbassa il km del 5%
                time += (time/100)*2;
                weight -= (weight/100)*5;
                setNodeData(G, 8, target, &transport, &weight, &weight_alt, &cost, &time);
            }
        }
        x += 3;
    }
    G->name[9] = "Budapest";
    x = 0;
    while (Budapest[x] != -1) {
        addEdge(G, 9, Budapest[x]);
        int j = 0;
        for (j = 0; j < 5; j++) {
            Transport transport = j;
            int target = Budapest[x];
            int weight = Budapest[x+1];
            int time = (int)Budapest[x+2];
            float cost = randomCost(transport, weight);
			int weight_alt = randomWeightAlt();
			if (transport == pullman || transport == taxi) {
                setNodeData(G, 9, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == treno){
                // abbassa del 20% il tempo
                time -= (time/100)*(rand() % 20);
                setNodeData(G, 9, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == aereo){
                //abbassa del 60% il tempo
                time -= (time/100)*(rand() % 70);
                setNodeData(G, 9, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == nave){
                // alza il tempo del 2% e abbassa il km del 5%
                time += (time/100)*2;
                weight -= (weight/100)*5;
                setNodeData(G, 9, target, &transport, &weight, &weight_alt, &cost, &time);
            }
        }
        x += 3;
    }
    G->name[10] = "Vienna";
    x = 0;
    while (Vienna[x] != -1) {
        addEdge(G, 10, Vienna[x]);
        int j = 0;
        for (j = 0; j < 5; j++) {
            Transport transport = j;
            int target = Vienna[x];
            int weight = Vienna[x+1];
            int time = (int)Vienna[x+2];
            float cost = randomCost(transport, weight);
			int weight_alt = randomWeightAlt();
			if (transport == pullman || transport == taxi) {
                setNodeData(G, 10, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == treno){
                // abbassa del 20% il tempo
                time -= (time/100)*(rand() % 20);
                setNodeData(G, 10, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == aereo){
                //abbassa del 60% il tempo
                time -= (time/100)*(rand() % 70);
                setNodeData(G, 10, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == nave){
                // alza il tempo del 2% e abbassa il km del 5%
                time += (time/100)*2;
                weight -= (weight/100)*5;
                setNodeData(G, 10, target, &transport, &weight, &weight_alt, &cost, &time);
            }
        }
        x += 3;
    }
    G->name[11] = "Monaco";
    x = 0;
    while (Monaco[x] != -1) {
        addEdge(G, 11, Monaco[x]);
        int j = 0;
        for (j = 0; j < 5; j++) {
            Transport transport = j;
            int target = Monaco[x];
            int weight = Monaco[x+1];
            int time = (int)Monaco[x+2];
            float cost = randomCost(transport, weight);
			int weight_alt = randomWeightAlt();
			if (transport == pullman || transport == taxi) {
                setNodeData(G, 11, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == treno){
                // abbassa del 20% il tempo
                time -= (time/100)*(rand() % 20);
                setNodeData(G, 11, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == aereo){
                //abbassa del 60% il tempo
                time -= (time/100)*(rand() % 70);
                setNodeData(G, 11, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == nave){
                // alza il tempo del 2% e abbassa il km del 5%
                time += (time/100)*2;
                weight -= (weight/100)*5;
                setNodeData(G, 11, target, &transport, &weight, &weight_alt, &cost, &time);
            }
        }
        x += 3;
    }
    G->name[12] = "Milano";
    x = 0;
    while (Milano[x] != -1) {
        addEdge(G, 12, Milano[x]);
        int j = 0;
        for (j = 0; j < 5; j++) {
            Transport transport = j;
            int target = Milano[x];
            int weight = Milano[x+1];
            int time = (int)Milano[x+2];
            float cost = randomCost(transport, weight);
			int weight_alt = randomWeightAlt();
			if (transport == pullman || transport == taxi) {
                setNodeData(G, 12, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == treno){
                // abbassa del 20% il tempo
                time -= (time/100)*(rand() % 20);
                setNodeData(G, 12, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == aereo){
                //abbassa del 60% il tempo
                time -= (time/100)*(rand() % 70);
                setNodeData(G, 12, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == nave){
                // alza il tempo del 2% e abbassa il km del 5%
                time += (time/100)*2;
                weight -= (weight/100)*5;
                setNodeData(G, 12, target, &transport, &weight, &weight_alt, &cost, &time);
            }
        }
        x += 3;
    }
    G->name[13] = "Praga";
    x = 0;
    while (Praga[x] != -1) {
        addEdge(G, 13, Praga[x]);
        int j = 0;
        for (j = 0; j < 5; j++) {
            Transport transport = j;
            int target = Praga[x];
            int weight = Praga[x+1];
            int time = (int)Praga[x+2];
            float cost = randomCost(transport, weight);
			int weight_alt = randomWeightAlt();
			if (transport == pullman || transport == taxi) {
                setNodeData(G, 13, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == treno){
                // abbassa del 20% il tempo
                time -= (time/100)*(rand() % 20);
                setNodeData(G, 13, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == aereo){
                //abbassa del 60% il tempo
                time -= (time/100)*(rand() % 70);
                setNodeData(G, 13, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == nave){
                // alza il tempo del 2% e abbassa il km del 5%
                time += (time/100)*2;
                weight -= (weight/100)*5;
                setNodeData(G, 13, target, &transport, &weight, &weight_alt, &cost, &time);
            }
        }
        x += 3;
    }
    G->name[14] = "Colonia";
    x = 0;
    while (Colonia[x] != -1) {
        addEdge(G, 14, Colonia[x]);
        int j = 0;
        for (j = 0; j < 5; j++) {
            Transport transport = j;
            int target = Colonia[x];
            int weight = Colonia[x+1];
            int time = (int)Colonia[x+2];
            float cost = randomCost(transport, weight);
			int weight_alt = randomWeightAlt();
			if (transport == pullman || transport == taxi) {
                setNodeData(G, 14, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == treno){
                // abbassa del 20% il tempo
                time -= (time/100)*(rand() % 20);
                setNodeData(G, 14, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == aereo){
                //abbassa del 60% il tempo
                time -= (time/100)*(rand() % 70);
                setNodeData(G, 14, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == nave){
                // alza il tempo del 2% e abbassa il km del 5%
                time += (time/100)*2;
                weight -= (weight/100)*5;
                setNodeData(G, 14, target, &transport, &weight, &weight_alt, &cost, &time);
            }
        }
        x += 3;
    }
    G->name[15] = "Napoli";
    x = 0;
    while (Napoli[x] != -1) {
        addEdge(G, 15, Napoli[x]);
        int j = 0;
        for (j = 0; j < 5; j++) {
            Transport transport = j;
            int target = Napoli[x];
            int weight = Napoli[x+1];
            int time = (int)Napoli[x+2];
            float cost = randomCost(transport, weight);
			int weight_alt = randomWeightAlt();
			if (transport == pullman || transport == taxi) {
                setNodeData(G, 15, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == treno){
                // abbassa del 20% il tempo
                time -= (time/100)*(rand() % 20);
                setNodeData(G, 15, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == aereo){
                //abbassa del 60% il tempo
                time -= (time/100)*(rand() % 70);
                setNodeData(G, 15, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == nave){
                // alza il tempo del 2% e abbassa il km del 5%
                time += (time/100)*2;
                weight -= (weight/100)*5;
                setNodeData(G, 15, target, &transport, &weight, &weight_alt, &cost, &time);
            }
        }
        x += 3;
    }
    G->name[16] = "Valencia";
    x = 0;
    while (Valencia[x] != -1) {
        addEdge(G, 16, Valencia[x]);
        int j = 0;
        for (j = 0; j < 5; j++) {
            Transport transport = j;
            int target = Valencia[x];
            int weight = Valencia[x+1];
            int time = (int)Valencia[x+2];
            float cost = randomCost(transport, weight);
			int weight_alt = randomWeightAlt();
			if (transport == pullman || transport == taxi) {
                setNodeData(G, 16, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == treno){
                // abbassa del 20% il tempo
                time -= (time/100)*(rand() % 20);
                setNodeData(G, 16, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == aereo){
                //abbassa del 60% il tempo
                time -= (time/100)*(rand() % 70);
                setNodeData(G, 16, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == nave){
                // alza il tempo del 2% e abbassa il km del 5%
                time += (time/100)*2;
                weight -= (weight/100)*5;
                setNodeData(G, 16, target, &transport, &weight, &weight_alt, &cost, &time);
            }
        }
        x += 3;
    }
    G->name[17] = "Stoccolma";
    x = 0;
    while (Stoccolma[x] != -1) {
        addEdge(G, 17, Stoccolma[x]);
        int j = 0;
        for (j = 0; j < 5; j++) {
            Transport transport = j;
            int target = Stoccolma[x];
            int weight = Stoccolma[x+1];
            int time = (int)Stoccolma[x+2];
            float cost = randomCost(transport, weight);
			int weight_alt = randomWeightAlt();
			if (transport == pullman || transport == taxi) {
                setNodeData(G, 17, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == treno){
                // abbassa del 20% il tempo
                time -= (time/100)*(rand() % 20);
                setNodeData(G, 17, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == aereo){
                //abbassa del 60% il tempo
                time -= (time/100)*(rand() % 70);
                setNodeData(G, 17, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == nave){
                // alza il tempo del 2% e abbassa il km del 5%
                time += (time/100)*2;
                weight -= (weight/100)*5;
                setNodeData(G, 17, target, &transport, &weight, &weight_alt, &cost, &time);
            }
        }
        x += 3;
    }
    G->name[18] = "Helsinki";
    x = 0;
    while (Helsinki[x] != -1) {
        addEdge(G, 18, Helsinki[x]);
        int j = 0;
        for (j = 0; j < 5; j++) {
            Transport transport = j;
            int target = Helsinki[x];
            int weight = Helsinki[x+1];
            int time = (int)Helsinki[x+2];
            float cost = randomCost(transport, weight);
			int weight_alt = randomWeightAlt();
			if (transport == pullman || transport == taxi) {
                setNodeData(G, 18, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == treno){
                // abbassa del 20% il tempo
                time -= (time/100)*(rand() % 20);
                setNodeData(G, 18, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == aereo){
                //abbassa del 60% il tempo
                time -= (time/100)*(rand() % 70);
                setNodeData(G, 18, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == nave){
                // alza il tempo del 2% e abbassa il km del 5%
                time += (time/100)*2;
                weight -= (weight/100)*5;
                setNodeData(G, 18, target, &transport, &weight, &weight_alt, &cost, &time);
            }
        }
        x += 3;
    }
    G->name[19] = "Dublino";
    x = 0;
    while (Dublino[x] != -1) {
        addEdge(G, 19, Dublino[x]);
        int j = 0;
        for (j = 0; j < 5; j++) {
            Transport transport = j;
            int target = Dublino[x];
            int weight = Dublino[x+1];
            int time = (int)Dublino[x+2];
            float cost = randomCost(transport, weight);
			int weight_alt = randomWeightAlt();
			if (transport == pullman || transport == taxi) {
                setNodeData(G, 19, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == treno){
                // abbassa del 20% il tempo
                time -= (time/100)*(rand() % 20);
                setNodeData(G, 19, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == aereo){
                //abbassa del 60% il tempo
                time -= (time/100)*(rand() % 70);
                setNodeData(G, 19, target, &transport, &weight, &weight_alt, &cost, &time);
            } else if (transport == nave){
                // alza il tempo del 2% e abbassa il km del 5%
                time += (time/100)*2;
                weight -= (weight/100)*5;
                setNodeData(G, 19, target, &transport, &weight, &weight_alt, &cost, &time);
            }
        }
        x += 3;
    }
    return G;
};
