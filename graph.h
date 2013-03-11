#ifndef _GRAPH_H_
#define _GRAPH_H_

    void createGraph ( );
    void displayGraph ( );
    void destroyGraph ( );
    
    void createEdge ( char *edgeLabel, int nodeID, char *nodeLabel, int nodeID_, char *nodeLabel_ );
    void createNode ( int ID, char *label );
    void addEdge ( Edge *edge );
    void addNode ( Node *node );

    char *fgeti ( FILE *file, char *instruction );
    void  formatCondition ( char *condition, char *instruction, int notNegated );
    
    extern FILE *file;
    extern Edge *edgeList;
    extern Node *nodeList;
    extern Instruction instructions[];

#endif
