/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "tsp.h"

float rand_fourmi(unsigned int* seed)
{
    *seed = (*seed * 1103515245 + 12345) & 0x7FFFFFFF;
    return (float)*seed / (float)0x7FFFFFFF;
}

Path* Graph_tspFromHeuristic(Graph* graph, int station)
{
    Path* path = Path_create(station);
    path->distance = 0.0f;

    int size = Graph_size(graph);
    int prev = station;
    int next = -1;

    bool* visited = (bool*)calloc(size, sizeof(bool));
    AssertNew(visited);
    visited[station] = true;

    for (int i = 1; i < size; i++)
    {
        float minDistance = INFINITY;

        //on regarde les voisin 
        ArcList* arc = Graph_getArcList(graph, prev);
        while (arc != NULL)
        {
            if (visited[arc->target])
            {
                arc = arc->next;
                continue;
            }
            if (arc->weight < minDistance)
            {
                minDistance = arc->weight;
                next = arc->target;
            }
            arc = arc->next;
        }

        if (next < 0)
        {
            break;
        }
        ListInt_insertLast(path->list, next);
        visited[next] = true;
        path->distance += minDistance;
        prev = next;
    }

    ListInt_insertLast(path->list, station);
    //la distance de retour 
    float* returnWeight = Graph_getArc(graph, prev, station);
    if (returnWeight != NULL)
    {
        path->distance += *returnWeight;
    }

    free(visited); // on ne l oublie pas sinon je vais me faire taper sur les doight Thomas
    return path;
}

float* Graph_acoGetProbabilities(
    Graph* graph, Graph* pheromones, int station,
    bool* explored, float alpha, float beta)
{
    int size = Graph_size(graph);
    float* tab = (float*)calloc(size, sizeof(float));
    AssertNew(tab);

    float somme = 0.0f;

    ArcList* arc = pheromones->vertices[station].arcList;
    while (arc != NULL)
    {
        int target = arc->target;
        if (!explored[target])
        {
            float* dist = Graph_getArc(graph, station, target);
            if (dist != NULL && *dist > 0.0f)
            {
                somme += pow(arc->weight, alpha) * pow(1.0f / *dist, beta);
            }
        }
        arc = arc->next;
    }

    if (somme == 0.0f) return tab;

    arc = pheromones->vertices[station].arcList;
    while (arc != NULL)
    {
        int target = arc->target;
        if (!explored[target])
        {
            float* dist = Graph_getArc(graph, station, target);
            if (dist != NULL && *dist > 0.0f)
            {
                tab[target] = pow(arc->weight, alpha) * pow(1.0f / *dist, beta) / somme;
            }
        }
        arc = arc->next;
    }

    return tab;
}


void Graph_acoPheromoneGlobalUpdate(Graph* pheromones, float rho)
{
    for (int i = 0; i < pheromones->vertexCount; i++)
    {
        ArcList* arc = pheromones->vertices[i].arcList;
        while (arc != NULL)
        {
            arc->weight *= (1.0f - rho);
            arc = arc->next;
        }
    }
}

void Graph_acoPheromoneUpdatePath(Graph* pheromones, Path* path, float q)
{

    if (path == NULL || path->list == NULL || path->distance <= 0) return;

    float deposit = q / path->distance;

    ListIntIter* iter = ListIntIter_create(path->list);

    while (ListIntIter_isValid(iter))
    {
        int u = ListIntIter_get(iter);
        ListIntNode* nextNode = iter->current->next;

        if (nextNode == iter->sentinel) break;

        int v = nextNode->value;

        ArcList* arc = pheromones->vertices[u].arcList;
        while (arc != NULL)
        {
            if (arc->target == v)
            {
                arc->weight += deposit;
                break;
            }
            arc = arc->next;
        }

        ArcList* arcRetour = pheromones->vertices[v].arcList;
        while (arcRetour != NULL)
        {
            if (arcRetour->target == u)
            {
                arcRetour->weight += deposit;
                break;
            }
            arcRetour = arcRetour->next;
        }

        ListIntIter_next(iter);
    }
    ListIntIter_destroy(iter);
}

Path* Graph_tspFromACO(Graph* graph, int station, int iterationCount, int antCount, float alpha, float beta, float rho, float q)
{
    printf("Running ACO with %d iterations and %d ants per iteration...\n", iterationCount, antCount);
    int size = Graph_size(graph);
    Graph* pheromones = Graph_create(size);
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (i != j) Graph_setArc(pheromones, i, j, 1.0f);
        }
    }

    Path* youpisafonction = NULL;
    for (int i = 0; i < iterationCount; i++)
    {
        Path** liste_chemins = (Path**)calloc(antCount, sizeof(Path*));
        AssertNew(liste_chemins);
        int j;

#pragma omp parallel for
        for (j = 0; j < antCount; j++)
        {
            // Creation d'une graine unique par fourmi et par coeur
            unsigned int local_seed = 123456789 + i + j + omp_get_thread_num();

            Path* path = Graph_acoConstructPath(graph, pheromones, station, alpha, beta, &local_seed);
            AssertNew(path);
            liste_chemins[j] = path;
        }

        Graph_acoPheromoneGlobalUpdate(pheromones, rho);

        for (int u = 0; u < antCount; u++)
        {
            Graph_acoPheromoneUpdatePath(pheromones, liste_chemins[u], q);
            if (youpisafonction == NULL || liste_chemins[u]->distance < youpisafonction->distance)
            {
                if (youpisafonction != NULL)
                {
                    Path_destroy(youpisafonction);
                }
                youpisafonction = liste_chemins[u];
            }
            if (liste_chemins[u] != youpisafonction)
            {
                Path_destroy(liste_chemins[u]);
            }
        }
        free(liste_chemins);
    }

    Graph_destroy(pheromones);
    return youpisafonction;
}



Path* Graph_acoConstructPath(Graph* graph, Graph* pheromones, int station, float alpha, float beta, unsigned int* seed)
{
    Path* path = Path_create(station);
    path->distance = 0.0f;

    int size = Graph_size(graph);
    int prev = station;
    int next = -1;

    bool* visited = (bool*)calloc(size, sizeof(bool));
    AssertNew(visited);
    visited[station] = true;

    for (int i = 1; i < size; i++)
    {
        next = -1;

        float* probabilities = Graph_acoGetProbabilities(graph, pheromones, prev, visited, alpha, beta);

        float r = rand_fourmi(seed); // Utilisation de notre fonction thread-safe
        float cursor = 0.0f;

        for (int j = 0; j < size; j++)
        {
            if (visited[j]) continue;
            cursor += probabilities[j];
            if (r <= cursor)
            {
                next = j;
                break;
            }
        }

        free(probabilities);

        if (next == -1)
        {
            for (int j = 0; j < size; j++)
            {
                if (!visited[j])
                {
                    next = j;
                    break;
                }
            }
        }

        if (next < 0)
        {
            break;
        }
        ListInt_insertLast(path->list, next);
        visited[next] = true;

        float* arcWeight = Graph_getArc(graph, prev, next);
        if (arcWeight != NULL)
        {
            path->distance += *arcWeight;
        }

        prev = next;
    }

    ListInt_insertLast(path->list, station);
    float* returnWeight = Graph_getArc(graph, prev, station);
    if (returnWeight != NULL)
    {
        path->distance += *returnWeight;
    }

    free(visited);
    return path;
}
