#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#define getchar_unlocked() getchar()

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
    struct Stazione *left;
    struct Stazione *right;
    struct Stazione *p;
    struct Stazione *puntata;
};


struct Stazione *inizio=NULL;
struct Stazione *inizio_albero=NULL;
struct Stazione *minima;
struct Stazione *massima;
struct Auto *soluzione=NULL;

void risposta(int x){
    struct Auto *temp= malloc(sizeof (struct Auto));
    temp->autonomia=x;
    temp->next=soluzione;
    soluzione=temp;
}
struct Stazione* dopo(struct Stazione* nodo, int valore){
    if (nodo==NULL) return NULL;
    else if(nodo->distanza>valore) return (nodo->left!=NULL&&nodo->left->distanza>valore)?dopo(nodo->left,valore):nodo;
    return (nodo->right!=NULL&&nodo->right->distanza>valore)? dopo(nodo->right,valore): dopo(nodo->p,valore);
}
struct Stazione* prima(struct Stazione* nodo, int valore){
    if (nodo==NULL) return NULL;
    else if(nodo->distanza<valore) return (nodo->right!=NULL&&nodo->right->distanza<valore)?prima(nodo->right,valore):nodo;
    return (nodo->left!=NULL&&nodo->left->distanza<valore)? prima(nodo->left,valore): prima(nodo->p,valore);
}
int leggi_numero(){
    char c;
    int x=0;
    c=getchar_unlocked();
    while(isdigit(c)){
        x=x*10;
        x+=c-'0';
        c=getchar_unlocked();
    }
    return x;
}
//controllo già preventivamente se inizio e fine esistono(quindi ipotizzo esistano di sicuro)
struct Stazione* pianifica_percorso(struct Stazione *curr, int finish){
    while (curr!=NULL&&curr->distanza<=minima->distanza) {
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
    struct Stazione *temp=NULL;
    struct Stazione *superiore=curr;
    struct Stazione *inferiore=curr;
    while(curr->distanza>finish){
        while(curr!=NULL&&superiore!=NULL&&curr->distanza<=superiore->distanza){
            int autonomia=(curr->maggiore==NULL)? 0: curr->maggiore->autonomia;
            int max_reach=curr->distanza-autonomia;
            if(max_reach<=massima->distanza) {
                temp = curr->prev;
                if (max_reach <= finish) {
                    while (temp->distanza != finish)
                        temp = temp->prev;

                    temp->puntata=curr;
                    return temp;
                }
                while (temp!=NULL&&temp->distanza>=max_reach){
                    if(temp->puntata==NULL)
                        temp->puntata=curr;
                    massima=temp;
                    temp=temp->prev;
                }
            }
            curr=curr->next;
        }
        if(inferiore!=NULL)
            superiore=inferiore->prev;
        inferiore=massima;
        curr=inferiore;
        if(superiore!=NULL&&inferiore!=NULL&&superiore->distanza<inferiore->distanza)
            return NULL;


    }
    return NULL;
}


void aggiungi_stazione(){

    int dist=leggi_numero();
    //crea una stazione nel punto corretto della lista
    struct Stazione *temp= malloc(sizeof(struct Stazione));
    temp->maggiore=NULL;
    temp->distanza=dist;
    temp->n_auto=0;
    temp->puntata=NULL;
    temp->prev=NULL;
    temp->next=NULL;
    temp->left=NULL;
    temp->right=NULL;
    temp->p=NULL;
    //se e la prima stazione da inserire
    if(inizio==NULL){
        inizio=temp;
        inizio_albero=temp;
        printf("%s","aggiunta\n");
    }
    else{
        //se va per primo devo cambiare anche inizio
        if(dist<inizio->distanza){
            inizio->prev=temp;
            inizio->left=temp;
            temp->p=inizio;
            temp->next=inizio;
            inizio=temp;
        }
        else {
            struct Stazione *prev=NULL;
            struct Stazione *succ = inizio_albero;
            while (succ != NULL && succ->distanza!=dist) {
                prev=succ;
                succ=(dist<succ->distanza)?succ->left:succ->right;
            }
            if (succ != NULL){
                printf("%s","non aggiunta\n");
                free(temp);
                char c=getchar_unlocked();
                while (c!='\n')
                    c=getchar_unlocked();
                return;
            }
            temp->p=prev;
            prev->distanza < dist ? (prev->right = temp) : (prev->left = temp);
            temp->next=dopo(temp,dist);
            if(temp->next!=NULL) temp->next->prev = temp;
            temp->prev=prima(temp,dist);
            temp->prev->next=temp;
        }
        printf("%s","aggiunta\n");
    }

    //riceve come input le autonomie
    int x=leggi_numero();
    temp->n_auto=x;

    for (int i = 0; i < x; ++i) {
        int autonomia=leggi_numero();
        //aggiunge le macchine alla stazione(in O(m) e non O(nm)), in testa sarà sempre la più grande
        struct Auto *creaAuto= malloc(sizeof(struct Auto));
        creaAuto->autonomia=autonomia;
        if(temp->maggiore==NULL){
            temp->maggiore=creaAuto;
            creaAuto->next=NULL;
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
        }
    }
}

void  demolisci_stazione() {
    struct Stazione *succ = inizio; //quella che poi andrò a distruggere
    struct Stazione *temp = inizio_albero;
    int dist = leggi_numero();
    //controlla se ci sono stazioni
    if (inizio == NULL) {
        printf("%s", "non demolita\n");
        return;
    }
    //aggiusto doppia lista(se non esiste faccio return)
    if (inizio->distanza == dist) {
        inizio = inizio->next;
        if (inizio != NULL) inizio->prev = NULL;
    } else {
        succ = inizio_albero;
        while (succ && succ->distanza != dist)
            succ = (dist < succ->distanza) ? succ->left : succ->right;
        if (succ == NULL) {
            printf("%s", "non demolita\n");
            return;
        } else {
            succ->prev->next = succ->next;
            if (succ->next) succ->next->prev = succ->prev;
        }
    }
    //aggiusto l'albero
    if (dist == inizio_albero->distanza) {
        //trovo nuova radice albero se e radice
        struct Stazione *destra = succ->right;
        struct Stazione *sinistra = succ->left;
        if (sinistra == NULL)
            inizio_albero = destra;
        else if (destra == NULL)
            inizio_albero = sinistra;
        else {
            inizio_albero = destra;
            temp = destra;
            while (temp->left)
                temp = temp->left;
            temp->left = sinistra;
            sinistra->p = temp;
        }
    }
    else {
            if (succ->distanza < succ->p->distanza) {
                if (succ->left == NULL)
                    succ->p->left = succ->right;
                else if (succ->right == NULL)
                    succ->p->left = succ->left;
                else {
                    succ->p->left = succ->left;
                    temp = succ->left;
                    while (temp->right != NULL)
                        temp = temp->right;
                    temp->right = succ->right;
                    succ->right->p = temp;
                }
            } else {
                if (succ->left == NULL)
                    succ->p->right = succ->right;
                else if (succ->right == NULL)
                    succ->p->right = succ->left;
                else {
                    succ->p->right = succ->left;
                    temp = succ->left;
                    while (temp->right != NULL)
                        temp = temp->right;
                    temp->right = succ->right;
                    temp->right->p = temp;
                }

            }

    }
    //distruggo auto
    struct Auto *tempAuto;
    while (succ->maggiore != NULL) {
        tempAuto = succ->maggiore;
        succ->maggiore = tempAuto->next;
        free(tempAuto);
    }
    free(succ);
    printf("%s", "demolita\n");
}



void aggiungi_auto(){
    struct Stazione *temp=inizio_albero;
    int dist=leggi_numero();
    int autonomia=leggi_numero();
    while(temp!=NULL && temp->distanza!=dist)
        temp=(temp->distanza<dist)?temp->right:temp->left;
    if(temp==NULL || temp->n_auto>=512){
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
    struct Stazione *temp = inizio_albero;
    int dist=leggi_numero();
    int autonomia=leggi_numero();
    while (temp != NULL && temp->distanza!= dist)
        temp =(temp->distanza<dist)?temp->right:temp->left;

    if (temp == NULL) {
        printf("%s", "non rottamata\n");
        return;
    }
    //ora guardo se le/la auto esiste/esistono
    int contoDistrutte=0;
    int nuovoMax=-1;
    bool daAggiustare=false;
    struct Auto *succ = temp->maggiore;
    struct Auto *prec= NULL;
    struct Auto *newMaggiore=NULL;
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
            if(precnewMaggiore!=NULL)
                precnewMaggiore->next=newMaggiore->next;
            if(newMaggiore!=precnewMaggiore)
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
    char c=getchar_unlocked();
    bool invertita=true;

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
                int stazioneInizio=leggi_numero();
                int stazioneFine=leggi_numero();
                struct Stazione *temp=inizio_albero;
                struct Stazione *start=temp;

                if(stazioneInizio==stazioneFine){
                    printf("%d",stazioneInizio);
                    goto exit;
                }


                while(temp->distanza!=stazioneInizio){
                    temp=(temp->distanza<stazioneInizio)?temp->right:temp->left;
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
                    while (temp!=NULL&&temp->distanza<=stazioneFine){
                        temp->puntata=NULL;
                        temp=temp->next;
                    }
                }
                else{
                    temp=start->prev;
                    while (temp!=NULL&&temp->distanza>=stazioneFine){
                        temp->puntata=NULL;
                        temp=temp->prev;
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


        c=getchar_unlocked();

    }




    return 0;
}


