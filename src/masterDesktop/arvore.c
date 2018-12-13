#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define SCALE 100
#define HEIGHT 480
#define PADDING 20
#define DEBUG 1

 #define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

struct node{
    int x;
    int y;
    int adress;
    int index;
    struct node *filhos[5];
};

void createNode(struct node *ptr, int endereco){
    ptr->adress = endereco;
    ptr->index = endereco;
    ptr->filhos[0] = NULL;
    ptr->filhos[1] = NULL;
    ptr->filhos[2] = NULL;
    ptr->filhos[3] = NULL;
    ptr->filhos[4] = NULL;
}

void insert(struct node *p, struct node *i){
    if (i->index < 10){
        p->filhos[(i->index)-1]= i;
        if (DEBUG) printf(">> Inseri o filho %d como filho %d do no %d\n",i->adress, i->index, p->adress);
    }else{
        int pos;
        pos = (i->index)%10; //get last digit
        i->index = floor((i->index)/10); //remove last digit

        struct node *pai;
        pai = p;
        p = p->filhos[pos-1];

        if (p==NULL){
            int tmp = pos*10;
            if (pai->adress > 9)
                tmp = pos*100;
            if (pai->adress > 99)
                tmp = pos*1000;

            if (DEBUG) printf("    NULL pos: %d, index: %d, pai: %d\n", pos, i->index, pai->adress);    
            if (DEBUG) printf("    Logo, tenho que criar o filho %d (%d+%d)\n", tmp+(pai->adress), tmp, pai->adress);

            struct node *ptr;
            ptr = malloc(sizeof(struct node));
            createNode(ptr, tmp+pai->adress);
            pai->filhos[(pos)-1]=ptr;
        }

        p = pai->filhos[pos-1];

        if (p!=NULL)
        insert(p, i);
        

        
    }
}

double calc(struct node *p, double offset, int level){
    p->y = (HEIGHT/2 - PADDING)-(level*SCALE);
    double fd = offset;
    double fe = offset;

    int i,j;
    for (i=0,j=0; j<5,i<5; i++){
        if (p->filhos[i] != NULL){
            fd = calc(p->filhos[i], fd+min(1,j),level+1);
            j++;
        }
    }

    p->x= (((fd-fe)/2)+offset)*SCALE;

    return fd;
}

void printTree(struct node *p){
    printf("%d (%3d,%3d)\n", p->adress, p->x, p->y);

    int i;
    for (i=0; i<5; i++){
        if (p->filhos[i] != NULL){
            printTree(p->filhos[i]);
        }
    }   
}


int main(){
    struct node *raiz;
    raiz = malloc(sizeof(struct node));
    createNode(raiz, 0);

    struct node *ptr;
    ptr = malloc(sizeof(struct node));
    createNode(ptr,1);
    insert(raiz,ptr);

    ptr = malloc(sizeof(struct node));
    createNode(ptr,2);
    insert(raiz,ptr);

    ptr = malloc(sizeof(struct node));
    createNode(ptr,3);
    insert(raiz,ptr);

    ptr = malloc(sizeof(struct node));
    createNode(ptr,11);
    insert(raiz,ptr);

    // ptr = malloc(sizeof(struct node));
    // createNode(ptr,31);
    // insert(raiz,ptr);

    // ptr = malloc(sizeof(struct node));
    // createNode(ptr,431);
    // insert(raiz,ptr);

    ptr = malloc(sizeof(struct node));
    createNode(ptr,2431);
    insert(raiz,ptr);

    printTree(raiz);    

    printf("\n--------------\n");

    calc(raiz,0,0);
    printTree(raiz);

    return 0;
}



