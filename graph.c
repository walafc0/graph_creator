#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "types.h"
#include "compute.h"
#include "graph.h"

void
createGraph ( )
{
    int ID  = -1;
    int ID_ = -1;
    
    Stack stack[8] = { { 0, FALSE, "" } };
    int top = -1;
    
    char condition[INSTLEN] = { 0 };
    char instruction[INSTLEN] = { 0 };
    Node *node = NULL;

    puts ( "Instructions:" );
    // Tant que l'on a pas lu et parser tout le fichier
    while ( fgeti ( file, instruction ) != NULL )
    {
        printf ( "   %s\n", instruction );
            
        // On récupère le dernier sommet
        node = getLastNode ();
        if ( node == NULL )
            ID = 0; // Si il n'existe pas, ID = 0
        else
            ID = node -> ID;
    
        if ( !indexOf ( instructions[ELSE].label, instruction, 0 ) )
        {
            ID_ = stack[top].ID;
            strncpy ( instruction, stack[top].label, strlen ( stack[top].label ) );
            top--;
            formatCondition ( condition, instruction , FALSE );
            createEdge ( condition, ID_, "", ID + 1, instructions[ELSE].label );
            
            top++;
            stack[top].ID = ID;
            stack[top].hasElse = TRUE;
        }
        else if ( !indexOf ( instructions[IF].label, instruction, 0 ) )
        {
            top++;
            stack[top].ID = ID;
            stack[top].hasElse = FALSE;
            strncpy ( stack[top].label, instruction, strlen ( instruction ) );
            formatCondition ( condition, instruction , TRUE );
            createEdge ( condition, ID, instructions[IF].label, ID + 1, instructions[THEN].label );
        }
        else if ( !indexOf ( instructions[ENDIF].label, instruction, 0 ) )
        {
            createEdge ( " ", ID, "", ID + 1, instructions[ENDIF].label );
            
            ID_ = stack[top].ID;
            if ( !stack[top].hasElse )
            {
                strncpy ( instruction, stack[top].label, strlen ( stack[top].label ) );
                formatCondition ( condition, instruction, FALSE );
                createEdge ( condition, ID_, "", ID + 1, instructions[ENDIF].label );
            }
            else
            {
                createEdge ( " ", ID_, "", ID + 1, instructions[ENDIF].label );
            }
            top--;
        }
        else if ( !indexOf ( instructions[LOOP].label, instruction, 0 ) )
        {
            top++;
            stack[top].ID = ID;
            strncpy ( stack[top].label, instruction, strlen ( instruction ) );
            formatCondition ( condition, instruction , TRUE );
            createEdge ( condition, ID, instructions[LOOP].label, ID + 1, instructions[DO].label );
        }
        else if ( !indexOf ( instructions[ENDLOOP].label, instruction, 0 ) )
        {
            ID_ = stack[top].ID;
            strncpy ( instruction, stack[top].label, strlen ( stack[top].label ) );
            formatCondition ( condition, instruction, FALSE );
            createEdge ( " ", ID, "", ID_, "" );
            createEdge ( condition, ID_, "", ID + 1, instructions[ENDLOOP].label );
            top--;
        }
        else
        {
            createEdge ( instruction, ID, "", ID + 1, " " );
        }
    }
}

// Fonction qui permet d'afficher le graphe créé
void
displayGraph ( )
{
    printf ( "\nSommets:\n" );
    Node *node = nodeList;
    while ( node != NULL )
    {
        printf ( "   [%d]\t\t%s\n", node -> ID, node -> label );
        node = node -> next;
    }

    printf ( "\nArcs:\n" );
    Edge *edge = edgeList;
    while ( edge != NULL )
    {
        printf ( "   [%d]\t->  [%d] \t%s\n", edge -> nodeID, edge -> nodeID_, edge -> label );
        edge = edge -> next;
    }
    
    printf ( "\n" );
}

// Fonction qui permet de détruire les deux listes que l'on a créé
void
destroyGraph ( )
{
    Edge *nextEdge = NULL;
    while ( edgeList != NULL )
    {
        nextEdge = edgeList -> next;
        if ( edgeList -> label != NULL )
            free ( edgeList -> label );
        if ( edgeList != NULL )
            free ( edgeList );
        edgeList = nextEdge;
    }
    
    Node *nextNode = NULL;
    while ( nodeList != NULL )
    {
        nextNode = nodeList -> next;
        if ( nodeList -> label != NULL )
            free ( nodeList -> label );
        if ( nodeList != NULL )
            free ( nodeList );
        nodeList = nextNode;
    }
}

// Permet de créer un arc. On a besoin de différents paramètres : 
// Le nom de l'arc, le numéro du sommet précédent, le nom du sommet précédent
// Le numéro du sommet suivant, le nom du sommet suivant

// À chaque arc créé, on créé également deux sommets
void
createEdge ( char *edgeLabel, int nodeID, char *nodeLabel, int nodeID_, char *nodeLabel_ )
{
    // On alloue la place nécessaire pour l'arc
    Edge *edge = ( Edge * ) malloc ( sizeof ( Edge ) );
    if ( edge == NULL )
    {
        perror ( "malloc" );
        exit ( errno );
    }
    
    // On ajoute l'arc à la liste
    addEdge ( edge );
    
    // On calcule la longueur du label de l'arc
    int edgeLabelLength = strlen ( edgeLabel );
    // S'il n'est pas nul
    if ( edgeLabelLength )
    {
        // On alloue la place pour le label
        edge -> label = ( char * ) malloc ( sizeof ( char ) * edgeLabelLength );
        if ( edge -> label == NULL )
        {
            perror ( "malloc" );
            exit ( errno );
        }
        // On met à jour le label de l'arc que l'on vient de créer
        strncpy ( edge -> label, edgeLabel, edgeLabelLength );
    }
    // Idem pour les indices des sommets et l'arc suivant
    edge -> nodeID  = nodeID;
    edge -> nodeID_ = nodeID_;
    edge -> next    = NULL;
    
    // On créer deux sommets avec les indices qu'on a trouvés et les labels
    createNode ( edge -> nodeID, nodeLabel );
    createNode ( edge -> nodeID_, nodeLabel_ );
}

// Permet d'ajouter un arc à la liste chainée
void
addEdge ( Edge *edge_ )
{
    Edge *edge = edgeList;
    if ( edge != NULL )
    {     
        while ( edge -> next != NULL )
            edge = edge -> next;
        
        edge -> next = edge_;
        return;
    }
    
    edgeList = edge_;
}

// Permet de créer un sommet et de l'ajouter à la liste
// Paramètres : l'ID du sommet et son label
void
createNode ( int ID, char *label )
{
    // On regarde si le noeud existe déjà
    Node *node = getNode ( ID );
    if ( node == NULL )
    {
        // Si non, on alloue la place pour le créer
        node = ( Node * ) malloc ( sizeof ( Node ) );
        if ( node == NULL )
        {
            perror ( "malloc" );
            exit ( errno );
        }
        // On l'ajoute à la liste
        addNode ( node );
        // On met a jour l'ID
        node -> ID = ID;
    }
    
    // On calcule la longueur du label
    int labelLength = strlen ( label );
    if ( labelLength )
    {
        // Si le label n'est pas nul, on l'enlève
        if ( node -> label != NULL )
            free ( node -> label );
        // Sinon, on alloue la place nécessaire
        node -> label = ( char * ) malloc ( sizeof ( char ) * labelLength );
        if ( node -> label == NULL )
        {
            perror ( "malloc" );
            exit ( errno );
        }
        // Et on le met à jour
        strncpy ( node -> label, label, labelLength );
    }
}

// Fonction qui permet d'ajouter un sommet à la liste
void
addNode ( Node *node_ )
{
    Node *node = nodeList;
    if ( node != NULL )
    {
        while ( node -> next != NULL )
            node = node -> next;
        
        node -> next = node_;
        return;
    }
    
    nodeList = node_;
}

char
*fgeti ( FILE *file, char instruction[] )
{
    char line_[INSTLEN] = { 0 };

    do
        if ( fgets ( line_, INSTLEN, file ) == NULL )
            return NULL;
    while ( line_[0] == '\n' );

    int lineLength = strlen ( line_ );
    char line[INSTLEN] = { 0 };
    
    int i = 0;
    int j = 0;
    for ( int stop = FALSE; i < lineLength && !stop; i++ )
    {
        switch ( line_[i] )
        {
            case ' ':
            case '\t':
                continue;
                break;
            default:
                line[j++] = line_[i];
                stop = TRUE;
        }
    }
    
    for ( ; i < lineLength; i++, j++ )
        line[j] = line_[i];

    int index = INSTLEN;

    // Si la ligne commence par "XXX"
    // On récupère l'index juste après cette instruction
    if ( !strncmp ( line, instructions[ELSE].label, strlen ( instructions[ELSE].label ) ) )
    {
        index = strlen ( instructions[ELSE].label );
    }
    // Ici on doit calculer l'index de "Alors" car il est sur la même ligne que le "Si"
    else if ( !strncmp ( line, instructions[IF].label, strlen ( instructions[IF].label ) ) )
    {
        index = indexOf ( instructions[THEN].label, line, 1 );
    }
    else if ( !strncmp ( line, instructions[ENDIF].label, strlen ( instructions[ENDIF].label ) ) )
    {
        index = strlen ( instructions[ENDIF].label );
    }
    else if ( !strncmp ( line, instructions[LOOP].label, strlen ( instructions[LOOP].label ) ) )
    {
        index = indexOf ( instructions[DO].label, line, 1 );
    }
    else if ( !strncmp ( line, instructions[ENDLOOP].label, strlen ( instructions[ENDLOOP].label ) ) )
    {
        index = strlen ( instructions[ENDLOOP].label );
    }
    // Si la ligne ne commence pas par une instruction de contrôle :
    else
    {
        int index_ = INSTLEN;
        // On va chercher si il y en a une sur la ligne
        for ( int i = 0; i < INSTNUM; i++ )
        {
            // On prend l'index avant la prochaine instruction de contrôle qui est sur la ligne
            index_ = indexOf ( instructions[i].label, line, 0 );
            // Permet de retourner le plus petit décalage avant la prochaine instruction de contrôle
            index = ( index < index_ ? index : index_ );
        }
        // On enlève 1 pour ajouter \0 à la fin
        if ( index == INSTLEN )
            index = strlen ( line ) - 1;
    }
    
    // On copie l'instruction dans le tableau
    strncpy ( instruction, line, index );
    instruction[index] = '\0';
    
    // On retourne là ou on c'était arrêter dans le traitement
    fseek ( file, ftell ( file ) - strlen ( line ) + index, SEEK_SET );
    
    return instruction;
}

// Fonction qui permet de parser l'instruction courante
void
formatCondition ( char *condition, char *instruction, int notNegated )
{
    int i = 0;
    int length = -1;
    // On initialise un tableau temporaire
    char condition_[INSTLEN] = { 0 };
    
    // Si l'instruction est un "Sinon"
    if ( !notNegated )
    {
        i = 2;
        // On ajoute la négation à la condition
        strncpy ( condition_, "¬", i );
    }
    else
    {
        i = 1;
        strncpy ( condition_, " ", i );
    }
    // Si c'est un "Si"
    if ( !indexOf ( instructions[IF].label, instruction, 0 ) )
    {
        length = strlen ( instruction ) - 7;
        // On copie l'instruction après le "Si" dans le tableau temporaire
        strncpy ( &(condition_[i]), &(instruction[2]), length );
    }
    // Si c'est un "Tantque"
    else if ( !indexOf ( instructions[LOOP].label, instruction, 0 ) )
    {
        length = strlen ( instruction ) - 12;
        // On copie l'instruction en enlevant le "Tantque"
        strncpy ( &(condition_[i]), &(instruction[7]), length );
    }
    else
    {
        // Sinon, on copie la condition
        length = strlen ( condition );
        strncpy ( &(condition_[i]), condition, length );
    }
    
    length = strlen ( condition_ );
    // On ajoute \0
    condition_[length] = '\0';
    // On modifie l'instruction que l'on avait en entrée par celle que l'on a trouver.
    strncpy ( condition, condition_, length+1 );
}
