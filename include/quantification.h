#ifndef QUANTIFICATION_H
#define QUANTIFICATION_H

#include "histogram.h"

/**
 * @file quantification.h
 * @brief Prototypes des fonctions liées à la quantification par popularité et au mapping d'image.
 * @author Binôme (Yassine Oukia - Yassir Douraid)
 * @date 2024-2025
 */

/**
 * @brief Sélectionne les K couleurs les plus fréquentes de l'histogramme (Quantification par popularité).
 * * Cette fonction parcourt l'histogramme pour identifier les K couleurs ayant le plus grand nombre d'occurrences.
 * Elle remplit le tableau @p tab avec les composantes RGB de ces couleurs.
 * * @param h L'histogramme de l'image source (déjà initialisé).
 * @param tab Un tableau d'entiers alloué par l'appelant, de taille K * 3. 
 * Il contiendra les triplets (R, G, B) des couleurs sélectionnées.
 * @param K Le nombre de couleurs à conserver (taille de la palette).
 */
void quantification(histo h, int* tab, int K);


/**
 * @brief Reconstruit une image en utilisant la palette de couleurs quantifiée (Mapping).
 * * Pour chaque pixel de l'image d'entrée @p input, cette fonction cherche la couleur la plus proche
 * (au sens d'une distance mathématique) dans le tableau @p tab et écrit cette couleur dans @p output.
 * * @param input L'image originale en couleurs réelles.
 * @param output L'image de destination (doit être déjà allouée avec les mêmes dimensions que input).
 * @param tab Le tableau contenant les K couleurs de la palette (généré par quantification).
 * @param K Le nombre de couleurs dans la palette.
 */
void mapping(image input, image output, int* tab, int K);

#endif
