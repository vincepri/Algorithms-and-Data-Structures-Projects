//
//  main.c
//  Graphs
//
//  Created by Vincenzo on 5/26/13.
//  Copyright (c) 2013 whatever. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"


void menu(int*);

int main(int argc, const char * argv[]){
    int choice = 0;
    Graph *Head = malloc(sizeof(Graph));
    do {
        menu(&choice);
        switch (choice) {
            case 1:
            {
                int init_choice = 0;
                printf("1. Inizializza grafo vuoto \n");
                printf("2. Inizializza grafo con SAMPLE \nScelta:");
                scanf("%d", &init_choice);
                if (init_choice == 1) {
                    printf("Quanti nodi vuoi nel grafo? ");
                    int nv = 0;
                    scanf("%d", &nv);
                    *Head = initGraph(nv);
                } else {
                    *Head = initGraphProject();
                }
                printf("Inizializzazione Completata. \n");
            }
                break;
            case 2:
            {
                printf("\n");
                printGraphWithNames(*Head);
                printf("\n");
            }
                break;
            case 3:
            {
                printf("\n");
                DFS(*Head);
                printf("\n");
            }
                break;
            case 4:
            {
                char *s = malloc(sizeof(char) * 20);
                printf("Nome citta': ");
                scanf("%s", s);
                addNode(*Head);
                (*Head)->name[(*Head)->nodes_count-1] = s;
                printf("Operazione completata. ");
            }
                break;
            case 5:
            {
                int node_to_remove = 0;
                do {
                    printf("Inserisci il nodo da rimuovere: ");
                    scanf("%d", &node_to_remove);
                } while (node_to_remove >= (*Head)->nodes_count);
                removeNode(*Head, node_to_remove);
                printf("Operazione completata. ");
            }
                break;
            case 6:
            {
                int source = 0;
                int target = 0;
                printf("Inserisci sorgente: ");
                scanf("%d", &source);
                printf("Inserisci destinazione: ");
                scanf("%d", &target);
                addEdge(*Head, source, target);
            }
                break;
            case 7:
            {
                int source = 0;
                int target = 0;
                printf("Inserisci sorgente: ");
                scanf("%d", &source);
                printf("Inserisci destinazione: ");
                scanf("%d", &target);
                removeEdge(*Head, source, target);
            }
                break;
            case 8:
            {
                int x = 0;
                char *s = malloc(sizeof(char) * 20);
                printf("Inserisci il nome del nodo da cercare: ");
                scanf("%s", s);
                for (x = 0; x < (*Head)->nodes_count; x++) {
                    if ((*Head)->name[x] != NULL && strcmp(s, (*Head)->name[x]) == 0) {
                        if ((*Head)->adj[x] != NULL) {
                            printf("Collegamenti: ");
                            printList((*Head)->adj[x]);
                        } else {
                            printf("Nessun collegamento disponibile per il nodo. \n");
                        }
                        printf("\n");
                        break;
                    }
                }
                if (x == (*Head)->nodes_count) {
                    printf("Nessun nodo trovato con il nome specificato. \n");
                }
            }
                break;
            case 9:
            {
                int to_modify = 0;
                do {
                    printf("Inserisci il nodo da modificare: ");
                    scanf("%d", &to_modify);
                } while (to_modify >= (*Head)->nodes_count);
                printf("Vecchio Nome: %s \n", (*Head)->name[to_modify]);
                char *s = malloc(sizeof(char) * 20);
                printf("Nuovo Nome: ");
                scanf("%s", s);
                (*Head)->name[to_modify] = s;
            }
                break;
            case 10:
            {
                int source = 0;
                int target = 0;
                Transport transport = NaN;
                int weight = 0;
                float cost = 0;
                int weight_alt = 0;
                int time = 0;
                do {
                    do {
                        printf("Inserisci sorgente: ");
                        scanf("%d", &source);
                    } while (source >= (*Head)->nodes_count || source < 0);
                    do {
                        printf("Inserisci destinazione: ");
                        scanf("%d", &target);
                    } while (target >= (*Head)->nodes_count || target < 0);
                } while (setNodeData(*Head, source, target, &transport, NULL, NULL, NULL, NULL) == 0);
                do {
                    printf("Inserisci distanza: ");
                    scanf("%d", &weight);
                } while (weight <= 0);
                do {
                    printTransports();
                    printf("Inserisci trasporto: ");
                    scanf("%d", &transport);
                } while (transport <= 0 && transport > 5);
                do {
                    printf("Inserisci costo: ");
                    scanf("%f", &cost);
                } while (cost <= 0);
                do {
                    printf("Inserisci tempo: ");
                    scanf("%d", &time);
                } while (source >= (*Head)->nodes_count);
                do {
                    printf("Inserisci indice turistico (0..3): ");
                    scanf("%d", &weight_alt);
                } while (source >= (*Head)->nodes_count);
                setNodeData(*Head, source, target, &transport, &weight, &weight_alt, &cost, &time);
                printf("Operazione completata. ");
            }
                break;
            case 11:
            {
                int source = 0;
                int target = 0;
                Transport transport = NaN;
                do {
                    do {
                        printf("Inserisci sorgente: ");
                        scanf("%d", &source);
                    } while (source >= (*Head)->nodes_count || source < 0);
                    do {
                        printf("Inserisci destinazione: ");
                        scanf("%d", &target);
                    } while (target >= (*Head)->nodes_count || target < 0);
                } while (unsetNodeData(*Head, source, target, &transport) == 0);
                do {
                    printTransports();
                    printf("Inserisci trasporto: ");
                    scanf("%d", &transport);
                } while (transport <= 0 && transport > 6);
                unsetNodeData(*Head, source, target, &transport);
                printf("Operazione completata. ");
            }
                break;
            case 12:
            {
                int source = 0;
                int transport = 0;
                int target = 0;
                do {
                    printf("Inserisci sorgente: ");
                    scanf("%d", &source);
                } while (source >= (*Head)->nodes_count || source < 0);
                do {
                    printf("Inserisci destinazione: ");
                    scanf("%d", &target);
                } while (target >= (*Head)->nodes_count || target < 0);
                do {
                    printTransports();
                    printf("Inserisci trasporto: ");
                    scanf("%d", &transport);
                } while (transport <= 0 && transport > 6);
                //dijkstraWithTarget(*Head, source, 2, transport, target, NULL, NULL, 0);
                if (transport != 5) {
                    int cost_res = 0;
                    int time_res = 0;
                    dijkstraWithTarget(*Head, source, 2, transport, target, &cost_res, &time_res, 1);
                    //                    suggestedTravel(*Head, source, 3, target, cost_res, time_res);
                } else {
                    int cost_res = 0;
                    int time_res = 0;
                    dijkstraWithTarget(*Head, source, 2, NaN, target, &cost_res, &time_res, 2);
                }
            }
                break;
            case 13:
            {
                int source = 0;
                int transport = 0;
                int target = 0;
                do {
                    printf("Inserisci sorgente: ");
                    scanf("%d", &source);
                } while (source >= (*Head)->nodes_count || source < 0);
                do {
                    printf("Inserisci destinazione: ");
                    scanf("%d", &target);
                } while (target >= (*Head)->nodes_count || target < 0);
                do {
                    printTransports();
                    printf("Inserisci trasporto: ");
                    scanf("%d", &transport);
                } while (transport <= 0 && transport > 6);
//                dijkstraWithTarget(*Head, source, 3, transport, target, NULL, NULL, 0);
                if (transport != 5) {
                    int cost_res = 0;
                    int time_res = 0;
                    dijkstraWithTarget(*Head, source, 3, transport, target, &cost_res, &time_res, 1);
                    //                    suggestedTravel(*Head, source, 3, target, cost_res, time_res);
                } else {
                    int cost_res = 0;
                    int time_res = 0;
                    dijkstraWithTarget(*Head, source, 3, NaN, target, &cost_res, &time_res, 2);
                }
            }
                break;
            case 14:
            {
                int source = 0;
                int transport = 0;
                int target = 0;
                do {
                    printf("Inserisci sorgente: ");
                    scanf("%d", &source);
                } while (source >= (*Head)->nodes_count || source < 0);
                do {
                    printf("Inserisci destinazione: ");
                    scanf("%d", &target);
                } while (target >= (*Head)->nodes_count || target < 0);
                do {
                    printTransports();
                    printf("Inserisci trasporto: ");
                    scanf("%d", &transport);
                } while (transport <= 0 && transport > 6);
                if (transport != 5) {
                    int cost_res = 0;
                    int time_res = 0;
                    dijkstraWithTarget(*Head, source, 0, transport, target, &cost_res, &time_res, 1);
//                    suggestedTravel(*Head, source, 3, target, cost_res, time_res);
                } else {
                    int cost_res = 0;
                    int time_res = 0;
                    dijkstraWithTarget(*Head, source, 0, NaN, target, &cost_res, &time_res, 2);
                }
                
            }
                break;
            case 15:
            {
                #ifdef _WIN32
                system("PAUSE");
                #endif
                return 0;
            }
                break;
        }
    } while (choice > 0 && choice <= 15);
    return 0;
}


void menu(int *choice){
    printf("\n\n1. Inizializza il grafo\n");
    printf("2. Stampa Grafo\n");
    printf("3. Esegui DFS\n");
    printf("4. Aggiungi un Nodo\n");
    printf("5. Rimuovi un Nodo\n");
    printf("6. Aggiungi un Arco\n");
    printf("7. Rimuovi un Arco\n");
    printf("8. Ricerca un Nodo\n");
    printf("9. Modifica Nome Nodo\n");
    printf("10. Inserisci/Modifica trasporto, costo e tempo \n");
    printf("11. Rimuovi trasporto da un arco \n");
    printf("12. Calcola percorso migliore (COSTO) \n");
    printf("13. Calcola percorso migliore (TEMPO) \n");
    printf("14. Calcola percorso migliore (DISTANZA) \n");
    printf("15. Esci");
    printf("\nScelta:");
    scanf("%d", &*choice);
}
