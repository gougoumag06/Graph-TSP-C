/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "graph.h"

Graph* Graph_create(int size)
{
    Graph* graph = (Graph*)calloc(1, sizeof(Graph));
    AssertNew(graph);

    graph->vertexCount = size;

    graph->vertices = (Vertex*)calloc(size, sizeof(Vertex));
    AssertNew(graph->vertices);

    for (int i = 0; i < size; i++)
    {
        Vertex* node = &(graph->vertices[i]);

        node->arcList = NULL;
        node->arcCount = 0;
    }

    return graph;
}


void Graph_destroy(Graph* graph)
{
    if (!graph) return;

    if (graph->vertices)
    {
        for (int i = 0; i < graph->vertexCount; i++)
        {
            Vertex* node = &(graph->vertices[i]);
            ArcList* currArc = node->arcList;
            while (currArc != NULL)
            {
                ArcList* nextArc = currArc->next;
                free(currArc);
                currArc = nextArc;
            }
        }
        free(graph->vertices);
    }

    free(graph);
}


void Graph_setArc(Graph* graph, int u, int v, float weight)
{
    assert(graph && "The Graph must be created");
    assert(0 <= u && u < graph->vertexCount && "The node ID is invalid");
    assert(0 <= v && v < graph->vertexCount && "The node ID is invalid");

    // Recherche si l'arc existe
    ArcList* prevArc = NULL;
    ArcList* currArc = graph->vertices[u].arcList;
    while (currArc != NULL && v > currArc->target)
    {
        prevArc = currArc;
        currArc = currArc->next;
    }

    if (currArc && currArc->target == v)
    {
        // L'arc est déjà présent, on modifie ses données
        currArc->weight = weight;
    }
    else
    {
        // L'arc n'existe pas dans le graphe
        // On l'ajoute en début de list
        ArcList* arc = (ArcList*)calloc(1, sizeof(ArcList));
        AssertNew(arc);

        arc->next = currArc;
        arc->target = v;
        arc->weight = weight;

        if (prevArc == NULL)
        {
            graph->vertices[u].arcList = arc;
        }
        else
        {
            prevArc->next = arc;
        }

        graph->vertices[u].arcCount++;
    }
}


void Graph_removeArc(Graph* graph, int u, int v)
{
    assert(graph && "The Graph must be created");
    assert(0 <= u && u < graph->vertexCount && "The node ID is invalid");
    assert(0 <= v && v < graph->vertexCount && "The node ID is invalid");

    // Recherche si l'arc existe
    ArcList* prevArc = NULL;
    ArcList* currArc = graph->vertices[u].arcList;
    while (currArc != NULL && v > currArc->target)
    {
        prevArc = currArc;
        currArc = currArc->next;
    }

    if (currArc && currArc->target == v)
    {
        // On supprime l'arc
        if (prevArc)
        {
            prevArc->next = currArc->next;
        }
        else
        {
            graph->vertices[u].arcList = currArc->next;
        }
        free(currArc);

        graph->vertices[u].arcCount--;
    }
}


float* Graph_getArc(Graph* graph, int u, int v)
{
    assert(graph && "The Graph must be created");
    assert(0 <= u && u < graph->vertexCount && "The node ID is invalid");
    assert(0 <= v && v < graph->vertexCount && "The node ID is invalid");


    ArcList* currArc = graph->vertices[u].arcList;
    while (currArc != NULL && v > currArc->target)
    {
        currArc = currArc->next;
    }

    if (currArc && currArc->target == v)
    {
        return &(currArc->weight);
    }

    return NULL;
}


Graph* Graph_load(char* filename)
{
    FILE* file = stdin;
    if (filename)
    {
        file = fopen(filename, "r");
        if (file == NULL)
        {
            assert(false);
            return NULL;
        }
    }

    int scanCount, size, arcCount;
    scanCount = fscanf(file, "%d %d", &size, &arcCount);
    assert(scanCount == 2);
    (void)scanCount;

    Graph* graph = Graph_create(size);
    for (int i = 0; i < arcCount; i++)
    {
        int u, v;
        float weight;

        scanCount = fscanf(file, "%d %d %f", &u, &v, &weight);
        assert(scanCount == 3);

        Graph_setArc(graph, u, v, weight);
    }

    if (filename)
        fclose(file);

    return graph;
}


void Graph_print(Graph* graph)
{
    assert(graph);

    int size = graph->vertexCount;
    int arcCount = 0;
    for (int currID = 0; currID < size; currID++)
    {
        for (ArcList* arc = Graph_getArcList(graph, currID);
            arc != NULL; arc = arc->next)
        {
            arcCount++;
        }
    }
    printf("%d %d\n", size, arcCount);

    for (int currID = 0; currID < size; currID++)
    {
        for (ArcList* arc = Graph_getArcList(graph, currID);
            arc != NULL; arc = arc->next)
        {
            printf("%d %d %.2f\n", currID, arc->target, arc->weight);
        }
    }
}
