#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include <graphics.h>

#define MAXIMG 20
#define M 50//Columnas
#define N 80//Renglones
#define AVAN 20
#define AVY 10
#define R 10

typedef struct//Estructura para botones
{
   int x,y,n; //Variables de trazado de boton
   int color,colorf; //Variable de color
   int edo;//Variable de estado
}BOTON;

typedef struct
{
   char nombre[40];//Nombre del jugador.
   int pts;//Puntaje del jugador.
}RECORD;

typedef struct{
    int i,j;
    int iCol2;
}stGuardar;
//Estructuras de Malla----------------------------------------------------------
typedef struct nodo{
	int x;
	int y;
	struct nodo *sig,*ant;
	struct nodo *arr,*aba;
}*NODO;

//------------------------------------------------------------------------------
//Funciones de Malla------------------------------------------------------------
void creaMalla(NODO *cab);
int creaNodo(NODO *nodo,int x,int y);//Cre un nodo nuevo
int insertaIn(NODO *ren,NODO *cab,NODO *ult,NODO *bot,int i,int x,int y);//Inserta nodos en ciclo interno
int insertaExt(NODO *ren,NODO *cab,NODO *ult,NODO *bot,int j,int x,int y);//Inserta nodos en ciclo externo(Malla)
int enlazaExt(NODO *ren1,NODO *ren2);//Enlaza renglones
int enlazaIn(NODO *ren1,NODO *ren2);//Enlaza columnas
//------------------------------------------------------------------------------
void principal();
void abrir(char archivo[] ,int *matriz, int xi,int yi,int xf,int yf);//Abre imagenes
void cargarImagenes(void *img , void *imgArr[MAXIMG]);//Carga todas las imagenes
void enemigos(int x,int y,void *img);
void enemy(int x,int y,void *img);
void jugador(int x,int y,int edo,void *img,void *img2);
void carretera(int x,int y,void *linea,void *acera);
void jugar(NODO cab,void *imgArr[],int tm,int *logro,int limite);//Funcion principal de juego
void mover(NODO *cab,char tecla);//Movimiento desde teclado
void marcador(int pts,int vida,int nvl,int tiempo);
void ayuda();
void records(int pts,int m);
//------------------------------------------------------------------------------
void menu();
int moverMouse();
void botonesInact();//Pinta botones color CYAN                                    *
void botonPasJugar(BOTON b);//Pinta boton inactivo jugar                          *
void botonPasRecords(BOTON b);//Pinta boton inactivo Records                      *
void botonPasAyuda(BOTON b);//Pinta boton inactivo Ayuda                          *
void botonAct(BOTON b,int y);//Pinta botones activos                              *
void decor();//Decoracion de portada

int main( )
{
	initwindow( 970 , 600 , "Road Fighter" );
    settextstyle(3,1,1);
    menu();
    //principal();

	closegraph();
	return(0);
}
//------------------------------------------------------------------------------
void principal(){
    int tamMalla;
    void *img;//Apuntador temporal a imagenes
    void *imgArr[MAXIMG];//Almacenando direccion de apuntador en arreglo
    int altura,tm,pts=0;//Control de altura de scroll
    int limite=80,velocidad=500;
    setbkcolor(BLACK);

    NODO cab=NULL,ren=NULL,ult=NULL,bot;//inicializando punteros
    int res,logro;//Manejo de errores

    res=insertaExt(&ren,&cab,&ult,&bot,0,0,0);//Creando malla
    res=enlazaExt(&cab,&(*cab).aba);//Terminando malla
    cleardevice();
    jugar(cab,imgArr,tamMalla,&res,10);
}
//------------------------------------------------------------------------------
void jugar(NODO cab,void *imgArr[],int tm,int *logro,int limite){//Moviendo jugador
     NODO aux,yVil;
     int pag=0,ti;
     int centro=430;//Centrando imagenes
     int xE,xEnemy,yEnemy=600;//Ubicando imagenes al centro
     char tecla,mensaje[40];
     time_t tm1,tm2;//Variables de tiempo
     void *img;//Apuntador temporal a imagenes
     int scroll=0;//Control de altura de pista
     int control=0;//Controlando funciones
     int acelerador=2;//Aceleracion
     int edo=1;//Estado de carro
     int pts=0,nvl=1,vida=3,tiempo;//Marcadores
     
     tm=20;
     while(cab->x < 530 ){//Fijando objetos en el centro
         cab=cab->sig;
     }
     
     xE=cab->x;
     aux=yVil=cab;//Regresa las coordenadas al principio
     cargarImagenes(img,imgArr);//Cargando imagenes
     srand(time(NULL));
     xEnemy=rand()% 280 + 350;
     ti=tm1=clock();//Iniciando tiempo
     
     setvisualpage(pag);
     setactivepage(!pag);
     
     while(tecla!=27 && control<2){
         if(kbhit()){//Movimiento
            tecla=getch();//Capturando teclado
            if(edo==1)//No puede moverse si auto no existe
                mover(&(cab),tecla);//Accediendo a valores de nodos
         }
         //Animacion de inicio
         if(control==0){
             enemigos(xE,yEnemy,imgArr[2]);//Enemigos
             carretera(230,scroll,imgArr[1],imgArr[3]);//Pintando pista
             yEnemy-=1;//Animando enemigos al salir
             if(yEnemy<=0)//Cuando recorren toda la pantalla
                 control=1;//Pasan control a jugador
         }
         else if(control==1){//Scroll de carretera
             carretera(230,scroll,imgArr[1],imgArr[3]);//Animando pista
             /*Jugador, controlado por teclas izquierda y derecha*/
             jugador(cab->x,getmaxy()-100,edo,imgArr[0],imgArr[4]);//JUGADOR
             if(control==1 && edo==1){
                scroll+=acelerador;//Acelerando poco a poco
             }
             if(scroll>=75)//Animacion de pista
                 scroll=0;   
         }
         /*Realizando funciones de enmigo
         *@param 1 x pos con random
         *@param 2 y pos con valores de malla
         *@param 3 imagen*/
         enemy(xEnemy,yVil->y,imgArr[2]);//Pintando enemigo movil
         
         /*Marcadores en pantalla*/    
         tiempo=(clock()-ti)/1000;//Obteniendo tiempo en segundos
         marcador(pts,vida,nvl,tiempo);
         
         tm2=clock();
         if(tm2-tm1>tm && tiempo >4){//Controlando movimientos por tiempo
             if(yVil->y < 700 && edo==1)//
                 yVil=yVil->aba;//Avanzando hacia abajo
             else//En caso de choque deja a jugador atras
                 yVil=yVil->arr;//Avanzando hacia arriba
             if(yVil->y > 680){// Una vez recorrida la pantalla regresa al principio
                 yVil = aux;//Reseteando coordenadas
                 xEnemy=rand()% 280 + 360;//Añadiendo coordenada x de enemigo
                 pts+=50;
             }
             if(yVil->y > getmaxy()-75  && cab->x > xEnemy-50 && cab->x < xEnemy+50){//En caso de choque
                 edo=0;//Auto destruido
                 vida--;//Disminuyendo vidas
                 if(vida==-1){
                    control=2;
                 }
             }
             if(yVil->y<10)
                 edo=1;//Reseteando carro
             
             tm1=tm2;//Control de tiempo
         }
         setvisualpage(!pag);
         setactivepage(pag);
         pag=!pag;
         cleardevice();
     }
     setvisualpage(0);
     setactivepage(0);
     records(pts,1);
     menu();
}

void marcador(int pts,int vida,int nvl,int tiempo)
{
   char mensaje[40];
				
   outtextxy( 200-textwidth("Puntaje:"), 70, "Puntaje:");//Letreros del marcador
   outtextxy( 200-textwidth("Vidas:"), 140, "Vidas:");//
   outtextxy( 200-textwidth("Nivel:"), 210, "Nivel:");
   outtextxy( 200-textwidth("Tiempo:"), 280, "Tiempo:");

   setcolor(WHITE);//Color para las cadenas de datos variables
   sprintf( mensaje, " %d", pts);
   outtextxy( 250-textwidth( mensaje),70, mensaje);//Puntos
   sprintf( mensaje, " %d", vida);
   outtextxy( 250-textwidth( mensaje),140, mensaje);//Vidas
   sprintf( mensaje, " %d", nvl);
   outtextxy( 250-textwidth( mensaje),210, mensaje);//Nivel
   sprintf( mensaje, " %d", tiempo);
   outtextxy( 250-textwidth( mensaje), 280, mensaje);//Tiempo
}

/*Movimiento del enemigo*/
void enemy(int x,int y,void *img){
    putimage(x,y-100,img,XOR_PUT);//Enemigo
}

void jugador(int x,int y,int edo,void *img,void *img2){
    if(edo==1)
        putimage(x,y,img,XOR_PUT);//JUGADOR
    else
        putimage(x,y,img2,XOR_PUT);//JUGADOR
}

/*Esta funcion coloca los enemigos y los hace avanzar al principio
 *del juega
 *@param x es la posicion en x
 *@param y es la posicion en y
 *@param img es el apuntador a imagen
*/
void enemigos(int x,int y,void *img){
     int dist=200,alt=100;

     putimage(x-dist,y-alt,img,XOR_PUT);//Enemigo
     putimage(x-dist,y-alt*2,img,XOR_PUT);//Enemigo
     putimage(x-dist,y-alt*3,img,XOR_PUT);//Enemigo
     putimage(x,y-alt*2,img,XOR_PUT);//Enemigo
     putimage(x,y-alt*3,img,XOR_PUT);//Enemigo    
}

/*Coloca la carretera y la hace moverse
 *@param linea es la linea blanca de la carretera
 *@param acera es la imagen de las barras laterales de la carretera
*/
void carretera(int x,int y,void *linea,void *acera){////////////////////////////
     int dist=75,ancho=430;
     putimage(x+200,y-100,linea,COPY_PUT);//Linea
     putimage(x+200,y,linea,COPY_PUT);//Linea
     putimage(x+200,y+100,linea,COPY_PUT);//Linea
     putimage(x+200,y+200,linea,COPY_PUT);//Linea
     putimage(x+200,y+300,linea,COPY_PUT);//Linea
     putimage(x+200,y+400,linea,COPY_PUT);//Linea
     putimage(x+200,y+500,linea,COPY_PUT);//Linea
     
     putimage(x,y-dist,acera,XOR_PUT);//Acera
     putimage(x,y,acera,XOR_PUT);//Acera
     putimage(x,y+dist,acera,XOR_PUT);//Acera
     putimage(x,y+dist*2,acera,XOR_PUT);//Acera
     putimage(x,y+dist*3,acera,XOR_PUT);//Acera
     putimage(x,y+dist*4,acera,XOR_PUT);//Acera
     putimage(x,y+dist*5,acera,XOR_PUT);//Acera
     putimage(x,y+dist*6,acera,XOR_PUT);//Acera
     putimage(x,y+dist*7,acera,XOR_PUT);//Acera
     
     putimage(x+ancho,y-dist,acera,XOR_PUT);//Acera
     putimage(x+ancho,y,acera,XOR_PUT);//Acera
     putimage(x+ancho,y+dist,acera,XOR_PUT);//Acera
     putimage(x+ancho,y+dist*2,acera,XOR_PUT);//Acera
     putimage(x+ancho,y+dist*3,acera,XOR_PUT);//Acera
     putimage(x+ancho,y+dist*4,acera,XOR_PUT);//Acera
     putimage(x+ancho,y+dist*5,acera,XOR_PUT);//Acera
     putimage(x+ancho,y+dist*6,acera,XOR_PUT);//Acera
     putimage(x+ancho,y+dist*7,acera,XOR_PUT);//Acera
}

/*Captura el teclado para realizar el movimiento del jugador
 *@param *cab apuntador a malla
 *@param tecla es la tecla presionada por el usuario
*/
void mover(NODO *cab,char tecla){//Moviendo jugador
     if(tecla==75){//Izquierda
         if((*cab)->x > 260)//Limites de carretera
             (*cab)=(*cab)->ant;
     }else
     if(tecla==77){//Derecha
         if((*cab)->x<620)
         (*cab)=(*cab)->sig;
     }
}
//------------------------------------------------------------------------------

void cargarImagenes(void *img , void *imgArr[MAXIMG])//Carga todas las imagenes
{
   int tamMalla=2;
   int i,cord=140,ang=4;

   setcolor(0);
   //settextstyle(3,0,4);
   outtextxy(300,470,"LOADING. . .");

   cleardevice();
   abrir("car.txt",&tamMalla,0,0,10,10);//Abriendo imagen
   img=malloc(imagesize(625,437,725,540));//Memoria de tamaño de imagen
   getimage(625,437,725,540,img);//Apuntando a imagen
   imgArr[0]=img;//Guardando apuntador a imagen en un arreglo de apuntadores
   
   cleardevice();
   abrir("linea.txt",&tamMalla,0,0,10,10);//Abriendo imagen
   img=malloc(imagesize(625,437,725,540));//Memoria de tamaño de imagen
   getimage(625,437,725,540,img);//Apuntando a imagen
   imgArr[1]=img;//Guardando apuntador a imagen en un arreglo de apuntadores
   
   cleardevice();
   abrir("enemy.txt",&tamMalla,0,0,10,10);//Abriendo imagen
   img=malloc(imagesize(625,437,725,540));//Memoria de tamaño de imagen
   getimage(625,437,725,540,img);//Apuntando a imagen
   imgArr[2]=img;//Guardando apuntador a imagen en un arreglo de apuntadores

   cleardevice();
   abrir("acera.txt",&tamMalla,0,0,10,10);//Abriendo imagen
   img=malloc(imagesize(625,437,725,540));//Memoria de tamaño de imagen
   getimage(625,437,725,540,img);//Apuntando a imagen
   imgArr[3]=img;//Guardando apuntador a imagen en un arreglo de apuntadores
   
   cleardevice();
   abrir("carCrash.txt",&tamMalla,0,0,10,10);//Abriendo imagen
   img=malloc(imagesize(625,437,725,540));//Memoria de tamaño de imagen
   getimage(625,437,725,540,img);//Apuntando a imagen
   imgArr[4]=img;//Guardando apuntador a imagen en un arreglo de apuntadores

}


void abrir(char archivo[], int *matriz, int xi,int yi,int xf,int yf)
{
     FILE *arch;//Puntero a archivo de texto
     stGuardar *abre;//Puntero a no se que
     int i,j,aux=0;

     arch=fopen(archivo,"rb");//Abriendo archivo
     if(arch==NULL){//Si el archivo no existe
          setcolor(WHITE);
          outtextxy(200,200,"Error al abrir el archivo!");
     }
     fread(&aux,sizeof(int),1,arch);//Leyendo archivo
     fread(matriz,sizeof(int),1,arch);
     abre=(stGuardar*)malloc(sizeof(stGuardar)*aux);
     for(int i=0;i<aux;i++)
     {
          fread(&abre[i],sizeof(stGuardar),1,arch);
          putpixel(xi+abre[i].i,yi+abre[i].j,abre[i].iCol2);
     }
     free(abre);
     fclose(arch);
}
//MALLA******************************************************************************
void creaMalla(NODO *cab){
     NODO ren=NULL,ult=NULL,bot=NULL;//inicializando punteros
     int res;

     res=enlazaExt(&(*cab),&(*cab)->aba);
}

int creaNodo(NODO *nodo,int x,int y){//
	int res=0;

	*nodo=(NODO)malloc(sizeof(struct nodo));//Reservando memoria
	if(*nodo){//Si hay memoria
        (*nodo)->x=x;//Apuntando a dato
        (*nodo)->y=y;
        (*nodo)->sig=NULL;//Sellando punteros
        (*nodo)->ant=NULL;
        (*nodo)->arr=NULL;
        (*nodo)->aba=NULL;
        res=1;//Manejo de errores
    }
    return res;
}
//----------------------------------------------------------------------------------------
int insertaExt(NODO *ren,NODO *cab,NODO *ult,NODO *bot,int j,int x,int y){//Inserta datos en columna
    int res;//Manejo de errores

	if(!*cab){//Si cab esta vacía
		res=insertaIn(&(*ren),&(*cab),&(*ult),&(*bot),0,x,y);//Generando renglon
            //*ren=*cab;
		    if(j<N)
		        res=insertaExt(ren,&(*cab)->aba,&(*ult)->aba,&(*bot),++j,x,AVY+y);
	}
    return res;
}
//----------------------------------------------------------------------------------------
int insertaIn(NODO *ren,NODO *cab,NODO *ult,NODO *bot,int i,int x,int y){//Inserta datos en renglon
    int res;//Manejo de errores

    if(i<M){//Nodos laterales
        if(!*cab)
			res=creaNodo(&(*cab),x,y);//Creando un nodo
			    if(i==0)
                  *ren=*cab;
            if(*cab)
                (*cab)->ant=*ult;
				(*cab)->arr=*bot;
                *bot=*ult=*cab;
				    res=insertaIn(&(*ren)->sig,&(*cab)->sig,ult,&(*bot)->sig,++i,AVAN+x,y);
				(*ren)=*cab;
    }
    return res;
}
//----------------------------------------------------------------------------------------
int enlazaExt(NODO *ren1,NODO *ren2){//Se desplaza por renglones
    int res;

    if(*ren2){
       res=enlazaIn(ren1,ren2);
            res=enlazaExt(&(*ren1)->aba,&(*ren2)->aba);
    }
    return res;
}
//----------------------------------------------------------------------------------------
int enlazaIn(NODO *ren1,NODO *ren2){//Enlaza datos verticalmente
    int res;

    if(*ren1){
        (*ren1)->aba=*ren2;
        (*ren2)->arr=*ren1;
        res=enlazaIn(&(*ren1)->sig,&(*ren2)->sig);
        res=1;
	}
	return res;
}


//Termina MALLA-----------------------------------------

//Funciones de menu
void nomJugador(char cadena[])
{
   int i, xText, yText,tecla,contador=0;
   
   cleardevice();
   xText=( getmaxx()-getmaxx()%4)/4-textwidth( cadena)/2;//Posicion del mensaje en la pantalla.
   yText=( getmaxy()-getmaxy()%3)*2/3-textheight( cadena)/2;
   setcolor(WHITE);
   strcpy( cadena, "NOMBRE: ");//Mensaje para mostrar en la pantalla.
   outtextxy( xText, yText, cadena);//Imprime el mensaje en la pantalla.
   
   
   cadena[0]='\0';//Inicializa la cadena.
   do{
      do{//parpadeo
         setcolor( LIGHTGRAY);//Escribe un guion bajo para indicar que lee el texto.
         outtextxy(xText+textwidth(cadena), yText+120, "_");
         delay(50);//Tiempo de espera.
         setcolor(BLACK);
         outtextxy(xText+textwidth(cadena), yText+120, "_");//Escribe el guion bajo, pero del color del fondo, para que no sea visible.
         delay(50);
      }while( !kbhit());
      setcolor(WHITE);
      tecla=getch();
      if(tecla==0)//si es codigo extendido
         tecla=getch();
      if(tecla==8 && contador>0)//Si presiona la tecla "backspace(<-)"...
      {
         outtextxy(xText+textwidth(cadena), yText+100, cadena);//Escribe la cadena en la pantalla con el color de fondo.
         cadena[--contador]='\0';//Se elimina el ultimo caracter de la cadena.
         setcolor(WHITE);//Selecciona el color del texto.
         outtextxy(xText+textwidth(cadena), yText+100, cadena);//Escribe la cadena en la pantalla.
      }
      else
      {
         if(tecla!=13)//Si presiona la tecla "ENTER"...
         {
            cadena[contador++]=tecla;//Termina la lectura del nombre y muestra la cadena en la pantalla.
            cadena[contador]='\0';
            outtextxy(xText+textwidth(cadena), yText+100, cadena);
         }
      }
   }while(tecla!=13);
}

void ordena( RECORD a[R])
{
    RECORD j;//Variable auxiliar de tipo RECORD para el intercambio.
    int rec, i;

    for( rec=1; rec<R; rec++)//Primer recorrido del arreglo.
        for( i=0; i<R-rec; i++)//Segundo recorrido del arreglo.
            if( a[i].pts<a[i+1].pts )//Si el numero de esta posicion es menor al de la posicion siguiente...
            {        
                j=a[i];//Se intercambia la posicion.
	            a[i]=a[i+1];
	            a[i+1]=j;
            }
}

void records(int pts,int m)//Llamada en botonPasRecords(); y al final de partida
{
   RECORD a[R];
   FILE *f;
   int i, y=190;
   char cadena[50];

   cleardevice();//Limpia pantalla.
   settextstyle(3,1,1);
   if(m==1)//Determina si la llamada es en menuControl o al terminar partida
   {
      nomJugador(cadena);//Funcion que pide el nombre al jugador.
   } 
   cleardevice();//Limpia la pantalla.

   f=fopen("Records.dat","ab");//Abre el archivo para inicializarlo.
   if(f!=NULL)
   {//Si no existe, lo crea.
      //fseek(f,0,SEEK_END);//Si existe, va al final del archivo...
      i=ftell(f)/sizeof(RECORD);//Calcula el tamaño.
      if(i<N)
         for( ;i<R;i++)
         {//Lo inicializa.
            a[i].nombre[0]='\0';
            a[i].pts=-1;
         }
         fwrite(&a,sizeof(RECORD),R,f);//Escribe los valores del arreglo en el archivo.
   }
   
   fclose(f);//Cierra el archivo.
   f=fopen("Records.dat","rb+");//Abre el archivo para lectura y escritura.
   fread(&a,sizeof(RECORD),R,f);//Lee el archivo y almacena los datos en el arreglo.
   strcpy(a[R-1].nombre, cadena);//Escribe el nombre del jugador en la ultima posicion del arreglo.
   a[R-1].pts=pts;//Guarda los puntos en el arreglo.
   ordena(a);//Ordena los datos por la puntuacion( de mayor a menor).
   outtextxy((getmaxx()/2+textwidth("Mejores puntuaciones:")/2)-10,y-80,"Mejores puntuaciones:");//Letrero.
   setcolor(WHITE);//Color del texto.
   for(i=0;i<R-1;i++)
      if(a[i].pts>=0)//Si existe un valor del juego almacenado en el arreglo...
      {
         y+=textheight("H")+10;//Cambia de renglon.
         cadena[0]='\0';//Inicializa la cadena para darle el valor de los puntos.
         sprintf(cadena,"%d",a[i].pts);//Pasa los puntos a una cadena.
         outtextxy((getmaxx()/3)+textwidth(a[i].nombre),y,a[i].nombre);//Nombre del 
                                                                                          //jugador
         outtextxy((getmaxx()/3)*2,y,cadena);//Puntos.
      }
   rewind(f);//Regresa al inicio del archivo.
   fwrite(&a,sizeof(RECORD),R,f);//Escribe los datos ordenados al archivo.
   getch();//Espera una tecla para salir del juego.
   setfillstyle(1,BLACK);//Definiendo color de 
   bar3d(0,0,getmaxx(),getmaxy(),0,0);
   fclose(f);      
}

void menu()//Llamada en el main
{
     BOTON b;
     int i;
     
     while(i!=27)
     {  
        setbkcolor(BLACK);//Color de fondo negro
        cleardevice();//Limpiando pantalla
        moverMouse();//Funcion para detectar movimiento de raton
        botonPasJugar(b);//Boton jugar
        botonesInact();//Solo pinta los botones
     }
}

void botonesInact()//Esta funcion es llamada en moverMouse();
{
     BOTON b;
     
     b.x=50;
     b.y=150;
     int i,tam=50;
     setfillstyle(1,BLACK);//Definiendo color de fondo
     bar3d(0,0,getmaxx(),getmaxy(),0,0);
     setcolor(CYAN);
     setbkcolor(CYAN);//Definiendo color de fondo
     setfillstyle(1,CYAN);//Define color de boton a negro
     bar3d(b.x,b.y,b.x+tam*2,b.y+tam,0,0);//Pinta boton Jugar inactivo
     bar3d(b.x,300,b.x+tam*2,300+tam,0,0);//Pinta boton Records inactivo
     bar3d(b.x,450,b.x+tam*2,450+tam,0,0);//Pinta boton Ayuda
     
     setcolor(WHITE);
     //settextstyle(3,1,1);//Define la fuente 
     outtextxy(122,483,"Ayuda");//Texto para boton ayuda
     outtextxy(120,183,"Jugar");//Texto de boton jugar
     outtextxy(129,333,"Records");//Texto para boton Records
     setbkcolor(BLACK);//Definiendo color de fondo
     outtextxy(getmaxx()/2+240,getmaxy()/2+25,"Elije una opcion para comenzar");//Salida de texto
     decor();
}

int moverMouse()//Llamada en menuControl()
{
     BOTON b;//Variable de coordenadas para funciones boton
     int page=0;
     
     setactivepage(page);  // 0
     setvisualpage(!page); // 1
     do
     {
        setfillstyle(1,BLACK);  
        if(mousex()>50 && mousex()<150 && mousey()>150 && mousey()<200)//Si el mouse esta en las 
        //coordenadas del boton 1
        {
           botonAct(b,150);//Llamada a boton activo 1
           setcolor(WHITE);
           //settextstyle(3,1,1);//Define la fuente
           setbkcolor(RED);
           outtextxy(120,183,"Jugar");
        }
        else
        if(mousex()>50 && mousex()<150 && mousey()>300 && mousey()<350)//Si el mouse esta en las 
        //coordenadas del boton 2
        {
           botonAct(b,300);//Llamada a boton activo 2
           setcolor(WHITE);
           //settextstyle(3,1,1);//Define la fuente
           setbkcolor(RED);
           outtextxy(129,333,"Records");
        }
        else
        if(mousex()>50 && mousex()<150 && mousey()>450 && mousey()<500)//Si el mouse esta en las 
        //coordenadas del boton 3
        {
           botonAct(b,450);//Llamada a boton activo 3
           setcolor(WHITE);
           //settextstyle(3,1,1);//Define la fuente
           setbkcolor(RED);
           outtextxy(122,483,"Ayuda");
        }
        else
        {
           botonesInact();
           setbkcolor(BLACK);
        }
     setbkcolor(BLACK);
     setactivepage(!page);// 1
     setvisualpage(page); // 0
     page=!page;
     }while(!ismouseclick(WM_LBUTTONDOWN));//Mientras que no se de click izquierdo
  cleardevice();
  setactivepage(0);
  setvisualpage(0);
}

void botonPasJugar(BOTON b)//Llamada en menuControl()    //Pinta boton jugar
{   
     b.x=50;
     b.y=150;
     int tam=50;
     int i=0,xr,yr;
     int pts; 
     
     if(ismouseclick(WM_LBUTTONDOWN))//Condicion de click en boton
     {
        floodfill(1,1,GREEN);
       
        xr=mousex();//Variable de raton en x
        yr=mousey();//Variable de raton en y
        if(xr>50 && xr<150 && yr>150 && yr<200)//Click en jugar (BOTON 1)
        {
           principal();//Funcion que contiene a opcionJugar();
               
        }
        else
        if(xr>50 && xr<150 && yr>300 && yr<350)//Click en records
        {
          records(pts,0);
        }
        else
        if(xr>50 && xr<150 && yr>450 && yr<500)//Click en ayuda
        {
          ayuda();
        }
     }
     clearmouseclick(WM_LBUTTONDOWN);
     setbkcolor(BLACK);
}

void botonAct(BOTON b,int y)//Llamada en moverMouse()    //Pinta determinado boton de color rojo
{   
     b.x=50;
     b.y=150;
     b.n=3;
     int tam=50;
     
     setcolor(RED);
     setfillstyle(1,RED);//Define color de boton a rojo   
     bar3d(b.x,y,b.x+tam*2,y+tam,0,0);//Pinta barra para boton activo
}

void decor()//Llamada en moverMouse();    //Decoracion de portada
{                  
     int i,j;
     int n1=250,n2=600;//Limite de inio y final
     
     for(i=200;i<n1;i+=5)//Incrementando coordenadas en x
         for(j=1;j<n2;j+=5)//Incrementando coordenadas en y
             putpixel(i,j,WHITE);//Trazando pixel color blanco
     line(252,0,252,getmaxy()-2);//Trazando linea
}

void ayuda(){
     
}
