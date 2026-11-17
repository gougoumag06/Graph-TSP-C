#pragma once

#include "../settings.h"
#include "graph.h"
#include "shortest_path.h"

//Structure d'un noeud
typedef struct
{
    long long index;
    double longitude;
    double latitude;
    char name[256];
}node;

/// <summary>
///  Cette fonction permet de charger les coordonées longitude/Latitude de chaque points qu'on à dans notre sous-graphe
/// </summary>
/// <param name="tab"> tab avec les noeuds du Graph principale</param>
/// <param name="graph"> sous graph chemin</param>
/// <param name="gps_path">String avec le chemin du fichier gps</param>
void Coo_load(node* tab, Graph* graph, char* gps_path);
Graph* Path_Matrix_mini_graph_load(node* tab, int nb_nodes, char* graphPath, char* gpsPath);
void Path_Matrix();
