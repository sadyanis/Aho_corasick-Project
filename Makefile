# Variables de compilation
CC = gcc
CFLAGS = -Wall -Wextra -O2

# Répertoires
TRANSITION_DIR = ./transitionTable
HASH_DIR = ./hach

# Fichiers sources et objets
AC_MATRICE_SRC = $(TRANSITION_DIR)/main.c
AC_HACHAGE_SRC = $(HASH_DIR)/hash.c
GENERE_MOTS_SRC = genere_mot.c
GENERE_TEXTE_SRC = genere_texte.c

AC_MATRICE_OBJ = $(AC_MATRICE_SRC:.c=.o)
AC_HACHAGE_OBJ = $(AC_HACHAGE_SRC:.c=.o)
GENERE_MOTS_OBJ = $(GENERE_MOTS_SRC:.c=.o)
GENERE_TEXTE_OBJ = $(GENERE_TEXTE_SRC:.c=.o)

# Noms des exécutables
AC_MATRICE_EXEC = ac-matrice
AC_HACHAGE_EXEC = ac-hachage
GENERE_MOTS_EXEC = genere-mots
GENERE_TEXTE_EXEC = genere-texte

# Cibles principales
all: $(AC_MATRICE_EXEC) $(AC_HACHAGE_EXEC) $(GENERE_MOTS_EXEC) $(GENERE_TEXTE_EXEC)

# Compilation des exécutables
$(AC_MATRICE_EXEC): $(AC_MATRICE_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(AC_HACHAGE_EXEC): $(AC_HACHAGE_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(GENERE_MOTS_EXEC): $(GENERE_MOTS_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(GENERE_TEXTE_EXEC): $(GENERE_TEXTE_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Compilation des fichiers objets
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers générés
clean:
	rm -f $(AC_MATRICE_OBJ) $(AC_HACHAGE_OBJ) $(GENERE_MOTS_OBJ) $(GENERE_TEXTE_OBJ)

mrproper: clean
	rm -f $(AC_MATRICE_EXEC) $(AC_HACHAGE_EXEC) $(GENERE_MOTS_EXEC) $(GENERE_TEXTE_EXEC)
