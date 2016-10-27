#include "header.h"

void generateBinary(Tree root, int code, int position, Table tab[]){
    if(root->left != NULL || root->right != NULL){
        generateBinary(root->left, code << 1, position+1, tab);
        generateBinary(root->right, (code << 1) + 1, position+1, tab);
    } else {
        tab[root->lettre -'A'].code = code;
        tab[root->lettre -'A'].nbBits = position;

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
    for(i=0; i<26; i++){
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
        Node* traveler = f->head;
        printf("\n");
        while(traveler != NULL){
            printf("%c:%d | ", traveler->lettre, traveler->poid);
            traveler = traveler->next;
        }
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
    printf("\n%c:%d | ", f->head->lettre, f->head->poid);
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
    unsigned char caractere;
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
    caractere = fgetc(fsource);

    while(caractere != '.'){
        buffer = buffer << tab[caractere - 'A'].nbBits;
        buffer = buffer | tab[caractere - 'A'].code;
        tailleBuffer += tab[caractere - 'A'].nbBits;

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
