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
#define N 50//Renglones
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
void jugar(NODO cab,void *imgArr[],int tm,int *logro,int limite,int *pts);//Funcion principal de juego
void mover(NODO *cab,char tecla);//Movimiento desde teclado
//------------------------------------------------------------------------------
int main( )
{
	initwindow( 970 , 600 , "Elevator Action" );

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
    outtextxy((getmaxx()/2)-textwidth
    ("Se supone que ya se creo la malla")/2,200,
    "Se supone que ya se creo la malla");
    jugar(cab,imgArr,tamMalla,&res,10,&pts);
    //jugar()
    //getch();

    //cargarImagenes(img,imgArr);//Cargando imagenes

}
//------------------------------------------------------------------------------
void jugar(NODO cab,void *imgArr[],int tm,int *logro,int limite,int *pts){//Moviendo jugador
     int tamMalla,hJuga,hEsce=0,i,anim,alerta,pag=0,tiempo=0,ti;
     int jExist=1,salida=0;
     int centro=430;//Ubicando imagenes al centro
     char tecla,mensaje[40];
     time_t tm1,tm2;//Variables de tiempo
     void *img;//Apuntador temporal a imagenes
     
     while(cab->x < 530){//Fijando objetos en el centro
         cab=cab->sig;
     }
     
     
     cargarImagenes(img,imgArr);//Cargando imagenes

     ti=tm1=clock();//Iniciando tiempo
     setvisualpage(pag);
     setactivepage(!pag);
     
     while(tecla!=27 && salida==0){
         if(kbhit()){//Movimiento
            tecla=getch();//Capturando teclado
                mover(&(cab),tecla);//Accediendo a valores de nodos
         }
         //Colocando imagenes---------------------------------------------------
         putimage(centro,cab->y,imgArr[1],XOR_PUT);//Linea
         putimage(cab->x,getmaxy()-100,imgArr[0],XOR_PUT);//JUGADOR
         putimage(cab->x,getmaxy()-200,imgArr[2],XOR_PUT);//Enemigo
         tm2=clock();
         if(tm2-tm1>tm){//Controlando movimientos por tiempo
             

             //-----------------------------------------------------------------
             tm1=tm2;//Control de tiempo
         }
         //---------------------------------------------------------------------
         
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
//------------------------------------------------------------------------------
void mover(NODO *cab,char tecla){//Moviendo jugador

     if(tecla==72){//arriba
         if((*cab)->y > 0)//
            (*cab)=(*cab)->arr;
     }else
     if(tecla==80){//Abajo
         if((*cab)->y < 500)
             (*cab)=(*cab)->aba;
     }else
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

