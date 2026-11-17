
#pragma once
#include "settings.h"
#include "graph/graph.h"
#include "utils/tools.h"
#include "graph/bonus/tourbar.h"
#include <omp.h> // pour faire du multithreading

#define MAX_ROWS 2000000
#define MAX_COLS 40
#define MAX_ROW_CYTIES 39203
#define MAX_COL_CYTIES 5


///@brief permt de charger le csv des points d'interet dans un tableau 3D
///@return tableau 3D contenant les données du csv des points d'interet
char*** interet_tableau();


/// @brief permet de libérer la mémoire allouée pour le tableau 3D des points d'intérêt
/// @param matrice le tableau 3D à libérer 
void free_interet_tableau(char*** matrice);


///@brief permet de charger le csv des villes dans un tableau 3D
///@return tableau 3D contenant les données du csv des villes
char*** ville_tableau();


/// @brief permet de libérer la mémoire allouée pour le tableau 3D des villes
/// @param matrice le tableau 3D à libérer
void free_ville_tableau(char*** matrice);

/// @brief Permet de récupérer les coordonnées GPS d'une ville depuis le tableau 3D des villes
/// @param matrice Le tableau 3D des villes
/// @param nom_ville Le nom de la commune recherchée (ex: "LAVAL")
/// @param code_postal Le code postal associé (non utilisé ici car non stocké dans la matrice)
/// @return Un tableau de 2 doubles alloué dynamiquement contenant [0] = longitude, [1] = latitude, ou NULL en cas d'échec
double* recupere_cord_ville(char*** matrice, const char* nom_ville, const char* code_postal);

/// @brief  permet de trouver les points d'intérêt a partir d'un critère de recherche
/// @param matrice le tableau 3D contenant les données des points d'intérêt
/// @param critere le critère de recherche à utiliser pour trouver les points d'intérêt (par exemple, "police" pour trouver les points d'intérêt liés à la police)
/// @return un tableau de long long contenant les ID des points d'intérêt trouvés correspondant au critère de recherche, suivi d'un 0 pour indiquer la fin du tableau
long long *point_interet_critere(char*** matrice, char* critere);


/// @brief  permet de trouver les points d'intérêt dans un rayon de 10 km autour d'une coordonnée donnée 
/// @param matrice le tableau 3D contenant les données des points d'intérêt 
/// @param lat latitude de la coordonnée centrale
/// @param lon longitude de la coordonnée centrale
/// @param rayon le rayon de recherche en kilomètres
/// @return un tableau de long long contenant les ID des points d'intérêt trouvés dans le rayon spécifié, suivi d'un 0 pour indiquer la fin du tableau
long long* point_interet_rayon(char*** matrice, double lat, double lon, float rayon);

/// @brief  permet de trouver les points d'intérêt dans un rayon de 10 km autour d'une coordonnée donnée et correspondant à un critère de recherche
/// @param matrice le tableau 3D contenant les données des points d'intérêt
/// @param lat latitude de la coordonnée centrale
/// @param lon longitude de la coordonnée centrale
/// @param rayon le rayon de recherche en kilomètres
/// @param critere le critère de recherche à utiliser pour trouver les points d'intérêt (par exemple, "police" pour trouver les points d'intérêt liés à la police)
/// @return un tableau de long long contenant les ID des points d'intérêt trouvés dans le rayon spécifié et correspondant au critère de recherche, suivi d'un 0 pour indiquer la fin du tableau
long long* point_interet_rayon_critere(char*** matrice, double lat, double lon, float rayon, char* critere);

/// @brief permet d'afficher les villes avec leur latitude et longitude à partir du tableau 3D des villes
/// @param matrice le tableau 3D contenant les données des villes
void affichage_ville(char*** matrice);

/// @brief permet d'afficher les coordonnées d'un point d'intérêt à partir de son ID
/// @param matrice le tableau 3D contenant les données des points d'intérêt
void print_cordener_with_ID(char*** matrice, long long id);

/// @brief permet de trouver la ville de la région qui a le plus de points d'intérêt correspondant à un critère (les bar biensur ) dans un rayon donne et de faire le tour de ces bars en marchant un minimum
/// @param supertableau le tableau 3D contenant les données des points d'intérêt
/// @param ville_tab le tableau 3D contenant les données des villes
/// @param nom_region le nom de la région à analyser
/// @param critere le critère de recherche à utiliser pour trouver les points d'intérêt
/// @param rayon le rayon de recherche en kilomètres pour trouver les points d'intérêt autour de chaque ville
/// @param graphPath le chemin vers le fichier de graphe à utiliser pour le tour des bars
/// @param gpsPath le chemin vers le fichier de coordonnées GPS à utiliser pour le tour des bars
/// @param time_limit_sec le temps limite en secondes pour la recherche de la ville gagnante (0 ou moins pour aucune limite)
void capital_de_la_bierre(char*** supertableau, char*** ville_tab, char* nom_region, char* critere, float rayon, char* graphPath, char* gpsPath, double time_limit_sec);

/// @brief permet de déterminer les coordonnées du carré parfait englobant une région à partir d'un fichier de coordonnées GPS
/// @param gpsPath le chemin vers le fichier de coordonnées GPS de la région
/// @param min_lat pointeur vers une variable pour stocker la latitude minimale du carré
/// @param max_lat pointeur vers une variable pour stocker la latitude maximale du carré
/// @param min_lon pointeur vers une variable pour stocker la longitude minimale du carré
/// @param max_lon pointeur vers une variable pour stocker la longitude maximale du carré
void cree_carre_region(const char* gpsPath, double* min_lat, double* max_lat, double* min_lon, double* max_lon);

/// @brief permet de charger dans un tableau 3D uniquement les points d'intérêt qui tombent dans une zone définie par des coordonnées GPS
/// @param min_lat la latitude minimale de la zone
/// @param max_lat la latitude maximale de la zone
/// @param min_lon la longitude minimale de la zone
/// @param max_lon la longitude maximale de la zone
/// @return un tableau 3D contenant les données des points d'intérêt qui tombent dans la zone définie par les coordonnées GPS
char*** interet_tableau_filtre(double min_lat, double max_lat, double min_lon, double max_lon);



/// @brief Permet de trouver le trio optimal (3 critères différents) le plus compact géométriquement avant de lancer l'ACO
void tour_3_criteres(char*** matrice, double lat_v, double lon_v, float rayon, char* c1, char* c2, char* c3, char* graphPath, char* gpsPath);
