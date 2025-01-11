#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<limits.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "structures.h"
// Algorithme Aho-Corasick
 int main(int argc, char  **argv){
    
    //@test
    struct stat st;
    char buffer[256];
    int maxNode = 1;

    if(argc != 3){
        printf("Usage: %s le nombre d'arguments invalide\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    char * wordFile = argv[1];
    char * textFile = argv[2];
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
            strcpy((char *)words[wordCount], (char *) bufferWord);

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
   
    // definire la taille de la table de hachage
    // creation du trie 
    Trie trie = createTrie(maxNode);

    for (int i = 0; i < wordCount; i++)
    {
    
       
        insertInTrie(trie,words[i]);
        /* code */
    }
    
    
    buildSuffixLink(trie);

   int result =  search(trie,texte);
    printf("%d",result);
    
    


     
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
            cmpt++;
        }
        int output_state = trie->outputLink[current_state];
        while (output_state != 0) {
            cmpt++;
            output_state = trie->outputLink[output_state];
        }
    }
    return cmpt;
}

