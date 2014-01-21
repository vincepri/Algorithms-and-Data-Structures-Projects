//
//  NTree.c
//  NTrees
//
//  Created by Vincenzo on 5/3/13.
//  Copyright (c) 2013 whatever. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "NTree.h"
#include <math.h>

// Inizializza un nodo
NTree init_node(int info){
    NTree N = malloc(sizeof(struct TNTree));
    N->info = info;
    N->siblings = NULL;
    N->children = NULL;
    return N;
}


// Inizializza radice con info a -1
NTree init_root(){
    NTree N = malloc(sizeof(struct TNTree));
    N->info = -1;
    N->siblings = NULL;
    N->children = NULL;
    return N;
}


// Inserimento nell'albero n-ario
void insert_in_tree(NTree N, int value){
    int index = -1;
    int chosen_index = 0;
    NTree indexes[MAX_NODES];
    print_available_fathers(N, indexes, &index, find_digits(value) - 1);  // -1 because we need fathers
    if (index >= 0) {
        do {
            printf("\nInserisci l'indice del padre dove inserire: ");
            scanf("%d", &chosen_index);
        } while (chosen_index > index);
        indexes[chosen_index]->children = insert_in_chosen_father(indexes[chosen_index]->children, value);
    } else {
        printf("Non sono stati trovati padri adatti al valore.\n");
    }
}


// Stampa i valori disponibili come padri del valore da inserire
void print_available_fathers(NTree N, NTree indexes[], int *index, int h){
    if (N != NULL) {
        if (h == 0) {
            *index += 1;
            indexes[*index] = N;
            printf("#%d: %d  ", *index, N->info);
        } else {
            print_available_fathers(N->children, indexes, index, h - 1);
        }
        print_available_fathers(N->siblings, indexes, index, h);
    }
}


// Inserisci il valore nel padre scelto
NTree insert_in_chosen_father(NTree N, int value){
    if (N != NULL){
        N->siblings = insert_in_chosen_father(N->siblings, value);
    } else {
        return init_node(value);
    }
    return N;
}


// Cancella le foglie dell'albero n-ario dato in ingresso
NTree delete_leaves(NTree N){
    if (N != NULL) {
        N->siblings = delete_leaves(N->siblings);
        if (N->children == NULL && N->siblings == NULL) {
            free(N);
            return NULL;
        } else if (N->children == NULL && N->siblings != NULL) {
            NTree tmp = N->siblings;
            free(N);
            return tmp;
        } else {
            N->children = delete_leaves(N->children);
        }
    }
    return N;
}


// Cerca un valore nell'albero
void find_value(NTree N, int value, int h, NTree *res){
    if (N != NULL) {
        if (h != find_digits(value)) {
            find_value(N->children, value, h + 1, res);
        } else {
            if (N->info == value) {
                *res = N;
            }
        }
        find_value(N->siblings, value, h, res);
    }
}


// Stampa l'albero
void print_tree(NTree N, int h){
    if (N != NULL) {
        if (h == 0) {
            printf(" %d (Root) \n", N->info);
            print_tree(N->children, h + 1);
        } else {
            printf("%d ", N->info);
            print_tree(N->children, h + 1);
            print_tree(N->siblings, h);
        }
    }
}


// Restituisce le cifre di un numero dato in ingresso
int find_digits(int number){
    return log10(number) + 1;
}


// Cancella i sottoalberi che hanno struttura uguale
void delete_similar_structured_subtrees(NTree Head){
    if (Head != NULL) {
        int max_height = 0;
        int x = 0;
        NTree indexes[MAX_NODES];
        find_max_height(Head, &max_height, 0);  // Trova l'altezza massima dell'albero
        for (x = 1; x < max_height; x++) {  // Cicla per ogni livello
            int index = 0;
            get_all_nodes_at_level(Head, indexes, &index, x);  // Trova tutti i nodi del livello con l'helper
            if (index >= 0) {  // Se trova piu' di 1 nodo allora inizia
                int i = index - 1;
                while (i > 0) {
                    if (indexes[i] != NULL && indexes[i]->children != NULL) {  // Controllo se effettivamente ha un sottoalbero
                        int y = 0;
                        for (y = 0; y < i; y++) {
                            int res = 0;
                            if (indexes[y] != NULL && indexes[i]->info == indexes[y]->info) {
                                compare_subtrees_structure(indexes[i]->children, indexes[y]->children, &res);
                                if (res == 0) {
                                    remove_a_node(Head, indexes[y]);
                                    indexes[y] = NULL;
                                }
                            }
                        }
                    }
                    i -= 1;
                }
            }
        }
    }
}


// Cancella tutti i sottoalberi la cui somma dei suoi nodi e' uguale
// alla somma dei nodi di un altro sottoalbero
void delete_equal_sum_subtrees(NTree Head){
    if (Head != NULL) {
        int max_height = 0;
        int x = 0;
        NTree indexes[MAX_NODES];
        find_max_height(Head, &max_height, 0);  // Trova l'altezza massima dell'albero
        int index = 0;
        for (x = 1; x < max_height; x++) {  // Cicla per ogni livello
            index = 0;
            get_all_nodes_at_level(Head, indexes, &index, x);  // Trova tutti i nodi del livello con l'helper
            if (index >= 0) {  // Se trova piu' di 1 nodo allora inizia
                int indexesResults[index];
                NTree indexesFirst[index];
                int i, y, z = 0;
                for (z = 0; z < index; z++) {  // Inizializzo array
                    indexesResults[z] = indexes[z]->info;
                    indexesFirst[z] = indexes[z]->children;
                }
                int empty = 0;
                sum_all_level_subtrees(indexesFirst, indexesResults, index, &empty);
                for (i = 0; i < index; i++) {
                    for (y = i+1; y < index; y++) {
                        if (indexesResults[y] != 0 && indexesResults[i] == indexesResults[y]) {
                            remove_a_node(Head, indexes[y]);
                            indexesResults[y] = 0;
                        }
                    }
                }
            }
        }
    }
}


// Cancella tutti i sottoalberi replica a meno di permutazione dei nodi, lasciandone uno solo
void delete_equal_values_subtrees(NTree Head){
    if (Head != NULL) {
        int max_height = 0;
        int x = 0;
        NTree indexes[MAX_NODES];
        find_max_height(Head, &max_height, 0);  // Trova l'altezza massima dell'albero
        int index;
        for (x = 1; x < max_height; x++) {  // Cicla per ogni livello
            index = 0;
            get_all_nodes_at_level(Head, indexes, &index, x);  // Trova tutti i nodi del livello con l'helper
            if (index >= 0) {  // Se trova piu' di 1 nodo allora inizia
                int i = index - 1;
                while (i > 0) {
                    if (indexes[i] != NULL && indexes[i]->children != NULL) {  // Controllo se effettivamente ha un sottoalbero
                        int firstValues[MAX_NODES];
                        int firstDim = 0;
                        get_subtrees_values(indexes[i]->children, firstValues, &firstDim);
                        qsort(firstValues, firstDim, sizeof(int), cmpfunc);
                        int y = 0;
                        for (y = 0; y < i; y++) {
                            if (indexes[y] != NULL && indexes[i]->info == indexes[y]->info) {
                                int secondValues[MAX_NODES];
                                int secondDim = 0;
                                get_subtrees_values(indexes[y]->children, secondValues, &secondDim);
                                qsort(secondValues, secondDim, sizeof(int), cmpfunc);
                                if (firstDim == secondDim) {
                                    int z = 0;
                                    int res = 0;
                                    for (z = 0; z < firstDim; z++) {
                                        if (firstValues[z] == secondValues[z]) {
                                            res = 0;
                                        } else {
                                            res = -1;
                                            break;
                                        }
                                    }
                                    if (res == 0) {
                                        remove_a_node(Head, indexes[y]);
                                        indexes[y] = NULL;
                                    }
                                }
                            }
                        }
                    }
                    i -= 1;
                }
            }
        }
    }
}


// Cancella tutti i sottoalberi replica a meno di permutazione dei nodi, lasciandone uno solo
// tiene conto a livello
void delete_equal_values_subtrees_mod(NTree Head){
    if (Head != NULL) {
        int max_height = 0;
        NTree indexes[MAX_NODES];
        find_max_height(Head, &max_height, 0);  // Trova l'altezza massima dell'albero
        int index = 0;
        int x = 0;
        int y = 0;
        get_all_nodes_at_level(Head, indexes, &index, 1);  // Trova tutti i nodi del livello 1 con l'helper
        for (x = index - 1 ; x > 0; x--){
            if (indexes[x] != NULL){
                for (y = 0; y < x; y++){
                    if (indexes[y] != NULL && indexes[x]->info == indexes[y]->info) {
                        int x_height = 0;
                        int y_height = 0;
                        find_max_height(indexes[x], &x_height, 0);
                        find_max_height(indexes[y], &y_height, 0);
                        if (x_height == y_height && delete_equal_values_subtrees_mod_compare(indexes[x], indexes[y], x_height, 0)) {
                            remove_a_node(Head, indexes[y]);
                            indexes[y] = NULL;
                        }
                    }
                }
            }
        }
    }
}

int delete_equal_values_subtrees_mod_compare(NTree X, NTree Y, int height, int t){
    if (t <= height && X != NULL && Y != NULL) {
        printf("\n\n X:%d, Y:%d", X->info, Y->info);
        int index_x = 0;
        int index_y = 0;
        int x = 0;
        int y = 0;
        NTree indexesX[MAX_NODES];
        NTree indexesY[MAX_NODES];
        get_all_nodes_at_level(X->children, indexesX, &index_x, 0);
        get_all_nodes_at_level(Y->children, indexesY, &index_y, 0);
        printf("index_x:%d, index_y:%d \n", index_x, index_y);
        if  (index_x == index_y) {
            printf("index_x:%d, index_y:%d \n", index_x, index_y);
            int found = index_x -1;
            for (x = index_x - 1; x >= 0; x--){
                for (y = index_y - 1; y >= 0; y--){
                    printf("    x:%d, y:%d \n", indexesX[x]->info, indexesY[y]->info);
                    if (indexesX[x]->info == indexesY[y]->info){
                        if (delete_equal_values_subtrees_mod_compare(indexesX[x], indexesY[y], height, t+1)){
                            found -= 1;
                        }
                    }
                }
            }
            if (found <= 0) {
                return 1;
            }
        }
    } else if (X == NULL && Y == NULL) {
        return 1;
    }
    return 0;
}


// Dato un sottoalbero N in ingresso, porta in values[] tutti i nodi del sottoalbero
void get_subtrees_values(NTree N, int values[], int *cursor){
    if (N != NULL) {
        values[*cursor] = N->info;
        *cursor += 1;
        get_subtrees_values(N->children, values, cursor);
        get_subtrees_values(N->siblings, values, cursor);
    }
}


// Dati due nodi confronta che i due sottoalberi siano uguali per valori e per struttura
// la variabile res sara' determinante, se == 0 allora saranno uguali,
// altrimenti no
void compare_subtrees_structure(NTree first, NTree second, int *res){
    if (first != NULL && second != NULL) {
        if (first->info == second->info) {
            compare_subtrees_structure(first->siblings, second->siblings, res);
            compare_subtrees_structure(first->children, second->children, res);
        } else {
            *res += 1;
        }
    } else if (first != NULL && second == NULL){
        *res += 1;
    } else if (first == NULL && second != NULL){
        *res += 1;
    }
}


// Esegue la somma di tutti i sottoalberi dati in ingresso
// tramite l'array indexesFirst[]
void sum_all_level_subtrees(NTree indexesFirst[], int indexesResults[], int dim, int *empty){
    if (*empty < dim) {
        int x = 0;
        NTree tmpSiblings[dim];
        NTree tmpChildren[dim];
        for (x = 0; x < dim; x++) {
            if (indexesFirst[x] != NULL) {
                indexesResults[x] += indexesFirst[x]->info;
                tmpSiblings[x] = indexesFirst[x]->siblings;
                tmpChildren[x] = indexesFirst[x]->children;
            } else {
                *empty += 1;
            }
        }
        sum_all_level_subtrees(tmpSiblings, indexesResults, dim, empty);
        sum_all_level_subtrees(tmpChildren, indexesResults, dim, empty);
    }

}


// Dato un nodo, rimuove tutti i suoi fratelli e rispettivi figli
NTree remove_this_subtree(NTree N){
    if (N != NULL) {
        N->siblings = remove_this_subtree(N->siblings);
        N->children = remove_this_subtree(N->children);
        free(N);
    }
    return NULL;
}


// Dato un nodo rimuove tutti i suoi figli e il nodo stesso
NTree remove_a_node(NTree N, NTree node_to_remove){
    if (N != NULL) {
        N->siblings = remove_a_node(N->siblings, node_to_remove);
        N->children = remove_a_node(N->children, node_to_remove);
        if (N == node_to_remove){
            NTree tmp = N->siblings;
            N->children = remove_this_subtree(N->children);
            free(N);
            return tmp;
        }
    }
    return N;
}


//NTree remove_node_with_value(NTree Head, NTree interesting_node, int value, int *h){
//    if (Head != NULL) {
//        if (h == 0 && interesting_node == NULL){
//            *h = find_digits(value);
//            find_value(Head, value, 0, &interesting_node);
//            remove_node_with_value(Head, interesting_node, value, h);
//        } else {
//            // Now we are going to remove thatttttttttt
//            if (interesting_node->children != NULL && interesting_node->siblings != NULL) {
//                insert_node_as_last_sibling(interesting_node->children, interesting_node->siblings->children);
//                interesting_node->children = NULL;
//            }
//        }
//    }
//}


NTree remove_node_with_value(NTree N, int value, int h, int *found){
    if (N != NULL) {
        if (h != find_digits(value)) {
            N->children = remove_node_with_value(N->children, value, h + 1, found);
        } else {
            if (N->info == value && !*found) {
                *found = 1;
                if (N->children != NULL && N->siblings != NULL) {
                    NTree tmp = N->siblings;
                    N->siblings->children = insert_node_as_last_sibling(N->children, N->siblings->children);
                    N->children = NULL;
                    free(N);
                    return tmp;
                } else if (N->children == NULL && N->siblings == NULL) {
                    free(N);
                    return NULL;
                } else if (N->children == NULL && N->siblings != NULL) {
                    NTree tmp = N->siblings;
                    free(N);
                    return tmp;
                } else {
                    printf("Il nodo non puo' essere eliminato. \n");
                }
            }
        }
        N->siblings = remove_node_with_value(N->siblings, value, h, found);
    }
    return N;
}


NTree insert_node_as_last_sibling(NTree node_to_insert, NTree this_sibling){
    if (node_to_insert != NULL && this_sibling != NULL) {
        this_sibling->siblings = insert_node_as_last_sibling(node_to_insert, this_sibling->siblings);
    } else if (node_to_insert != NULL && this_sibling == NULL){
        return node_to_insert;
    }
    return this_sibling;
}

// Prende tutti i nodi ad un determinato livello h e li restituisce al programma
// chiamante tramite l'array di puntatori indexes[]
void get_all_nodes_at_level(NTree N, NTree indexes[], int *index, int h){
    if (N != NULL) {
        get_all_nodes_at_level(N->siblings, indexes, index, h);
        if (h == 0) {
            indexes[*index] = N;
            *index += 1;
        } else {
            get_all_nodes_at_level(N->children, indexes, index, h - 1);
        }
    }
}


// Trova l'altezza massima dell'albero n-ario
void find_max_height(NTree N, int *res, int h){
    if (N != NULL) {
        find_max_height(N->siblings, res, h);
        if (N->children != NULL) {
            if (*res <= h) {
                *res += 1;
            }
            find_max_height(N->children, res, h + 1);
        }
    }
}


// Funzione di comparazione per qsort
int cmpfunc(const void *a, const void *b){
    return (*(int*)a - *(int*)b);
}
