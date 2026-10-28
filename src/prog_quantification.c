#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#include "quantification.h"
#include "histogram.h"
#include "type_obj.h"
#include "image.h"


int main(int argc, char * argv[]) {
    image input, output;
    int K;
    int * tab;
    histo h;

    if (argc != 3) {
        fprintf(stderr, "Usage : %s <input.ppm> <K>\n", argv[0]);
        return 1;
    }

    K = atoi(argv[2]);
    if (K <= 0) {
        fprintf(stderr, "Erreur : K doit etre un entier positif\n");
        return 1;
    }

    tab = (int *)malloc(K * 3 * sizeof(int));
    if (tab == NULL) return 1;

    input = FAIRE_image();
    if (image_charger(input, argv[1]) < 0) {
        fprintf(stderr, "Erreur chargement image\n");
        free(tab);
        return 1;
    }

    output = FAIRE_image();
    image_initialize(output,
                     image_give_dim(input),
                     image_give_largeur(input),
                     image_give_hauteur(input));

    h = create_histo();
    init_histo(h, input);

    quantification(h, tab, K);
    mapping(input, output, tab, K);

    if (image_sauvegarder(output, "output.ppm") < 0) {
        fprintf(stderr, "Erreur sauvegarde image sortie\n");
        return 1;
    }

    printf("Termine ! Resultat dans output.ppm\n");

    free(tab);
    delete_histo(h);
    DEFAIRE_image(input);
    DEFAIRE_image(output);

    return 0;
}
