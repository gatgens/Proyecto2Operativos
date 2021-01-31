#define FUSE_USE_VERSION 30
#define _FILE_OFFSET_BITS 64
#include <fuse.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
// r g b r g b r g b - - - r g b
/* imagenes tienen 1920*1080 pixeles, cada 4 pixeles hay un byte entonces 1920*1080/4 = 518 400*/
#define TOTAL_BYTES 518400
#define TAM_BLOQUE 15
typedef struct bmpFileHeader
{
  /* 2 bytes de identificación */
  uint32_t size;        /* Tamaño del archivo */
  uint16_t resv1;       /* Reservado */
  uint16_t resv2;       /* Reservado */
  uint32_t offset;      /* Offset hasta hasta los datos de imagen */
} bmpFileHeader;

typedef struct bmpInfoHeader
{
  uint32_t headersize;      /* Tamaño de la cabecera */
  uint32_t width;               /* Ancho */
  uint32_t height;          /* Alto */
  uint16_t planes;                  /* Planos de color (Siempre 1) */
  uint16_t bpp;             /* bits por pixel */
  uint32_t compress;        /* compresión */
  uint32_t imgsize;     /* tamaño de los datos de imagen */
  uint32_t bpmx;                /* Resolución X en bits por metro */
  uint32_t bpmy;                /* Resolución Y en bits por metro */
  uint32_t colors;              /* colors used en la paleta */
  uint32_t imxtcolors;      /* Colores importantes. 0 si son todos */
} bmpInfoHeader;

typedef struct i_node{ 

	int id;
	char path[80];
	char nombre[80];
  
	mode_t permisos;
	time_t fecha_acceso;
	time_t fecha_modificacion;
	time_t fecha_creacion;

	int cantidad_punteros;
	int punteros[TOTAL_BYTES]; 
  int es_fichero;
  int padre[1]; // 7
  int hijos[100]; // 15 19 24

}my_i_node;


typedef struct SuperBloque{
	  int bytes_disponibles;
    int bloques[TOTAL_BYTES];
    my_i_node *lista_inode[100];

}my_super_bloque;

my_super_bloque *sb;
my_i_node lista_inode[100];

bmpInfoHeader header;     /* cabecera de la imagen*/
unsigned char *imgdata;   /* datos de imagen 1920*1080*3 */

int buscar_archivo(const char *filename){
  for(int i = 0; i < 100; i++){
    if(!strcmp(filename,lista_inode[i].path) && lista_inode[i].id != -1){
      return i;
    }
  }
  return -1;
}


int byte_invalido(int byte){
  double numero = (((double)byte+3)/12.0);
  if(!byte) return 0;
  return (fabs(floor(numero) - (double)numero) < 0.00001);
}


unsigned char *LoadBMP(char *filename, bmpInfoHeader *bInfoHeader)
{

  FILE *f;
  bmpFileHeader header;     /* cabecera */
  unsigned char *imgdata;   /* datos de imagen */
  uint16_t type;        /* 2 bytes identificativos */

  f=fopen (filename, "rb");
  if (!f)
    return NULL;        /* Si no podemos leer, no hay imagen*/

  /* Leemos los dos primeros bytes */
  fread(&type, sizeof(uint16_t), 1, f);
  if (type !=0x4D42)        /* Comprobamos el formato */
    {
      fclose(f);
      return NULL;
    }

  /* Leemos la cabecera de fichero completa */
  fread(&header, sizeof(bmpFileHeader), 1, f);
  /* Leemos la cabecera de información completa */
  fread(bInfoHeader, sizeof(bmpInfoHeader), 1, f);

  
  imgdata=(unsigned char*)malloc(bInfoHeader->imgsize);
  fseek(f, header.offset, SEEK_SET);

  /* Leemos los datos de imagen, tantos bytes como imgsize */
  fread(imgdata, bInfoHeader->imgsize,1, f);
  /* Cerramos */
  fclose(f);
  /* Devolvemos la imagen */
  return imgdata;
}

void DisplayInfo(bmpInfoHeader *info)
{
  printf("Tamaño de la cabecera: %u\n", info->headersize);
  printf("Anchura: %d\n", info->width);
  printf("Altura: %d\n", info->height);
  printf("Planos (1): %d\n", info->planes);
  printf("Bits por pixel: %d\n", info->bpp);
  printf("Compresión: %d\n", info->compress);
  printf("Tamaño de datos de imagen: %u\n", info->imgsize);
  printf("Resolucón horizontal: %u\n", info->bpmx);
  printf("Resolucón vertical: %u\n", info->bpmy);
  printf("Colores en paleta: %d\n", info->colors);
  printf("Colores importantes: %d\n", info->imxtcolors);
}

void SaveBMP(char *filename, bmpInfoHeader *info, unsigned char *imgdata)
{
  bmpFileHeader header;
  FILE *f;
  uint16_t type;
 
  f=fopen(filename, "wb");
  header.size=info->imgsize+sizeof(bmpFileHeader)+sizeof(bmpInfoHeader);
 
  header.offset=sizeof(bmpFileHeader)+sizeof(bmpInfoHeader)+2;
  
  type=0x4D42;
  fwrite(&type, sizeof(type),1,f);
 
  fwrite(&header, sizeof(bmpFileHeader),1,f);
 
  fwrite(info, sizeof(bmpInfoHeader),1,f);
  
  fwrite(imgdata, info->imgsize, 1, f);
  fclose(f);
}

void actualizar(){
 
  FILE *estructura, *nodos;
  estructura = fopen("Estructura.dat","wb");
  int e = fwrite(sb,sizeof(my_super_bloque),1,estructura);
  if(e) printf("Bloque escrito correctamente! \n");
  fclose(estructura);
  estructura = NULL;
    
  nodos = fopen("Nodos.dat","wb");
  fwrite(lista_inode,sizeof(struct i_node),100,nodos);
}


static int my_rename(const char *path1,const char *path2){
  printf("cambiando nombre de %s a %s",path1,path2);

  int nodo = buscar_archivo(path1);
  if(nodo != -1){
      strcpy(lista_inode[nodo].path,path2);
      actualizar();
      return 0;
  }
  return -1;
}

static int my_access(const char *path, int mask){

  int nodo = buscar_archivo(path);
  if(nodo == -1 && strcmp(path,"/")){
    return -2; // no existe
  }
  else{//aqui deberia de haber una opcion para limitar el acceso a los usuarios que tengan permiso
      //pero lo vamos a dejar asi, para eso sirve la mascara.
    return 0;
  }

}

int find_i_father(const char *path) {
  //my_i_node *father;
  //father = sb->lista_inode[0];

  char name[80];
  int name_i = 0;
  int father_i = -1;
  int father_j = -1;
  
  for(int i=strlen(path)-1; i > 0 ; i--) {
    if(path[i] == '/'){ // Saltamos el nombre del archivo y tomamos el padre
      father_i = i;
      for(int j = i-1; j > 0; j--) { //Guardamos los índices que abarcar el nombre del padre
        if(path[j] == '/' || j == 0) father_j = j+1;
        break;
      }
    }
    if(father_j != -1) break;
  }

  for(int father_j; father_j < father_i; father_j++){
    name[name_i] = path[father_j];
    name_i++; 
  }

  for(int i=0; i <100; i++) {
    if(sb->lista_inode[i] == NULL) continue;
    if(strcmp(sb->lista_inode[i]->nombre,name) == 0) return i;
  }

  return 0;
}



char *get_name_form_path(const char* path) {

  char *name = (char *)malloc(80*sizeof(char));
  int name_i = 0;
  
  for(int i=strlen(path)-1; i >= 0 ; i--) {
    if(path[i] == (char)'/'){ // Saltamos el nombre del archivo y tomamos el padre
      name_i = i+1;
      break;
    }
  }
  int i=0;
  for(; name_i < strlen(path); name_i++) {
    name[i] = path[name_i];
    i ++;
  }

  return name;
}

void file_to_sb(int nodo_libre){
  lista_inode[nodo_libre].id = sb->lista_inode[nodo_libre]->id;
  strcpy(lista_inode[nodo_libre].path, sb->lista_inode[nodo_libre]->path);
  strcpy(lista_inode[nodo_libre].nombre, sb->lista_inode[nodo_libre]->nombre);
  lista_inode[nodo_libre].permisos = sb->lista_inode[nodo_libre]->permisos;
  lista_inode[nodo_libre].fecha_acceso = sb->lista_inode[nodo_libre]->fecha_acceso;
  lista_inode[nodo_libre].fecha_modificacion = sb->lista_inode[nodo_libre]->fecha_modificacion;
  lista_inode[nodo_libre].fecha_creacion = sb->lista_inode[nodo_libre]->fecha_creacion;
  lista_inode[nodo_libre].cantidad_punteros = sb->lista_inode[nodo_libre]->cantidad_punteros;
  lista_inode[nodo_libre].padre[0] = sb->lista_inode[nodo_libre]->padre[0];
  for(int i=0; i <100; i++) {
    lista_inode[nodo_libre].hijos[i] = sb->lista_inode[nodo_libre]->hijos[i];
    //printf("Los hijos: %i\n",sb->lista_inode[nodo_libre]->hijos[i]);
  }

  for(int p = 0; p < lista_inode[nodo_libre].cantidad_punteros; p++){
      lista_inode[nodo_libre].punteros[p] = sb->lista_inode[nodo_libre]->punteros[p];
  }
  lista_inode[nodo_libre].es_fichero = sb->lista_inode[nodo_libre]->es_fichero;

}
// borrar vuffer cuando pueda
int crear_inodo(const char *path, size_t size, mode_t modo, int es_fichero){


  int nodo_libre;
  for(int i = 0; i < 100;i++){
      if(sb->lista_inode[i] == NULL){
        printf("Nodo %i LIBRE\n",i);
        sb->lista_inode[i] = (my_i_node*)malloc(sizeof(my_i_node));
          
          
          for(int j=0; j<strlen(get_name_form_path(path)); j++) {
            ////printf("CHAR %s\n",sb->lista_inode[i]->nombre);
            ////printf("---------------------------------------------\n");
            
            sb->lista_inode[i]->nombre[j] = get_name_form_path(path)[j];
          }
          for(int j=0; j<100; j++) {
            sb->lista_inode[i]->hijos[j] = -1;
          }
          sb->lista_inode[i]->padre[0] = find_i_father(path);


        sb->lista_inode[i]->id = i;
        strcpy(sb->lista_inode[i]->path,path);
        sb->lista_inode[i]->cantidad_punteros = (int)size;
        //nuevo_inode->punteros = (int*)malloc(sizeof(int)*(int)size);
        sb->lista_inode[i]->permisos = modo;
        sb->lista_inode[i]->es_fichero = es_fichero;
        sb->lista_inode[i]->fecha_creacion = time(NULL);
        sb->lista_inode[i]->fecha_modificacion = time(NULL);
        sb->lista_inode[i]->fecha_acceso = time(NULL);
        sb->lista_inode[i]->padre[0] = find_i_father(path);
        nodo_libre = i;

        file_to_sb(nodo_libre);

        return nodo_libre;
        break;
      }
  }

  return -1;
}


static int my_mkdir(const char* path, mode_t mode) {
  //printf("El path es: %s y es largo es: %li\n",path,strlen(path));
  int father_id = find_i_father(path);
  //printf("El id del padre es: %i\n",father_id);
  //char name[] = get_name_form_path(path);
  //strcpy(name,get_name_form_path(path));
  //printf("El nombre es: %s\n", get_name_form_path(path));

  for(int i=0; i<100; i++) {
    if(sb->lista_inode[i]->nombre == get_name_form_path(path) && sb->lista_inode[i]->padre[0] == father_id) {
      return -1; //El fichero ya existe
    }
  }

  int new;

  new = crear_inodo(path, mode, 0, 1);
  ////printf("newnode es: %i\n",new);
  for(int i = 0; i<100; i++) {
    sb->lista_inode[new]->hijos[i] = -1;
  }

  sb->lista_inode[new]->padre[0] = father_id;
  //lista_inode[new] = sb->lista_inode[new];

  file_to_sb(new);
  actualizar();

  return 0;
}


static int my_rmdir(const char* path) {
  int father_id = find_i_father(path);
  //printf("Sí entra.....................\n");
  for(int i=0; i<100; i++) {
    if(sb->lista_inode[i] == NULL) continue;
    //printf("El nombre en sb: %s .... y el nombre en func: %s\n",sb->lista_inode[i]->nombre,get_name_form_path(sb->lista_inode[i]->path));
    //printf("father_id: %i  ....  el NPE: %i\n",father_id,sb->lista_inode[i]->padre[0]);
    //printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    
    if(strcmp(sb->lista_inode[i]->nombre,get_name_form_path(path)) == 0 && sb->lista_inode[i]->padre[0] == father_id) {
      //printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!-----------\n");
      for(int j=0; j<100; j++) {
        //if(sb->lista_inode[i]->hijos[j] == NULL) continue;
        //printf("El i: %i  ............... el valor: %i\n",j,lista_inode[i].hijos[j]);
        if(lista_inode[i].hijos[j] != -1) return -1;// No se puede eliminar el fichero porque tinen hijos
      }
      //printf("Me lleva la puta!!!!!!!!!!!!!!!11\n");
      sb->lista_inode[i]->id = -1;
      //sb->lista_inode[i] = NULL;
      file_to_sb(i);
      actualizar();
      return 0;
    }
  }
  //printf("Aquí está mal!!!!!!!!!!!!!!!11\n");
  return -1; //El fichero no existe
}

static int my_readdir( const char* path, void* buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info* fi ) {
    
  //const char *, void *, fuse_fill_dir_t, off_t,struct fuse_file_info *
  filler( buf, ".", NULL, 0 ); // Actual
  filler( buf, "..", NULL, 0 ); // Padre

  //printf("%s",path);

    for ( int curr_idx = 0; curr_idx < 100; curr_idx++ ){
      //printf("El indice es: %i\n",curr_idx);
      printf("El padre sefun: %i\n",find_i_father(path));
      if(sb->lista_inode[curr_idx] != NULL && sb->lista_inode[curr_idx]->padre[0] == find_i_father(path)) {
        if(sb->lista_inode[curr_idx]->id != -1){

          //if(sb->lista_inode[curr_idx] != NULL){
            filler( buf, sb->lista_inode[curr_idx]->nombre, NULL, 0 );
            //printf("Es: %s\n",sb->lista_inode[curr_idx]->nombre);
          //}
          //printf("Aquí???????????????????????????\n");
        }
      }
    }
	

  return 0;
}





int my_open(const char *path, struct fuse_file_info *fi) {
	printf("open\n");

  int nodo = buscar_archivo(path);
  printf("%i\n",nodo);
	return 0;
}

static int my_read(const char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi ){

  int nodo = buscar_archivo(path);
  if(nodo == -1)
    return -1;
  
  /*el archivo existe y es directorio */
  if(sb->lista_inode[nodo]->es_fichero)
    return 0;
  
  
  /*el archivo existe */
  /* sacamos los 8 bits de cada byte */
  int infobyte = 0;
  char *data = (char*)malloc(sb->lista_inode[nodo]->cantidad_punteros*sizeof(char));
  for(int byte = 0; byte < sb->lista_inode[nodo]->cantidad_punteros; byte++){
    infobyte = 0;
    int bloque = sb->bloques[ sb->lista_inode[nodo]->punteros[byte] ]; // apunta al primer pixel
    for(int bit = 0, bytepix = 0; bit < 8; bit++){
      
      if(byte_invalido(bloque+bytepix)){
              bytepix += 3;
      }
      infobyte += ((imgdata[bloque+bytepix]&1) << bit) ;
      if(bit%2) bytepix+=2;
      else bytepix++;
    }
    data[byte] = infobyte;
  }
  memcpy(buffer,data+offset,size);

  return strlen(data)-offset;




}

static int my_getattr(const char *path, struct stat *st){

/*aqui hay que tomar el i-node que contenga el path, y llenar la informacion de abajo*/
  printf("GETATTR %s \n",path);
  int nodo = buscar_archivo(path);
st->st_uid = getuid();
	st->st_gid = getgid();
	st->st_atime = time( NULL );
	st->st_mtime = time( NULL );

  st->st_mode = S_IFREG | 0644;
		st->st_nlink = 1;
		st->st_size = 1024;

  if(!strcmp(path,"/")){
    st->st_uid = getuid();
    st->st_gid = getgid();
    st->st_ctime = time(NULL);
    st->st_atime = time( NULL ); // ultimo acceso
	  st->st_mtime = time( NULL ); // ultima modificacion
    st->st_mode = S_IFDIR | 0755;
    st->st_nlink = 2; // ver http://unix.stackexchange.com/a/101536
  }
  else if(nodo != -1){


    st->st_uid = getuid();
    st->st_gid = getgid(); 
    st->st_ctime = lista_inode[nodo].fecha_creacion;
    st->st_atime = lista_inode[nodo].fecha_acceso; // ultimo acceso
    st->st_mtime = lista_inode[nodo].fecha_modificacion; // ultima modificacion

    if(lista_inode[nodo].es_fichero){ //hay que agregar otra condicion por si el el path resulta ser un fichero
        st->st_mode = S_IFDIR | 0755;
        st->st_nlink = 2; // ver http://unix.stackexchange.com/a/101536
    }
    else{
      st->st_mode = S_IFREG | 0644;
      st->st_nlink = 1;
      st->st_size = lista_inode[nodo].cantidad_punteros; // este es el tama;o del archivo, hay que ver como reconcoer el tama;o
    }
  }
  else{
    printf("NO EXISTE XD\n");
    return -2;
  }
  return 0;

}






static int my_statfs(const char *path, struct statvfs *fs){
  fs->f_bsize = 1;
  fs->f_blocks = TOTAL_BYTES;
  
  int freeblocks = 0;
  int freefiles = 0;
  for(int i = 0; i < TOTAL_BYTES; i++){
    if(sb->bloques[i] == -1)
      freeblocks++;
  }

  for(int i = 0; i < 100; i++){
    if(sb->lista_inode[i]->id == -1)
      freefiles++;
  }

  fs->f_bfree = freeblocks;
  fs->f_files = TOTAL_BYTES;
  fs->f_ffree = freefiles;
  
  return 0;
}



// %zu size %jd off_t
static int my_write(const char *path, const char *buffer, size_t size, off_t offset, struct fuse_file_info *info )
{
    int nodo_libre = buscar_archivo(path);//crear_inodo(path,size,0,0);
    sb->lista_inode[nodo_libre]->cantidad_punteros = (int)size;
    int bytes_necesarios = (int)size;
    int byte_actual = 0;

    for(int i = 0; bytes_necesarios; i++){
      
      /* si esta libre apuntamos */
        if(sb->bloques[i] == -1){

          sb->bloques[i] = i*TAM_BLOQUE;
          sb->lista_inode[nodo_libre]->punteros[byte_actual] = i;
          
          /*saco cada bit del byte del elemento del buffer que voy a guardar */        
          for(int bit = 0,byte_pixel = 0; bit < 8; bit++){ //cambie esto recordar...
            /* veo si cae un byte dentro de la seccion prohibida */
            if(byte_invalido(byte_pixel+sb->bloques[i])){
              byte_pixel += 3;
            }          
          int estado = (int)buffer[byte_actual] & (1<<bit);
          
          if(estado){ //bit prendido, entonces prendemos el bit menos signficativ
            imgdata[ sb->bloques[i] + byte_pixel ] |= 1;
          }
          else{ //apagamos el bit menos significativo 
            imgdata[ sb->bloques[i] + byte_pixel ] &= ~1;
          }

          /* cada 2 bits hay que pasar de pixel */
            if(bit%2)
              byte_pixel += 2;
            else
              byte_pixel++;
          }

          byte_actual++;
          bytes_necesarios--;
        }
      
    }
    file_to_sb(nodo_libre);
    SaveBMP("disco.bmp",&header,imgdata);
    actualizar();
  
  //fwrite("halo3.bmp",sizeof(sb->imgdata),(size_t)108,sb->imgdata);
	return size;
}


static int my_mknod( const char *path, mode_t mode, dev_t rdev )
{

  printf("creando en %s: ",path);
  crear_inodo(path,0,0,0);
	return 0;
}

static struct fuse_operations operations = {
    .getattr	= my_getattr,
    .write    = my_write,
    .rename   = my_rename,
    .read     = my_read,
    .mkdir    = my_mkdir,
    .statfs   = my_statfs,
    .access   = my_access,
    .mknod    = my_mknod,
    .rmdir    = my_rmdir,
    .readdir  = my_readdir
};


void inicializar_nodos(){

  for(int i = 0; i < 100; i++){
    lista_inode->id = -1;
  }

}



int main(int argc, char *argv[]){

    FILE *estructura,*nodos;
    estructura = fopen("Estructura.dat","rb");

    bmpInfoHeader info;
    unsigned char *img;
    img=LoadBMP("disco.bmp", &info);

    header = info;
    imgdata =img;
    if(estructura == NULL){
      estructura = fopen("Estructura.dat","wb");
      nodos = fopen("Nodos.dat","wb");
    
      sb = (my_super_bloque*)malloc(sizeof(my_super_bloque));
        
      sb->bytes_disponibles = TOTAL_BYTES;
      for(int i = 0; i < TOTAL_BYTES; i++)
          sb->bloques[i] = -1;
                
      for(int i = 0; i < 100; i++){
           sb->lista_inode[i] = (my_i_node*)malloc(sizeof(my_i_node));
           sb->lista_inode[i] = NULL;
           
      }
    inicializar_nodos();
    int e2 = fwrite(lista_inode,sizeof(struct i_node),100,nodos);
    if(!e2) printf("HUBO UN ERORR EN LSO NODOS NOOOO\n");

      int e = fwrite(sb,sizeof(my_super_bloque),1,estructura);
      if(e) printf("Bloque escrito correctamente! \n");
      fclose(estructura);
      estructura = NULL;
        
    }
    else{
            
      my_super_bloque prueba;      
      sb = (my_super_bloque*)malloc(sizeof(my_super_bloque));
      while(1){
         int e = fread(&prueba,sizeof(my_super_bloque),1,estructura);
         if(!e) break;
      }
      sb->bytes_disponibles = prueba.bytes_disponibles;
      for(int i = 0; i < TOTAL_BYTES; i++){
         sb->bloques[i] = prueba.bloques[i];
      }
      fclose(estructura);
      estructura = NULL;
      
      nodos = fopen("Nodos.dat","rb");
      fread(lista_inode,sizeof(my_i_node),100,nodos);

      for(int i = 0; i < 100; i++){
         sb->lista_inode[i] = (my_i_node*)malloc(sizeof(my_i_node));
         sb->lista_inode[i] = prueba.lista_inode[i];
         
         if(sb->lista_inode[i] != NULL){
            
            sb->lista_inode[i] = (my_i_node*)malloc(sizeof(my_i_node));
            sb->lista_inode[i]->id = lista_inode[i].id;
          
            strcpy(sb->lista_inode[i]->path, lista_inode[i].path);
            strcpy(sb->lista_inode[i]->nombre, lista_inode[i].nombre);
            sb->lista_inode[i]->permisos = lista_inode[i].permisos;
            sb->lista_inode[i]->fecha_acceso = lista_inode[i].fecha_acceso;
            sb->lista_inode[i]->fecha_modificacion = lista_inode[i].fecha_modificacion;
            sb->lista_inode[i]->fecha_creacion = lista_inode[i].fecha_creacion;
            sb->lista_inode[i]->cantidad_punteros = lista_inode[i].cantidad_punteros;
            for(int p = 0; p < lista_inode[i].cantidad_punteros; p++){
                sb->lista_inode[i]->punteros[p] = lista_inode[i].punteros[p];
            }
            sb->lista_inode[i]->es_fichero = lista_inode[i].es_fichero;



         }
      }
      
    }
    return fuse_main( argc, argv, &operations, NULL );
}

/* compilar -> gcc -Wall -lm imageFS.c `pkg-config fuse --cflags --libs` -o fs */
/* montar -> ./fs -f [punto de montaje] */