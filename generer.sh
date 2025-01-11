#!/bin/bash

# Vérifier si les exécutables existent

# Dossiers pour stocker les fichiers générés
mkdir -p textes mots resultats

# Paramètres
TEXT_LENGTH=5000000
ALPHABETS=(2 4 20 70)
WORD_SETS=(
    "100 5 15"
    "100 15 30"
    "100 30 60"
)

# Génération des textes
echo "Génération des textes..."
for ALPHABET in "${ALPHABETS[@]}"; do
    TEXT_FILE="textes/texte_${ALPHABET}.txt"
    ./genere-texte $TEXT_LENGTH $ALPHABET > $TEXT_FILE
    echo "Texte généré : $TEXT_FILE"
done

# Génération des ensembles de mots
echo "Génération des ensembles de mots..."
for ALPHABET in "${ALPHABETS[@]}"; do
    WORD_FILE="mots/mots_${ALPHABET}.txt"
    for WORD_PARAMS in "${WORD_SETS[@]}"; do
        WORD_COUNT=$(echo $WORD_PARAMS | cut -d' ' -f1)
        MIN_LENGTH=$(echo $WORD_PARAMS | cut -d' ' -f2)
        MAX_LENGTH=$(echo $WORD_PARAMS | cut -d' ' -f3)
        
        ./genere-mots $WORD_COUNT $MIN_LENGTH $MAX_LENGTH $ALPHABET >> $WORD_FILE
        
    done
    echo "Mots générés : $WORD_FILE"
done