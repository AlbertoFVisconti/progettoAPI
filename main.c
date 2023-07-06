#include <stdio.h>
#include <stdlib.h>
struct Auto{
    int autonomia;
    struct Auto *next;
};
struct Stazione{
    int distanza;
    int n_auto;
    struct Auto *maggiore;
    struct Stazione *next;
};
struct Stazione *inizio;

void aggiungi_stazione(){
    int x;
    int dist;
    int autonomia;
    printf("distanza stazione: ");
    scanf("%d",&dist);
    //crea una stazione nel punto corretto della lista
    struct Stazione *temp= malloc(sizeof(struct Stazione));
    temp->maggiore=NULL;
    temp->distanza=dist;
    //se e la prima stazione da inserire
    if(inizio==NULL){
        inizio=temp;
        temp->next=NULL;
    }
    else{
        //se va per primo devo cambiare anche inizio
        if(dist<inizio->distanza){
            temp->next=inizio;
            inizio=temp;

        }
        else {
            struct Stazione *prev;
            struct Stazione *succ = inizio;
            while (succ != NULL && succ->distanza < dist) {
                prev = succ;
                succ = succ->next;
            }
            if (succ != NULL && succ->distanza == dist)
                return;


            prev->next = temp;
            temp->next = succ;
            temp->distanza = dist;
        }
    }

    //riceve come input le autonomie
    printf("numero di auto: ");
    scanf("%d",&x);
    temp->n_auto=x;
    for (int i = 0; i < x; ++i) {
        printf("valore delle auto \n");
        scanf("%d",&autonomia);
        //aggiunge le macchine alla stazione(in O(m) e non O(nm)), in testa sarà sempre la più grande
        struct Auto *creaAuto= malloc(sizeof(struct Auto));
        if(temp->maggiore==NULL){
            temp->maggiore=creaAuto;
            temp->n_auto=1;
            creaAuto->next=NULL;
            creaAuto->autonomia=autonomia;
        }
        else{

            //inserisci in testa se piu grande
            if(temp->maggiore->autonomia<autonomia){
                creaAuto->next=temp->maggiore;

                temp->maggiore=creaAuto;
            }
            else{
                creaAuto->next=temp->maggiore->next;
                temp->maggiore->next=creaAuto;
            }
            creaAuto->autonomia=autonomia;

        }
    }
}

void  demolisci_stazione(){
    int dist;
    struct Stazione *temp=inizio; //quella che poi andro a distruggere
    struct Stazione *prec;
    printf("inserire distanza della stazione da demolire\n");
    scanf("%d",&dist);
    //controlla se ci sono stazioni
    if(inizio==NULL) {
        printf("non demolita");
        return;
    }
    else{
        //se e la prima devo spostare anche inizio
        if(inizio->distanza==dist){
            temp=inizio;
            inizio=inizio->next;
        }
        //scorro array per trovarlo(se non lo trovo faccio return)
        else{

            while(temp!=NULL && temp->distanza<dist){
                prec=temp;
                temp=temp->next;
            }
            if(temp==NULL|| temp->distanza!=dist){
                printf("non demolita");
                return;
            }
            prec->next=temp->next;
        }


        //distruggo auto
        struct Auto *tempAuto;
        while(temp->maggiore!=NULL){
            tempAuto=temp->maggiore;
            temp->maggiore=tempAuto->next;
            free(tempAuto);
        }
        free(temp);
        printf("demolita");
    }

}

void stampa_auto(struct Auto *automobile){
    while(automobile!=NULL){
        printf("%d ",automobile->autonomia);
        automobile=automobile->next;
    }
    printf("\n");
}
void stampa_stazioni(struct Stazione *stazione){
    while(stazione!=NULL){
        printf("%d \n",stazione->distanza);
        stampa_auto(stazione->maggiore);
        stazione=stazione->next;
    }
}






int main() {
    aggiungi_stazione();
    stampa_stazioni(inizio);
    aggiungi_stazione();
    stampa_stazioni(inizio);
    demolisci_stazione();
    stampa_stazioni(inizio);




    return 0;
}


