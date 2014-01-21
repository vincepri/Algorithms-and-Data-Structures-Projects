//
//  main.c
//  NTrees
//
//  Created by Vincenzo on 5/3/13.
//  Copyright (c) 2013 whatever. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "NTree.h"

void menu(int*);

int main(int argc, const char * argv[]){
    int choice = 0;
    NTree *Head = (NTree*)malloc(sizeof(NTree));
    do {
        menu(&choice);
        switch (choice) {
            case 1:
            {
                *Head = init_root();
                printf("Inizializzazione effettuata. \n");
            }
                break;
            case 2:
            {
                print_tree(*Head, 0);
            }
                break;
            case 3:
            {
                delete_leaves(*Head);
                printf("Cancellazione di tutti i nodi foglia effettuata. \n");
            }
                break;
            case 4:
            {
                delete_similar_structured_subtrees(*Head);
                printf("Operazione completata. ");
            }
                break;
            case 5:
            {
                printf("\n Attenzione: stando all'esercizio questa funzione cancella i sottoalberi che hanno i valori permutati senza tener conto dei loro padri. \n");
                delete_equal_values_subtrees(*Head);
                printf("Operazione completata. ");
            }
                break;
            case 6:
            {
                delete_equal_sum_subtrees(*Head);
                printf("Operazione completata.");
            }
                break;
            case 7:
            {
                int value_to_remove = 0;
                int found = 0;
                printf("Inserisci il valore da rimuovere: ");
                scanf("%d", &value_to_remove);
                remove_node_with_value(*Head, value_to_remove, 0, &found);
                if (found) {
                    printf("Operazione completata. \n");
                } else {
                    printf("Il nodo non e' presente. \n");
                }
            }
                break;
            case 8:
            {
                int value_to_find = 0;
                NTree res = NULL;
                printf("Inserisci il valore da cercare: ");
                scanf("%d", &value_to_find);
                find_value(*Head, value_to_find, 0, &res);
                if (res != NULL) {
                    printf("Operazione completata. \n");
                } else {
                    printf("Il nodo non e' presente. \n");
                }
            }
                break;
            case 9:
            {
                int value = 0;
                int how_many = 0;
                printf("Quanti valori vuoi aggiungere? ");
                scanf("%d", &how_many);
                while (how_many > 0) {
                    printf("Inserisci il valore da aggiungere: ");
                    scanf("%d", &value);
                    insert_in_tree(*Head, value);
                    how_many -= 1;
                }
            }
                break;
            case 10:
            {
                delete_equal_values_subtrees_mod(*Head);
                printf("Operazione completata. ");
            }
                break;
            case 11:
            {
                #ifdef _WIN32
                system("PAUSE");
                #endif
                return 0;
            }
                break;
        }
    } while (choice > 0 && choice <= 10);
    return 0;
}

void menu(int *choice){
    printf("\n1. Inizializza l'albero\n");
    printf("2. Stampa\n");
    printf("3. Cancella tutti i nodi foglia\n");
    printf("4. Cancella sottoalberi per struttura e contenuto\n");
    printf("5. Cancella sottoalberi permutati\n");
    printf("6. Cancella sottoalberi che hanno somma uguale\n");
    printf("7. Cancella un nodo\n");
    printf("8. Ricerca un nodo\n");
    printf("9. Inserisci\n");
    printf("10. Cancella sottoalberi permutati tenendo conto del padre\n");
    printf("11. Esci");
    printf("\nScelta:");
    scanf("%d", &*choice);
}
