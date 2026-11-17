/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "settings.h"
#include "graph/tsp.h"
#include "utils/geojson.h"
#include "graph/2_Path_Matrix.h"
#include "graph/bonus/bonus_interet.h"
#include "graph/bonus/tourbar.h"
#include "graph/bonus/test.h"

int main()
{
    ConfigApp config = { 0 };

    if (!config_app("../application/src/config.txt", &config))
    {
        printf("il y a un probleme avec le fichier de conf\n");
        return EXIT_FAILURE;
    }
    if (!verifier_coherence_config(&config))
    {
        printf("Veuillez corriger votre fichier config.txt avant de relancer.\n");
        return EXIT_FAILURE; 
    }
    printf("=== Configuration chargee ===\n");
    printf("\033[3;31mVille   : \033[0m \033[31m %s (%s)\033[0m\n", config.villeNom, config.codePostal);
    printf("\033[3;32mRegion  :\033[0m \033[32m %s\033[0m\n", config.regionNom);
    printf("\033[3;33mCritere :\033[0m \033[33m %s\033[0m\n", config.critere);
    printf("\033[3;38mCritere1:\033[0m \033[33m %s\033[0m\n", config.critere1);
    printf("\033[3;39mCritere2:\033[0m \033[33m %s\033[0m\n", config.critere2);
    printf("\033[3;40mCritere3:\033[0m \033[33m %s\033[0m\n", config.critere3);
    printf("\033[3;34mRayon   :\033[0m \033[34m %.2f km\033[0m\n", config.rayon);
    printf("\033[3;35mGraphe  :\033[0m \033[35m %s\033[0m\n", config.graphPath);
    printf("\033[3;36mOptions :\033[0m \033[36m %d\033[0m\n", config.options);

    printf("=============================\n\n");

    switch (config.options)
    {
    case 1:
        option1(config);
        break;
    case 2:
        option2(config);
        break;
    case 3:
        option3(config); 
        break;
    case 4:
        option4(config);
        break;
    default:
        printf("l option selectionnee n'est pas valide\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
