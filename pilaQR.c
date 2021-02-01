#include "pilaQR.h"
#include <string.h>

void rellenarPila(struct pilaQR* pila, int cantRegistros);

struct nodoQR* crearNodoQR(char* nombreQR){
    struct nodoQR* nodoTemp = (struct nodoQR*) malloc(sizeof(struct nodoQR));
    nodoTemp->nombreQR = nombreQR;
    nodoTemp->sig = NULL;
    return nodoTemp;
}

struct pilaQR* crearPila(int cantRegistros){
    struct pilaQR* pilaTempQR = (struct pilaQR*) malloc(sizeof(struct pilaQR));
    pilaTempQR->cabeza = pilaTempQR->actual = NULL;
    rellenarPila(pilaTempQR, cantRegistros);
    tamanioPila = cantRegistros;
    return pilaTempQR;
}

void rellenarPila(struct pilaQR* pila, int cantRegistros)
{
    for (int i = 0; i < cantRegistros; ++i)
    {
        char* name = (char*) malloc(5*sizeof(char));
        sprintf(name, "%d", i);
        struct nodoQR* temp = crearNodoQR(name);
        if(pila->cabeza == NULL)
        {
            pila->cabeza = temp;
            pila->actual = temp;
        } else{
            pila->actual->sig = temp;
            pila->actual = temp;
        }
    }
}

void pushQR(struct pilaQR* pila, char* nombreQR){
    struct nodoQR* nodoTemp = crearNodoQR(nombreQR);

    if(pila->actual == NULL){
        pila->cabeza = nodoTemp;
        tamanioPila = 1;
        return;
    }

    nodoTemp->sig = pila->cabeza;
    pila->cabeza = nodoTemp;
    tamanioPila = tamanioPila + 1;
}
char* popQR(struct pilaQR* pila){
    if(pila->cabeza == NULL){
        return 0;
    }
    struct  nodoQR* temp = pila->cabeza;

    pila->cabeza = pila->cabeza->sig;
    char* qrLibre = temp->nombreQR;
    free(temp);
    tamanioPila = tamanioPila - 1;
    return  qrLibre;
}

int getTam()
{
    return tamanioPila;
}

struct pilaQR* restaurarPila(struct pilaQR* pila, char* qr)
{
    struct nodoQR* temp = crearNodoQR(qr);
    if (pila == NULL)
    {
        struct pilaQR* pilaTempQR = (struct pilaQR*) malloc(sizeof(struct pilaQR));
        pilaTempQR->cabeza = pilaTempQR->actual = NULL;
        pilaTempQR->cabeza = temp;
        pilaTempQR->actual = temp;
        return  pilaTempQR;
    } else {
        pila->actual->sig = temp;
        pila->actual = temp;
        return pila;
    }
}

void verificarPila(struct pilaQR* pila)
{
    struct nodoQR* actual = pila->cabeza;
    while (actual != NULL)
    {
        printf("%s \n", actual->nombreQR);
        actual = actual->sig;
    }
}