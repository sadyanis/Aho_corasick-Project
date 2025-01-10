#ifndef STRUCTUREHASH_H
#define STRUCTUREHASH_H
struct _list{
    int startNode,targetNode;
    unsigned char letter;
    struct _list *next;

} ;
typedef struct _list *List;

struct _trie {
    int maxNode; //nbt max de noeud
    int nextNode;
    List *transition; // table hachage
    char *finite;
    int *suffixLink; // Tableau de liens de suffixe(suppleance)
    int *outputLink; // Tableau de liens de sortie
};

typedef struct _trie *Trie;

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

Trie createTrie(int maxNode);

unsigned int hash_function(char c, int num);

void insertInTrie(Trie trie, unsigned char *w);

int isInTrie(Trie trie, unsigned char *w);

List createNewTransition(int startNode, unsigned char letter, int targetNode);

List findExistingTransition(Trie trie,int hachCode, int currentNode,char letter);

void buildSuffixLink(Trie trie);

int search(Trie trie, unsigned char *text);

// crrer une file
Queue createQueue();
//fonction pour enfiler un element
void enqueue(Queue q, int etat);
//fonction pour defiler un element
int dequeue(Queue q);


#endif