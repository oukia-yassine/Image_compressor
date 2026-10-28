/**
 * @file test_projet.c
 * @brief Tests unitaires pour histogramme et quantification
 * @author 
 * @version 1.0.0
 * @date 2025
 */

#include <stdlib.h>
#include <stdio.h>
#include "minunit.h"

#include "image.h"
#include "histogram.h"
#include "quantification.h"

/* Remplit une image 4x1 avec des couleurs connues pour prédire le résultat */
void remplir_image_mock(image img) {
    /* Nous allons mettre :
       - 2 pixels NOIRS (0,0,0) -> Fréquence 2
       - 1 pixel ROUGE (255,0,0) -> Fréquence 1
       - 1 pixel VERT (0,255,0)  -> Fréquence 1
    */
    int noir[3] = {0, 0, 0};
    int rouge[3] = {255, 0, 0};
    int vert[3]  = {0, 255, 0};

    image_debut(img);
    image_ecrire_pixel(img, noir);  /* Pixel 1 */
    image_pixel_suivant(img);
    image_ecrire_pixel(img, noir);  /* Pixel 2 */
    image_pixel_suivant(img);
    image_ecrire_pixel(img, rouge); /* Pixel 3 */
    image_pixel_suivant(img);
    image_ecrire_pixel(img, vert);  /* Pixel 4 */
}

void test_setup(void) {
}

void test_teardown(void) {
}

/* --- TESTS UNITAIRES --- */

MU_TEST(test_histogram_init) {
    image img;
    histo h;
    int freq_noir, freq_rouge, freq_bleu;

    printf("Testing histogram counting (init_histo & give_freq)...\n");

    /* 1. Préparation */
    img = FAIRE_image();
    image_initialize(img, 3, 4, 1); /* Image 4 pixels de large, 1 de haut, dim 3 */
    remplir_image_mock(img);

    /* 2. Exécution */
    h = create_histo();
    init_histo(h, img);

    /* 3. Vérifications */
    freq_noir = give_freq_histo(h, 0, 0, 0);
    freq_rouge = give_freq_histo(h, 255, 0, 0);
    freq_bleu = give_freq_histo(h, 0, 0, 255); /* Couleur absente */

    mu_assert_int_eq(2, freq_noir);
    mu_assert_int_eq(1, freq_rouge);
    mu_assert_int_eq(0, freq_bleu);

    /* 4. Nettoyage */
    delete_histo(h);
    DEFAIRE_image(img);
    printf(" -- passed\n");
}

MU_TEST(test_iterator_full_scan) {
    image img;
    histo h;
    histo_iter it;
    int compteur_couleurs = 0;
    int somme_frequences = 0;
    int rgb[3];

    printf("Testing iterator traversal (create_iter & next)...\n");

    /* 1. Préparation */
    img = FAIRE_image();
    image_initialize(img, 3, 4, 1);
    remplir_image_mock(img);
    h = create_histo();
    init_histo(h, img);

    /* 2. Exécution */
    it = create_histo_iter(h);

    do {
        give_color_histo_iter(it, rgb);
        /* On additionne les fréquences trouvées */
        somme_frequences += give_freq_histo(h, rgb[0], rgb[1], rgb[2]);
        compteur_couleurs++;
    } while (next_histo_iter(it, h));

    /* 3. Vérifications */
    /* Il y a 3 couleurs uniques (Noir, Rouge, Vert) */
    mu_assert_int_eq(3, compteur_couleurs);
    /* La somme des pixels doit faire 4 */
    mu_assert_int_eq(4, somme_frequences);

    /* 4. Nettoyage */
    delete_histo_iter(it);
    delete_histo(h);
    DEFAIRE_image(img);
    printf(" -- passed\n");
}

MU_TEST(test_quantification_logic) {
    image img;
    histo h;
    int K = 2;
    int * tab;
    int is_not_black;

    printf("Testing quantification sort (Top K colors)...\n");

    /* 1. Préparation */
    img = FAIRE_image();
    image_initialize(img, 3, 4, 1);
    remplir_image_mock(img); /* Noir(2), Rouge(1), Vert(1) */
    
    h = create_histo();
    init_histo(h, img);
    
    tab = (int *)malloc(K * 3 * sizeof(int));

    /* 2. Exécution */
    quantification(h, tab, K);

    /* 3. Vérifications */
    /* Le tableau doit être trié par fréquence décroissante.
       La 1ère couleur DOIT être le Noir (0,0,0) car fréquence = 2 */
    
    /* Vérif Couleur 1 (Index 0, 1, 2) */
    mu_assert_int_eq(0, tab[0]);
    mu_assert_int_eq(0, tab[1]);
    mu_assert_int_eq(0, tab[2]);

    /* La 2ème couleur (Index 3, 4, 5) doit être Rouge OU Vert (fréquence = 1).
       L'important est qu'elle ne soit pas noire (0,0,0) */
    is_not_black = (tab[3] != 0 || tab[4] != 0 || tab[5] != 0);
    mu_check(is_not_black);

    /* 4. Nettoyage */
    free(tab);
    delete_histo(h);
    DEFAIRE_image(img);
    printf(" -- passed\n");
}

/* --- SUITE DE TESTS --- */

MU_TEST_SUITE(test_suite) {
    MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

    MU_RUN_TEST(test_histogram_init);
    MU_RUN_TEST(test_iterator_full_scan);
    MU_RUN_TEST(test_quantification_logic);
}

int main(int argc, char *argv[]) {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
