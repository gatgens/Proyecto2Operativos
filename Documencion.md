# Tecnológico de Costa Rica
## Escuela de Ingeniería en Computación
### Principios de sistemas operativos

# RememberRemember

### Desarrollado por
* #### Kevin Segura Rojas (2017153767)
* #### Carlos Mora Murillo (2017238926)
* #### Jorge Gatgens Villalobos (2017110109)

##### Alajuela, II-S 2020

******

## Introdución

En este proyecto se realizará la reimplementación de varias funciones del file system, las
cuales son el montaje de archivos. Los archivos que se manejaran seran archivos QR, los
cuales seran montados en el sistema operativo para su almacenamiento y manejo de los
mismos.
El proyecto esta compuesto por las siguientes partes:
- QRFS: Son las funciones que se va a reimplementar y las cuales se encargan del manejo de los archivos.
- mkfs.qrfs: Es el medio por el cual se crea el sistema de archivos y define con cual es el primer archivo de la particion.
- fsck.qrfs: Realiza el checkeo del QRFS
- mount.qrfs: Es el medio por el cual se montan los archivos en memoria para su almacenamiento



## Ambiente de Desarrollo

CLion: Mediante estos editores se realizó el código de todo el proyecto y el debbugeo necesario.

GCC: mediante esta herramienta compilamos el código de proyecto.

Ubuntu Linux: Sistema operativo en el que se desarrolló todo el proyecto.

Github: Utilizamos este servicio como alojamiento de nuestro repositorio del proyecto.


## Estructuras de datos usadas y funciones

#### Listas circulares

### Librerías utilizadas 

#### Ncurses.h
### UContext.h
### String.h


### Biblioteca mypthreads


#### my_thread_init

Esta función se encarga de iniciar un hilo que va a controlar todos los demás y además inicia toda la biblioteca.

#### my_thread_create

esta función se encarga de crear un hilo mas no realiza su ejecución, solamente le asigna su espacio y setea sus parámetros y contextos para su futura ejecución.

#### my_thread_yield
Con esta función se logra pausar la ejecución de un hilo.


#### my_thread_exit
Esta función se encarga de terminar la ejecución de un hilo sin embargo este sigue activo.

#### my_thread_chsched
Función que se encarga de cambiar el scheduler de un hilo durante su ejecución.

#### my_thread_sleep
Esta función se encarga de dormir la ejecución de un hilo por una cantidad de tiempo.

#### my_thread_cancel

Esta función se encarga de cancelar la ejecución de un hilo y dejarlo inutilizable.

#### my_thread_join

Esta función es la encargada de agregar hilos a la cola de ejecución de hilos y esperar a que termine la ejecución de los demás hilos.

#### my_thread_detach
Esta función hace que el hilo no retorna ningún valor al finalizar su ejecución. 

#### my_mutex_init
Esta función se encarga de inicializar las estructuras para el mutex.

#### my_mutex_lock

Esta función se encarga de bloquear la memoria compartida mediante un mutex.

#### my_mutex_unlock

Esta función se encarga de desbloquear la memoria compartida del mutex para el uso de los demás hilos.

#### realTime

Función que se encarga de llevar a cabo el scheduler de Real Time

#### SorteScheduler 

Esta es la función principal que ejecula el scheduler de sorteo.

#### RoundRobinScheduler

Función que ejecuta el scheduler round robin en los hilos.

### Biblioteca Animación

#### AnimarFigura e imprimirFigura

Funciones que llevan a cabo el animar y mostrar la objeto en el canvas.

#### MoverFigura

Función que mueve la animación en el canvas.

#### PrepararFigura

Función que carga la figura seleccionada en memoria con los parametros elegidos para esta.

## Instrucciones para ejecutar el programa

#### Para probar los hilos

gcc  -Wall pruebaHilos.c Schedulers.c mypthreads.c DataStructures.c -o prueba

./prueba

#### Para probar la animación

gcc -Wall Animacion.c Lenguaje.c -o Animacion -lncurses

./Animacion

## Actividades realizadas por estudiante

#### Carlos Mora M

|                                  Actividad                                      |    Fecha   | Tiempo     |
|:-------------------------------------------------------------------------------:|:----------:|------------|
| Se inicio la investigación sobre el trabajo a realizar                          | 08/11/2020 | 5 horas    |
| Se realizo lectura sobre manejo de hilos en C                                   | 13/11/2020 | 5 horas    |
| Se realizo lectura e investigación sobre la implementación de hilos en C        | 18/11/2020 | 6 horas    |
| Se realizaron pruebas de manejo de funcionalidad de u_context en C              | 25/11/2020 | 5 horas    |
| Se realizo el primer intento de implementación de hilos en C                    | 26/11/2020 | 4 horas    |
| Se hizo la primer implementación de manejo de hilos en C medainamente funcional | 27/11/2020 | 3 horas    |
| Se realizó más investigación sobre la implementación de hilos en C              | 30/11/2020 | 3 horas    |
| Se creo el primer prototipo de hilos                                            | 03/12/2020 | 5 horas    |
| Se creo un versión más pulida de los hilos                                      | 04/12/2020 | 3 horas    |
| Se investigo sobre el mutex y animaciones                                       | 04/12/2020 | 3 horas    |
| Se implemento el mutex                                                          | 05/12/2020 | 3 horas    |
| Se realizaron las pruebas de funcionamiento                                     | 06/12/2020 | 2 horas    |
| Se realizo la documentación escrita de la tarea                                 | 06/12/2020 | 3 horas    |

Horas Totales: 50 horas.


#### Kevin Segura

|                                  Actividad                                      |    Fecha   | Tiempo     |
|:-------------------------------------------------------------------------------:|:----------:|------------|
| Se inicio la investigación sobre el trabajo a realizar                          | 08/11/2020 | 4 horas    |
| Se realizo lectura sobre manejo de Scheduers en C                               | 12/11/2020 | 4 horas    |
| Se realizo lectura e investigación sobre la implementación de schedulers en C   | 15/11/2020 | 4 horas    |
| Se realizaron pruebas de manejo de funcionalidad de u_context en C              | 20/11/2020 | 5 horas    |
| Se realizo el primer intento de implementación de scheduler en C                | 25/11/2020 | 4 horas    |
| Se hizo la primer implementación de manejo de hilos en C medianamente funcional | 26/11/2020 | 2 horas    |
| Se realizó más investigación sobre la implementación de hilos en para la        |                         | 
solución de errores encontrados                                                   | 28/11/2020 | 3 horas    |
| Se creo el primer prototipo de hilos                                            | 29/12/2020 | 5 horas    |
| Se creo un versión más pulida de los hilos                                      | 30/12/2020 | 1 horas    |
| Se investigo sobre el mutex                                                     | 01/12/2020 | 3 horas    |
| Se implemento el mutex                                                          | 01/12/2020 | 3 horas    |
| Se realizaron las pruebas de funcionamiento                                     | 01/12/2020 | 2 horas    |
| Se comenzo con el diseño de la animación y buscar arte ASCII                    | 02/12/2020 | 3 Horas    |
| Se realizaron funciones como la de cargar archivos y una animación básica.      | 03/12/2020 | 4 Horas    |
| Se comenzo a programas la funcionalidad de multiples Displays.                  | 04/12/2020 | 3 Horas    |
| Se detuvo el anterior trabajo para la investigación de errores encontrados.     | 05/12/2020 | 4 Horas    |
| Se continuo investigando sobre los errores.                                     | 06/12/2020 | 5 Horas    |
| Se realizo la documentación escrita de la tarea                                 | 06/12/2020 | 1 hora     |

Horas Totales: 60 horas.


#### Jorge Gatgens

|                                  Actividad                                      |    Fecha   | Tiempo     |
|:-------------------------------------------------------------------------------:|:----------:|------------|
| Se inicio la investigación sobre el trabajo a realizar                          | 08/11/2020 | 5 Horas    |
| Se investigo y diseño las animaciones                                           | 03/12/2020 | 6 Horas    |
| Implementación de canvas                                                        | 04/12/2020 | 3 Horas    |
| Implementación del menú de las animaciones junto con sus restricciones          | 04/12/2020 | 3 Horas    |
| Investigaciones sobre errores encontrados (Malloc principalmente)               | 05/12/2020 | 5 Horas    |
| Se realizaron las pruebas de funcionamiento                                     | 06/12/2020 | 2 Horas    |
| Se siguió investigando posibles soluciones para los errores                     | 06/12/2020 | 4 Horas    |
| Se realizo la documentación escrita de la tarea                                 | 06/12/2020 | 3 Horas    |

Horas Totales: 31 horas.

## Evaluación

## Autoevaluación

#### Carlos Mora

#### Kevin Segura 


#### Jorge Gatgens


## Estado Final del proyecto

## Problemas encontrados y limitaciones

## Reporte de Commits


## Lecciones Aprendidas


## Bibliografía

Droid, L. I. (2015). Machinary. Obtenido de Machinary:
https://machiry.wordpress.com/2012/01/02/mfiber-simple-user-land-threadlibrary/
McGraw-Hill, E. (2008). MailxMail. Obtenido de MailxMail:
http://www.mailxmail.com/curso-informatica-procesos/procesos-c-funcion-sigprocmask
Perepelitsa, C. (2013). Quora. Obtenido de Quora: https://www.quora.com/Howdo-you-write-a-C-program-to-split-a-string-by-a-delimiter
Pina, C.(2004). MANUAL DE INICIACIÓN A LA LIBRERÍA NCURSES. Recuperado de: http://index-of.co.uk/Programming/ncurses.pdf
ASCII Art Archive.(s.f). Recuperado de: https://www.asciiart.eu/
