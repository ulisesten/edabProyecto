#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include <graphics.h>

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
//------------------------------------------------------------------------------
#define MAXIMG 20
#define M 50//Columnas
#define N 80//Renglones
#define AVAN 20
#define AVY 10
#define R 10

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
void jugador(int x,int y,void *img);
void carretera(int x,int y,void *linea,void *acera);
void jugar(NODO cab,void *imgArr[],int tm,int *logro,int limite,int *pts);//Funcion principal de juego
void mover(NODO *cab,char tecla);//Movimiento desde teclado
//------------------------------------------------------------------------------
int main( )
{
	initwindow( 970 , 600 , "Road Fighter" );

    principal();

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
    jugar(cab,imgArr,tamMalla,&res,10,&pts);
}
//------------------------------------------------------------------------------
void jugar(NODO cab,void *imgArr[],int tm,int *logro,int limite,int *pts){//Moviendo jugador
     int pag=0,tiempo=500,ti;
     int centro=430;
     int xEnemy,yEnemy=600;//Ubicando imagenes al centro
     char tecla,mensaje[40];
     time_t tm1,tm2;//Variables de tiempo
     void *img;//Apuntador temporal a imagenes
     int scroll=0;
     int control=0;
     int acelerador=2;
     
     tm=100;
     while(cab->x < 530){//Fijando objetos en el centro
         cab=cab->sig;
     }
     
     xEnemy=cab->x;
     cargarImagenes(img,imgArr);//Cargando imagenes

     ti=tm1=clock();//Iniciando tiempo
     
     setvisualpage(pag);
     setactivepage(!pag);
     
     while(tecla!=27){
         if(kbhit()){//Movimiento
            tecla=getch();//Capturando teclado
            mover(&(cab),tecla);//Accediendo a valores de nodos
         }
         //Animacion de inicio
         if(control==0){
             enemigos(xEnemy,yEnemy,imgArr[2]);//Enemigos
             carretera(230,scroll,imgArr[1],imgArr[3]);
             yEnemy-=1;
             if(yEnemy<=0)
                 control=1;
         }
         else if(control==1){//Scroll de carretera
             carretera(230,scroll,imgArr[1],imgArr[3]);
             jugador(cab->x,getmaxy()-100,imgArr[0]);//JUGADOR
             if(control==1){
                scroll+=acelerador;
                //if(acelerador<=10)
                  //  acelerador++;
             }
             if(scroll>=75)
                 scroll=0;
             
         }
         /*Realizando funciones de enmigo
         *@param 1 x pos con random
         *@param 2 y pos con valores de malla
         *@param 3 imagen*/
         enemy(300,cab->y,imgArr[2]);
         
         tm2=clock();
         if(tm2-tm1>tm){//Controlando movimientos por tiempo
             if(cab->y < 600)//
             cab=cab->aba;
             tm1=tm2;//Control de tiempo
             
         }
         tiempo=(clock()-ti)/1000;
         
         setvisualpage(!pag);
         setactivepage(pag);
         pag=!pag;
         cleardevice();
     }
     setvisualpage(0);
     setactivepage(0);
     cleardevice();
}
/*Movimiento del enemigo*/
void enemy(int x,int y,void *img){
    putimage(x,y-100,img,XOR_PUT);//Enemigo
}

void jugador(int x,int y,void *img){
    putimage(x,y,img,XOR_PUT);//JUGADOR
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

     /*if(tecla==72){//arriba
         if((*cab)->y > 10)//
            (*cab)=(*cab)->arr;
     }else
     if(tecla==80){//Abajo
         if((*cab)->y < 500)
             (*cab)=(*cab)->aba;
     }else*/
     if(tecla==75){//Izquierda
         if((*cab)->x > 0)//
             (*cab)=(*cab)->ant;
     }else
     if(tecla==77){//Derecha
         if((*cab)->x<900)
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

