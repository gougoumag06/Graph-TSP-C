#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "graph/graph.h"
#include "graph/min_binary_heap.h"
#include "graph/shortest_path.h"
#include "graph/tsp.h"

static bool floatNear(float a, float b, float eps)
{
    return fabsf(a - b) <= eps;
}

static void testMinBinaryHeap(void)
{
    Minbinaryheap *heap = min_binary_heap_create(8);
    assert(heap != NULL);

    min_binary_heap_add(heap, 2, 10.0f);
    min_binary_heap_add(heap, 0, 2.5f);
    min_binary_heap_add(heap, 1, 5.0f);
    assert(heap->size == 3);

    distance_node n0 = min_binary_heap_pop(heap);
    assert(n0.index == 0 && floatNear(n0.distance, 2.5f, 1e-5f));

    distance_node n1 = min_binary_heap_pop(heap);
    assert(n1.index == 1 && floatNear(n1.distance, 5.0f, 1e-5f));

    distance_node n2 = min_binary_heap_pop(heap);
    assert(n2.index == 2 && floatNear(n2.distance, 10.0f, 1e-5f));

    assert(heap->size == 0);
    min_binary_heap_free(&heap);
    assert(heap == NULL);

    printf("[OK] testMinBinaryHeap\n");
}

static void testShortestPath(void)
{
    Graph *g = Graph_create(4);
    Graph_setArc(g, 0, 1, 1.0f);
    Graph_setArc(g, 1, 2, 2.0f);
    Graph_setArc(g, 0, 2, 10.0f);
    Graph_setArc(g, 2, 3, 3.0f);

    Path *path = Graph_shortestPath(g, 0, 3);
    assert(path != NULL);
    assert(floatNear(path->distance, 6.0f, 1e-5f));
    assert(ListInt_size(path->list) == 4);

    Path_destroy(path);
    Graph_destroy(g);

    printf("[OK] testShortestPath\n");
}

static void testTspHeuristic(void)
{
    Graph *g = Graph_create(4);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (i != j)
            {
                float w = ((i + 1) % 4 == j || (j + 1) % 4 == i) ? 1.0f : 5.0f;
                Graph_setArc(g, i, j, w);
            }
        }
    }

    Path *tour = Graph_tspFromHeuristic(g, 0);
    assert(tour != NULL);
    assert(floatNear(tour->distance, 4.0f, 1e-5f));
    assert(ListInt_size(tour->list) == 5);

    Path_destroy(tour);
    Graph_destroy(g);

    printf("[OK] testTspHeuristic\n");
}

int main(void)
{
    testMinBinaryHeap();
    testShortestPath();
    testTspHeuristic();
    printf("All tests passed.\n");
    return 0;
}

