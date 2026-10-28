#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#include "quantification.h"
#include "histogram.h"
#include "type_obj.h"
#include "image.h"

struct cell {
    unsigned char B;    /**< La valeur de la composante Bleue (0-255). */
    int freq;           /**< Le nombre d'occurrences (fréquence) de cette couleur. */
    cell next;          /**< Pointeur vers la cellule suivante dans la liste. */
};


struct histo_iter {
    int R, G;
    cell current;
};

int main(int argc, char * argv[]) {
    image img = FAIRE_image();
    histo h;
    
    histo_iter it;
    int tab_couleurs[3];
    int freq;
    int lum;

    if (argc != 2) {
        printf("Usage : %s <image.ppm>\n", argv[0]);
        return 1;
    }
    
    if (image_charger(img, argv[1]) < 0) {
            fprintf(stderr, "Erreur de chargement de l'image\n");
            return 1;
    }
    
    h = create_histo();
    init_histo(h, img);

    it = create_histo_iter(h);
    
    printf("R G B Lum Freq\n");
    
    do {
        give_color_histo_iter(it, tab_couleurs);
        
        freq = it->current->freq;
        
        lum = (tab_couleurs[0] + tab_couleurs[1] + tab_couleurs[2]) / 3;
        
        printf("%d %d %d %d %d\n", tab_couleurs[0], tab_couleurs[1], tab_couleurs[2], lum, freq);
        

    } while (next_histo_iter(it, h));


    delete_histo_iter(it);
    delete_histo(h);
    DEFAIRE_image(img);

    return 0;
}
