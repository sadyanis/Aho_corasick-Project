#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<limits.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "structureHash.h"


#define TRUE 0
#define FALSE 1
//#define TABLE_SIZE UCHAR_MAX  // Un nombre premier proche du nombre d'éléments prévus a definir plutard
struct stat st;

int TABLE_SIZE; // variable global representant la taille de la table de hachage

int main(int argc , char** argv) {
    struct stat st;
    char buffer[256];
    int maxNode = 1;

    if(argc != 3){
        printf("Usage: %s le nombre d'arguments invalide\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    char * textFile = argv[1];
    char * wordFile = argv[2];
    // récuperer les descriteurs de fichier 
    int fdTexte = open(textFile, O_RDONLY);
    if (fdTexte == -1) {
        perror("Erreur lors de l'ouverture du fichier texte");
        return EXIT_FAILURE;
    }
    int fdWord = open(wordFile, O_RDONLY);
    if (fdWord == -1) {
        perror("Erreur lors de l'ouverture du fichier texte");
        return EXIT_FAILURE;
    }
    fstat(fdTexte, &st);
    int textSize = st.st_size;
    

    unsigned char *texte = malloc(textSize + 1);
    if (texte == NULL) {
        perror("Erreur d'allocation pour le texte");
        close(fdTexte);
        return EXIT_FAILURE;
    }
     ssize_t n = read(fdTexte, texte, textSize);
    if (n == -1) {
        perror("Erreur lors de la lecture du fichier texte");
        free(texte);
        close(fdTexte);
        return EXIT_FAILURE;
    }
    texte[textSize] = '\0'; 
    close(fdTexte);
    int index = 0;
    
unsigned char **words = malloc(100 * sizeof(char*)); // tableau pour stocker les mots 100 mots max
    if (words == NULL) {
        perror("Erreur d'allocation pour le tableau de mots");
        free(texte);
        close(fdWord);
        return EXIT_FAILURE;
    }

    int wordCount = 0; // Compteur de mots
      // Somme des tailles des mots

    char bufferWord[256]; // Buffer temporaire pour un mot
    int bytesRead;
    while ((bytesRead = read(fdWord, &bufferWord[index], 1)) > 0) {
        if (bufferWord[index] == '\n' || index == sizeof(bufferWord) - 1) {
             bufferWord[index] = '\0';
            
            // Allouer de la mémoire pour le mot et le copier dans le tableau
            words[wordCount] = malloc((index + 1) * sizeof(char));
            if (words[wordCount] == NULL) {
                perror("Erreur d'allocation pour un mot");
                free(texte);
                for (int i = 0; i < wordCount; i++) {
                    free(words[i]);
                }
                free(words);
                close(fdWord);
                return EXIT_FAILURE;
            }
            strcpy(words[wordCount], (unsigned char *) bufferWord);

            maxNode += index-1; // Ajouter la taille du mot à la somme
            wordCount++;

            index = 0; // Réinitialiser l'indice pour lire le mot suivant
        } else {
            index++;
        }

        // Vérifier si on dépasse la capacité du tableau
        if (wordCount >= 1000) {
            fprintf(stderr, "Erreur : Trop de mots dans le fichier\n");
            break;
        }
    }

    if (bytesRead == -1) {
        perror("Erreur lors de la lecture du fichier de mots");
    }

    close(fdWord);

    // Afficher le nombre total de mots et la somme des tailles
    printf("Nombre total de mots : %d\n", wordCount);
    printf("le premier mot :%s",words[0]);
    TABLE_SIZE = maxNode*2; // definire la taille de la table de hachage
    // creation du trie 
    Trie trie = createTrie(maxNode);

    for (int i = 0; i < wordCount; i++)
    {
        printf("insertion du mot :%ld \n",strlen(words[i]));
       
        insertInTrie(trie,words[i]);
        /* code */
    }
    
    
    buildSuffixLink(trie);

   int result =  search(trie,texte);
    printf("\n le nombre mot trouvé est %d",result);
    
    


     
    return 0;
}
// fonction pour creer un trie
Trie createTrie(int maxNode){
    Trie tr = malloc(sizeof(struct _trie));
   if (tr == NULL)
   {
    perror("Error allocation");
    exit(EXIT_FAILURE);
   }
   //initialisation du trie
   tr->maxNode = maxNode; 
   tr->nextNode = 1;
   tr->finite =(char*) malloc(maxNode * sizeof(char));
   //initialisation du tableau finite par des 0
    memset(tr->finite,0,maxNode * sizeof(char));
   // initialisation de la table de hachage
   tr->transition = malloc(TABLE_SIZE * sizeof(List));
    if (tr->transition == NULL) {
        perror("Error allocation");
        exit(EXIT_FAILURE);
    }

    // initialiser chaque élément de `transition` à NULL
    for (int i = 0; i < TABLE_SIZE; i++) {
        tr->transition[i] = NULL;
    }
    //  initialiser les liens de suppleance
    tr->suffixLink = malloc(maxNode * sizeof(int));
    if(tr->suffixLink == NULL){
        perror("Erreur d'allocation");
        exit(EXIT_FAILURE);
    }

    // initialiser
    for (int i = 0; i < maxNode; i++)
    {
        tr->suffixLink[i] = 0; // tous les liens de suppleance pointe vers l'etat 0

    }
    // 
    tr->outputLink = malloc(maxNode * sizeof(int));
    if(tr->suffixLink == NULL){
        perror("erreur d'initialisation");
        exit(EXIT_FAILURE);
    }   
    for (int i = 0; i < maxNode; i++){
        tr->outputLink[i] = 0;
    } 
    
    return tr;
    
}
// fonction permet de d'inserer dans un trie 
void insertInTrie(Trie trie, unsigned char *w){
    int wordLength = strlen((char *)w);
    int currentNode = 0;
    int hashcode;
    for (int i = 0; i < wordLength; i++)
    {
        hashcode = (hash_function(w[i],currentNode));
        // on verifie si il existe deja une transition
        List point = findExistingTransition(trie,hashcode,currentNode,w[i]);
        // cas ou il n'existe pas de transition
        if (point==NULL)
        {
            List newTrans = createNewTransition(currentNode,w[i],trie->nextNode);
            newTrans->next = trie->transition[hashcode]; // inserer en tete
             trie->transition[hashcode] = newTrans;
            currentNode = trie->nextNode;
            trie->nextNode += 1;
           
        }else{ // si il existe une transition
            currentNode = point->targetNode;
        }
        
    }
    // on marque le dernire etat commme final
    trie->finite[currentNode] = 1;    
}
// fonction pour verifier si un mot appartien a un trie  
int isInTrie(Trie trie, unsigned char *w) {
    int wordLength = strlen((char *)w);
    int currentNode = 0;
    int hashcode;

    for (int i = 0; i < wordLength; i++) {
        hashcode = hash_function(w[i], currentNode);
        
        List point = findExistingTransition(trie, hashcode, currentNode, w[i]);
        
        if (point == NULL) {
            return FALSE; // Mot non trouvé
        }
        
        currentNode = point->targetNode;
    }

    return (trie->finite[currentNode] == 1) ? TRUE : FALSE;
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




unsigned int hash_function(char c, int num) {
    unsigned int hash = 0;
    
    // Utilisation du caractère ASCII
    hash = (unsigned int)c;
    
    // Incorporation de l'entier
    hash = hash * 31 + num;
    
    // Réduction de la valeur de hachage avec le modulo
    hash = hash % TABLE_SIZE;
    
    return hash;
 }




// fonction pour creer une nouvelle transition

List createNewTransition(int startNode, unsigned char letter, int targetNode) {
    List newTrans = malloc(sizeof(struct _list));
    newTrans->startNode = startNode;
    newTrans->letter = letter;
    newTrans->targetNode = targetNode;
    newTrans->next = NULL;
    return newTrans;
}
// fonction qui verifie  si il existe une transition
List findExistingTransition(Trie trie,int hachCode, int currentNode,char letter){
    List pointeur = trie->transition[hachCode];
    while (pointeur != NULL)
    {
        if (pointeur->startNode == currentNode && pointeur->letter == letter)
        {
            return pointeur;
        }
        pointeur =  pointeur->next;
        
    }
    return NULL;
}

// fonction pour construire les liens de suffixe
void buildSuffixLink(Trie trie) {
    Queue q = createQueue();

    // Initialiser les liens de suffixe pour les états initiaux
    for (int i = 0; i < TABLE_SIZE; i++) {
        List current = trie->transition[i];
        while (current != NULL) {
            if (current->startNode == 0) { // Si l'état initial est 0
                int child = current->targetNode;
                trie->suffixLink[child] = 0; // Lien de suffixe de la racine
                enqueue(q, child);
            }
            current = current->next;
        }
    }

    // Construire les liens de suffixe pour tous les autres états
    while (q->head != NULL) {
        int current = dequeue(q);

        // Explorer les transitions de l'état actuel
        for (int i = 0; i < TABLE_SIZE; i++) {
            List transition = trie->transition[i];
            while (transition != NULL) {
                if (transition->startNode == current) {
                    int child = transition->targetNode;
                    unsigned char letter = transition->letter;

                    // Trouver le lien de suffixe de l'état courant
                    int failState = trie->suffixLink[current];
                    while (findExistingTransition(trie, hash_function(letter, failState), failState, letter) == NULL && failState != 0) {
                        failState = trie->suffixLink[failState];
                    }
                    List failTransition = findExistingTransition(trie, hash_function(letter, failState), failState, letter);
                    if (failTransition != NULL) {
                        failState = failTransition->targetNode;
                    }

                    // Définir le lien de suffixe et le lien d'output
                    trie->suffixLink[child] = failState;
                    if (trie->finite[trie->suffixLink[child]]) {
                        trie->outputLink[child] = trie->suffixLink[child];
                    } else {
                        trie->outputLink[child] = trie->outputLink[trie->suffixLink[child]];
                    }

                    // Ajouter l'état enfant à la file
                    enqueue(q, child);
                }
                transition = transition->next;
            }
        }
    }

    // Libérer la file
    free(q);
}


int search(Trie trie, unsigned char *text) {
    int n = strlen((char *)text);
    int cmpt = 0;
    int current_state = 0; // partir de l'état initial
    for (int i = 0; i < n; i++) {
        unsigned char letter = text[i];
        while (findExistingTransition(trie,hash_function(letter,current_state),current_state,letter) == NULL && current_state != 0) {
            current_state = trie->suffixLink[current_state];
        }
        List transition = findExistingTransition(trie,hash_function(letter,current_state),current_state,letter);
        if (transition != NULL) {
            current_state = transition->targetNode;
        }
        // verifier les motifs terminaux
        int temp = current_state;
        while (temp != 0) {
            if (trie->finite[temp]) {
                cmpt++;
                printf("Mot trouvé à la position %d\n", i);
            }
            temp = trie->outputLink[temp];
        }
        
    }
    return cmpt;
}



