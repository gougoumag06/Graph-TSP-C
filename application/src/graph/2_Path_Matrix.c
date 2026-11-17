#include "2_Path_Matrix.h"


Graph* Path_Matrix_mini_graph_load(node* tab, int nb_nodes, char* graphPath, char* gpsPath)
{
    Graph* Laval = Graph_load(graphPath);
    if (!Laval) return NULL;

    Graph* chemin = Graph_create(nb_nodes);
    for (int i = 0; i < nb_nodes; i++)
    {
        for (int j = i + 1; j < nb_nodes; j++)
        {
            Path* p = Graph_shortestPath(Laval, tab[i].index, tab[j].index);


            if (p != NULL)
            {
                Graph_setArc(chemin, i, j, p->distance);
                Graph_setArc(chemin, j, i, p->distance);
                Path_destroy(p);
            }
        }
    }
    Graph_destroy(Laval);
    return chemin;
}



void Coo_load(node* tab, Graph* graph, char* gps_path)
{
    FILE* coo = fopen(gps_path, "r");
    AssertNew(coo);

    int size = 0;
    int scount;
    scount = fscanf(coo, "%d", &size);
    assert(scount == 1);
    (void)scount;


    double longitude, latitude;
    for (int i = 0; i < size; i++)
    {
        scount = fscanf(coo, "%lf %lf", &longitude, &latitude);
        assert(scount == 2);

        if (i < graph->vertexCount)
        {
            tab[i].longitude = longitude;
            tab[i].latitude = latitude;
        }

    }

    fclose(coo);
}

void Path_Matrix()
{
    FILE* file = fopen("../application/src/test/2_path_matrix/input1.txt", "r");
    AssertNew(file);
    char graph_path[256];
    char gps_path[256];
    int nb, Scount;
    Scount = fscanf(file,"%s", graph_path);
    assert(Scount == 1);
    Scount = fscanf(file,"%s", gps_path);
    assert(Scount == 1);
    Scount = fscanf(file,"%d", &nb);
    assert(Scount == 1);
    (void)Scount;

    node* tab = calloc(nb, sizeof(node));
    AssertNew(tab);
    for (int i = 0; i < nb; i++)
    {
        Scount = fscanf(file, "%lld", &tab[i].index);
        assert(Scount == 1);
    }

    Graph* chemin = Path_Matrix_mini_graph_load(tab, nb, graph_path, gps_path);
    
    for (int i = 0; i < nb; i++)
    {
        
        for (int j = 0; j < nb; j++)
        {
            ArcList* u = chemin->vertices[i].arcList;
            float poids = 0.0f;
            while (u != NULL)
            {
                if (u->target == j)
                {
                    poids = u->weight;
                }
                u = u->next;
            }
            printf("%d, %d, %.1f\n", i, j, poids);
        }
    }
    Graph_destroy(chemin);
    free(tab);
}
