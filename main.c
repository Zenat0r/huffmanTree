#include "header.h"

int main()
{
    int menu = 3;
    printf("Bienvenu dans le compresseur/decompresseur de fichier !\n");
    while(menu == 3){
        printf("\n  Entre 1 pour compresser un fichier\n        2 pour decompresser un fichier\n");
        do{
            scanf("%d", &menu);
        }while(menu > 2 || menu < 1);
        switch(menu){
            case 1:
                wholeCompression();

                printf("Fichier compresse sous le nom de 'result.txt' avec success !");
            break;
            case 2:
                wholeDecompression();

                printf("\nFichier decompresse sous le nom de 'decompress.txt' avec success !");
            break;
        }
        printf("\nAppuiez sur 3 pour effectuer une nouvelle operation\nTapez autre chose pour quitter !");
        scanf("%d", &menu);
    }
    return 0;
}
