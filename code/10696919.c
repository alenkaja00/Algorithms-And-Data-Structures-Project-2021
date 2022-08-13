#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>


//LIMITE ARBITRARIO LUNGHEZZA DI RIGA DELL'INPUT
#define LIM 400*10+500


//STRUTTURA CHE CONTIENE IL PESO DI UN GRAFO E IL RELATIVO INDICE
typedef struct{
    int peso;
    int index;
}nodo;


//STRUTTURA DI HEAP, CONTIENE L'ARRAY DEI GRAFI(PESO+INDICE), LA DIMENSIONE E IL NUMERO DI ELEMENTI EFFETTIVAMENTE PRESENTI AL SUO INTERNO
typedef struct{
    nodo *array;
    int heapsize;
    int nelem;
}TopK;



//FUNZIONI DI ALLOCAZIONE E CANCELLAZIONE GRAFO
void AggiungiGrafo(int ***g,int dim);
void EliminaGrafo(int ***g,int dim);

//FUNZIONI DI SCAMBIO, FIGLIO SX E DX, PADRE IN UN HEAP(PENSATO COME ALBERO)
void Swap(nodo *p1,nodo *p2);
int Left(int n);
int Right(int n);
int Parent(int n);

//FUNZIONE DI ALLOCAZIONE HEAP, INSERIMENTO, RISTABILISCE LE PROPRIETA', CAMBIA IL VALORE DI UN ELEMENTO 
TopK *CreaClassifica(int length);
void MaxHeapify(TopK *l,int indice);
void Inserisci(TopK *l,nodo g,long int dim);
void AbbassaPriorita(TopK *l,int x,nodo g);


//FUNZIONI DI CALCOLO SOMMA DEI PESI DEI GRAFI E NODO PIU VICINO(MENO COSTOSO) BASATE SULL'ALGORITMO DI DIJKSTRA
int NodoVicino(long long int dist[],int MST[],int dim);
int SommaPesi(int **graph,int dim);





//MAIN DEL PROGRAMMA
int main(){
    
    //parte dichiarativa di variabili
    char buffer[LIM];
    int **graph;
    int i,d;
    long int k;

    TopK *lista;
    nodo g;
    
    g.index=-1;

    //acquisizione parametri d e k
    if(scanf("%d %ld",&d,&k)){}
    
    lista=CreaClassifica(k);


    while(fgets(buffer,LIM,stdin)!=NULL){
        if(buffer[0]=='A'){
            //Eseguo l'istruzione AggiungiGrafo
            AggiungiGrafo(&graph,d);   
            g.peso=SommaPesi(graph,d);
            printf("\nSomma pesi %d: \n",g.peso);
            g.index++;
            Inserisci(lista,g,k);
            EliminaGrafo(&graph,d);
        }
        else if(buffer[0]=='T'){
            //Eseguo l'istruzione TopK
            for(i=0;i<=g.index && i<k;i++){
                printf("%d",lista->array[i].index);
                if(i==g.index || i==k-1)
                    break;
                fputs(" ",stdout);
            }
            fputs("\n",stdout);
        }
    }
    return 0;
}
//FINE DEL MAIN





//FUNZIONI DI ALLOCAZIONE E CANCELLAZIONE GRAFO
void AggiungiGrafo(int ***g,int dim){
    int w=0,z;
    char buffer[LIM];
    int k,ex;
    unsigned long int cont;
    int count=0;
    int x=0;
    int len=0;

    //utilizzo una calloc per inizializzare la matrice a zero
    *g=calloc(dim,sizeof(int *));
    for(int i=0;i<dim;i++){
        (*g)[i]=calloc(dim,sizeof(int));
    }
    
    //leggo le linee e ottengo i valori della matrice mediante una conversione "custom" dell'input
    do{
        k=0;
        cont=1;
        ex=0;
        z=0;
        if(fgets(buffer,LIM,stdin)){}
        len=strlen(buffer)-1;
                
        while(buffer[k]!='\0'){
            cont=cont*10;
            k++;
            if(buffer[k]==','|| k==len){
                if(k==len){
                    k--;
                }
                for(int a=ex;a<=k;a++){
                    cont=cont/10;
                    if(cont==0)
                        break;
                    *(((*g)[w])+z)+=((unsigned long int)(buffer[a])-48)*cont;
                }
                z++;
                x++;
                ex=k+1;
                cont=1;
                k++;
            }
        }
        w++;
        count++;
    }while(count<dim);
}

//libero la memoria occupata dal grafo
void EliminaGrafo(int*** g,int dim){
    int i;
    for(i=0;i<dim;i++)
        free ((*g)[i]);
    free(*g);
    *g=NULL;
}


//funzione di swap
void Swap(nodo *p1,nodo *p2){
    nodo temp=*p1;
    *p1=*p2;
    *p2=temp;
}

//funzione di figlio sx in un array di heap
int Left(int n){
    return 2*n+1;
}

//funzione di figlio dx in un array di heap
int Right(int n){
    return 2*n+2;
}

//funzione di padre in un array di heap
int Parent(int n){
    return (int)(n/2);
}


//questa funzione alloca l'array della classifica
TopK *CreaClassifica(int length){
    TopK *l;
    l=(TopK *)malloc(sizeof(TopK));
    l->array=(nodo*)malloc(length*sizeof(nodo));
    l->heapsize=length;
    l->nelem=0;
    return l;
}

//questa funzione ristabilisce l'ordine dopo un inserimento ad esempio
void MaxHeapify(TopK *l,int indice){
    int i=(indice-1)/2;
    if(l->array[i].peso<l->array[indice].peso){
        Swap(&(l->array[i]),&(l->array[indice]));
        MaxHeapify(l,i);
    }
}

//funzione di inserimento pesi e indici dei grafi all'interno del maxheap
void Inserisci(TopK *l,nodo g,long int dim){
    if(l->nelem < l->heapsize){
        l->array[l->nelem].peso=g.peso;
        l->array[l->nelem].index=g.index;
        MaxHeapify(l,l->nelem);
        l->nelem++;
    }
    else if(l->nelem==dim){
        if(l->array[0].peso>g.peso){
            AbbassaPriorita(l,0,g);
        }
    }
}

//funzione che sostituisce il massimo(primo elemento dell'heap) quando arriva un grafo migliore in classifica
void AbbassaPriorita(TopK *l,int x,nodo g){
    if(g.peso>l->array[x].peso){
        exit(-1);
    }
    l->array[x].peso=g.peso;
    l->array[x].index=g.index;
    while((l->array[Left(x)].peso>l->array[x].peso ||l->array[Right(x)].peso>l->array[x].peso )){
        if(l->array[Left(x)].peso>l->array[Right(x)].peso){
            Swap(&(l->array[x]),&(l->array[Left(x)]));
            x=Left(x);
            if(Right(x)>l->nelem || Left(x)>=l->nelem)
                break;
        }
        else{
            Swap(&(l->array[x]),&(l->array[Right(x)]));
            x=Right(x);
            if(Right(x)>l->nelem ||Left(x)>=l->nelem)
                break;
        }
    }
}


//funzione che restituisce la posizione (indice) del nodo piu conveniente per la somma dei cammini
int NodoVicino(long long int d[],int MST[],int dim){
    long long int inf=LLONG_MAX;
    int nodovicino=-1;
    int j=0;
    
    while(j<dim){
        if(*(d+j)<=inf && *(MST+j)==0){
            inf = *(d+j);
            nodovicino = j;
        }
    j++;
    }

    return nodovicino;
}

//funzione che calcola la somma dei pesi basata sul noto algoritmo di Dijkstra
int SommaPesi(int **link,int dim){
    long long int d[dim];
    int MST[dim];
    int h=0;
    while(h<dim){
        d[h]=LLONG_MAX;
        MST[h]=0;
        h++;
    }
    d[0]=0;
    for(int j=0;j<dim-1;j++){
        int next=NodoVicino(d,MST,dim);
        printf("Nodo minore %d  ",next);
        MST[next]=1;

        for(int n=0;n<dim;n++){
            if(d[next]+link[next][n]<d[n] && d[next]!=LLONG_MAX && MST[n]==0 && link[next][n])
                d[n]=d[next]+link[next][n];
        }   
    }
    long long int sum=0;
    for(int i=0;i<dim;i++){
        if(d[i]==LLONG_MAX){
            d[i]=0;
        }
        sum=sum+d[i];      
    }
    return sum;
}