#include <stdio.h>
#include <ncurses.h>
#include <string.h>

int codigoValido(char nombre[100],char codigo[100]);

int main(){
    char nombre[100],codigo[100];
    scanf("%s",nombre);
    scanf("%s",codigo);

    printf("%d",codigoValido(nombre,codigo));

}

int codigoValido(char nombre[100],char codigo[100]){
    char aux[100],aux2[100];
    FILE * usuario;
    usuario = fopen(nombre,"r");
    fscanf(usuario,"%s %s",aux,aux2);

    if(strcmp(codigo,aux2) == 0){
        return 1;
    }
    else
        return 0;
    
}