/*============================================================================================
 *                    UNIFAL - UNIVERSIDADE FEDERAL DE ALFENAS
 *                     BACHARELADO EM CIÊNCIA DA COMPUTAÇÃO
 *  Trabalho   : Imagem ASCII
 *  Disciplina : Processamento de Imagens
 *  Professor  : Luiz Eduardo da Silva
 *  Aluno (a)  : Flaviane Moura Oliveira
 *  Data       : 27/04/2023
 * 
 *===========================================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imagelib.h"

void ascii_pgm(image In, int nl, int nc, int mn, int n_col, int n_lin, char* caracteres, FILE* img_ascii) {
    for (int i = 0; i < n_lin; i++) {
        for (int j = 0; j < n_col; j++) {
            // Calcula a posição do pixel na imagem original
            int px_i = i * nl / n_lin;
            int px_j = j * nc / n_col;
            
            float valor = (float)In[px_i * nc + px_j] / mn;
            
            int id = (int)(valor * (strlen(caracteres) - 1));
            // Escreve o caractere correspondente no arquivo de saída
            fputc(caracteres[id], img_ascii);
        }
        fputc('\n', img_ascii);
    }
}

void msg(char *s) {
    printf("\nASCII image");
    printf("\n-------------------------------");
    printf("\nUsage:  %s  image-name[.pgm] ncolunas nlinhas string\n\n", s);
    printf("    image-name[.pgm] is image file file \n");
    printf("    ncolunas and nlinhas definem o tamanho da imagem ASCII que será gerada\n");
    exit(1);
}


/*-------------------------------------------------------------------------
 * main function
 *-------------------------------------------------------------------------*/
int main(int argc, char *argv[]) {
    int nc, nr, ml, tp;
    char *p, nameIn[100], nameOut[100], cmd[110];
    image In;
    if (argc != 5)
        msg(argv[0]);
    tp = 2;
    //-- define input/output file name
    img_name(argv[1], nameIn, nameOut, tp);
    //-- read image
    In = img_get(nameIn, &nr, &nc, &ml, tp);
    //-- write ASCII image
    FILE* img_ascii = fopen("result.txt", "w");
    int n_col = atoi(argv[2]);
    int n_lin = atoi(argv[3]);
    char* caracteres = argv[4];
    ascii_pgm(In, nr, nc, ml, n_col, n_lin, caracteres, img_ascii);
    fclose(img_ascii);
    img_free(In);
    return 0;
}