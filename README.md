# Graph & TSP (Projet étudiant)

Projet d'algorithmique sur les graphes en **C11** et **OpenMP**, réalisé en 2e année à l'ESIEA (S4) par Thomas Couanon et Goulven Magliulo.

## Fonctionnalités

* **Plus courts chemins (`shortest_path.c`, `min_binary_heap.c`)** : algorithme de Dijkstra avec tas binaire min.
* **Voyageur de commerce (`tsp.c`)** : heuristique gloutonne et colonie de fourmis (ACO).
* **Traitement géographique & bonus (`bonus_interet.c`, `tourbar.c`, `geojson.c`)** :
  * **Option 1** : tournée des points d'intérêt (ex. bars) autour d'une ville et export en GeoJSON (`application/exit/output.geojson`).
  * **Option 2** : recherche de la ville ayant le plus de points d'intérêt dans une région, puis calcul de tournée.
  * **Option 3** : chargement et export complet d'un graphe routier.
  * **Option 4** : recherche des points d'intérêt les plus proches pour 3 critères donnés.

## Configuration & données

Les paramètres s'éditent dans [application/src/config.txt](application/src/config.txt) (`GRAPH_PATH`, `GPS_PATH`, `VILLE_NOM`, `CODE_POSTAL`, `CRITERE`, `RAYON`, `OPTIONS`, `REGION_NOM`).

Les 3 régions des exemples (`bretagne`, `ile-de-france`, `pays-de-la-loire`) sont incluses dans `data/tp_final/` et les communes dans `data/BonusData/communes-departement-region.csv`.

Le fichier `poi.csv` (> 100 Mo) est fourni compressé dans `data/BonusData/poi.csv.xz`. Avant le premier lancement, décompressez-le sur place :
* **Linux** : `unxz -k data/BonusData/poi.csv.xz`
* **Windows** : extraire `data/BonusData/poi.csv.xz` dans le même dossier avec 7-Zip ou WinRAR (pour obtenir `data/BonusData/poi.csv`).

---

## Compiler et lancer

### Windows (Visual Studio / MSVC)

1. Ouvrir le dossier `Graph-TSP-C` dans **Visual Studio** (détection automatique de `CMakeLists.txt`).
2. Sélectionner la cible `application` et compiler/lancer avec **F5** ou **Ctrl+F5**.

### Linux (GCC / CMake)

Installer `cmake` et `gcc` (avec OpenMP), puis compiler et exécuter depuis `_build_linux/` (pour que les chemins relatifs `../application/...` et `../data/...` soient valides) :

```bash
cmake -B _build_linux -DCMAKE_BUILD_TYPE=Release
cmake --build _build_linux -j4
cd _build_linux && ../_bin/Release/application
```

---

## Tests unitaires

Une suite de tests pour le tas binaire, Dijkstra et le TSP est disponible dans `tests/test_graph.c` :

```bash
gcc -std=c11 -Wall -fopenmp -Iapplication/src \
    tests/test_graph.c \
    application/src/graph/graph.c \
    application/src/graph/min_binary_heap.c \
    application/src/graph/shortest_path.c \
    application/src/graph/tsp.c \
    application/src/utils/list_int.c \
    -lm -o test_graph && ./test_graph
```

---

## Structure du projet

```text
Graph-TSP-C/
├── application/
│   ├── exit/        # Sorties GeoJSON générées
│   └── src/         # Graphes, Dijkstra, TSP, bonus, utilitaires et config.txt
├── data/            # Dossiers locaux pour tp_final/ et BonusData/ (ignorés par Git)
└── tests/           # Tests unitaires C
```

## Licence

Ce projet est distribué sous licence MIT. Voir le fichier [LICENSE.md](LICENSE.md).
