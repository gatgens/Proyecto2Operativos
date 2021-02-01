//
// Created by kevin on 25/01/21.
//

#ifndef PROYECTO2_QR_ADMIN_H
#define PROYECTO2_QR_ADMIN_H

#include <stdio.h>
#include <string.h>
#include "png.h"
#include "qrencode.h"
#include <stdlib.h>

#define INCHES_PER_METER (100.0/2.54)

static int casesensitive = 0;
static int eightbit = 0;
static int version = 0;
static int size = 3;
static int margin = 3;
static int dpi = 72;
static int structured = 0;
static int rle = 0;
static int micro = 0;
static QRecLevel level = QR_ECLEVEL_L;
static QRencodeMode hint = QR_MODE_8;
static unsigned int fg_color[4] = {0, 0, 0, 255};
static unsigned int bg_color[4] = {255, 255, 255, 255};

int createQR(char* filename, const char* msg);
int lectura(char* file);
char* leerQR(char* filename);
void crearSuperBloque(int cantRegistros);
void guardarPila();
void reestablecerPila(int cant);

#endif //PROYECTO2_QR_ADMIN_H
