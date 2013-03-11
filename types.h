#ifndef _TYPES_H_
#define _TYPES_H_

    #define TRUE  1
    #define FALSE 0
    #define INSTLEN 256
    #define FILENAME_LENGTH 128

    typedef struct _Node Node;
    struct _Node {
        int ID;
        char *label;
        Node *next;
    };

    typedef struct _Edge Edge;
    struct _Edge {
        int nodeID;
        int nodeID_;
        char *label;
        Edge *next;
    };

    // Instructions de contrôle
    typedef enum _InstructionID {
        IF = 0,
        THEN,
        ELSE,
        ENDIF,
        LOOP,
        DO,
        ENDLOOP,
        INSTNUM
    } InstructionID;

    // Structure pour l'utilisation de l'énumération
    typedef struct _Instruction {
        InstructionID instruction;
        char *label;
    } Instruction;
    
    // Structure permettant de sauvegarder un contexte en cours de création de graph.
    typedef struct _Stack {
        int ID;
        int hasElse;
        char label[INSTLEN];
    } Stack;

#endif
