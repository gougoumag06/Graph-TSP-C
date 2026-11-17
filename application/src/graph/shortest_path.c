/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "shortest_path.h"

Path* Graph_shortestPath(Graph* graph, int start, int end)
{
    assert(graph && "The Graph must be created");
    assert(0 <= start && start < graph->vertexCount && "The start is invalid");
    assert(end < graph->vertexCount && "The end is invalid");

    const int size = Graph_size(graph);

    int* predecessors = (int*)calloc(size, sizeof(int));
    AssertNew(predecessors);

    float* distances = (float*)calloc(size, sizeof(float));
    AssertNew(distances);

    Graph_dijkstra(graph, start, end, predecessors, distances);

    Path* path = Graph_dijkstraGetPath(predecessors, distances, end);

    free(predecessors);
    predecessors = NULL;
    free(distances);
    distances = NULL;

    return path;
}


void Graph_dijkstra(Graph* graph, int start, int end, int* predecessors, float* distances)
{
    assert(graph && "The Graph must be created");
    assert(0 <= start && start < graph->vertexCount && "The start is invalid");
    assert(end < graph->vertexCount && "The end is invalid");
    assert(predecessors && distances);

    const int size = Graph_size(graph);
    bool* explored = (bool*)calloc(size, sizeof(bool));
    AssertNew(explored);

    for (int i = 0; i < size; i++)
    {
        predecessors[i] = -1;
        distances[i] = INFINITY;
    }
    distances[start] = 0.0f;

    //creation du tas bianire 
    Minbinaryheap* heap = min_binary_heap_create(size * 10);

    min_binary_heap_add(heap, start, 0.0f);

    while (heap->size > 0)
    {
        distance_node node = min_binary_heap_pop(heap);
        int currID = node.index;

        if (explored[currID] == true) continue;
        explored[currID] = true;

        // Condition d'arret
        if (currID < 0 || currID == end)
            break;


        for (ArcList* arc = Graph_getArcList(graph, currID); arc != NULL; arc = arc->next)
        {
            int nextID = arc->target;
            float dist = distances[currID] + arc->weight;

            if (distances[nextID] > dist)
            {
                distances[nextID] = dist;
                predecessors[nextID] = currID;

                //on met dans le tas bianaire 
                min_binary_heap_add(heap, nextID, dist);
            }
        }
    }

    min_binary_heap_free(&heap);
    free(explored);
}

Path* Graph_dijkstraGetPath(int* predecessors, float* distances, int end)
{
    assert(predecessors && distances);
    assert(end >= 0);

    if (predecessors[end] < 0)
    {
        return NULL;
    }

    Path* path = Path_create(end);
    int currID = end;

    while (predecessors[currID] >= 0)
    {
        currID = predecessors[currID];
        ListInt_insertFirst(path->list, currID);
    }

    path->distance = distances[end];

    return path;
}


Path* Path_create(int start)
{
    Path* path = (Path*)calloc(1, sizeof(Path));
    AssertNew(path);

    path->distance = 0.0f;
    path->list = ListInt_create();
    ListInt_insertLast(path->list, start);

    return path;
}


void Path_destroy(Path* path)
{
    if (path == NULL) return;

    ListInt_destroy(path->list);
    free(path);
}


void Path_print(Path* path)
{
    if (path == NULL)
    {
        printf("path : NULL\n");
        return;
    }

    printf("path (distance = %f) : ", path->distance);
    ListInt_print(path->list);
}
