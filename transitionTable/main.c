#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<limits.h>
#include "structures.h"
// Algorithme Aho-Corasick
 int main(int argc, char  **argv){
    
    //@test
    if(argc != 3){
        printf("Usage: %s <text>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int result =0;
    int maxNode = 1;
      unsigned char *words[] = {
        (unsigned char *)"bonjour",
        (unsigned char *)"hello",
        (unsigned char *)"ell",
         (unsigned char *)"ll"
    };
    for (int i = 0; i < (int) sizeof(words) / (int) sizeof(words[0]); i++) {
        maxNode += strlen((char *)words[i]);
    }

    unsigned char *text = (unsigned char *)"bonjoursadsadhellosdadasd";
    Trie trie = createTrie(maxNode);
    for (int i = 0; i < (int) sizeof(words) / (int)sizeof(words[0]); i++){
        insertInTrie(trie,words[i]);
    }
    buildSuffixLink(trie);
    result = search(trie,text);
    printf("le nombre d'occurence trouvé est %d \n",result);
    
    

   

   
     

  
    return 0;
}

Trie createTrie(int maxNode){
    //initialisation
    Trie monTrie = malloc(sizeof(struct _trie));
    // initialisation du trie  
    monTrie->maxNode = maxNode;
    monTrie->nextNode = 1;
    monTrie->transition = malloc(maxNode * sizeof(int *));

    monTrie->finite = malloc(maxNode * sizeof(char));
    //initialisation du tableau par des 0
    memset(monTrie->finite,0,maxNode * sizeof(char));
    // initialisation de la matrice
     initialiser(monTrie->transition,maxNode);
    //initialisation du lien de suffixe
    monTrie->suffixLink = calloc(maxNode , sizeof(int));//calloc initialise le tableau par des 0
    //initialisation du lien de sortie
    monTrie->outputLink = calloc(maxNode , sizeof(int));//calloc initialise le tableau par des 0

    return monTrie;

}
// fonction pour inserer un mot dans un trie  

void insertInTrie(Trie trie, unsigned char *w){
    int n = strlen((char *)w);
    int current_node = 0;
    for (int i = 0; i < n; i++)
    {   
        // si il existe pas de transition
        if (trie->transition[current_node][w[i]] == -1)
        {
            trie->transition[current_node][w[i]] = trie->nextNode;
           
            current_node = trie->nextNode;
            trie->nextNode = trie->nextNode + 1;
            
        }else{
            current_node = trie->transition[current_node][w[i]];

        }
        
        
        
    }
    // declarer le derniere etat comme final
    trie->finite[current_node] = 1;
    

    
}
// fonction pour initialiser une matrice
void initialiser(int ** matrice , int max_node){
    for (int i = 0; i < max_node ; i++)
    {
        matrice[i] = malloc(UCHAR_MAX * sizeof( int));
        if (matrice[i]==NULL){
            perror("erreur d'allocation");
            exit(EXIT_FAILURE);
        }
        memset(matrice[i],-1,UCHAR_MAX * sizeof(int));
    }
    
}

// fonction pour savoir si un mot est dans un trie
int isInTrie(Trie trie, unsigned char *w){
    int N = strlen((char *)w);// la taille du mot
    int i = 0;
    int current_node = 0; // l'etat initial
    while (i < N)
    {
        if(trie->transition[current_node][w[i]] == -1){
            return 1; // il n'existe pas de transition
        }else // si il y'a une transition on poursuit le parcours
        current_node = trie->transition[current_node][w[i]];
        i++;
        
        

    }
    if(trie->finite[current_node]== 0){
    return 1;
    }
    return 0;
    
}
//fonction qui construit un trie pour les prefix d'un mot
Trie trie_pref(unsigned char *w){
    int maxNode  = strlen((char *)w)+1;
    Trie tr =  createTrie(maxNode);
    insertInTrie(tr,w);
    for (int i = 0; i < tr->nextNode; i++)
    {   
        // tous les noeud devienne finaux
        tr->finite[i] = 1;
        /* code */
    }
    return tr;
    
}
// fonction qui construit un trie pour les suffixe d'un mot
Trie trie_suff(unsigned char *w){
   
    int n = strlen((char *)w);
    int maxNode  = n*(n+1)/2;  // la somme des longeurs des mots
    printf("%d",n);
    Trie tr =  createTrie(maxNode+1);
     
    for (int i = 0; i < n; i++)
    {
        unsigned char* suffix = w + i;
        insertInTrie(tr,suffix);
        /* code */
    }
        
    return tr;
}
// fonction qui construit un trie pour les facteur d'un mot
Trie trie_facteur(unsigned char *w) {
    int n = strlen((char *)w);
    int maxNode  = (n * n); // Nombre maximum de nœuds
    Trie trie = createTrie(maxNode);

    // Parcourir tous les suffixes
    for (int i = 0; i < n; i++) {
        // Pour chaque suffixe, insérer les facteurs
        for (int j = i; j < n; j++) {
            // La longueur du facteur est (j - i + 1)
            char *factor = strndup((char *)(w + i), j - i + 1); // Créer un facteur
            insertInTrie(trie, (unsigned char *)factor); // Insérer dans le trie
            free(factor); // Libérer la mémoire allouée
        }
    }

    return trie;
}
// fonction pour creer une file
Queue createQueue(){
Queue q = (struct queue *)malloc(sizeof(struct queue));
if (q == NULL)
{
    perror("Error allocation");
    exit(EXIT_FAILURE);
}
q->head = NULL;
q->tail = NULL;
return q;
}
// fonction pour enfiler un element
void enqueue(Queue q, int etat){
    Node *n = (Node *)malloc(sizeof(Node));
     if (n == NULL)
    {
        perror("Error allocation");
        exit(EXIT_FAILURE);
    }
    n->etat = etat;
    n->suivant = NULL;
    if(q->tail == NULL){
        q->head = n;
        q->tail = n;
    }else{
        q->tail->suivant = n;
        q->tail = n;
    }
}
// fonction pour defiler un element
int dequeue(Queue q){
    if(q->head == NULL){
        printf("La file est vide\n");
        return -1;
    }
    Node *n = q->head;
    int etat = n->etat;
    q->head = q->head->suivant;
    if(q->head == NULL){
        q->tail = NULL;
    }
    free(n);
    return etat;
}

// fonction pour construire les liens de suffixe
void buildSuffixLink(Trie trie){
    Queue q = createQueue();
    // Initialiser les liens de suffixe pour les états initiaux
    for (int i = 0; i < UCHAR_MAX; i++) {
        if (trie->transition[0][i] != -1) {
            trie->suffixLink[trie->transition[0][i]] = 0;
            enqueue(q, trie->transition[0][i]);
        }
    }

    while (q->head != NULL) {
        int current_state = dequeue(q);
        // recherche des fils de l'état current_state
        for (int i = 0; i < UCHAR_MAX; i++) {
            if (trie->transition[current_state][i] != -1) {
                int child = trie->transition[current_state][i];
                int suffix_state = trie->suffixLink[current_state];
                while (trie->transition[suffix_state][i] == -1 && suffix_state != 0) {
                    suffix_state = trie->suffixLink[suffix_state];
                }
                if (trie->transition[suffix_state][i] != -1)
                {
                    suffix_state = trie->transition[suffix_state][i];
                }
                
                trie->suffixLink[child] = suffix_state;
                trie->outputLink[child] = trie->finite[trie->suffixLink[child]] ? trie->suffixLink[child] : trie->outputLink[trie->suffixLink[child]];
                enqueue(q, child);
            }
        }
    }
    free(q);
}

int search(Trie trie, unsigned char *text) {
    int n = strlen((char *)text);
    int cmpt = 0; //compteur d'occurences
    int current_state = 0; // partir de l'état initial
    for (int i = 0; i < n; i++) {
        while (trie->transition[current_state][text[i]] == -1 && current_state != 0) {
            current_state = trie->suffixLink[current_state];
        }
        if (trie->transition[current_state][text[i]] != -1) {
            current_state = trie->transition[current_state][text[i]];
        }
        if (trie->finite[current_state]) {
            printf("Mot trouvé à la position %d\n", i);
            cmpt++;
        }
        int output_state = trie->outputLink[current_state];
        while (output_state != 0) {
            printf("Mot trouvé à la position %d\n", i);
            cmpt++;
            output_state = trie->outputLink[output_state];
        }
    }
    return cmpt;
}

