#include "test.h"

bool config_app(const char* filename, ConfigApp* config)
{
    FILE* file = fopen(filename, "r");
    if (!file) return false;

    config->rayon = 1.0;
    config->timeLimit = -1.0;

    char ligne[256];
    while (fgets(ligne, sizeof(ligne), file))
    {
        if (ligne[0] == '#' || ligne[0] == '\n' || ligne[0] == '\r') continue;

        char cle[128] = { 0 };
        char valeur[128] = { 0 };

        if (sscanf(ligne, "%127[^=]=%127[^\n\r]", cle, valeur) == 2)
        {
            if (strcmp(cle, "GRAPH_PATH") == 0) strcpy(config->graphPath, valeur);
            else if (strcmp(cle, "GPS_PATH") == 0) strcpy(config->gpsPath, valeur);
            else if (strcmp(cle, "VILLE_NOM") == 0) strcpy(config->villeNom, valeur);
            else if (strcmp(cle, "CODE_POSTAL") == 0) strcpy(config->codePostal, valeur);
            else if (strcmp(cle, "REGION_NOM") == 0) strcpy(config->regionNom, valeur);
            else if (strcmp(cle, "CRITERE") == 0) strcpy(config->critere, valeur);
            else if (strcmp(cle, "CRITERE_1") == 0) strcpy(config->critere1, valeur);
            else if (strcmp(cle, "CRITERE_2") == 0) strcpy(config->critere2, valeur);
            else if (strcmp(cle, "CRITERE_3") == 0) strcpy(config->critere3, valeur);
            else if (strcmp(cle, "RAYON") == 0) config->rayon = atof(valeur);
            else if (strcmp(cle, "OPTIONS") == 0) config->options = atoi(valeur);
            else if (strcmp(cle, "TIME_LIMIT") == 0) config->timeLimit = atof(valeur);
        }
    }
    fclose(file);
    return true;
}

bool verifier_coherence_config(ConfigApp* config)
{
    printf("\033[32m=== Verification globale de la coherence de la configuration ===\033[0m\n");

    if (strcmp(config->regionNom, "Bretagne") == 0)
    {
        if (strstr(config->graphPath, "bretagne") == NULL || strstr(config->gpsPath, "bretagne") == NULL)
        {
            printf("\033[31mAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAttention La region 'Bretagne' est selectionnee, mais les fichiers GRAPH ou GPS ne correspondent pas !\033[0m\n");
            return false;
        }
    }
    else if (strcmp(config->regionNom, "Pays de la Loire") == 0)
    {
        bool ok_pays_de_la_loire = (strstr(config->graphPath, "pays_de_la_loire") != NULL && strstr(config->gpsPath, "pays_de_la_loire") != NULL);
        bool ok_laval = (strstr(config->graphPath, "laval") != NULL && strstr(config->gpsPath, "laval") != NULL);
        bool ok_mayenne = (strstr(config->graphPath, "mayenne") != NULL && strstr(config->gpsPath, "mayenne") != NULL);

        if (!ok_pays_de_la_loire && !ok_laval && !ok_mayenne)
        {
            printf("\033[31mAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAttentionLa region 'Pays de la Loire' est selectionnee, mais les fichiers GRAPH ou GPS ne correspondent pas (attendu : pays_de_la_loire, laval ou mayenne) !\033[0m\n");
            return false;
        }
    }
    else if (strcmp(config->regionNom, "Île-de-France") == 0)
    {
        if (strstr(config->graphPath, "ile-de-france") == NULL || strstr(config->gpsPath, "ile-de-france") == NULL)
        {
            printf("\033[31mAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAttention La region 'Île-de-France' est selectionnee, mais les fichiers GRAPH ou GPS ne correspondent pas !\033[0m\n");
            return false;
        }
    }
    else
    {
        printf("\033[31mAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAttention La region '%s' n'est pas referencee pour la verification automatique des fichiers.\033[0m\n", config->regionNom);
    }

    printf("Verification de l'existence et de l'emplacement de la ville...\n");

    char*** ville_tab = ville_tableau();
    if (ville_tab == NULL)
    {
        printf("\033[31mil y a un probleme avec la fonction ville_tableau()\033[0m\n");
        return false;
    }

    bool ville_trouvee = false;
    //bool code_postal_correct = false;
    bool region_correcte = false;
    char region_reelle[128] = "Inconnue";

    for (int i = 0; i < MAX_ROW_CYTIES; i++)
    {
        if (ville_tab[i][0] != NULL && strcmp(ville_tab[i][0], config->villeNom) == 0)
        {
            ville_trouvee = true;
            if (ville_tab[i][3] != NULL && strcmp(ville_tab[i][3], config->codePostal) == 0)
            {
                //code_postal_correct = true;

                if (ville_tab[i][4] != NULL)
                {
                    strcpy(region_reelle, ville_tab[i][4]);
                    if (strcmp(ville_tab[i][4], config->regionNom) == 0)
                    {
                        region_correcte = true;
                        break;
                    }
                }
            }
        }
    }
    free_ville_tableau(ville_tab);

    if (!ville_trouvee)
    {
        printf("\033[31mon a pas trouve la ville %s dans la base de donnees\033[0m\n", config->villeNom);
        return false;
    }

    if (!region_correcte)
    {
        printf("\033[31mIncoherence detectee ! La ville '%s' est situee en '%s', mais vous avez configure 'REGION_NOM=%s'.\n\033[0m",
            config->villeNom, region_reelle, config->regionNom);
        return false;
    }

    printf("\033[32m[OK] Configuration valide : La ville '%s' appartient bien a la region '%s'.\033[0m\n\n", config->villeNom, config->regionNom);
    return true;
}

void option1(ConfigApp config)
{
    printf("--- Lancement de l'Option 1 : Tour des %ss a %s ---\n", config.critere, config.villeNom);
    double min_lat, max_lat, min_lon, max_lon;
    cree_carre_region(config.gpsPath, &min_lat, &max_lat, &min_lon, &max_lon);
    char*** supertableau = interet_tableau_filtre(min_lat, max_lat, min_lon, max_lon);
    char*** ville_tab = ville_tableau();

    double* cord_ville = recupere_cord_ville(ville_tab, config.villeNom, config.codePostal);

    if (cord_ville != NULL)
    {
        printf("Coordonnees trouvees : Longitude %lf, Latitude %lf\n", cord_ville[0], cord_ville[1]);

        long long* resultats_critere_rayon = point_interet_rayon_critere(
            supertableau, cord_ville[1], cord_ville[0], config.rayon, config.critere
        );

        tourBar(supertableau, resultats_critere_rayon, config.graphPath, config.gpsPath);

        free(resultats_critere_rayon);
        free(cord_ville);
    }
    else
    {
        printf("\033[31mErreur : Ville %s introuvable.\033[0m\n", config.villeNom);
    }

    free_interet_tableau(supertableau);
    free_ville_tableau(ville_tab);
    printf("\033[32mExecution de l'Option 1 terminee.\033[0m\n");
}


void option2(ConfigApp config)
{
    printf("--- Lancement de l'Option 2 : Capitale des %ss en %s ---\n", config.critere, config.regionNom);
    printf("Temps limite charge : %.1f secondes\n", config.timeLimit);
    double min_lat, max_lat, min_lon, max_lon;
    cree_carre_region(config.gpsPath, &min_lat, &max_lat, &min_lon, &max_lon);
    char*** supertableau = interet_tableau_filtre(min_lat, max_lat, min_lon, max_lon);
    char*** ville_tab = ville_tableau();

    capital_de_la_bierre(supertableau, ville_tab, config.regionNom, config.critere, config.rayon, config.graphPath, config.gpsPath, config.timeLimit);

    free_interet_tableau(supertableau);
    free_ville_tableau(ville_tab);
    printf("\033[32mExecution de l'Option 2 terminee.\033[0m\n");
}

void option3(ConfigApp config)
{
    printf("--- Lancement de l'Option 3 : GraphRoutier de %s ---\n", config.graphPath);
    geojson_graphRoutier(config.graphPath, config.gpsPath);
    printf("\033[32mExecution de l'Option 3 terminee.\033[0m\n");
}


void option4(ConfigApp config)
{
    printf("--- Lancement de l'Option 4 : chemin le plus court entre trois trucs de trois critere differents (%s, %s, %s) a %s ---\n", config.critere1, config.critere2, config.critere3, config.villeNom);
    double min_lat, max_lat, min_lon, max_lon;
    cree_carre_region(config.gpsPath, &min_lat, &max_lat, &min_lon, &max_lon);
    char*** supertableau = interet_tableau_filtre(min_lat, max_lat, min_lon, max_lon);
    char*** ville_tab = ville_tableau();

    double* cord_ville = recupere_cord_ville(ville_tab, config.villeNom, config.codePostal);

    if (cord_ville != NULL)
    {
        tour_3_criteres(supertableau, cord_ville[1], cord_ville[0], config.rayon, config.critere1, config.critere2, config.critere3, config.graphPath, config.gpsPath);
        free(cord_ville);
    }
    else
    {
        printf("\033[31mil y a un probleme avec la ville %s tes sur quel est dans la base de donnees chef?\033[0m\n", config.villeNom);
    }

    free_interet_tableau(supertableau);
    free_ville_tableau(ville_tab);
    printf("\033[32mExecution de l'Option 4 terminee.\033[0m\n");
}
