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
    int nextNode;// prochain noeud disponible
    List *transition; // table hachage
    char *finite;// tableau des états terminaux
    int *suffixLink; // tableau de liens de suffixe(suppleance)
    int *outputLink; // tableau permettant d'identifier tous les mots reconnus via liens de sortie
};

typedef struct _trie *Trie;
// structure d'une file
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


List createNewTransition(int startNode, unsigned char letter, int targetNode);

List findExistingTransition(Trie trie,int hachCode, int currentNode,char letter);

void buildSuffixLink(Trie trie);

int search(Trie trie, unsigned char *text);

// creer une file
Queue createQueue();
//fonction pour enfiler un élément
void enqueue(Queue q, int etat);
//fonction pour défiler un élément
int dequeue(Queue q);


#endif