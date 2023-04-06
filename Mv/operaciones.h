typedef char hexa[2];
typedef char reg[3];
typedef char matR[5];

typedef struct{
char tipo;
matR mat;
}regmat;


/*void lee_operando(int *ip,unsigned short int vec[],hexa a[]){
    char reg[1],mem[5],inme[3];
    char reg2[1],mem2[5];
    if (cant_operando(a[*ip])==2){
        if (vec[0]==1){
            (*ip)++;
            reg[0]=a[*ip][0];
            reg[1]=a[*ip][1];
            printf("%c %c\n",reg[0],reg[1]);
        }else{
           (*ip)++;
            mem[0]=a[*ip][0];
            mem[1]=a[*ip][1];
           (*ip)++;
            mem[2]=a[*ip][0];
            mem[3]=a[*ip][1];
           (*ip)++;
            mem[4]=a[*ip][0];
            mem[5]=a[*ip][1];
            printf("%c %c %c %c %c %c\n",mem[0],mem[1],mem[2],mem[3],mem[4],mem[5]);
        }
        if (vec[2]==1){
           (*ip)++;
            reg2[0]=a[*ip][0];
            reg2[1]=a[*ip][1];
            printf("%c %c\n",reg2[0],reg2[1]);
        }else{
            if (vec[3]==0){
                (*ip)++;
                mem2[0]=a[*ip][0];
                mem2[1]=a[*ip][1];
               (*ip)++;
                mem2[2]=a[*ip][0];
                mem2[3]=a[*ip][1];
                (*ip)++;
                mem2[4]=a[*ip][0];
                mem2[5]=a[*ip][1];
                printf("%c %c %c %c %c %c\n",mem2[0],mem2[1],mem2[2],mem2[3],mem2[4],mem2[5]);
            }else{
               (*ip)++;
                inme[0]=a[*ip][0];
                inme[1]=a[*ip][1];
                (*ip)++;
                inme[2]=a[*ip][0];
                inme[3]=a[*ip][1];
                printf("%c %c\n",inme[0],inme[1],inme[2],inme[3]);
            }
        }
    }else{
         if (vec[0]==1){
            (*ip)++;
            reg[0]=a[*ip][0];
            reg[1]=a[*ip][1];
            printf("%c %c\n",reg[0],reg[1]);
        }else{
            if (vec[1]==0){
                 (*ip)++;
                mem[0]=a[*ip][0];
                mem[1]=a[*ip][1];
               (*ip)++;
                mem[2]=a[*ip][0];
                mem[3]=a[*ip][1];
                (*ip)++;
                mem[4]=a[*ip][0];
                mem[5]=a[*ip][1];
                printf("%c %c %c %c %c %c\n",mem[0],mem[1],mem[2],mem[3],mem[4],mem[5]);
            }else{
                (*ip)++;
                inme[0]=a[*ip][0];
                inme[1]=a[*ip][1];
                (*ip)++;
                inme[2]=a[*ip][0];
                inme[3]=a[*ip][1];
                printf("%c %c\n",inme[0],inme[1],inme[2],inme[3]);
            }
        }
    }
    (*ip)++;
}
*/
void lee_operando(int *ip,unsigned short int vec[],hexa a[]){
    regmat matriz[1];
    if (cant_operando(a[*ip])==2){
        if (vec[0]==1){
            (*ip)++;
            matriz[0].mat[0]=a[*ip][0];
            matriz[0].mat[1]=a[*ip][1];
            matriz[0].tipo='r';
            printf("%c %c\n",matriz[0].mat[0],matriz[0].mat[1]);
        }else{
           (*ip)++;
            matriz[0].mat[0]=a[*ip][0];
            matriz[0].mat[1]=a[*ip][1];
           (*ip)++;
            matriz[0].mat[2]=a[*ip][0];
            matriz[0].mat[3]=a[*ip][1];
           (*ip)++;
            matriz[0].mat[4]=a[*ip][0];
            matriz[0].mat[5]=a[*ip][1];
            matriz[0].tipo='m';
            printf("%c %c %c %c %c %c\n",matriz[0].mat[0],matriz[0].mat[1],matriz[0].mat[2],matriz[0].mat[3],matriz[0].mat[4],matriz[0].mat[5]);
        }
        if (vec[2]==1){
           (*ip)++;
            matriz[0].mat[0]=a[*ip][0];
            matriz[0].mat[1]=a[*ip][1];
            matriz[0].tipo='r';
            printf("%c %c\n",matriz[0].mat[0],matriz[0].mat[1]);
        }else{
            if (vec[3]==0){
                (*ip)++;
                matriz[0].mat[0]=a[*ip][0];
                matriz[0].mat[1]=a[*ip][1];
               (*ip)++;
                matriz[0].mat[2]=a[*ip][0];
                matriz[0].mat[3]=a[*ip][1];
                (*ip)++;
                matriz[0].mat[4]=a[*ip][0];
                matriz[0].mat[5]=a[*ip][1];
                matriz[0].tipo='m';
                printf("%c %c %c %c %c %c\n",matriz[0].mat[0],matriz[0].mat[1],matriz[0].mat[2],matriz[0].mat[3],matriz[0].mat[4],matriz[0].mat[5]);
            }else{
               (*ip)++;
                matriz[0].mat[0]=a[*ip][0];
                matriz[0].mat[1]=a[*ip][1];
                (*ip)++;
                matriz[0].mat[2]=a[*ip][0];
                matriz[0].mat[3]=a[*ip][1];
                matriz[0].tipo='i';
                printf("%c %c\n",matriz[0].mat[0],matriz[0].mat[1],matriz[0].mat[2],matriz[0].mat[3]);
            }
        }
      //operando2
    }else{
         if (vec[0]==1){
            (*ip)++;
            matriz[1].mat[0]=a[*ip][0];
            matriz[1].mat[1]=a[*ip][1];
            matriz[1].tipo='r';
            printf("%c %c\n",matriz[1].mat[0],matriz[1].mat[1]);
        }else{
            if (vec[1]==0){
                 (*ip)++;
               matriz[1].mat[0]=a[*ip][0];
                matriz[1].mat[1]=a[*ip][1];
               (*ip)++;
                matriz[1].mat[2]=a[*ip][0];
                matriz[1].mat[3]=a[*ip][1];
                (*ip)++;
                matriz[1].mat[4]=a[*ip][0];
                matriz[1].mat[5]=a[*ip][1];
                matriz[1].tipo='m';
                printf("%c %c %c %c %c %c\n",matriz[1].mat[0],matriz[1].mat[1],matriz[1].mat[2],matriz[1].mat[3],matriz[1].mat[4],matriz[1].mat[5]);
            }else{
                (*ip)++;
                matriz[1].mat[0]=a[*ip][0];
                matriz[1].mat[1]=a[*ip][1];
                (*ip)++;
                matriz[1].mat[2]=a[*ip][0];
                matriz[1].mat[3]=a[*ip][1];
                matriz[1].tipo='i';
                printf("%c %c\n",matriz[1].mat[0],matriz[1].mat[1],matriz[1].mat[2],matriz[1].mat[3]);
            }
        }
    }
    (*ip)++;

}
