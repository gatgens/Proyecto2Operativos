//
// Created by kevin on 25/01/21.
//

#include "Qr_Admin.h"
#include "pilaQR.h"

struct pilaQR* pila = NULL;
int qrID = 0;
char resultado[128];
int restablecido = 0;

static int writePNG(QRcode *qrcode, const char *outfile);
char* cambiarDireccion(char* token);

/**
 * Lee un QR a partir de el nombre de este
 * @param filename
 * @return
 */
char* leerQR(char* filename)
{
    memset(resultado, 0, sizeof resultado);
    FILE* fp;
    char path[128];
    char command[50] = "zbarimg ";
    strcat(command, filename);
    strcat(command, " -q");

    fp = popen(command, "r");
    if (fp == NULL) {
        printf("Failed to run command\n" );
        return "-1";
    }

    while (fgets(path, sizeof(path), fp) != NULL) {
        strcat(resultado, path);
    }

    pclose(fp);
    return resultado;
}

/**
 * Funcion para escribir los bloques del fs en QRs
 * @param buffer
 * @param tam
 */
void QRDeBuffer(const char* buffer, int tam)
{
    int qrIds[100];
    memset(qrIds, -1, sizeof qrIds);
    int leido = 0;
    int i = 0;
    int actId = 0;

    do {
        char datos[128];
        while (leido < tam && i < 127)
        {
            datos[i] = buffer[leido];
            leido++;
            i++;
        }
        i = 0;
        char* qrName = popQR(pila);
        createQR(qrName, datos);
        memset(datos, 0 , sizeof datos);
        qrIds[actId] = atoi(qrName);
        actId++;
    }while (leido < tam);
}

/**
 * lee un archivo y genera QRs de 128 bytes para el archivo
 * @param file
 * @return
 */
int lectura(char* file)
{
    FILE *archivo;
    char caracter = 0;

    archivo = fopen(file,"r");

    if (archivo == NULL)
    {
        printf("\nError de apertura del archivo. \n\n");
        return -1;
    }
    else
    {
        do {
            char datos[128];
            int i = 0;
            if (caracter != 0)
            {
                datos[i] = caracter;
                i++;
            }

            while ((caracter = fgetc(archivo)) != EOF && i < 127) {
                datos[i] = caracter;
                i++;
            }
            char* qrName = popQR(pila);
            //char name[5];
            //sprintf(name, "%d", qrID);
            //qrID++;
            createQR(qrName, datos);
            free(qrName);
            memset(datos, 0, sizeof datos);
        }while (caracter != EOF);
    }
    fclose(archivo);
    return 0;
}

/**
 * Funcion que crea un QR de 128 bytes
 * @param filename
 * @param msg
 * @return
 */
int createQR(char* filename, const char* msg)
{
    QRcode *myqrcode; //estructura del qr
    myqrcode = QRcode_encodeString(msg, 4, QR_ECLEVEL_H, QR_MODE_8,1);//generador del bitmap del qr con la bibliotena qrencode
    writePNG(myqrcode, filename);//traduccion del bitmap a una imagen png
    QRcode_free(myqrcode);//liberador de memoria para los qr una vez se alamacenan como imagenes
    return 0;
}

/*
 * funcion que traduce el bitmap a una imagen*/
static int writePNG(QRcode *qrcode, const char *outfile) {
    static FILE *fp; // avoid clobbering by setjmp.
    png_structp png_ptr;
    png_infop info_ptr;
    png_colorp palette;
    png_byte alpha_values[2];
    unsigned char *row, *p, *q;
    int x, y, xx, yy, bit;
    int realwidth;

    realwidth = (qrcode->width + margin * 2) * size;
    row = (unsigned char *) malloc((realwidth + 7) / 8);
    if (row == NULL) {
        fprintf(stderr, "Failed to allocate memory.\n");
        exit(EXIT_FAILURE);
    }

    if (outfile[0] == '-' && outfile[1] == '\0') {
        fp = stdout;
    } else {
        fp = fopen(outfile, "wb");
        if (fp == NULL) {
            fprintf(stderr, "Failed to create file: %s\n", outfile);
            perror(NULL);
            exit(EXIT_FAILURE);
        }
    }

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        fprintf(stderr, "Failed to initialize PNG writer.\n");
        exit(EXIT_FAILURE);
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        fprintf(stderr, "Failed to initialize PNG write.\n");
        exit(EXIT_FAILURE);
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fprintf(stderr, "Failed to write PNG image.\n");
        exit(EXIT_FAILURE);
    }

    palette = (png_colorp) malloc(sizeof(png_color) * 2);
    if (palette == NULL) {
        fprintf(stderr, "Failed to allocate memory.\n");
        exit(EXIT_FAILURE);
    }
    palette[0].red = fg_color[0];
    palette[0].green = fg_color[1];
    palette[0].blue = fg_color[2];
    palette[1].red = bg_color[0];
    palette[1].green = bg_color[1];
    palette[1].blue = bg_color[2];
    alpha_values[0] = fg_color[3];
    alpha_values[1] = bg_color[3];
    png_set_PLTE(png_ptr, info_ptr, palette, 2);
    png_set_tRNS(png_ptr, info_ptr, alpha_values, 2, NULL);

    png_init_io(png_ptr, fp);
    png_set_IHDR(png_ptr, info_ptr,
                 realwidth, realwidth,
                 1,
                 PNG_COLOR_TYPE_PALETTE,
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);
    png_set_pHYs(png_ptr, info_ptr,
                 dpi * INCHES_PER_METER,
                 dpi * INCHES_PER_METER,
                 PNG_RESOLUTION_METER);
    png_write_info(png_ptr, info_ptr);

/* top margin */
    memset(row, 0xff, (realwidth + 7) / 8);
    for (y = 0; y < margin * size; y++) {
        png_write_row(png_ptr, row);
    }

/* data */
    p = qrcode->data;
    for (y = 0; y < qrcode->width; y++) {
        bit = 7;
        memset(row, 0xff, (realwidth + 7) / 8);
        q = row;
        q += margin * size / 8;
        bit = 7 - (margin * size % 8);
        for (x = 0; x < qrcode->width; x++) {
            for (xx = 0; xx < size; xx++) {
                *q ^= (*p & 1) << bit;
                bit--;
                if (bit < 0) {
                    q++;
                    bit = 7;
                }
            }
            p++;
        }
        for (yy = 0; yy < size; yy++) {
            png_write_row(png_ptr, row);
        }
    }
/* bottom margin */
    memset(row, 0xff, (realwidth + 7) / 8);
    for (y = 0; y < margin * size; y++) {
        png_write_row(png_ptr, row);
    }

    png_write_end(png_ptr, info_ptr);
    png_destroy_write_struct(&png_ptr, &info_ptr);

    fclose(fp);
    free(row);
    free(palette);

    return 0;
}

/**
 * Funcion que inicializa la pila de bloques
 * @param cantRegistros
 */
void crearSuperBloque(int cantRegistros)
{
    //{creationTime: 1376483073, mounted: 50, devId:20, freeStart:1, freeEnd:25, root:26, maxBlocks:10000}
    //inodes seran del 1 al 400
    pila = crearPila(cantRegistros);
}

/**
 * Funcion que guarda el estado de la pila en QRs
 */
void guardarPila()
{
    char datosGuardar[128];
    char* qr = ".QRP";
    char registroPila[10];
    int registro = 0;

    char* actual = popQR(pila);

    do {
        int datosAct = 0;
        if (actual != 0) {
            strcpy(datosGuardar, ",");
            strcat(datosGuardar, actual);
            datosAct = datosAct + strlen(actual) + 1;
        }

        while (actual != 0 && datosAct < 128) {
            free(actual);
            actual = popQR(pila);
            if (actual != 0) {
                datosAct = datosAct + strlen(actual) + 1;
                if (datosAct < 128) {
                    strcat(datosGuardar, ",");
                    strcat(datosGuardar, actual);
                }
            }
        }
        strcat(datosGuardar, ",");

        sprintf(registroPila, "%d", registro);
        strcat(registroPila, qr);
        createQR(registroPila, datosGuardar);
        memset(registroPila, 0, sizeof registroPila);
        memset(datosGuardar, 0, sizeof datosGuardar);
        registro++;
    }while (actual != 0);
    free(pila);
}

/**
 * Funcion que lee los QR para reestablecer la pila
 * @param cant
 */
void reestablecerPila(int cant)
{
    char datosLeidos[128*cant*2];
    char qrName[10];
    restablecido = 1;
    char* token;

    for(int i = 0; i < cant; ++i) {
        memset(qrName, 0, sizeof qrName);
        sprintf(qrName, "%d", i);
        strcat(qrName, ".QRP");
        char *datos = (char *) malloc(5 * sizeof(char));
        datos = leerQR(qrName);

        if(strcmp(datos, "-1") == 0)
        {
            printf("No se encontro el archivo\n");
        } else {
            token = strtok(datos, ":");
            token = strtok(NULL, ",");
            if(token != NULL){
                while(token != NULL && strcmp(token, "\n") != 0){
                    strcat(datosLeidos, token);
                    strcat(datosLeidos, ",");
                    token = strtok(NULL, ",");
                }
                token = NULL;
            }
        }
    }
    //printf("%s\n", datosLeidos);
    token = strtok(datosLeidos, ",");
    if(token != NULL){
        while(token != NULL && strcmp(token, "\n") != 0){
            pila = restaurarPila(pila, cambiarDireccion(token));
            token = strtok(NULL, ",");
        }
        token = NULL;
    }
    //printf("Pila:\n");
    //verificarPila(pila);
}

/**
 * Funcion que reemplaza memoria con una nueva
 * @param token
 * @return
 */
char* cambiarDireccion(char* token)
{
    char* tokenTemp = (char*) malloc(4 * sizeof(char));
    strcpy(tokenTemp, token);
    return tokenTemp;
}
