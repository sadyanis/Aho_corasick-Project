#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include <unistd.h>
#include<time.h>
#include <sys/types.h>
#include <sys/stat.h>

int generate_random_text(long text_length, int alphabet_size) {
    if (alphabet_size < 1 || alphabet_size > 70) {
        fprintf(stderr, "La taille de l'alphabet doit être entre 1 et 70.\n");
        return -1;
    }

    srand(time(NULL));

    

    // Taille du buffer (peut être ajustée selon les performances)
    const int buffer_size = 4096;
    char buffer[buffer_size];
    int buffer_index = 0;

    for (long i = 0; i < text_length; i++) {
        buffer[buffer_index++] = 'a' + (rand() % alphabet_size);

        // Si le buffer est plein, on l'écrit dans le fichier
        if (buffer_index == buffer_size) {
            printf("%s",buffer);
            buffer_index = 0; // Réinitialiser l'index du buffer
        }
    }

    // Écrire le reste du buffer si nécessaire
    if (buffer_index > 0) {
       printf("%s",buffer);
    }

    
    
    return 0;
}
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s  <text_length> <alphabet_size>\n", argv[0]);
        return 1;
    }
    long text_length = atoi(argv[1]);
    int alphabet_size = atoi(argv[2]);

    // Appel de la fonction pour générer le texte
    return generate_random_text(text_length, alphabet_size);
}