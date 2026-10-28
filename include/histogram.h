/**
 * @file histogram.h
 * @brief Définition du type histogramme et prototypes des fonctions de manipulation. 
 * Déclarations des structures et fonctions pour la gestion des listes chaînées de l'histogramme.
 * @author Binôme (Yassine Oukia - Yassir Douraid)
 * @date 2024-2025
 */

#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <stdio.h>
#include <stdlib.h>
#include "image.h"  /* Nécessaire pour le type 'image' */
#include "type_obj.h"

/* ---------------- DEFINITIONS DES TYPES ---------------- */

typedef struct cell * cell; 

typedef cell * histo;

typedef struct histo_iter * histo_iter;

typedef enum {false, true} boolean;

/* ---------------- DEFINITIONS DES PROTOTYPES ---------------- */

/* --- GESTION DES LISTES CHAINEES ET CELLULES --- */

/**
 * @brief Alloue et initialise une nouvelle cellule.
 * * Cette fonction alloue dynamiquement la mémoire pour une nouvelle structure cell.
 * Elle initialise la fréquence à 1 par défaut.
 * * @param B La valeur de la composante bleue à stocker.
 * @param next Pointeur vers la cellule suivante (ou NULL).
 * @return cell Retourne la nouvelle cellule allouée, ou quitte le programme en cas d'erreur.
 */
cell create_cell(int B, cell next);

/**
 * @brief Insère une valeur dans la liste chaînée en maintenant l'ordre croissant.
 * * Si une cellule contenant la valeur @p B existe déjà, sa fréquence est incrémentée.
 * Sinon, une nouvelle cellule est créée et insérée à la position correcte pour 
 * garantir que la liste reste triée par ordre croissant de @p B.
 * * @param head Pointeur vers la tête de la liste actuelle.
 * @param B La valeur de la composante bleue à insérer ou mettre à jour.
 * @return cell Le pointeur vers la tête de la liste (qui peut avoir changé si l'insertion s'est faite en tête).
 */
cell insert_cell(cell head, int B);

/**
 * @brief Supprime l'intégralité d'une liste chaînée et libère la mémoire.
 * * Parcourt la liste en libérant chaque cellule une par une via free().
 * * @param list Pointeur vers la tête de la liste à supprimer.
 * @return cell Retourne toujours NULL pour permettre une mise à jour sécurisée.
 */
cell delete_list(cell list);

/* --- GESTION DE L'HISTOGRAMME --- */

/**
 * @brief Alloue et initialise un histogramme vide.
 * * Cette fonction alloue un tableau contigu de 256*256 pointeurs de cellules.
 * L'utilisation de calloc garantit que toutes les cases sont initialisées à NULL.
 * * @return histo Le pointeur vers le tableau alloué, ou quitte en cas d'erreur.
 */
histo create_histo();

/**
 * @brief Remplit l'histogramme à partir des données d'une image.
 * * Parcourt l'image @p img pixel par pixel. Pour chaque pixel (R, G, B),
 * calcule l'index correspondant dans le tableau @p h et insère la composante B
 * dans la liste chaînée appropriée.
 * * @param h L'histogramme (déjà alloué) à remplir.
 * @param img L'image source (doit être chargée en mémoire).
 */
void init_histo(histo h, image img);

/**
 * @brief Libère toute la mémoire associée à l'histogramme.
 * * Parcourt chaque case du tableau @p h pour libérer les listes chaînées
 * via delete_list(), puis libère le tableau lui-même.
 * * @param h L'histogramme à détruire. Si @p h est NULL, la fonction ne fait rien.
 */
void delete_histo(histo h);

/**
 * @brief Récupère la fréquence (nombre d'occurrences) d'une couleur précise.
 * * @param h L'histogramme à interroger.
 * @param R Composante Rouge (0-255).
 * @param G Composante Verte (0-255).
 * @param B Composante Bleue (0-255).
 * @return int Le nombre de pixels de cette couleur présents dans l'image (0 si la couleur n'existe pas).
 */
int give_freq_histo(histo h, int R, int G, int B);

/* --- GESTION DE L'ITERATEUR --- */

/**
 * @brief Crée et initialise un itérateur sur l'histogramme.
 * * L'itérateur est positionné sur la première couleur (R, G, B) existante 
 * (la première cellule non vide du tableau).
 * Arrête le programme si l'histogramme est totalement vide.
 * * @param h L'histogramme à parcourir.
 * @return histo_iter Un nouvel itérateur alloué dynamiquement.
 */
histo_iter create_histo_iter(histo h);

/**
 * @brief Réinitialise l'itérateur au début de l'histogramme.
 * * Repositionne l'itérateur sur la première couleur valide trouvée lors de la création.
 * * @param h_i L'itérateur à réinitialiser.
 * @param h L'histogramme associé.
 */
void start_histo_iter(histo_iter h_i, histo h);

/**
 * @brief Avance l'itérateur à la couleur suivante.
 * * Parcourt la liste chaînée courante (B). Si la fin de la liste est atteinte,
 * cherche la prochaine entrée (R, G) non vide dans le tableau.
 * * @param h_i L'itérateur à faire avancer.
 * @param h L'histogramme associé.
 * @return boolean 'true' si une nouvelle couleur a été trouvée, 'false' si la fin de l'histogramme est atteinte.
 */
boolean next_histo_iter(histo_iter h_i, histo h);

/**
 * @brief Récupère la couleur courante pointée par l'itérateur.
 * * @param h_i L'itérateur (doit être valide).
 * @param rgb Un tableau d'entiers de taille 3 (alloué par l'appelant) qui sera rempli :
 * rgb[0] = Rouge, rgb[1] = Vert, rgb[2] = Bleu.
 */
void give_color_histo_iter(histo_iter h_i, int *rgb);

/**
 * @brief Libère la mémoire allouée pour l'itérateur.
 * * @param h_i L'itérateur à supprimer.
 */
void delete_histo_iter(histo_iter h_i);

#endif
