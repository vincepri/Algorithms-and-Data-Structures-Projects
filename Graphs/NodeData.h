//
//  NodeData.h
//  Graphs
//
//  Created by Vincenzo on 5/26/13.
//  Copyright (c) 2013 whatever. All rights reserved.
//

#ifndef Graphs_NodeData_h
#define Graphs_NodeData_h

// ---------------------------------------------------- //
// Structures                                           //
// ---------------------------------------------------- //

enum TTransport {
    NaN = -1,
    pullman = 0,
    treno = 1,
    aereo = 2,
    nave = 3,
    taxi = 4
};

typedef enum TTransport Transport;

//struct TNodeData {
//    float cost;
//    unsigned long time;
//    
//};
//
//typedef struct TNodeData* NodeData;
//
//
//// ---------------------------------------------------- //
//// Prototypes                                           //
//// ---------------------------------------------------- //
//
//NodeData initNodeData();
//float getCost(NodeData E);
//unsigned long getTime(NodeData E);
//void setCost(NodeData E, float cost);
//void setTime(NodeData E, unsigned long time);
void printTransports();

#endif
