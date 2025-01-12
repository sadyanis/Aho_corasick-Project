#!/usr/bin/env python
# coding: utf-8

# In[7]:


import matplotlib.pyplot as plt
import pandas as pd

# Charger les données depuis le fichier CSV
csv_file = "resultats/resultats.csv"
data = pd.read_csv(csv_file)

# Extraire les algorithmes uniques
algorithms = data['algorithme'].unique()

# Tracer un graphique pour chaque algorithme
for algorithm in algorithms:
    plt.figure(figsize=(10, 6))
    
    # Filtrer les données pour l'algorithme en cours
    algo_data = data[data['algorithme'] == algorithm]
    
    # Extraire les plages de tailles de mots uniques
    word_ranges = algo_data['taille_mots'].unique()

    # Tracer les courbes pour chaque plage de tailles de mots
    for word_range in word_ranges:
        subset = algo_data[algo_data['taille_mots'] == word_range]
        plt.plot(subset['taille_alphabet'], subset['temps_execution'], marker='o', label=f"Taille des mots: {word_range}")

    # Configurer le graphique
    plt.title(f"Temps d'exécution ({algorithm})")
    plt.xlabel("Taille de l'alphabet")
    plt.ylabel("Temps d'exécution (s)")
    plt.legend()
    plt.grid(True)
    plt.tight_layout()

    # Sauvegarder le graphique ou l'afficher
    plt.savefig(f"resultats/{algorithm}_execution.png")  # Sauvegarder dans un fichier
    # plt.show()  # Afficher le graphique (optionnel)

print("Les graphiques ont été générés et enregistrés dans le dossier 'resultats/'.")



# In[ ]:




