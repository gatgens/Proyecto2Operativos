static int my_mkdir(const char* path, mode_t mode) {
  printf("El path es: %s y es largo es: %li\n",path,strlen(path));
  int father_id = find_i_father(path);
  printf("El id del padre es: %i\n",father_id);
  //char name[] = get_name_form_path(path);
  //strcpy(name,get_name_form_path(path));
  printf("El nombre es: %s\n", get_name_form_path(path));

  for(int i=0; i<100; i++) {
    if(sb->lista_inode[i]->nombre == get_name_form_path(path) && sb->lista_inode[i]->padre[0] == father_id) {
      return -1; //El fichero ya existe
    }
  }

  int new;

  new = crear_inodo(path, mode, 0, 1);
  //printf("newnode es: %i\n",new);
  for(int i = 0; i<100; i++) {
    sb->lista_inode[new]->hijos[i] = -1;
  }

  sb->lista_inode[new]->padre[0] = father_id;
  //lista_inode[new] = sb->lista_inode[new];

  file_to_sb(new);
  actualizar();

  return 0;
}