#include "tourbar.h"
#include "graph/bonus/bonus_interet.h"
#include <math.h>   
#include <stdlib.h> 

void tourBar(char*** bigTab, long long* points, char* graphPath, char* gpsPath)
{
    //récupération de la taille 
    int size = 0;
    while (points[size] != 0)
    {
        size++;
    }

    //Tableau pour créer le sous-graphe
    node* court = calloc(size, sizeof(node));
    AssertNew(court);

    double lat;
    double lon;
    char name[256];
    int nb_uniques = 0;

    Graph* graph = Graph_load(graphPath);
    AssertNew(graph);
    node* graphTab = calloc(Graph_size(graph), sizeof(node));
    AssertNew(graphTab);
    for (int i = 0; i < Graph_size(graph); i++)
    {
        graphTab[i].index = i;
    }
    Coo_load(graphTab, graph, gpsPath);


    for (int i = 0; i < size; i++)
    {
        bonneCo(&lat, &lon, name, bigTab, points[i]);
        
        long long index = pluscourt(graph, graphTab, lat, lon);

        int deja_present = -1;
        for (int k = 0; k < nb_uniques; k++)
        {
            if (court[k].index == index)
            {
                deja_present = k; 
                break;
            }
        }

        if (deja_present == -1)
        {
            court[nb_uniques].index = index;
            court[nb_uniques].latitude = lat;
            court[nb_uniques].longitude = lon;
            //printf("%s\n", name);
            strncpy(court[nb_uniques].name, name, 255);
            court[nb_uniques].name[255] = '\0';
            nb_uniques++; 
        }
        else 
        {
            char nom_fusionne[520]; 
            snprintf(nom_fusionne, sizeof(nom_fusionne), "%s + %s", court[deja_present].name, name);
            strncpy(court[deja_present].name, nom_fusionne, 255);
            court[deja_present].name[255] = '\0'; 
        }
    }

    if (nb_uniques < 2)
    {
        printf("il y a que un point qui respecte les critere \n");
        free(court);
        return;
    }
    Graph_destroy(graph);
    free(graphTab);
    // Création du sous graphe
    Graph* chemin = Path_Matrix_mini_graph_load(court, nb_uniques, graphPath, gpsPath);

    int station = 0;
    int iterationCount = 200;
    int antCount = 60;
    float alpha = 2.0f;
    float beta = 3.0f;
    float rho = 0.1f;
    float q = 2.0f;

    Path* tsp = Graph_tspFromACO(chemin, station, iterationCount, antCount, alpha, beta, rho, q);
    Path_print(tsp);
    geojson_create(court, nb_uniques, tsp, graphPath, gpsPath);
    printf("\033[1;32mLe tour de ces bars a ete cree dans le fichier output.geojson\033[0m\n");
    Path_destroy(tsp);
    Graph_destroy(chemin);

    free(court);
}

void bonneCo(double* lat, double* lon, char* name, char*** bigTab, long long id)
{
    for (int i = 0; i < MAXROWS; i++)
    {
        if (bigTab[i][0] != NULL && atoll(bigTab[i][0]) == id)
        {
            *lat = atof(bigTab[i][2]);
            *lon = atof(bigTab[i][1]);
            if (bigTab[i][4] != NULL)
            {
                strncpy(name, bigTab[i][4], 255);
                name[255] = '\0'; 
            }
            else
            {
                strncpy(name, "Nom inconnu", 255);
            }

            break;
        }
    }
}

int pluscourt(Graph* graph, node *graphTab, double lat, double lon)
{
    //Création du graph et de son tableau
    


    double distance = 100000000.0; 
    long long index = 0;
    for (int i = 0; i < Graph_size(graph); i++)
    {
        double d = distance_2_coo(graphTab[i].latitude, graphTab[i].longitude, lat, lon);
        if (d < distance)
        {
            distance = d; 
            index = graphTab[i].index;
        }
    }
    return index;
}

double distance_2_coo(double lat1, double lon1, double lat2, double lon2)
{
    double lat1_rad = TO_RAD(lat1);
    double lon1_rad = TO_RAD(lon1);
    double lat2_rad = TO_RAD(lat2);
    double lon2_rad = TO_RAD(lon2);

    double diflat = lat1_rad - lat2_rad;
    double diflon = lon1_rad - lon2_rad;

    double a = (sin(diflat / 2.0) * sin(diflat / 2.0)) +
        cos(lat2_rad) * cos(lat1_rad) * (sin(diflon / 2.0) * sin(diflon / 2.0));

    double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));
    double d = R * c;

    return d;
}
