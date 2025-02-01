# Manuel d'Utilisation :

## Description du Projet

L'objectif de ce projet est d'implémenter l'algorithme **Aho-corasick** en utilisant deux méthodes pour construire l'arbre des suppléances:

- **une matrice de transitions**
- **une table de hachage**

## Structure du projet:

### Répertoires et fichiers pricipaux

Les deux différentes implémentations de l'algorithme **Aho-corasick** ont été partagées en deux répertoires distincts:

1. **`hach/`** : Contient le code pour l'implémentation de l'algorithme utilisant une table de hachage.
2. **`transitionTable/`** : Contient le code pour l'implémentation de l'algorithme utilisant une matrice de transitions.
3. **Générateurs:**
   - genere-texte.c : Générateur de texte de 5M caractères sur un alphabet donné(2,4,20,70)
   - genere-mot.c : Générateur de mots de tailles variables(5-15,15-30,30-60) sur un alphabet donné.
4. **Scripts:**
   - **generer.sh**:script Bash pour générer automatiquement les jeux de données à l’aide des générateurs de texte et de mots, les mots et les textes générés seront stockés respectivement dans les dossiers **./mots** , **./textes**
5. **test**:
   - **benchmark.c**: Programme principal qui exécute les deux implémentation de l'algorithme de recherche sur les données générées, mesure leur performance, et enregistre les résultats dans un fichier CSV(./resultats/resultats.csv).

## Compilation des exécutables

- pour compiler et générer les exécutables(ac-matrice,ac-hachage,genere-mots,genere-texte,benchmark) de ce projet il faudra juste lancer le makefile depuis le répertoire racine du projet:

```bash
make all
```

**ou simplement via le script fourni**

```bash
./script.sh
```

- **Test des temps d'exécution des deux algorithmes**
- executez le script `generer.sh` pour générer le jeu de donnés :

```bash
./generer.sh
```

- puis Lancer le programme de test

```bash
./benchmark
```

Les résultats (temps d’exécution pour chaque combinaison texte/mot/algorithme) sont enregistrés dans un fichier CSV appelé results.csv.

## Pour generer les graphes :

1. Assurez-vous d'avoir Python 3 installé sur votre machine.

2. Installez pipenv avec pip pour pouvoir créer un environnement virtuel :

```bash
pip install pipenv
```

3. Activez l'environnement virtuel avec la commande suivante :

```bash
pipenv shell
```

4. Installez les dépendances nécessaires :

```bash
pipenv install
```

5. Lancez le script pour générer les graphes

```bash
 python3 graphe.py
```
