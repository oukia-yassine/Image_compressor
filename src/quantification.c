#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#include "quantification.h"
#include "histogram.h"
#include "type_obj.h"
#include "image.h"


void quantification(histo h, int * tab, int K) {
    int * freqs;
    int color[3];  
    int color2[3]; 
    int count;
    int f_actuelle, frequence_actuelle2;
    int j;
    histo_iter it;
    int est_valide;
    
    /* pour savoir si l'itérateur est encore valide */
    est_valide = 1; 
    count = 0;

    freqs = (int*)malloc(K * sizeof(int));
    it = create_histo_iter(h);

    /* --- PHASE 1 : Remplissage initial --- */

    while (est_valide && count < K) {
        give_color_histo_iter(it, color);
        f_actuelle = give_freq_histo(h, color[0], color[1], color[2]);

        /* Tri par insertion */
        j = count - 1;
        while (j >= 0 && freqs[j] < f_actuelle) {
            freqs[j + 1] = freqs[j];
            tab[(j + 1) * 3 + 0] = tab[j * 3 + 0];
            tab[(j + 1) * 3 + 1] = tab[j * 3 + 1];
            tab[(j + 1) * 3 + 2] = tab[j * 3 + 2];
            j--;
        }

        freqs[j + 1] = f_actuelle;
        tab[(j + 1) * 3 + 0] = color[0];
        tab[(j + 1) * 3 + 1] = color[1];
        tab[(j + 1) * 3 + 2] = color[2];

        count++;

        if (count < K) {
            est_valide = next_histo_iter(it, h);
        }
    }

    /* Si on a fini la boucle précédente parce que count == K, 
       il faut avancer d'un cran pour passer à la suite. */
    if (est_valide && count == K) {
        est_valide = next_histo_iter(it, h);
    }

    /* --- PHASE 2 --- */

    while (est_valide) {
        give_color_histo_iter(it, color2);
        
        frequence_actuelle2 = give_freq_histo(h, color2[0], color2[1], color2[2]);

        if(frequence_actuelle2 > freqs[K-1]){
            j = K-2;
            while(j >= 0 && freqs[j] < frequence_actuelle2){
                freqs[j+1] = freqs[j];
                tab[(j + 1) * 3 + 0] = tab[j * 3 + 0];
                tab[(j + 1) * 3 + 1] = tab[j * 3 + 1];
                tab[(j + 1) * 3 + 2] = tab[j * 3 + 2];
                j--;
            }
            freqs[j+1] = frequence_actuelle2;
            
            tab[(j + 1) * 3 + 0] = color2[0];
            tab[(j + 1) * 3 + 1] = color2[1];
            tab[(j + 1) * 3 + 2] = color2[2];
        }
        
        est_valide = next_histo_iter(it, h);
    }
    
    free(freqs);
    delete_histo_iter(it);
}
    
int distance(int R1, int G1, int B1, int R2, int G2, int B2) {
    int r = R1 - R2;
    int g = G1 - G2;
    int b = B1 - B2;

    return (r * r) + (g * g) + (b * b);
}


void mapping(image input, image output, int * tab, int K) { 
    int i;
    int indice_pixel_sub;
    int * tab_rbg;
    int r, b, g;
    int r_tab, g_tab, b_tab;
    int distance_min, distance_actuelle;
    
    image_debut(input);
    image_debut(output);

    do {

        tab_rbg = image_lire_pixel(input);
        r = tab_rbg[0];
        g = tab_rbg[1];
        b = tab_rbg[2];
        
        distance_min = INT_MAX;

        for (i = 0 ; i < K ; i++) {
            r_tab = tab[i * 3];
            g_tab = tab[i * 3 + 1];
            b_tab = tab[i * 3 + 2];
            
            distance_actuelle = distance(r, g, b, r_tab, g_tab, b_tab);

            if (distance_actuelle < distance_min) {
                distance_min = distance_actuelle;
                indice_pixel_sub = i;
            }
        }
        image_ecrire_pixel(output, &tab[indice_pixel_sub * 3]);

    } while (image_pixel_suivant(input) && image_pixel_suivant(output));
}

/* int main(int argc, char * argv[]) { */
/*     image input, output; */
/*     int K; */
/*     int * tab; */
/*     histo h; */

/*     if (argc != 3) { */
/*         fprintf(stderr, "Usage : %s <input.ppm> <K>\n", argv[0]); */
/*         return 1; */
/*     } */

/*     K = atoi(argv[2]); */
/*     if (K <= 0) { */
/*         fprintf(stderr, "Erreur : K doit etre un entier positif\n"); */
/*         return 1; */
/*     } */

/*     tab = (int *)malloc(K * 3 * sizeof(int)); */
/*     if (tab == NULL) return 1; */

/*     input = FAIRE_image(); */
/*     if (image_charger(input, argv[1]) < 0) { */
/*         fprintf(stderr, "Erreur chargement image\n"); */
/*         free(tab); */
/*         return 1; */
/*     } */

/*     output = FAIRE_image(); */
/*     image_initialize(output, */
/*                      image_give_dim(input), */
/*                      image_give_largeur(input), */
/*                      image_give_hauteur(input)); */

/*     h = create_histo(); */
/*     init_histo(h, input); */

/*     quantification(h, tab, K); */
/*     mapping(input, output, tab, K); */

/*     if (image_sauvegarder(output, "output.ppm") < 0) { */
/*         fprintf(stderr, "Erreur sauvegarde image sortie\n"); */
/*         return 1; */
/*     } */

/*     printf("Termine ! Resultat dans output.ppm\n"); */

/*     free(tab); */
/*     delete_histo(h); */
/*     DEFAIRE_image(input); */
/*     DEFAIRE_image(output); */

/*     return 0; */
/* } */
