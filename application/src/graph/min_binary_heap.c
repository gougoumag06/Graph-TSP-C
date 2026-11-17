#include "min_binary_heap.h"



Minbinaryheap* min_binary_heap_create(int capacity)
{
    assert(capacity > 0 && "Capacity invalid");
    Minbinaryheap* heap = (Minbinaryheap*)calloc(1,sizeof(Minbinaryheap));
    if (!heap) {
        fprintf(stderr, "Memory allocation failed for Minbinaryheap\n");
        return NULL;
    }
    heap->size = 0;
    heap->capacity = capacity;
    heap->data = (distance_node*)calloc(capacity , sizeof(distance_node));
    if (!heap->data) {
        fprintf(stderr, "Memory allocation failed for Minbinaryheap data\n");
        free(heap);
        return NULL;
    }
    return heap;
}

void min_binary_heap_free(Minbinaryheap** heap)
{
    if (!heap || !*heap) return;
    free((*heap)->data);
    free(*heap);
    *heap = NULL;
}

void min_binary_heap_add(Minbinaryheap* heap, int index, float distance)
{
    if (heap->size >= heap->capacity) {
        fprintf(stderr, "Heap is full, cannot add more elements\n");
        return;
    }
    heap->data[heap->size].index = index;
    heap->data[heap->size].distance = distance;
    min_binary_heap_heapify_up(heap, heap->size);
    heap->size++;
}

void min_binary_heap_heapify_up(Minbinaryheap* heap, int index)
{
    while (index > 0) {
        int parent_index = (index - 1) / 2;
        if (heap->data[index].distance < heap->data[parent_index].distance) {
            distance_node temp = heap->data[index];
            heap->data[index] = heap->data[parent_index];
            heap->data[parent_index] = temp;
            index = parent_index;
        } else {
            break;
        }
    }
}

void min_binary_heap_heapify_down(Minbinaryheap* heap, int index)
{
    int smallest = index;
    int left_child = 2 * index + 1;
    int right_child = 2 * index + 2;
    if (left_child < heap->size && heap->data[left_child].distance < heap->data[smallest].distance) {
        smallest = left_child;
    }
    if (right_child < heap->size && heap->data[right_child].distance < heap->data[smallest].distance) {
        smallest = right_child;
    }
    if (smallest != index) {
        distance_node temp = heap->data[index];
        heap->data[index] = heap->data[smallest];
        heap->data[smallest] = temp;
        min_binary_heap_heapify_down(heap, smallest);
    }
}

distance_node min_binary_heap_pop(Minbinaryheap* heap)
{
    if (heap->size == 0) {
        fprintf(stderr, "Heap is empty, cannot pop elements\n");
        return (distance_node){-1, -1}; // retourn un noeud invalide
    }
    distance_node min_node = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    min_binary_heap_heapify_down(heap, 0);
    return min_node;
}

void min_binary_heap_print(Minbinaryheap* heap, int pos, int level)
{
    if (heap == NULL || pos >= heap->size)
    {
        return;
    }

    // On commence par le fils droit (affiché en haut)
    min_binary_heap_print(heap, 2 * pos + 2, level + 1);

    // Affichage de l'espacement pour l'indentation
    for (int i = 0; i < level; i++)
    {
        printf("    ");
    }

    // Affichage du nœud actuel (index et distance)
    printf("[%d] (%.2f)\n", heap->data[pos].index, heap->data[pos].distance);

    // On finit par le fils gauche (affiché en bas)
    min_binary_heap_print(heap, 2 * pos + 1, level + 1);
}


void min_binary_heap_display(Minbinaryheap* heap)
{
    printf("\n--- Structure du Tas Binaire (Taille: %d) ---\n", heap->size);
    if (heap->size == 0)
    {
        printf("Le tas est vide.\n");
    }
    else
    {
        min_binary_heap_print(heap, 0, 0);
    }
    printf("--------------------------------------------\n");
}
