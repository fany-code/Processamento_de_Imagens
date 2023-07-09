/*============================================================================================
 *                    UNIFAL - UNIVERSIDADE FEDERAL DE ALFENAS
 *                     BACHARELADO EM CIÊNCIA DA COMPUTAÇÃO
 *  Trabalho   : Esteganografia
 *  Disciplina : Processamento de Imagens
 *  Professor  : Luiz Eduardo da Silva
 *  Aluno (a)  : Flaviane Moura Oliveira
 *  Data       : 08/07/2023
 * 
 *===========================================================================================*/

/*---------------------------------------------------------
 * Image Processing
 * 
 * Steganography - decode
 *---------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "imagelib.h"
#define DEBUG(x) x

typedef enum {
    R,
    G,
    B
} CorBanda;

typedef enum {
    false,
    true
} bool;

int ultimoBit(int pixel, CorBanda *bd) {
    int deslocamento[3] = {16, 8, 0};                       // deslocamentos para R, G e B
    int px = (pixel >> deslocamento[*bd]) & 0x01;           // pega o bit menos significativo e aplica a mascara
    *bd = (*bd + 1) % 3;                                    // avança para a próxima banda, 'mod 3' garante que o resultado 
                                                            // esteja sempre entre 0, 1 e 2 (R, G e B)
    return px;
}

void decode(image In) {
    FILE *file;
    char name[100] = "";
    int fsize;
    unsigned char byte;
    CorBanda bd = R;
    int px = 0, contaBit = 0;
    int pos;
    bool byteFinal = false;

    // decode the name
    for (int i = 0; !byteFinal; i++, contaBit++) {                      // enquanto não encontra o byte final
        if (contaBit < 8) {
            px = ((px << 1) | ultimoBit(In->px[i], &bd));   
        } else {
            byte = (unsigned char)px;                                   // byte formado
            name[strlen(name)] = byte;                                  // adiciona o byte à string "name"
            if (byte == 0) {
                byteFinal = true;                                       // ultimo byte encontrado
            }  else { 
                px = ((px << 1) | ultimoBit(In->px[i], &bd));           // reinicia px com o próximo bit 
                contaBit = 0;                                           // reseta o contador de bits
            }
        }
        pos = i;
    }    

    name[strlen(name)] = '\0';                                          
    printf("File name: %s\n", name);
    
    // decode file size
    contaBit = 0;
    while (contaBit < 32) {
        fsize = ((fsize << 1) | ultimoBit(In->px[pos], &bd));
        pos++;
        contaBit++;
    } 
    printf("File size: %d bytes\n", fsize);
    
    // decode file
    file = fopen(name, "wb");
    if (!file) {
        printf("Cannot create file %s\n", name);
        exit(10);
    }
    
    while (fsize) {
        px = 0;
        for (int i = 0; i < 8; i++, pos++)                          // constrói byte de informação
            px = ((px << 1) | ultimoBit(In->px[pos], &bd));
        fprintf(file, "%c", (unsigned char)px);                     // escreve o byte no arquivo
        fsize--;    
    }

    fclose(file);
}

void msg(char *s){
    printf("\nSteganography - decode");
    printf("\n-------------------------------");
    printf("\nUSE.:  %s  <IMG.PPM>", s);
    printf("\nWhere:\n");
    printf("    <IMG.PPM>  Image file in PPM ASCII format\n\n");
    exit(20);
}

/*-----------------------------------------
  Main program
------------------------------------------*/
int main(int argc, char *argv[]) {
    char name[100];
    image In;
    char *p;
    if (argc < 2)
        msg(argv[0]);
    strcpy(name, argv[1]);
    p = strstr(name, ".ppm");
    if (p)
        *p = 0;
    strcat(name, ".ppm");
    In = img_get(name, COLOR);
    decode(In);
    img_free(In);
    return 0;
}
