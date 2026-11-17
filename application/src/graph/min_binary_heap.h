#pragma once
#include "../settings.h"

//Structure d'un noeud
typedef struct {
    int index;
    float distance;
}distance_node;

//structure du tab binaire
typedef struct {
    int size;
    int capacity;
    distance_node *data; 
}Minbinaryheap;

//Fonction pour créer le tas binaire
Minbinaryheap* min_binary_heap_create(int capacity);

//fonction pour libérer le tas binaire
void min_binary_heap_free(Minbinaryheap** heap);

//Fonction pour ajouter un noeud
void min_binary_heap_add(Minbinaryheap* heap, int index, float distance);

//Fonction pour faire monter un noeud
void min_binary_heap_heapify_up(Minbinaryheap* heap, int index);

//Fonction pour faire descendre un noeud
void min_binary_heap_heapify_down(Minbinaryheap* heap, int index);

//FOnction pour récupérer la racine du tas
distance_node min_binary_heap_pop(Minbinaryheap* heap);

//Fonction pour print le tas binaire
void min_binary_heap_print(Minbinaryheap* heap, int pos, int level);


//Fonction pour afficher le tas binaire
void min_binary_heap_display(Minbinaryheap* heap);
