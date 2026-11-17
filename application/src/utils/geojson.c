#include "geojson.h"


void geojson_create(node* tab, int size, Path* tsp, char* graphPath, char* gps_path)
{
    FILE* file = fopen("../application/exit/output.geojson", "w");
    AssertNew(file);

    // EN - TETE
    fprintf(file, "{\n");
    fprintf(file, "  \"type\": \"FeatureCollection\",\n");
    fprintf(file, "  \"features\": [\n");

    Graph* graph = Graph_load(graphPath);
    node* graphTab = (node*)calloc(Graph_size(graph), sizeof(node));
    Coo_load(graphTab, graph, gps_path);
    AssertNew(graphTab);


    /// On Met tous les points du sous-graph
    for (int i = 0; i < size; i++)
    {
        int valeur = tab[i].index;
        fprintf(file, "     {\n");
        fprintf(file, "         \"type\": \"Feature\",\n");
        fprintf(file, "         \"properties\": { \n");
        fprintf(file, "              \"_umap_options\": {\n");
        fprintf(file, "                  \"color\": \"Purple\",\n");
        fprintf(file, "                  \"weight\": 6,\n");
        fprintf(file, "                   \"iconClass\": \"Drop\"\n");
        fprintf(file, "               },\n");
        fprintf(file, "              \"name\": \"%s\",\n", tab[i].name);
        fprintf(file, "              \"description\": \"Longitude : %.8f latitude : %.8f\"\n", tab[i].longitude, tab[i].latitude);
        fprintf(file, "         },\n");
        fprintf(file, "         \"geometry\": {\n");
        fprintf(file, "              \"type\": \"Point\",\n");
        fprintf(file, "              \"coordinates\" : [%.8lf,%.8lf]\n", graphTab[valeur].longitude, graphTab[valeur].latitude);
        fprintf(file, "         }\n");
        fprintf(file, "     },\n");
    }

    ///On fait les chemins avec ces points

    ListIntIter* iter = ListIntIter_create(tsp->list);

    while (ListIntIter_isValid(iter))
    {
        ListIntNode* nextNode = iter->current->next;

        if (nextNode == iter->sentinel) break;
        int start = iter->current->value;
        int end = nextNode->value;
        fprintf(file, "     {\n");
        fprintf(file, "         \"type\": \"Feature\",\n");
        fprintf(file, "         \"properties\": { \n");
        fprintf(file, "              \"name\": \"TSP From %d to %d\"\n", start, end);
        fprintf(file, "         },\n");
        fprintf(file, "         \"geometry\": {\n");
        fprintf(file, "              \"type\": \"LineString\",\n");
        fprintf(file, "              \"coordinates\" : [\n");
        fprintf(file, "                  ");



        Path* chemin = Graph_shortestPath(graph, tab[start].index, tab[end].index);
        ListIntIter* iterGraph = ListIntIter_create(chemin->list);
        // Pour chaque chemin du graph
        while (iterGraph->current != iterGraph->sentinel)
        {
            int value = iterGraph->current->value;
            /*printf("La valeur est : %d\n", value);*/
            if (iterGraph->current->next == iterGraph->sentinel) fprintf(file, "[%.8f,%.8f]", graphTab[value].longitude, graphTab[value].latitude);
            else  fprintf(file, "[%.8f,%.8f],", graphTab[value].longitude, graphTab[value].latitude);

            ListIntIter_next(iterGraph);
        }
        /*printf("YES\n");*/
        // Fin d'un chemin de point
        ListIntIter_next(iter);
        fprintf(file, "\n              ] \n");
        fprintf(file, "         } \n");
        if (end == 0) fprintf(file, "     }\n");
        else fprintf(file, "     },\n");

        ListIntIter_destroy(iterGraph);
        Path_destroy(chemin);

    }

    fprintf(file, "  ]\n}"); // Pied de page
    ListIntIter_destroy(iter);
    free(graphTab);
    Graph_destroy(graph);
    fclose(file);
}



void geojson_graphRoutier(char* graphPath, char* gpsPath)
{
    FILE* file = fopen("../application/exit/graphRoutier.geojson", "w");
    AssertNew(file);

    size_t buffer_size = 65536;
    char* buffer = malloc(buffer_size);
    AssertNew(buffer);

    setvbuf(file, buffer, _IOFBF, buffer_size);

    Graph* graph = Graph_load(graphPath);
    AssertNew(graph);
    int size = Graph_size(graph);
    node* graphTab = calloc(size, sizeof(node));
    AssertNew(graphTab);
    Coo_load(graphTab, graph, gpsPath);

    fprintf(file,"{\n  \"type\": \"FeatureCollection\",\n  \"features\": [\n");

    bool isFirst = true;

    for (int i = 0; i < size; i++)
    {
        ArcList* arc = graph->vertices[i].arcList;
        while (arc != NULL)
        {
            if (i < arc->target)
            {
                if (!isFirst)
                {
                    fputs(",\n", file);
                }
                isFirst = false;

                fprintf(file, "     {\n");
                fprintf(file, "         \"type\": \"Feature\",\n");
                fprintf(file, "         \"geometry\": {\n");
                fprintf(file, "              \"type\": \"LineString\",\n");
                fprintf(file, "              \"coordinates\" : [\n");
                fprintf(file, "                 [%.8lf,%.8lf], [%.8lf,%.8lf]\n", graphTab[i].longitude, graphTab[i].latitude,
                    graphTab[arc->target].longitude, graphTab[arc->target].latitude);
                fprintf(file, "              ]\n");
                fprintf(file, "         }\n");
                fprintf(file, "     }");

            }
            arc = arc->next;
        }
    }

    fprintf(file,"\n  ]\n}");

    fclose(file);
    free(buffer);

    Graph_destroy(graph);
    free(graphTab);
}
