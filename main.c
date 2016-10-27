#include <stdio.h>
#include <stdlib.h>
#include "header.h"

int main()
{
    char data[] = "BACFGABDDACEACG";

    Table tab[26];
    int i;
    for(i = 0; i<26; i++){
        tab[i].lettre = 'A' + i;
        tab[i].code = 0;
        tab[i].nbBits = 0;
        tab[i].poid = 0;
    }
    for(i=0; i<strlen(data); i++){
        tab[data[i] - 'A'].lettre = data[i];
        tab[data[i] - 'A'].poid++;
    }
    File * maFile = createFile();

    maFile = empiler(maFile,tab);

    Node * traveler = maFile->head;
    while(traveler != NULL){
        printf("%c:%d\n", traveler->lettre, traveler->poid);
        traveler = traveler->next;
    }
    Tree monArbre = NULL;

    printf("\n\n");
    monArbre = defiler(maFile);
    printf("\n");
    travelPreFixe(monArbre);

    printf("\n\n");

    generateBinary(monArbre,0,0,tab);

    for(i=0;i<26;i++){
        if(tab[i].nbBits != 0) printf("%c:%x:%d\n",tab[i].lettre, tab[i].code, tab[i].nbBits);
    }
    compression(tab, "deBase.txt", "result.txt");
    return 0;
}
