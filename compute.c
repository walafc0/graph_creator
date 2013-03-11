#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "types.h"
#include "compute.h"

void
macCabe ()
{
    int edgeNumber = 0;
    Edge *edge = edgeList;
    for ( ; edge != NULL; edgeNumber++ )
        edge = edge -> next;
    
    int nodeNumber = 0;
    Node *node = nodeList;
    for ( ; node != NULL; nodeNumber++ )
        node = node -> next;
    
    printf ( "   |E|\t- |X|\t+ 2\n" );
    printf ( "    %d\t-  %d\t+ 2\t= %d\n", edgeNumber, nodeNumber, edgeNumber - nodeNumber + 2 );
}

void
pathCoverage ( int current )
{
    static int stack[64] = { 0 };
    static int top = -1;

    stack[++top] = current;
    if ( current != getLastNode () -> ID )
    {
        int inStack = FALSE;
        for ( int i = 0; i < top; i++ )
            if ( current == stack[i] )
                inStack = TRUE;
                
        if ( !inStack )
            pathCoverage ( getNextNode ( current, 0 ) -> ID );
        
        Node *node = NULL;
        if ( ( node = getNextNode ( current, 1 ) ) != NULL )
            pathCoverage ( node -> ID );
    }
    else
    {
        printf ( "   " );
        for ( int i = 0; i < top; i++ )
            printf ( "%d, ", stack[i] );
        if ( top != -1 )
            printf ( "%d.\n", stack[top] );
    }
    --top;
}

void
nodeCoverage ( int current )
{
    static int stack[64] = { 0 };
    static int top = -1;
    
    static int stack_[64] = { 0 };
    static int top_ = -1;
    
    stack[++top] = current;
    if ( current != getLastNode () -> ID )
    {
        int inStack = FALSE;
        for ( int i = 0; i < top; i++ )
            if ( current == stack[i] )
                inStack = TRUE;
                
        if ( !inStack )
            nodeCoverage ( getNextNode ( current, 0 ) -> ID );
        
        Node *node = NULL;
        if ( ( node = getNextNode ( current, 1 ) ) != NULL )
            nodeCoverage ( node -> ID );
    }
    else
    {
        int allInStack = TRUE;
        
        for ( int i = 0; i <= top; i++ )
        {
            int inStack = FALSE;
            for ( int j = 0; j <= top_; j++ )
                if ( stack[i] == stack_[j] )
                    inStack = TRUE;
        
            if ( !inStack )
            {
                allInStack = FALSE;
                stack_[++top_] = stack[i];
            }
        }
    
        if ( !allInStack )
        {
            printf ( "   " );
            for ( int i = 0; i < top; i++ )
                printf ( "%d, ", stack[i] );
            if ( top != -1 )
                printf ( "%d.\n", stack[top] );
        }
    }
    --top;
}

void
edgeCoverage ( int current )
{
    static int stack[64] = { 0 };
    static int top = -1;
    
    static Edge stack_[64] = { { 0, 0, NULL, NULL } };
    static int top_ = -1;
    
    stack[++top] = current;
    if ( current != getLastNode () -> ID )
    {
        int inStack = FALSE;
        for ( int i = 0; i < top; i++ )
            if ( current == stack[i] )
                inStack = TRUE;
                
        if ( !inStack )
            edgeCoverage ( getNextNode ( current, 0 ) -> ID );
        
        Node *node = NULL;
        if ( ( node = getNextNode ( current, 1 ) ) != NULL )
            edgeCoverage ( node -> ID );
    }
    else
    {
        int allInStack = TRUE;
        
        for ( int i = 0; i < top; i++ )
        {
            int inStack = FALSE;
            for ( int j = 0; j <= top_; j++ )
                if ( stack[i] == stack_[j].nodeID 
                  && stack[i+1] == stack_[j].nodeID_ )
                    inStack = TRUE;
        
            if ( !inStack )
            {
                allInStack = FALSE;
                ++top_;
                stack_[top_].nodeID  = stack[i];
                stack_[top_].nodeID_ = stack[i+1];
            }
        }
    
        if ( !allInStack )
        {
            printf ( "   " );
            for ( int i = 0; i < top; i++ )
                printf ( "%d, ", stack[i] );
            if ( top != -1 )
                printf ( "%d.\n", stack[top] );
        }
    }
    --top;
}

void
instructionCoverage ( int current )
{
    static int stack[64] = { 0 };
    static int top = -1;
    
    static int stack_[64] = { 0 };
    static int top_ = -1;

    static int findInstr = TRUE;
    if ( findInstr )
    {
        Node *node = nodeList;
        while ( node != NULL )
        {
            if ( node -> label != NULL )
            {
                if ( !indexOf ( instructions[ELSE].label, node -> label, 0 ) )
                    ; // Permet de ne pas reconnaitre un "Sinon" en tant que "Si"
                else if ( !indexOf ( instructions[IF].label, node -> label, 0 ) )
                    stack_[++top_] = node -> ID;
                else if ( !indexOf ( instructions[LOOP].label, node -> label, 0 ) )
                    stack_[++top_] = node -> ID;
            }
                
            node = node -> next;
        }
        
        printf ( "   " );
        for ( int i = 0; i < top_; i++ )
            printf ( "%d, ", stack_[i] );
        if ( top_ != -1 )
            printf ( "%d. > Instructions de controle.\n", stack_[top_] );
        
        findInstr = FALSE;
    }

    stack[++top] = current;
    if ( current != getLastNode () -> ID )
    {
        int inStack = FALSE;
        for ( int i = 0; i < top; i++ )
            if ( current == stack[i] )
                inStack = TRUE;
                
        if ( !inStack )
            instructionCoverage ( getNextNode ( current, 0 ) -> ID );
        
        Node *node = NULL;
        if ( ( node = getNextNode ( current, 1 ) ) != NULL )
            instructionCoverage ( node -> ID );
    }
    else
    {
        int allInStack = TRUE;
        
        for ( int i = 0; i <= top; i++ )
        {
            for ( int j = 0; j <= top_; j++ )
            {
                if ( stack[i] == stack_[j] )
                {
                    allInStack = FALSE;
                    stack_[j] = -1;
                }
            }
        }

        if ( !allInStack )
        {
            printf ( "   " );
            for ( int i = 0; i < top; i++ )
                printf ( "%d, ", stack[i] );
            if ( top != -1 )
                printf ( "%d.\n", stack[top] );
        }
    }
    --top;
}

void
conditionCoverage ( int current )
{
    static int stack[64] = { 0 };
    static int top = -1;
    
    static int stack_[64] = { 0 };
    static int top_ = -1;

    static int findInstr = TRUE;
    if ( findInstr )
    {
        Node *node = nodeList;
        while ( node != NULL )
        {
            if ( node -> label != NULL )
            {
                if ( !indexOf ( instructions[ELSE].label, node -> label, 0 ) )
                ; // Permet de ne pas reconnaitre un "Sinon" en tant que "Si"
                else if ( !indexOf ( instructions[IF].label, node -> label, 0 ) )
                    stack_[++top_] = node -> ID;
                else if ( !indexOf ( instructions[LOOP].label, node -> label, 0 ) )
                    stack_[++top_] = node -> ID;
            }
                
            node = node -> next;
        }
        
        printf ( "   " );
        for ( int i = 0; i < top_; i++ )
            printf ( "%d, ", stack_[i] );
        if ( top_ != -1 )
           printf ( "%d. > Instructions de controle.\n", stack_[top_] );
        
        findInstr = FALSE;
    }

    stack[++top] = current;
    if ( current != getLastNode () -> ID )
    {
        int inStack = FALSE;
        for ( int i = 0; i < top; i++ )
            if ( current == stack[i] )
                inStack = TRUE;
                
        if ( !inStack )
            conditionCoverage ( getNextNode ( current, 0 ) -> ID );
        
        Node *node = NULL;
        if ( ( node = getNextNode ( current, 1 ) ) != NULL )
            conditionCoverage ( node -> ID );
    }
    else
    {
        int allInStack = TRUE;
        
        for ( int i = 0; i <= top; i++ )
            for ( int j = 0; j <= top_; j++ )
                if ( stack[i] == stack_[j] )
                    allInStack = FALSE;

        if ( !allInStack )
        {
            printf ( "   " );
            for ( int i = 0; i < top; i++ )
                printf ( "%d, ", stack[i] );
            if ( top != -1 )
                printf ( "%d.\n", stack[top] );
        }
    }
    --top;
}

// Fonction qui permet de trouver un noeud selon son ID
Node
*getNode ( int ID )
{
    Node *node = nodeList;
    while ( node != NULL )
    {
        if ( node -> ID == ID )
            return node;
            
        node = node -> next;
    }
    
    return NULL;
}

// Fonction qui permet de retourner le dernier sommet de la liste
Node
*getLastNode ( )
{
    Node *node = nodeList;
    if ( node != NULL )
    {
        while ( node -> next != NULL )
            node = node -> next;
            
        return node;
    }
    
    return NULL;
}

Node
*getNextNode ( int ID, int n )
{
    Edge *edge = edgeList;
    while ( edge != NULL )
    {
        if ( edge -> nodeID == ID )
        {
            if ( !n )
                return getNode ( edge -> nodeID_ );
            else
                n = FALSE;
        }
            
        edge = edge -> next;
    }
    
    return NULL;
}

// Fonction qui retourne l'index de l'instruction de contrôle cherchée
int
indexOf ( const char *pattern, char *line, int after )
{
    // On récupère la longueur de la ligne courante et du motif cherché
    int linelength = strlen ( line );
    int patternlength = strlen ( pattern );

    for ( int i = 0; i < linelength; i++ )
        for ( int j = 0; j < patternlength; j++ )
            if ( pattern[j] != line[i+j] )
                break;
            else if ( j == patternlength - 1 )
                return after ? ++j+i : i;

    return INSTLEN;
}
