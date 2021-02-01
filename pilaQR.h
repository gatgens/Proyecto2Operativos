#ifndef PROYECTO2_PILAQR_H
#define PROYECTO2_PILAQR_H

#include <stdio.h>
#include <stdlib.h>

struct nodoQR{
    char* nombreQR;
    struct  nodoQR* sig;
};

struct pilaQR{
    struct nodoQR *cabeza, *actual;
};

int tamanioPila;

struct nodoQR* crearNodoQR(char* nombreQR);
struct pilaQR* crearPila(int cantRegistros);
void pushQR(struct pilaQR* pila, char* nombreQR);
char* popQR(struct pilaQR* pila);
void verificarPila(struct pilaQR* pila);
int getTam();
struct pilaQR* restaurarPila(struct pilaQR* pila, char* qr);

#endif //PROYECTO2_PILAQR_H
