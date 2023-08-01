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
    struct Stazione *puntata;
};


struct Stazione *inizio=NULL;
struct Stazione *minima;
struct Stazione *massima;
struct Auto *soluzione=NULL;

void risposta(int x){
    struct Auto *temp= malloc(sizeof (struct Auto));
    temp->autonomia=x;
    temp->next=soluzione;
    soluzione=temp;
}
int leggi_numero(){
    char c;
    int x=0;
    c=getchar();
    while(isdigit(c)){
        x=x*10;
        x+=c-'0';
        c=getchar();
    }
    return x;
}
//controllo già preventivamente se inizio e fine esistono(quindi ipotizzo esistano di sicuro)
struct Stazione* pianifica_percorso(struct Stazione *curr, int finish){
    while (curr->distanza<=minima->distanza) {
        int autonomia = (curr->maggiore == NULL) ? 0 : curr->maggiore->autonomia;
        int max_reach = (curr->distanza + autonomia);
        if (max_reach > minima->distanza) {
            struct Stazione *temp = minima->next;
            if (max_reach >= finish) {
                while (temp->distanza!=finish)
                    temp=temp->next;
                temp->puntata=curr;
                return temp;
            }
            while (temp!=NULL&&temp->distanza <= max_reach){
                temp->puntata=curr;
                temp = temp->next;
            }
            minima=temp->prev;
        }
        curr=curr->next;
    }
    return NULL;
}
struct Stazione* pianifica_percorso_inverso( struct Stazione *curr,int finish) {
    struct Stazione *old_max=curr;
    while (curr->distanza<=old_max->distanza){
            int autonomia = (curr->maggiore == NULL) ? 0 : curr->maggiore->autonomia;
            int max_reach = (curr->distanza - autonomia);
            if (max_reach < massima->distanza) {
                old_max=massima;
                struct Stazione *temp = massima->prev;
                if (max_reach <= finish) {
                    while (temp->distanza!=finish)
                        temp=temp->prev;
                    temp->puntata=curr;
                    return temp;
                }
                while (temp->distanza >= max_reach){
                    temp->puntata=curr;
                    temp = temp->prev;
                }
                massima=temp->next;
            }
            if (old_max==massima&&curr==old_max)
                return NULL;
            else if (curr==old_max)
                curr=massima->prev;
            curr=curr->next;

        }
    return NULL;

}


void aggiungi_stazione(){
    int x;
    int dist;
    int autonomia;
    dist=leggi_numero();
    //crea una stazione nel punto corretto della lista
    struct Stazione *temp= malloc(sizeof(struct Stazione));
    temp->maggiore=NULL;
    temp->distanza=dist;
    temp->n_auto=0;
    temp->puntata=NULL;
    temp->prev=NULL;
    temp->next=NULL;
    //se e la prima stazione da inserire
    if(inizio==NULL){
        inizio=temp;
        temp->next=NULL;
        temp->prev=NULL;
        printf("%s","aggiunta\n");
    }
    else{
        //se va per primo devo cambiare anche inizio
        if(dist<inizio->distanza){
            inizio->prev=temp;
            temp->next=inizio;
            temp->prev=NULL;
            inizio=temp;
            printf("%s","aggiunta\n");

        }
        else {
            struct Stazione *prev=NULL;
            struct Stazione *succ = inizio;
            while (succ != NULL && succ->distanza < dist) {
                prev = succ;
                succ = succ->next;
            }
            if (succ != NULL && succ->distanza == dist){
                printf("%s","non aggiunta\n");
                free(temp);
                char c=getchar();
                while (c!='\n')
                    c=getchar();
                return;
            }
            else if(succ!=NULL){
                succ->prev=temp;
                prev->next = temp;
                temp->prev=prev;
                temp->next = succ;
            } else{
                prev->next=temp;
                temp->prev=prev;
            }
            printf("%s","aggiunta\n");
        }
    }

    //riceve come input le autonomie
    x=leggi_numero();
    for (int i = 0; i < x; ++i) {
        temp->n_auto++;
        autonomia=leggi_numero();
        //aggiunge le macchine alla stazione(in O(m) e non O(nm)), in testa sarà sempre la più grande
        struct Auto *creaAuto= malloc(sizeof(struct Auto));
        if(temp->maggiore==NULL){
            temp->maggiore=creaAuto;
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
    struct Stazione *prec=NULL;
    dist=leggi_numero();
    //controlla se ci sono stazioni
    if(inizio==NULL) {
        printf("%s","non demolita\n");
        return;
    }
    else{
        //se e la prima devo spostare anche inizio
        if(inizio->distanza==dist){
            temp=inizio;
            inizio=inizio->next;
            inizio->prev=NULL;
        }
        //scorro lista per trovarlo(se non lo trovo faccio return)
        else{
            while(temp!=NULL && temp->distanza<dist){
                prec=temp;
                temp=temp->next;
            }
            if(temp==NULL|| temp->distanza!=dist){
                printf("%s","non demolita\n");
                return;
            }
            else{
                prec->next=temp->next;
                temp->next->prev=prec;
            }

        }


        //distruggo auto
        struct Auto *tempAuto;
        while(temp->maggiore!=NULL){
            tempAuto=temp->maggiore;
            temp->maggiore=tempAuto->next;
            free(tempAuto);
        }
        free(temp);
        printf("%s","demolita\n");
    }

}

void aggiungi_auto(){
    int dist,autonomia;
    struct Stazione *temp=inizio;
    dist=leggi_numero();
    autonomia=leggi_numero();
    while(temp!=NULL && temp->distanza<dist)
        temp=temp->next;

    if(temp==NULL || temp->distanza!=dist){
        printf("%s","non aggiunta\n");
        return;
    }
    struct Auto *tempAuto= malloc(sizeof(struct Auto));
    tempAuto->autonomia=autonomia;
    temp->n_auto++;
    if(temp->maggiore==NULL || temp->maggiore->autonomia<autonomia){
        tempAuto->next=temp->maggiore;
        temp->maggiore=tempAuto;
    }
    else{
        tempAuto->next=temp->maggiore->next;
        temp->maggiore->next=tempAuto;
    }
    printf("%s","aggiunta\n");
}
// tempo di completamento è t(n +m)
void rottama_auto() {
    //solita roba per trovare la stazione (se esiste)
    int dist, autonomia;
    struct Stazione *temp = inizio;
    dist=leggi_numero();
    autonomia=leggi_numero();
    while (temp != NULL && temp->distanza < dist)
        temp = temp->next;

    if (temp == NULL || temp->distanza != dist) {
        printf("%s", "non rottamata\n");
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
        printf("%s","non rottamata\n");
    else {
        if(daAggiustare && newMaggiore!=NULL){
            precnewMaggiore->next=newMaggiore->next;
            newMaggiore->next=temp->maggiore;
            temp->maggiore=newMaggiore;
        }
        printf("%s","rottamata\n");
        temp->n_auto=temp->n_auto-contoDistrutte;

    }



}
void stampa_auto(struct Auto *automobile){
    while(automobile!=NULL&& automobile->next!=NULL){
        printf("%d ",automobile->autonomia);
        struct Auto *temp=automobile;
        automobile=automobile->next;
        free(temp);
    }
    if(automobile!=NULL) {
        printf("%d", automobile->autonomia);
    }
    printf("\n");
    soluzione=NULL;
}







int main() {
    freopen("output.txt", "w", stdout);
    char comando[18];
    int i=0;
    char c=getchar();
    bool invertita;
    while(c!=EOF){
        if(isalpha(c)||c=='-'){
            comando[i]=c;
            i++;
        }
        else if (isspace(c)){

            //comando di demolisci stazione
            if(comando[0]=='d')
                demolisci_stazione();


            //comando di rottama auto
            else if(comando[0]=='r')
                rottama_auto();
            //comando di crea percorso
            else if(comando[0]=='p'){
                int stazioneInizio,stazioneFine;
                stazioneInizio=leggi_numero();
                stazioneFine=leggi_numero();
                struct Stazione *temp=inizio;
                struct Stazione *start=temp;

                if(stazioneInizio==stazioneFine){
                    printf("%d",stazioneInizio);
                    goto exit;
                }


                while(temp->distanza<stazioneInizio){
                    temp=temp->next;
                }
                start=temp;

                if(start->distanza<stazioneFine) {
                    invertita=false;
                    minima = start;
                    temp=pianifica_percorso(start, stazioneFine);
                }
                else{
                    invertita=true;
                    massima=start;
                    temp=pianifica_percorso_inverso(start, stazioneFine);
                }
                if(temp==NULL)
                {
                    printf("%s","nessun percorso");
                    goto exit;
                }
                else {
                    while (temp != NULL) {
                        risposta(temp->distanza);
                        temp=temp->puntata;
                    }
                }
                //metti a posto i puntatori di puntata quando hai finito
                exit:
                if(!invertita){
                    temp=start->next;
                    while (temp!=NULL&&temp->puntata!=NULL){
                        temp->puntata=NULL;
                        temp=temp->next;
                    }
                }
                else{
                    temp=start->prev;
                    while (temp!=NULL&&temp->puntata!=NULL){
                        temp->puntata=NULL;
                        temp=temp->next;
                    }
                }

                stampa_auto(soluzione);
            }
            else if(comando[9]=='a'){
                aggiungi_auto();
            }
            else if(comando[9]=='s'){
                aggiungi_stazione();
            }
            i=0;


        }


        c=getchar();

    }




    return 0;
}


