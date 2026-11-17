
//structure pour le CSV
//@id;@lon;@lat;name;operator;ref;aerialway;aerodrome;aeroway;amenity;boundary;bridge;craft;emergency;heritage;highway;historic;junction;landuse;leisure;man_made;military;mountain_pass;natural;office;place;railway;shop;tourism;tunnel;waterway

#include "bonus_interet.h"


char*** interet_tableau()
{
    char*** matrice = calloc(MAX_ROWS, sizeof(char**));
    for (int r = 0; r < MAX_ROWS; r++)
    {
        matrice[r] = calloc(MAX_COLS, sizeof(char*));
    }

    FILE* file = fopen("../data/BonusData/poi.csv", "r");
    if (file == NULL)
    {
        fprintf(stderr, "Erreur : fichier introuvable : ../data/BonusData/poi.csv\n");
        free_interet_tableau(matrice);
        return NULL;
    }

    char line[4096]; 
    int j = 0;

    while (j < MAX_ROWS && fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\r\n")] = '\0';

        char* curseur = line;

        for (int i = 0; i < 39; i++)
        {
            if (curseur == NULL) break; 

            char* prochaine_tab = strchr(curseur, '\t');

            if (prochaine_tab != NULL)
            {
                *prochaine_tab = '\0';
            }
            if (*curseur == '\0')
            {
                matrice[j][i] = NULL;
            }
            else
            {
                matrice[j][i] = strdup(curseur);
            }
            if (prochaine_tab != NULL)
            {
                curseur = prochaine_tab + 1;
            }
            else
            {
                curseur = NULL; 
            }
        }

        j++;
    }
    fclose(file);
    return matrice;


}

void free_interet_tableau(char*** matrice)
{
    for (int r = 0; r < MAX_ROWS; r++)
    {
        for (int c = 0; c < MAX_COLS; c++)
        {
            free(matrice[r][c]);
        }
        free(matrice[r]);
    }
    free(matrice);
}

char*** ville_tableau()
{
    char*** matrice = calloc(MAX_ROW_CYTIES, sizeof(char**));
    for (int r = 0; r < MAX_ROW_CYTIES; r++)
    {
        matrice[r] = calloc(MAX_COL_CYTIES, sizeof(char*));
    }

    FILE* file = fopen("../data/BonusData/communes-departement-region.csv", "r");
    if (file == NULL)
    {
        fprintf(stderr, "Erreur : fichier introuvable : ../data/BonusData/communes-departement-region.csv\n");
        free_ville_tableau(matrice);
        return NULL;
    }
    char line[4096];
    int j = 0;

    while (j < MAX_ROW_CYTIES && fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\r\n")] = '\0';

        char* curseur = line;

        for (int i = 0; i <= 14; i++)
        {
            if (curseur == NULL) break;

            char* prochaine_tab = strchr(curseur, ',');

            if (prochaine_tab != NULL)
            {
                *prochaine_tab = '\0';
            }

            if (i == 1) // nom_commune_postal
            {
                matrice[j][0] = (*curseur == '\0') ? NULL : strdup(curseur);
            }
            else if (i == 2) // code_postal
            {
                matrice[j][3] = (*curseur == '\0') ? NULL : strdup(curseur);
            }
            else if (i == 5) // latitude
            {
                matrice[j][1] = (*curseur == '\0') ? NULL : strdup(curseur);
            }
            else if (i == 6) // longitude
            {
                matrice[j][2] = (*curseur == '\0') ? NULL : strdup(curseur);
            }
            else if (i == 14) // nom_region
                matrice[j][4] = (*curseur == '\0') ? NULL : strdup(curseur);

            if (prochaine_tab != NULL)
            {
                curseur = prochaine_tab + 1;
            }
            else
            {
                curseur = NULL;
            }
        }

        j++;
    }

    fclose(file);
    return matrice;
}

void free_ville_tableau(char*** matrice)
{
    for (int r = 0; r < MAX_ROW_CYTIES; r++)
    {
        for (int c = 0; c < MAX_COL_CYTIES; c++)
        {
            free(matrice[r][c]);
        }
        free(matrice[r]);
    }
    free(matrice);
}

double* recupere_cord_ville(char*** matrice, const char* nom_ville, const char* code_postal)
{
    double* coords = calloc(2, sizeof(double));
    if (coords == NULL) return NULL;

    for (int i = 0; i < MAX_ROW_CYTIES; i++)
    {
        if (matrice[i][0] != NULL && matrice[i][3] != NULL)
        {
            if (strcmp(matrice[i][0], nom_ville) == 0 && strcmp(matrice[i][3], code_postal) == 0)
            {
                if (matrice[i][1] != NULL && matrice[i][2] != NULL)
                {
                    coords[0] = atof(matrice[i][2]); // Longitude
                    coords[1] = atof(matrice[i][1]); // Latitude
                    return coords;
                }
            }
        }
    }
    free(coords);
    return NULL;
}

long long *point_interet_critere(char*** matrice, char* critere)
{
    int toto = 0;
    long long* resultats = calloc(MAX_ROWS, sizeof(long long));
    for (int i = 0; i < MAX_ROWS; i++)
    {
        for (int j = 0; j < 39; j++)
        {
          
            if (matrice[i][j] != NULL && strcmp(matrice[i][j], critere) == 0)
            {
                toto++;
                resultats[toto - 1] = atoll(matrice[i][0]);
            }
        }
    }
    return resultats;
}

long long* point_interet_rayon(char*** matrice, double lat, double lon, float rayon)
{
    long long* resultats = calloc(MAX_ROWS, sizeof(long long));
    if (resultats == NULL) return NULL;

    int nb_trouves = 0;

    for (int i = 0; i < MAX_ROWS; i++)
    {
        if (matrice[i][0] != NULL && matrice[i][1] != NULL && matrice[i][2] != NULL)
        {
            double poi_lat = atof(matrice[i][2]);
            double poi_lon = atof(matrice[i][1]);

            double distance = distance_2_coo(lat, lon, poi_lat, poi_lon);

            if (distance <= rayon)
            {
                printf("Distance entre les coordonnees et le point d'interet %s: %f km\n", matrice[i][0], distance);
                printf("Point d'interet %s est dans un rayon de %f km\n", matrice[i][0], rayon);

                resultats[nb_trouves] = atoll(matrice[i][0]);
                nb_trouves++;
            }
        }
    }


    resultats[nb_trouves] = 0;

    return resultats; 
}

long long* point_interet_rayon_critere(char*** matrice, double lat, double lon, float rayon, char* critere)
{
    long long* resultats = calloc(MAX_ROWS, sizeof(long long));
    if (resultats == NULL) return NULL;

    int nb_trouves = 0;

    for (int i = 0; i < MAX_ROWS; i++)
    {
        if (matrice[i][0] == NULL)
            break; 
        if (matrice[i][0] != NULL && matrice[i][1] != NULL && matrice[i][2] != NULL)
        {
            double poi_lat = atof(matrice[i][2]);
            double poi_lon = atof(matrice[i][1]);

            double distance = distance_2_coo(lat, lon, poi_lat, poi_lon);

            if (distance <= rayon)
            {
                for (int j = 0; j < 39; j++)
                {
                    if (matrice[i][j] != NULL && strcmp(matrice[i][j], critere) == 0)
                    {
                        resultats[nb_trouves] = atoll(matrice[i][0]);
                        nb_trouves++;
                        break;
                    }
                }
            }
        }
    }

    resultats[nb_trouves] = 0;

    return resultats; 
}

void print_cordener_with_ID(char*** matrice, long long id)
{
    for (int i = 0; i < MAX_ROWS; i++)
    {
        if (matrice[i][0] != NULL && atoll(matrice[i][0]) == id)
        {
            printf("cordoner %s %s\n", matrice[i][2], matrice[i][1]);
            return;
        }
    }
    printf("Aucun point d'interet trouve avec l'ID: %lld\n", id);
}

void affichage_interet_ID(long long* id_points)
{
    int nb_trouves = 0;
    for (int i = 0; i < MAX_ROWS && id_points[i] != 0; i++)
    {
        printf("ID du point d'interet: %lld\n", id_points[i]);
        nb_trouves++;
    }
    printf("Nombre de points d'interet trouves: %d\n", nb_trouves);
   
}


void affichage_ville(char*** matrice)
{
    for (int i = 0; i < MAX_ROW_CYTIES; i++)
    {
       if (matrice[i][0] != NULL && matrice[i][1] != NULL && matrice[i][2] != NULL)
        {
            printf("Ville: %s, Latitude: %s, Longitude: %s\n", matrice[i][0], matrice[i][1], matrice[i][2]);
        }
    }
}





void capital_de_la_bierre(char*** supertableau, char*** ville_tab, char* nom_region, char* critere, float rayon, char* graphPath, char* gpsPath, double time_limit_sec)
{
    int max = -1;
    char ville_gagnante[128] = "bug";
    long long* meilleurs_points = NULL;

    printf("La recherche va etre tres longue, prenez un cafe !\n");

    int i;
    double start_time = omp_get_wtime();
    bool timeout = false; 

#pragma omp parallel for shared(timeout)
    for (i = 0; i < MAX_ROW_CYTIES; i++)
    {
        if (timeout) continue;

        // si le temps limite est depasse on previen tout les coeur et on arrete les calculs sauf si le la limite de temps est de 0 ou moins
        if (time_limit_sec > 0 && (omp_get_wtime() - start_time) > time_limit_sec)
        {
            timeout = true; // sa stop tout les coeurs
            continue;
        }

        if (ville_tab[i][4] != NULL && strcmp(ville_tab[i][4], nom_region) == 0)
        {
            if (ville_tab[i][1] != NULL && ville_tab[i][2] != NULL)
            {
                double lat = atof(ville_tab[i][1]);
                double lon = atof(ville_tab[i][2]);
                //char* nom_ville = ville_tab[i][0];

                long long* points_trouves = point_interet_rayon_critere(supertableau, lat, lon, rayon, critere);

                // on compte le nombre de points trouvés pour cette ville
                int compteur = 0;
                while (points_trouves[compteur] != 0)
                {
                    compteur++;
                }

                if (compteur > max)
                {
                    // permet de ne pas avoir d erreur vu qu on utilise plusieurs coeurs en même temps
#pragma omp critical
                    {
                        if (compteur > max)
                        {
                            max = compteur;
                            strncpy(ville_gagnante, ville_tab[i][0], 128);

                            if (meilleurs_points != NULL) free(meilleurs_points);
                            meilleurs_points = points_trouves;

                            printf("\033[31m[Coeur %d] Nouveau record detecter a %s avec %d %ss !\033[0m\n", omp_get_thread_num(), ville_gagnante, max, critere);
                        }
                        else
                        {
                            free(points_trouves);
                        }
                    }
                }
                else
                {
                    free(points_trouves);
                }
            }
        }
    }

    if (timeout)
    {
        printf("\n\033[33mFin de la recherche apres %.1f secondes. Voici les resultats partiels !\033[0m\n", time_limit_sec);
    }

    printf("\nLa meilleur ville dans la region %s pour se boure est %s avec %d %ss !\n", nom_region, ville_gagnante, max, critere);

    if (meilleurs_points != NULL && max >= 2)
    {
        printf("on vous prepare le tour de ces bars a %s en marchant un minimum\n", ville_gagnante);
        tourBar(supertableau, meilleurs_points, graphPath, gpsPath);
        free(meilleurs_points);
    }
    else
    {
        printf("il y a pas assez de %s dans cette region pour faire un tour.\n", critere);
        if (meilleurs_points != NULL) free(meilleurs_points);
    }
}



void cree_carre_region(const char* gpsPath, double* min_lat, double* max_lat, double* min_lon, double* max_lon)
{
    FILE* f = fopen(gpsPath, "r");
    AssertNew(f);
    unsigned int nb_cord = 0;
    fscanf(f, "%u", &nb_cord);
    printf("Nombre de coordonnees dans le fichier GPS: %u\n", nb_cord); 

    double val1, val2;

    *min_lat = 90.0;  *max_lat = -90.0;
    *min_lon = 180.0; *max_lon = -180.0;

    for (unsigned int i = 0; i < nb_cord; i++)
    {
        fscanf(f, "%lf %lf", &val1, &val2);
        double lon = val1;
        double lat = val2;

        if (lat < *min_lat) *min_lat = lat;
        if (lat > *max_lat) *max_lat = lat;
        if (lon < *min_lon) *min_lon = lon;
        if (lon > *max_lon) *max_lon = lon;
    }
    fclose(f);
    //on ajoute une petite marge pour les ville en bordure de la région 10km environ =0.1 degrés
    *min_lat -= 0.1;
    *max_lat += 0.1;
    *min_lon -= 0.1;
    *max_lon += 0.1;
    printf("le carre fait : \nmin_lat: %f\nmax_lat: %f\nmin_lon: %f\nmax_lon: %f\n", *min_lat, *max_lat, *min_lon, *max_lon);
}

char*** interet_tableau_filtre(double min_lat, double max_lat, double min_lon, double max_lon)
{
    char*** matrice = calloc(MAX_ROWS, sizeof(char**));
    for (int r = 0; r < MAX_ROWS; r++)
    {
        matrice[r] = calloc(MAX_COLS, sizeof(char*));
    }

    FILE* file = fopen("../data/BonusData/poi.csv", "r");
    if (file == NULL)
    {
        fprintf(stderr, "Erreur : fichier introuvable : ../data/BonusData/poi.csv\n");
        free_interet_tableau(matrice);
        return NULL;
    }

    char line[4096];
    int j = 0; 

    while (j < MAX_ROWS && fgets(line, sizeof(line), file))
    {
        char temp_line[4096];
        strncpy(temp_line, line, sizeof(temp_line));

        strtok(temp_line, "\t");
        char* lon_str = strtok(NULL, "\t");
        char* lat_str = strtok(NULL, "\t");

        if (lon_str != NULL && lat_str != NULL)
        {
            double poi_lon = atof(lon_str);
            double poi_lat = atof(lat_str);

            // verification si on est dans le carré de la région
            if (poi_lat >= min_lat && poi_lat <= max_lat && poi_lon >= min_lon && poi_lon <= max_lon)
            {
                line[strcspn(line, "\r\n")] = '\0';
                char* curseur = line;

                for (int i = 0; i < 39; i++)
                {
                    if (curseur == NULL) break;

                    char* prochaine_tab = strchr(curseur, '\t');
                    if (prochaine_tab != NULL) *prochaine_tab = '\0';

                    if (*curseur == '\0') matrice[j][i] = NULL;
                    else matrice[j][i] = strdup(curseur);

                    if (prochaine_tab != NULL) curseur = prochaine_tab + 1;
                    else curseur = NULL;
                }
                j++;
            }
        }
    }

    fclose(file);
    return matrice;
}



void tour_3_criteres(char*** matrice, double lat_v, double lon_v, float rayon, char* c1, char* c2, char* c3, char* graphPath, char* gpsPath)
{
    printf("recherche de tout les point de le rayon %f pour les criteres : %s, %s et %s\n", rayon, c1, c2, c3);
    long long* list1 = point_interet_rayon_critere(matrice, lat_v, lon_v, rayon, c1);
    long long* list2 = point_interet_rayon_critere(matrice, lat_v, lon_v, rayon, c2);
    long long* list3 = point_interet_rayon_critere(matrice, lat_v, lon_v, rayon, c3);

    int t1 = 0, t2 = 0, t3 = 0;
    if (list1) while (list1[t1] != 0) t1++;
    if (list2) while (list2[t2] != 0) t2++;
    if (list3) while (list3[t3] != 0) t3++;

    if (t1 == 0 || t2 == 0 || t3 == 0)
    {
        printf("\033[31m il y a %d %s, %d %s et %d %s dans le rayon %2f donc sa fonctionne pas teste avec un autre critere ou augmenter le rayon\033[0m\n", t1, c1, t2, c2, t3, c3, rayon);
        if (list1) free(list1);
        if (list2) free(list2);
        if (list3) free(list3);
        return;
    }

    double* lat1 = calloc(t1, sizeof(double));
    double* lon1 = calloc(t1, sizeof(double));
    double* lat2 = calloc(t2, sizeof(double));
    double* lon2 = calloc(t2, sizeof(double));
    double* lat3 = calloc(t3, sizeof(double));
    double* lon3 = calloc(t3, sizeof(double));
    char name[256];

    for (int i = 0; i < t1; i++) bonneCo(&lat1[i], &lon1[i], name, matrice, list1[i]);
    for (int i = 0; i < t2; i++) bonneCo(&lat2[i], &lon2[i], name, matrice, list2[i]);
    for (int i = 0; i < t3; i++) bonneCo(&lat3[i], &lon3[i], name, matrice, list3[i]);

    double min_dist = 100000000.0;
    long long best_trio[4] = { 0, 0, 0, 0 };

    printf("calcul des distances pour %d combinaisons pour que nos 3 critere soient le plus rapproches et aussi le plus pres du centre de la ville \n", t1 * t2 * t3);
    int i;
#pragma omp parallel for
    for (i = 0; i < t1; i++)
    {
        for (int j = 0; j < t2; j++)
        {
            for (int k = 0; k < t3; k++)
            {
                double d1 = distance_2_coo(lat_v, lon_v, lat1[i], lon1[i]);
                double d2 = distance_2_coo(lat_v, lon_v, lat2[j], lon2[j]);
                double d3 = distance_2_coo(lat_v, lon_v, lat3[k], lon3[k]);
                double d12 = distance_2_coo(lat1[i], lon1[i], lat2[j], lon2[j]);
                double d23 = distance_2_coo(lat2[j], lon2[j], lat3[k], lon3[k]);
                double d13 = distance_2_coo(lat1[i], lon1[i], lat3[k], lon3[k]);

                double score = d1 + d2 + d3 + d12 + d23 + d13;

                if (score < min_dist)
                {
#pragma omp critical
                    {
                        if (score < min_dist)
                        {
                            min_dist = score;
                            best_trio[0] = list1[i];
                            best_trio[1] = list2[j];
                            best_trio[2] = list3[k];
                        }
                    }
                }
            }
        }
    }

    if (best_trio[0] != 0)
    {
        printf("\033[32mc est trouver\033[0m\n");
        tourBar(matrice, best_trio, graphPath, gpsPath);
    }

    free(lat1); free(lon1);
    free(lat2); free(lon2);
    free(lat3); free(lon3);
    free(list1); free(list2); free(list3);
}
