#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
        printf("%s","non demolita");
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
                printf("%s","non demolita");
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
        printf("%s","demolita");
    }

}

void aggiungi_auto(){
    int dist,autonomia;
    struct Stazione *temp=inizio;
    printf("distanza stazione e autonomia auto");
    scanf("%d %d",&dist,&autonomia);
    while(temp!=NULL && temp->distanza<dist)
        temp=temp->next;

    if(temp==NULL || temp->distanza!=dist){
        printf("%s","non aggiunta");
        return;
    }
    struct Auto *tempAuto= malloc(sizeof(struct Auto));
    tempAuto->autonomia=autonomia;
    if(temp->maggiore==NULL || temp->maggiore->autonomia<autonomia){
        tempAuto->next=temp->maggiore;
        temp->maggiore=tempAuto;
    }
    else{
        tempAuto->next=temp->maggiore->next;
        temp->maggiore->next=tempAuto;
    }
    printf("%s","aggiunta");
}
// tempo di completamento è t(n +m)
void rottama_auto() {
    //solita roba per trovare la stazione (se esiste)
    int dist, autonomia;
    struct Stazione *temp = inizio;
    printf("distanza stazione e autonomia auto");
    scanf("%d %d", &dist, &autonomia);
    while (temp != NULL && temp->distanza < dist)
        temp = temp->next;

    if (temp == NULL || temp->distanza != dist) {
        printf("%s", "non rottamata");
        return;
    }
    //ora guardo se le/la auto esiste/esistono
    int contoDistrutte=0;
    int nuovoMax=-1;
    bool daAggiustare=false;
    struct Auto *succ = temp->maggiore;
    struct Auto *prec= temp->maggiore;
    struct Auto *newMaggiore=temp->maggiore->next;
    struct Auto *precnewMaggiore=NULL;

    while (succ != NULL){
        if(succ->autonomia==autonomia){
            contoDistrutte++;
            if(succ==temp->maggiore){
                prec=succ->next;
                temp->maggiore=prec;
                free(succ);
                succ=prec;
                daAggiustare = true;
                continue;
            }
            prec->next=succ->next;
            free(succ);
            succ=prec->next;
            continue;

        }
        else if (daAggiustare && nuovoMax<succ->autonomia ){
            precnewMaggiore=prec;
            newMaggiore=succ;
            nuovoMax=succ->autonomia;
        }
        prec=succ;
        succ = succ->next;

    }

    if(contoDistrutte==0)
        printf("%s","non rottamata");
    else {
        if(daAggiustare && newMaggiore!=NULL){
            precnewMaggiore->next=newMaggiore->next;
            newMaggiore->next=temp->maggiore;
            temp->maggiore=newMaggiore;
        }
        printf("%s","rottamata");

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
    rottama_auto();
    stampa_stazioni(inizio);




    return 0;
}


