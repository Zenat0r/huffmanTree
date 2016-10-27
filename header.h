#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct node{
    char lettre;
    int poid;
    bool toSort;

    struct node * next;

    struct node * left;
    struct node * right;
} Node;

typedef struct file{
    struct node * head;
    struct node * foot;
} File;

typedef struct stat{
    char lettre;
    int poid;
    int code;
    int nbBits;
} Table;

typedef Node* Tree;

#define TAILLE_MAX 255
#define DYNAMIC_RANGE 255
char * getSourceFile(char* fileName);
File * empiler(File * f, Table* tab);
Tree createNode(char cara, int poid);
File * createFile();
Tree defiler(File * f);
void travelPreFixe(Tree root);
void generateBinary(Tree root, int code, int position, Table tab[]);
File * empilerForTree(File * f, Node* monNode);
void compression(Table tab[], char nomFichier[], char nomFichierCompresse[]);
void decompression(char nomFichierCompresse[], char nomFichierdecompresse[]);
void wholeCompression();
void wholeDecompression();

#endif // HEADER_H_INCLUDED
