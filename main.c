#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "types.h"
#include "graph.h"
#include "compute.h"

FILE *file     = NULL;
Edge *edgeList = NULL;
Node *nodeList = NULL;

// Tableau pour l'utilisation des instructions de contrôle via l'énumération
Instruction instructions[] = {
    { IF,      "Si"       },
    { THEN,    "Alors"    },
    { ELSE,    "Sinon"    },
    { ENDIF,   "Fsi"      },
    { LOOP,    "Tantque"  },
    { DO,      "Faire"    },
    { ENDLOOP, "Ftantque" }
};

int
main ( int argc, char **argv )
{
    char filename[FILENAME_LENGTH] = { 0 };

    printf ( "Nom du ficher: " );
    scanf ( "%s", ( char * ) filename );
    puts ( "" );

    file = fopen ( filename, "r" );
    if ( file == NULL )
    {
        perror ( "fopen" );
        return errno;
    }
    
    createGraph ( );
    displayGraph ( );

    /////
    puts ( "MacCabe:" );
    macCabe ();
    
    puts ( "\nCouverture des chemins:" );
    pathCoverage ( 0 );
    
    puts ( "\nCouverture des sommets:" );
    nodeCoverage ( 0 );
    
    puts ( "\nCouverture des arcs:" );
    edgeCoverage ( 0 );
    
    puts ( "\nCouverture des instructions de controle:" );
    instructionCoverage ( 0 );
    
    puts ( "\nCouverture des conditions élémentaires:" );
    conditionCoverage ( 0 );
    
    puts ( "" );
    /////
    
    destroyGraph ( );
    fclose ( file );
    
    return 0;
}
