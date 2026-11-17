/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#pragma once

#include "settings.h"

typedef struct TreeIntNode TreeIntNode;
typedef struct TreeInt TreeInt;

struct TreeInt
{
    /// @brief Racine de l'arbre
    TreeIntNode* root;
};

/// @brief Structure représentant un arbre d'entiers.
/// Chaque noeud peut avec un nombre quelconque d'enfants.
struct TreeIntNode
{
    /// @brief Pointeur vers le premier enfant du noeud.
    TreeIntNode* firstChild;

    /// @brief Pointeur vers le noeud frère suivant.
    TreeIntNode* nextSibling;

    /// @brief Valeur du noeud.
    int value;
};

/// @brief Crée un nouveau noeud d'arbre.
/// @param value la valeur du noeud.
/// @return Le noeud nouvellement créé.
TreeIntNode* TreeInt_nodeCreate(int value);

/// @brief Crée un arbre avec uniquement un noeud.
/// @param value la valeur du noeud (la racine de l'arbre).
/// @return L'arbre créé.
TreeInt* TreeInt_create(void);

/// @brief Détruit récursivement tous les noeuds d'un arbre donné par sa racine.
/// @param tree la racine de l'arbre.
void TreeInt_destroy(TreeInt* tree);

/// @brief Ajoute un enfant à un noeud d'un arbre.
/// @param node le noeud. Peut être null pour un ajout en racine
/// @param child l'enfant à ajouter.
void TreeInt_addChild(TreeIntNode* node, TreeIntNode* child);

/// @brief Recherche un noeud dans un arbre.
/// @param tree la racine de l'arbre.
/// @param value la valeur recherchée.
/// @return Un noeud ayant la valeur recherchée ou NULL si aucun noeud ne
/// possède cette valeur.
TreeIntNode* TreeInt_get(TreeInt* tree, int value);

/// @brief Affiche un arbre.
/// @param tree la racine de l'arbre.
void TreeInt_print(TreeInt* tree);
