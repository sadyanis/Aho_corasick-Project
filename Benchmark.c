#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void execute_and_measure(const char *algorithm, const char *text_file, const char *word_file, int alphabet_size, int word_length_range, int csv_fd) {
    char command[512];
    snprintf(command, sizeof(command), "./%s %s %s", algorithm, word_file, text_file);

    clock_t start = clock();
    int result = system(command);
    clock_t end = clock();

    if (result != 0) {
        dprintf(STDERR_FILENO, "Erreur lors de l'exécution de la commande : %s\n", command);
        return;
    }

    double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;
    dprintf(csv_fd, "%s,%d,%d,%.6f\n", algorithm, alphabet_size, word_length_range, elapsed_time);
}

int main() {
    const int ALPHABETS[] = {2, 4, 20, 70};
    const int LENGTH_RANGES[][2] = {{5, 15}, {15, 30}, {30, 60}};
    const char *TEXT_FILES[] = {
        "textes/texte_2.txt",
        "textes/texte_4.txt",
        "textes/texte_20.txt",
        "textes/texte_70.txt"
    };
    const char *ALGORITHMS[] = {"ac-matrice", "ac-hachage"};

    // Ouverture du fichier CSV avec POSIX
    int csv_fd = open("resultats/resultats.csv", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (csv_fd == -1) {
        perror("Erreur lors de la création du fichier CSV");
        return EXIT_FAILURE;
    }

    // Écrire l'en-tête dans le fichier CSV
    dprintf(csv_fd, "algorithme,taille_alphabet,taille_mots,temps_execution\n");

    // Exécuter les algorithmes pour chaque texte et ensemble de mots
    for (int i = 0; i < 4; i++) {// itèrer sur les fichiers texte
        for (int j = 0; j < 3; j++) {// itèrer sur les fichiers mots
            for (int k = 0; k < 2; k++) { // itèrer sur les algorithme=)
                char word_file[256];
                snprintf(word_file, sizeof(word_file), "mots/mots_%d_%d_%d.txt", ALPHABETS[i], LENGTH_RANGES[j][0], LENGTH_RANGES[j][1]);

                printf("Exécution de %s avec alphabet de taille %d et mots de longueur [%d, %d]...\n", 
                       ALGORITHMS[k], ALPHABETS[i], LENGTH_RANGES[j][0], LENGTH_RANGES[j][1]);

                execute_and_measure(ALGORITHMS[k], TEXT_FILES[i], word_file, ALPHABETS[i], LENGTH_RANGES[j][1], csv_fd);
            }
        }
    }

    close(csv_fd);
    printf("Les résultats ont été enregistrés dans resultats/resultats.csv\n");

    return EXIT_SUCCESS;
}
