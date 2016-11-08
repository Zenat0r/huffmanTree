#include "header.h"

void generateBinary(Tree root, int code, int position, Table tab[]){
    if(root->left != NULL || root->right != NULL){
        generateBinary(root->left, code << 1, position+1, tab);
        generateBinary(root->right, (code << 1) + 1, position+1, tab);
    } else {
        tab[root->lettre].code = code;
        tab[root->lettre].nbBits = position;

    }
}
Tree createNode(char cara, int poid){
    Tree tmp = (Tree)malloc(sizeof(Node));
    tmp->lettre = cara;
    tmp->poid = poid;
    tmp->next = NULL;
    tmp->left = NULL;
    tmp->right = NULL;
    return tmp;
}

File * createFile(){
    File * maFile = (File*)malloc(sizeof(File));

    maFile->head = NULL;
    maFile->foot = NULL;

    return maFile;
}

File * empiler(File * f, Table* tab){
    int i;
    for(i=0; i<DYNAMIC_RANGE; i++){
        if(tab[i].poid != 0){
            if(f->head == NULL){
                f->head = createNode(tab[i].lettre,tab[i].poid);
                f->foot = f->head;
            }else if(f->head == f->foot){
                if(f->head->poid >= tab[i].poid){
                    f->head = createNode(tab[i].lettre,tab[i].poid);
                    f->head->next = f->foot;
                }else{
                    f->foot->next = createNode(tab[i].lettre,tab[i].poid);
                    f->foot = f->foot->next;
                }
            }else if(f->head->next == f->foot){
                if(f->head->poid >= tab[i].poid){
                    Node * tmp = f->head;
                    f->head = createNode(tab[i].lettre,tab[i].poid);
                    f->head->next = tmp;
                }else if(f->foot->poid >= tab[i].poid && f->head->poid < tab[i].poid){
                    f->head->next = createNode(tab[i].lettre,tab[i].poid);
                    f->head->next->next = f->foot;
                }
            }else{
                if(f->head->poid > tab[i].poid){
                    Node * tmp = createNode(tab[i].lettre,tab[i].poid);
                    tmp->next = f->head;
                    f->head = tmp;
                }else if(f->foot->poid <= tab[i].poid){
                    f->foot->next = createNode(tab[i].lettre,tab[i].poid);
                    f->foot = f->foot->next;
                }else{
                    Node * traveler = f->head;
                    Node* tmp = createNode(tab[i].lettre,tab[i].poid);
                    while(traveler->next->poid <= tab[i].poid){
                        traveler = traveler->next;
                    }
                    tmp->next = traveler->next;
                    traveler->next = tmp;
                }
            }
        }
    }
    return f;
}
File * empilerForTree(File * f, Node* monNode){
    if(f->head == NULL){
        f->head = monNode;
        f->foot = monNode;
    }else if(f->head == f->foot){
        if(f->head->poid >= monNode->poid){
            monNode->next = f->head;
            f->head = monNode;
        }else{
            f->foot->next = monNode;
            f->foot = monNode;
        }
    }else if(f->head->next == f->foot){
        if(f->head->poid >= monNode->poid){
            monNode->next = f->head;
            f->head = monNode;
        }else if(f->head->poid < monNode->poid && f->foot->poid >= monNode->poid){
            monNode->next = f->foot;
            f->head->next = monNode;
        }else{
            f->foot->next = monNode;
            f->foot = monNode;
        }
    }else{
        if(f->head->poid > monNode->poid){
            monNode->next = f->head;
            f->head = monNode;
        }else if(f->foot->poid <= monNode->poid){
            f->foot->next = monNode;
            f->foot = monNode;
        }else{
            Node * traveler = f->head;
            while(traveler->next->poid <= monNode->poid){
                traveler = traveler->next;
            }
            monNode->next = traveler->next;
            traveler->next = monNode;
        }
    }
    return f;
}
Tree defiler(File * f){
    Node * principal;
    Node * right = NULL;
    Node * left = NULL;

    while(f->head != f->foot){
        right = NULL;
        left = NULL;

        right = f->head;
        f->head = f->head->next;


        left = f->head;
        f->head = f->head->next;

        principal = createNode(NULL, right->poid + left->poid);
        principal->right = right;
        principal->left = left;

       f = empilerForTree(f, principal);
    }
    return f->head;
}
void travelPreFixe(Tree root){
    if(root!=NULL){
        printf("%c",root->lettre);
        travelPreFixe(root->left);
        travelPreFixe(root->right);
    }
}
void compression(Table tab[], char nomFichier[], char nomFichierCompresse[]){
    FILE * fsource, * fDest;
    char caractere;
    unsigned int buffer = 0;
    unsigned int tailleBuffer = 0;
    unsigned char code;

    fDest = fopen(nomFichierCompresse, "wb");

    if(fDest == NULL){
        perror("Erreur dans l'ouverture du fichier compressé");
        exit(EXIT_FAILURE);
    }

    fsource = fopen(nomFichier, "rb");
    if(fsource == NULL){
        perror("Erreur dans l'ouverture du fichier source");
        exit(EXIT_FAILURE);
    }

    int i, nbCaraSource = 0;
    fseek(fDest, 4, SEEK_SET);
    for(i=0; i<DYNAMIC_RANGE; i++){
        nbCaraSource += tab[i].poid;
        if(tab[i].poid != 0){
            fputc(tab[i].lettre,fDest);
            fwrite(&tab[i].poid, sizeof(int), 1, fDest);
        }
    }
    long position =  ftell(fDest);
    fseek(fDest, 0, SEEK_SET);
    fwrite(&nbCaraSource, sizeof(int), 1, fDest);

    fseek(fDest, position, SEEK_SET);

    caractere = fgetc(fsource);

    while(caractere != EOF){
        buffer = buffer << tab[caractere].nbBits;
        buffer = buffer | tab[caractere].code;
        tailleBuffer += tab[caractere].nbBits;

        caractere = fgetc(fsource);
        while(tailleBuffer >= 8){
            tailleBuffer -= 8;
            code = buffer >> tailleBuffer;
            fputc(code, fDest);
        }
    }
    if(tailleBuffer != 0){
        fputc(code, fDest);
    }
    fclose(fsource);
    fclose(fDest);
}
void decompression(char nomFichierCompresse[], char nomFichierdecompresse[]){
    Table tab[DYNAMIC_RANGE];
    int i;
    for(i = 0; i<DYNAMIC_RANGE; i++){
        tab[i].lettre = i;
        tab[i].code = 0;
        tab[i].nbBits = 0;
        tab[i].poid = 0;
    }
    FILE * fComp, *fDest;

    fComp = fopen(nomFichierCompresse, "rb");
    if(fComp == NULL){
        perror("Erreur dans l'ouverture du fichier compresse");
        exit(EXIT_FAILURE);
    }

    fDest = fopen(nomFichierdecompresse, "wb");
    if(fDest == NULL){
        perror("Erreur dans l'ouverture du fichier compresse");
        exit(EXIT_FAILURE);
    }

    /***récuparation du header******/
    /****longeurs plus table********/
    int nbCaraSource;
    fread(&nbCaraSource, sizeof(int), 1, fComp);

    fseek(fComp, sizeof(int), SEEK_SET);
    int caraCpt = 0;
    char cara;
    while(caraCpt != nbCaraSource){
        cara = fgetc(fComp);
        tab[cara].lettre = cara;
        fread(&tab[cara].poid, sizeof(int), 1, fComp);
        caraCpt += tab[cara].poid;
    }
    long position =  ftell(fComp);
    fseek(fComp, position, SEEK_SET);

    /****reconstitution de l'arbre***********/
    File * maFile = createFile();
    maFile = empiler(maFile, tab);

    Tree monArbre = NULL;

    monArbre = defiler(maFile);
    travelPreFixe(monArbre);

    /***rechercher dans l'arbre*****/
    caraCpt = 0;
    unsigned int buffer = fgetc(fComp);
    int j = 7;
    unsigned int bit;
    Tree traveler = monArbre;
    while(caraCpt != nbCaraSource){
        bit = (buffer >> j) & 1;
        if(bit == 0){
            traveler = traveler->left;
        }else{
            traveler = traveler->right;
        }
        if(traveler->left == NULL && traveler->right == NULL){
            fputc(traveler->lettre, fDest);
            traveler = monArbre;
            caraCpt++;
        }
        if(j==0){
            buffer = fgetc(fComp);
            j = 8;
        }
        j--;
    }
    fclose(fComp);
    fclose(fDest);
}
char * getSourceFile(char* fileName){
    char * data;

    FILE* fichier = NULL;
    fichier = fopen(fileName, "r");
    if(fichier == NULL){
        perror("Erreur dans l'ouverture du fichier a compresser");
        exit(EXIT_FAILURE);
    }

    fseek(fichier, 0, SEEK_END);
    int length = ftell(fichier);
    fseek(fichier, 0, SEEK_SET);

    data = malloc(length * sizeof(char));

    fgets(data, length + 1, fichier);
    fclose(fichier);

    return data;
}
void wholeCompression(){
    char fileToComp[FILE_MAX_CHAR];
    printf("Entrez le fichier a decompresser : ");
    scanf("%s", &fileToComp);

    char * data = getSourceFile(fileToComp);

    Table tab[DYNAMIC_RANGE];
    int i;
    for(i = 0; i<DYNAMIC_RANGE; i++){
        tab[i].lettre = i;
        tab[i].code = 0;
        tab[i].nbBits = 0;
        tab[i].poid = 0;
    }
    for(i=0; i<strlen(data); i++){
        tab[data[i]].poid++;
    }
    /**creation de la file tiée**/
    File * maFile = createFile();
    maFile = empiler(maFile,tab);


    /**reation de l'arbre d'huffman**/
    Tree monArbre = NULL;
    monArbre = defiler(maFile);
    travelPreFixe(monArbre);

    /***generation du code binaire**/
    generateBinary(monArbre,0,0,tab);

    /***encodage du fichier compressé****/
    compression(tab, fileToComp, "result.txt");
}
void wholeDecompression(){
    char fileToDecomp[FILE_MAX_CHAR];
    printf("Entrez le fichier a decompresser : ");
    scanf("%s", &fileToDecomp);

    decompression(fileToDecomp, "decompress.txt");
}
