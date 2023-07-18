#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

struct Auto{
    int autonomia;
    struct Auto *next;
};
struct Stazione{
    int distanza;
    int n_auto;
    struct Auto *maggiore;
    struct Stazione *next;
    struct Stazione *prev;
};
struct Stazione *inizio=NULL;
struct Stazione *minima;
struct Stazione *massima;
bool inversione;
struct Auto *soluzione=NULL;
void risposta(int x){
    struct Auto *temp= malloc(sizeof (struct Auto));
    temp->autonomia=x;
    temp->next=soluzione;
    soluzione=temp;
}
//controllo già preventivamente se inizio e fine esistono(quindi ipotizzo esistano di sicuro)
bool pianifica_percorso(struct Stazione *curr, struct Stazione *finish){
    if(curr->maggiore==NULL)
        curr->maggiore->autonomia=0;
    int max_reach=(curr->distanza+curr->maggiore->autonomia);
    if(max_reach>=finish->distanza){
        risposta(finish->distanza);
        risposta(curr->distanza);
        return true;
    }
    if(max_reach>minima->distanza){
        struct Stazione *temp=curr;
        struct Stazione *old_minima=minima->next;
        while(temp->next->distanza<=max_reach)
            temp=temp->next;
        minima=temp;
        while(old_minima->distanza<=minima->distanza){
            if(pianifica_percorso(old_minima, finish)){
                risposta(curr->distanza);
                return true;
            }
            old_minima=old_minima->next;
        }
    }
    return false;
}
bool pianifica_percorso_inverso(struct Stazione *curr, struct Stazione *finish) {
    if (finish->maggiore == NULL)
        finish->maggiore->autonomia = 0;

    int max_reach = finish->distanza - finish->maggiore->autonomia;

    if (max_reach <= curr->distanza) {
        risposta(curr->distanza);
        risposta(finish->distanza);
        return true;
    }

    if (max_reach < massima->distanza) {
        struct Stazione *temp = finish;
        struct Stazione *old_massima = massima->next;

        while (temp->prev->distanza >= max_reach)
            temp = temp->prev;

        massima = temp;

        while (old_massima->distanza >= massima->distanza) {
            if (pianifica_percorso(curr, old_massima)) {
                risposta(finish->distanza);
                return true;
            }
            old_massima = old_massima->prev;
        }
    }
    return false;
}


void aggiungi_stazione(){
    int x;
    int dist;
    int autonomia;
    //todo rimuovere printf
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
        temp->prev=NULL;
    }
    else{
        //se va per primo devo cambiare anche inizio
        if(dist<inizio->distanza){
            inizio->prev=temp;
            temp->next=inizio;
            temp->prev=NULL;
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
            temp->prev=prev;
            temp->next = succ;
            succ->prev=temp;
            temp->distanza = dist;
        }
    }

    //riceve come input le autonomie
    //todo rimuovere printf
    printf("numero di auto: ");
    scanf("%d",&x);
    temp->n_auto=x;
    for (int i = 0; i < x; ++i) {
        //todo rimuovere printf
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
    //todo rimuovere printf
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
            inizio->prev=NULL;
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
            temp->next->prev=prec;
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
    //todo rimuovere printf
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
    //todo rimuovere printf
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
        struct Auto *temp=automobile;
        automobile=automobile->next;
        free(temp);
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
    //TODO implementare funzioni per la lettura di input
    char c=getchar();
    char comando[18];
    int i=0;
    while(c!=EOF){
        if(!isspace(c)){
            comando[i]=c;
            i++;
        }
        else{
            //comando di demolisci stazione
            if(comando[0]=='d')
                demolisci_stazione();


            //comando di rottama auto
            else if(comando[0]=='r')
                rottama_auto();
            //comando di crea percorso
            else if(comando[0]=='p'){
                int stazioneInizio,stazioneFine;
                scanf("%d%d", &stazioneInizio, &stazioneFine);
                if(stazioneInizio==stazioneFine){
                    printf("%d",stazioneInizio);
                    goto exit;
                }

                struct Stazione *temp=inizio;
                struct Stazione *start=NULL,*finish=NULL;
                while(temp!=NULL){
                    if(temp->distanza == stazioneInizio)
                        start=temp;
                    if(temp->distanza==stazioneFine)
                        finish=temp;
                    temp=temp->next;
                }

                if(!inversione){
                    minima=start;
                    pianifica_percorso(start, finish);
                }

                else{
                    massima=finish;
                    pianifica_percorso_inverso(start, finish);
                }
                if(soluzione==NULL)
                {
                    printf("%s","nessun percorso");
                    goto exit;
                }
                stampa_auto(soluzione);
            }
            else if(comando[9]=='a')
                aggiungi_auto();
            else if(comando[9]=='s')
                aggiungi_stazione();
            else
                stampa_stazioni(inizio);
            exit:
            c=getchar();
            i=0;
        }


        c=getchar();

    }





    return 0;
}


