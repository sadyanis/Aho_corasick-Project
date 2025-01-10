#ifndef STRUCTURES_H
#define STRUCTURES_H
//struct de table de table de transition
struct _trie {
    int maxNode; //nbt max de noeud
    int nextNode; // Prochain noeud disponible
    int **transition; // Table de transition entre les noeuds
    char *finite; // Indique si le noeud est final
    int *suffixLink; // Lien de suppléance
    int *outputLink; // Tableau de sortie
};

typedef struct Node{
    int etat;
    struct Node *suivant;
}Node;

 struct queue{
     Node *head;
     Node *tail;
};
typedef struct queue *Queue;
typedef struct _trie *Trie;

//fonction pour creer un trie

Trie createTrie(int maxNode);

// crrer une file
Queue createQueue();
//fonction pour enfiler un element
void enqueue(Queue q, int etat);
//fonction pour defiler un element
int dequeue(Queue q);


void insertInTrie(Trie trie, unsigned char *w);

int isInTrie(Trie trie, unsigned char *w);

void initialiser(int ** matrice , int max_node);

Trie trie_pref(unsigned char *w);

int isInTrie(Trie trie, unsigned char *w);

Trie trie_suff(unsigned char *w);

Trie trie_facteur(unsigned char *w);

// fonction pour construire les liens de suffixe
void buildSuffixLink(Trie trie);
int search(Trie trie, unsigned char *text);

#endif
//memset