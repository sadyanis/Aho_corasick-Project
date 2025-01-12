
CC = gcc
CFLAGS = -Wall -Wextra -O2


TRANSITION_DIR = ./transitionTable
HASH_DIR = ./hach


AC_MATRICE_SRC = $(TRANSITION_DIR)/main.c
AC_HACHAGE_SRC = $(HASH_DIR)/hash.c
GENERE_MOTS_SRC = genere_mot.c
GENERE_TEXTE_SRC = genere_texte.c
BENCHMARK_SRC = Benchmark.c

AC_MATRICE_OBJ = $(AC_MATRICE_SRC:.c=.o)
AC_HACHAGE_OBJ = $(AC_HACHAGE_SRC:.c=.o)
GENERE_MOTS_OBJ = $(GENERE_MOTS_SRC:.c=.o)
GENERE_TEXTE_OBJ = $(GENERE_TEXTE_SRC:.c=.o)
BENCHMARK_OBJ = $(BENCHMARK_SRC:.c=.o)

AC_MATRICE_EXEC = ac-matrice
AC_HACHAGE_EXEC = ac-hachage
GENERE_MOTS_EXEC = genere-mots
GENERE_TEXTE_EXEC = genere-texte
BENCHMARK_EXEC = benchmark


all: $(AC_MATRICE_EXEC) $(AC_HACHAGE_EXEC) $(GENERE_MOTS_EXEC) $(GENERE_TEXTE_EXEC) $(BENCHMARK_EXEC)


$(AC_MATRICE_EXEC): $(AC_MATRICE_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(AC_HACHAGE_EXEC): $(AC_HACHAGE_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(GENERE_MOTS_EXEC): $(GENERE_MOTS_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(GENERE_TEXTE_EXEC): $(GENERE_TEXTE_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(BENCHMARK_EXEC): $(BENCHMARK_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(AC_MATRICE_OBJ) $(AC_HACHAGE_OBJ) $(GENERE_MOTS_OBJ) $(GENERE_TEXTE_OBJ) $(BENCHMARK_OBJ)
	rm -f $(AC_MATRICE_EXEC) $(AC_HACHAGE_EXEC) $(GENERE_MOTS_EXEC) $(GENERE_TEXTE_EXEC) $(BENCHMARK_EXEC)

mrproper: clean
	rm -f $(AC_MATRICE_EXEC) $(AC_HACHAGE_EXEC) $(GENERE_MOTS_EXEC) $(GENERE_TEXTE_EXEC) $(BENCHMARK_EXEC)
