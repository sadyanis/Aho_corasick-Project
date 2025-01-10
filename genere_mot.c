#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include <unistd.h>
#include<time.h>
#include <sys/types.h>
#include <sys/stat.h>
#define BUFFER_SIZE 4096







// Fonction pour générer un mot aléatoire d'une longueur spécifiée
void generate_random_word(char *buffer, int word_length, int alphabet_size) {
    for (int i = 0; i < word_length; i++) {
        buffer[i] = 'a' + (rand() % alphabet_size); // Génère un caractère aléatoire
    }
    buffer[word_length] = '\0'; // Terminaison du mot
}

// Fonction pour générer et écrire un nombre de mots aléatoires dans un fichier
int generate_random_words(int num_words, int Min_word_length,int Max_word_length , int alphabet_size) {
    // Vérification des paramètres
    if (alphabet_size < 1 || alphabet_size > 70) {
        fprintf(stderr, "La taille de l'alphabet doit être entre 1 et 70.\n");
        return -1;
    }

    srand(time(NULL)); // Initialisation du générateur de nombres aléatoires

    // Ouverture du fichier
    // int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    // if (fd == -1) {
    //     perror("Erreur lors de l'ouverture du fichier");
    //     return -1;
    // }

    char *word = malloc((Max_word_length + 1) * sizeof(char)); // Buffer pour stocker chaque mot
    if (!word) {
        perror("Erreur d'allocation mémoire");
        
        return -1;
    }

    // Génération et écriture des mots
    for (int i = 0; i < num_words; i++) {
        int word_length = (rand() % (Max_word_length - Min_word_length + 1)) + Min_word_length;
        generate_random_word(word, word_length, alphabet_size);
        printf("%s\n",word);
    }

    // Nettoyage
    free(word);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s  <num_words> <word_length> <alphabet_size>\n", argv[0]);
        return 1;
    }

    int num_words = atoi(argv[1]);
    int Max_word_length = atoi(argv[2]);
    int Min_word_length = atoi(argv[3]);
    int alphabet_size = atoi(argv[4]);

    return generate_random_words( num_words, Max_word_length,Min_word_length, alphabet_size);
}
