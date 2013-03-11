#ifndef _COMPUTE_H_
#define _COMPUTE_H_

    void macCabe ();
    void pathCoverage ( int ID );
    void nodeCoverage ( int ID );
    void edgeCoverage ( int ID );
    void instructionCoverage ( int ID );
    void conditionCoverage ( int ID );
    
    Node *getNode ( int ID );
    Node *getLastNode ( );
    Node *getNextNode ( int ID, int n );
    
    int indexOf ( const char *pattern, char *line, int after );
    
    extern Edge *edgeList;
    extern Node *nodeList;
    extern Instruction instructions[];

#endif
