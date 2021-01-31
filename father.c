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
    if(sb->lista_inode[i]->nombre == name) return i;
  }

  return 0;
}