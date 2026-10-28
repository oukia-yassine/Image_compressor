#include "histogram.h"
#include "image.h"
#include "type_obj.h"
#include <assert.h>


/**
 * @struct cell
 * @brief Représente un élément (noeud) d'une liste chaînée pour une couleur donnée.
 * * Cette structure est utilisée pour gérer les collisions dans l'histogramme ou 
 * pour stocker les composantes de couleur (Bleu) associées à un couple (Rouge, Vert).
 */
struct cell {
    unsigned char B;    /**< La valeur de la composante Bleue (0-255). */
    int freq;           /**< Le nombre d'occurrences (fréquence) de cette couleur. */
    cell next;          /**< Pointeur vers la cellule suivante dans la liste. */
};


struct histo_iter {
    int R, G;
    cell current;
};


cell create_cell(int B, cell next) {

    cell nouvelle_cell = (cell)malloc(sizeof(struct cell));

    if (nouvelle_cell == NULL) {
        fprintf(stderr, "Erreur : allocation de la mémoire échouée\n");
        exit(EXIT_FAILURE);
    }

    nouvelle_cell->B = (unsigned char)B;
    nouvelle_cell->freq = 1;
    nouvelle_cell->next = next;

    return nouvelle_cell;
}


cell insert_cell(cell head, int B) {
    cell cel;
    cell current;

    /* If the list is empty */
    if (head == NULL || B < head->B) {
        return create_cell(B, head);
    }

    if (head->B == B) {
        head->freq += 1;
        return head;
    }

    /* Cas general */

    current  = head;

    while (current->next != NULL && current->next->B < B) {
        current = current->next;
    }

    if (current->next != NULL && current->next->B == B) {
        current->next->freq++;
    } else {
        cel = create_cell(B, current->next);
        current->next = cel;
    }

    return head;
}

cell delete_list(cell list) {
    cell temp;

    while (list != NULL) {
        temp = list->next;
        free(list);
        list = temp;
    }
    return NULL;
}


histo create_histo() {
    /* Allocation dynamique du tableau 2D 256*256 */
    histo nouveau_histo = (histo)calloc(256*256, sizeof(cell));

    if (nouveau_histo == NULL) {
        fprintf(stderr, "Erreur: allocation échouée!\n");
        exit(EXIT_FAILURE);
    }

    return nouveau_histo;
}


void init_histo(histo h, image img) {
   int R;
   int G;
   int B;
   int * rgb;
   int position;

   image_debut(img);

   do {
       rgb = image_lire_pixel(img);

       R = rgb[0];
       G = rgb[1];
       B = rgb[2];

       position = 256 * R + G;

       h[position] = insert_cell(h[position], B);
    } while (image_pixel_suivant(img) == vrai);
} 

void delete_histo(histo h) {
    int i; 
    int taille = 256 * 256;

    if (h == NULL) {
        return;
    }

    for (i = 0 ; i < taille ; i++) {  
        h[i] = delete_list(h[i]);
    }
    free(h);
}


int give_freq_histo(histo h, int R, int G, int B) {
    int position;
    cell current;

    position = 256 * R + G;
     
    current = h[position];  /* On récupère ici la tête de la liste */

    while (current != NULL) {
        if (current->B == B) {
            return current->freq;
        }
        current = current->next;
    }

    return 0;
}


histo_iter create_histo_iter(histo h){
    int r, g;
    int position;
    int found;

    histo_iter nouveau_histo_iter;

    nouveau_histo_iter = (histo_iter)malloc(sizeof(struct histo_iter));

    if (nouveau_histo_iter == NULL) {
        exit(1);
    }

    found = 0;

    for (r = 0; r < 256 && !found; r++) {
        for (g = 0; g < 256; g++) {
            position = r * 256 + g;
            if (h[position] != NULL) {
                nouveau_histo_iter->R = r;
                nouveau_histo_iter->G = g;
                nouveau_histo_iter->current = h[position];
                found = 1;
                break;
            }
        }
    }

    assert(found && "Erreur : L'histogramme est vide !!!");
                
    return nouveau_histo_iter;
}


void start_histo_iter(histo_iter h_i, histo h) { 
    int r, g;
    int found;
    int position;

    found = 0;

    for (r = 0; r < 256 && !found; r++) {
        for (g = 0; g < 256; g++) {
            position = r * 256 + g;
            if (h[position] != NULL) {
                h_i->R = r;
                h_i->G = g;
                h_i->current = h[position];
                found = 1;
                break;
            }
        }
    }
}


boolean next_histo_iter(histo_iter h_i, histo h) {
    int start_G;
    int R;
    int G;
    int index;

    /* on avance dans la liste chainée (B) */
    if (h_i->current != NULL && h_i->current->next != NULL) {
        h_i->current = h_i->current->next;
        return true;
    }

    /*On ne peut pas avancer dans la liste de B*/
    /*donc, on cherche la nouvelle liste à partir des coordonnés (G+1,R) */
    for (R = h_i->R; R < 256; R++) {
        
        /* si on est sur la même ligne R, on commence à G+1. 
           Sinon (nouvelle ligne), on commence à 0. */
        start_G = (R == h_i->R) ? h_i->G + 1 : 0;

        for (G = start_G; G < 256; G++) {
            
            index = R * 256 + G;
            
            if (h[index] != NULL) {
                h_i->R = R;
                h_i->G = G;
                h_i->current = h[index];
                return true;
            }
        }
    }
    return false;
}


void give_color_histo_iter(histo_iter h_i, int * tab){
    tab[0] = h_i->R;
    tab[1] = h_i->G;
    tab[2] = (int)h_i->current->B;
}


void delete_histo_iter(histo_iter h_i){
    if (h_i != NULL) {
        free(h_i);
    }
}
