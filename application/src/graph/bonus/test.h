#pragma once
#include "settings.h"
#include "graph/graph.h"
#include "graph/2_Path_Matrix.h"
#include "utils/geojson.h"
#include "graph/tsp.h"
#include "graph/bonus/bonus_interet.h" 
#include "graph/bonus/tourbar.h"

typedef struct
{
    char graphPath[256];
    char gpsPath[256];
    char villeNom[128];
    char codePostal[16];
    char regionNom[128]; 
    char critere[64];
    char critere1[64]; 
    char critere2[64]; 
    char critere3[64];
    float rayon;
    int options;
    double timeLimit;
} ConfigApp;

bool config_app(const char* filename, ConfigApp* config);
bool verifier_coherence_config(ConfigApp* config);
void option1(ConfigApp config); // Tour des bars dans une ville a partir d'un rayon
void option2(ConfigApp config); // Capitale de la bière a partir d'une région
void option3(ConfigApp config); // GraphRoutier
void option4(ConfigApp config); // permet de récupérer les 3 points des critères choisis les plus proches entre eux
