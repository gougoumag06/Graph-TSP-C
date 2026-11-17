#pragma once
#include "settings.h"
#include "graph/graph.h"
#include "graph/2_Path_Matrix.h"
#include "utils/geojson.h"
#include "graph/tsp.h"

#define MAXROWS 2000000
#define MAXCOL 40
#define R 6371.0

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Macro pour convertir les degrés en radians
#define TO_RAD(deg) ((deg) * M_PI / 180.0)

/// @brief Permet de créer un tour de bars à partir d'une liste de points d'intérêt et de leurs coordonnées, en utilisant un algorithme de résolution du TSP pour trouver le chemin optimal entre ces points. Le résultat est ensuite exporté au format GeoJSON pour visualisation.
/// @param bigTab Le tableau 3D contenant les données des points d'intérêt, utilisé pour récupérer les coordonnées et les noms des points à partir de leurs ID.
/// @param points Tableau des IDs des points d'intérêt à inclure dans le tour.
/// @param graphPath Chemin vers le fichier de graphe.
/// @param gpsPath Chemin vers le fichier GPS.
void tourBar(char*** bigTab, long long* points, char* graphPath, char* gpsPath);

int pluscourt(Graph* graph, node* graphTab, double lat, double lon);

/// @brief Permet de récupérer les coordonnées GPS et le nom d'un point d'intérêt à partir de son ID en recherchant dans le tableau 3D des points d'intérêt.
/// @param lat Pointeur pour stocker la latitude du point d'intérêt.
/// @param lon Pointeur pour stocker la longitude du point d'intérêt.
/// @param name Tableau de caractères pour stocker le nom du point d'intérêt.
/// @param bigTab Le tableau 3D contenant les données des points d'intérêt.
/// @param id L'ID du point d'intérêt à rechercher.
void bonneCo(double* lat, double* lon, char* name, char*** bigTab, long long id);


/// @brief Calcule la distance entre deux points géographiques donnés par leurs latitudes et longitudes en utilisant la formule de Haversine.
/// @param lat1 Latitude du premier point.
/// @param lon1 Longitude du premier point.
/// @param lat2 Latitude du second point.
/// @param lon2 Longitude du second point.
/// @return La distance en kilomètres entre les deux points.
double distance_2_coo(double lat1, double lon1, double lat2, double lon2);
