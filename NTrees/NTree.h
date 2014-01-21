//
//  NTree.h
//  NTrees
//
//  Created by Vincenzo on 5/3/13.
//  Copyright (c) 2013 whatever. All rights reserved.
//

#ifndef NTrees_NTree_h
#define NTrees_NTree_h
#define MAX_NODES 256

struct TNTree {
    int info;
    struct TNTree* siblings;
    struct TNTree* children;
};

typedef struct TNTree* NTree;

NTree init_node(int);
NTree init_root();
void insert_in_tree(NTree, int);
void print_available_fathers(NTree, NTree[], int*, int);
NTree insert_in_chosen_father(NTree, int);
NTree delete_leaves(NTree);
void print_tree(NTree, int);
int find_digits(int);
void find_value(NTree, int, int, NTree*);
void find_max_height(NTree, int*, int);
void compare_subtrees_structure(NTree, NTree second, int*);
void get_all_nodes_at_level(NTree, NTree[], int*, int);
void compare_all_level_subtrees(NTree[], int[], NTree, int);
void delete_similar_structured_subtrees(NTree Head);
void delete_equal_sum_subtrees(NTree Head);
void delete_equal_values_subtrees(NTree Head);
void sum_all_level_subtrees(NTree[], int[], int, int*);
void get_subtrees_values(NTree N, int values[], int *cursor);
int cmpfunc(const void *a, const void *b);
NTree remove_a_node(NTree, NTree);
NTree remove_this_subtree(NTree N);
NTree insert_node_as_last_sibling(NTree node_to_insert, NTree this_sibling);
NTree remove_node_with_value(NTree N, int value, int h, int *found);

void delete_equal_values_subtrees_mod(NTree Head);
int delete_equal_values_subtrees_mod_compare(NTree X, NTree Y, int height, int x);
#endif
