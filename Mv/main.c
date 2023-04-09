#include <stdio.h>
#include <stdlib.h>
#include "funciones.h"
#include "dissasembler.h"

int main()
{
    int max=16384;
    TDD TablaDeDatos[1];//tabla de datos, pos 1 CS, pos 2 DS.
    long int TablaRegistros[15];//long int ya que son 4 bytes.
    char TablaMemoria[16384]; //TablaMemoria es la tabla de memoria donde las primeras n posiciones pertencen al CS.


    Inicia_registro(TablaRegistros);//inicializo la tabla de registros.
    Inicia_memoria(TablaMemoria,TablaDeDatos);//inicio la tabla de memoria y la de datos.
    Dissasembler_mostrar(TablaMemoria,TablaRegistros,TablaDeDatos);
    //Lectura(TablaMemoria,TablaRegistros,TablaDeDatos);
}

void Inicia_registro(long int TablaRegistros[]){
        TablaRegistros[0]=0;//cs
        TablaRegistros[1]=0x00010000;//ds
        TablaRegistros[5]=0;//ip
        TablaRegistros[8]=0;//cc
        TablaRegistros[9]=0;//ac
}

void Inicia_memoria(char *TablaMemoria,TDD TablaDeDatos[]){
    char lec;
    FILE *arch=fopen("prueba4.vmx","rb");
    int i=0;

    TablaDeDatos[0].pos=0;
    while (!feof(arch)){
        fread(&lec,sizeof(char),1,arch);
        if (i==7){
            TablaDeDatos[0].tamano=(int)lec;
            TablaDeDatos[1].pos=lec;
            TablaDeDatos[1].tamano=sizeof(TablaMemoria)-(int)lec;
        }
        if (i>7){
            TablaMemoria[i-8]=lec;
        }
        i++;
    }
    fclose(arch);
}

void Lectura(char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]){
    char top1,top2,operacion;
    short int cant;
    long int op1,op2;
    int corte;
    while (TablaRegistros[5]!=TablaDeDatos[1].pos && cant!=0)
    {
        op1=0;
        op2=0;
        operacion=TablaMemoria[TablaRegistros[5]]&0x0F;
        Cant_op(TablaMemoria[TablaRegistros[5]],&top1,&top2,&cant);
        corte=top1^0x3;//le asigno el opuesto del tipo1 el cual representa la longitud
        if (cant==2){
            for (int i=1;i<=corte;i++){
                TablaRegistros[5]++;
                op1+=(TablaMemoria[TablaRegistros[5]])&0x000000FF;
                if (i!=corte)
                    op1=op1<<8;
            }
            corte=top2^0x3;//le asigno el opuesto del tipo2 el cual representa la longitud
            for (int i=1;i<=corte;i++){
                TablaRegistros[5]++;
                op2+=(TablaMemoria[TablaRegistros[5]])&0x000000FF;
                if (i!=corte)
                    op2=op2<<8;
            }
        }else{
            for (int i=1;i<=corte;i++){
                TablaRegistros[5]++;
                op1+=(TablaMemoria[TablaRegistros[5]])&0x000000FF;
                if (i!=corte)
                    op1=op1<<8;
            }
        }
        TablaRegistros[5]++;// le sumo uno mas al ip antes de que se ejecute la operacion.
        Pre_Funcion(cant,op1,op2,top1,top2,operacion,TablaMemoria,TablaRegistros,TablaDeDatos);//con esto arracamos la parte de hacer la funcion
    }
}

void Cant_op(char instruccion,char *top1,char *top2,short int *cant){
    *top1=(instruccion>>6)&0x3;//haciendo el corrimiento me quedaria 00XX donde XX es el valor del operando
    *top2=(instruccion>>4)&0x3;//haciendo la operacion tambien quedaria 00XX.
    if (*top1==3)
        *cant=0;
    if (*top2==3)
        *cant=1;
    else
        *cant=2;
}
void Dissasembler_mostrar(long int TablaRegistros[],char *TablaMemoria,TDD TablaDeDatos[]){
int corte;
char top1,top2,operacion;
short int cant;
long int op1,op2,contador=TablaRegistros[5];
    printf("%d",contador);
    while (contador!=TablaDeDatos[1].pos){
        operacion=TablaMemoria[contador]&0x0F;
        Muestra_mem(contador);
        Muestra_byte((TablaMemoria[contador])&0x0F);
        Cant_op(TablaMemoria[contador],&top1,&top2,&cant);
        corte=top1^0x3;
        if (cant==2){
            for (int i=1;i<=corte;i++){
                contador++;
                Muestra_byte(TablaMemoria[contador]);
                op1+=(TablaMemoria[contador])&0x000000FF;
                if (i!=corte)
                    op1=op1<<8;
            }
            corte=top2^0x3;//le asigno el opuesto del tipo2 el cual representa la longitud
            for (int i=1;i<=corte;i++){
                contador++;
                Muestra_byte(TablaMemoria[contador]);
                op2+=(TablaMemoria[contador])&0x000000FF;
                if (i!=corte)
                    op2=op2<<8;
            }
        }else{
            for (int i=1;i<=corte;i++){
                contador++;
                Muestra_byte(TablaMemoria[contador]);
                op1+=(TablaMemoria[contador])&0x000000FF;
                if (i!=corte)
                    op1=op1<<8;
            }
        }
        Muestra_dissasembler(cant,op1,op2,top1,top2,operacion,TablaMemoria,TablaRegistros,TablaDeDatos);
    }
}
