//
// Created by David PROSPÉRIN on 12/11/2025.
//

#include "outils.h"
#include <cassert>

/**
 * @brief Applique un seuillage à un niveau sur une image.
 *
 * Cette fonction transforme une image en noir et blanc en remplaçant les pixels
 * dont le niveau de gris est inférieur au seuil @p s par 0 (noir), et tous les
 * autres pixels par 255 (blanc).
 *
 * @param image Pointeur vers la structure de l'image à traiter.
 *              Les dimensions de l'image doivent être inférieures ou égales à TMAX.
 * @param s Seuil de seuillage. Doit être compris entre 0 et 255 inclus.
 *
 * @pre image != nullptr
 * @pre image->w <= TMAX
 * @pre image->h <= TMAX
 * @pre 0 <= s <= 255
 *
 * @note Cette fonction modifie directement les pixels de l'image passée en paramètre.
 */
void seuillage(t_Image *image, const unsigned int s) {
    const int w = image->w;
    const int h = image->h;

    assert(h <= TMAX && "La hauteur de image doit être <= 800");
    assert(w <= TMAX && "La largeur de image doit être <= 800");
    assert(s <= 255  && "La valeur du seuil doit respecter : 0 <= s <= 255");

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            image->im[i][j] = image->im[i][j] < s ? 0 : 255;
        }
    }
}

/**
 * @brief Effectue une dilatation morphologique sur une image.
 *
 * La dilatation est une opération de traitement d'image binaire qui "agrandit"
 * les objets présents dans l'image. Pour chaque position de l'élément structurant
 * déplacé sur l'image d'entrée, si une partie de l'élément chevauche un pixel actif
 * (valeur non nulle) de l'image originale, le pixel correspondant au centre de
 * l'élément structurant dans l'image de sortie est mis à la valeur @p couleur_remplissage.
 *
 * Cette opération permet notamment de :
 * - combler de petits trous dans les objets,
 * - élargir les formes,
 * - connecter des éléments proches.
 *
 * @param image_entree Pointeur vers l'image d'entrée (const) ne devant pas être modifiée.
 * @param image_sortie Pointeur vers l'image de sortie qui recevra le résultat.
 *                     Doit avoir les mêmes dimensions que @p image_entree.
 * @param element Pointeur vers l'élément structurant utilisé pour la dilatation.
 *                Doit être carré et de taille impaire.
 * @param couleur_remplissage Valeur du pixel utilisé pour "allumer" les positions
 *                            correspondantes dans l'image de sortie. Doit être
 *                            comprise entre 0 et 255 inclus.
 *
 * @pre image_entree != nullptr
 * @pre image_sortie != nullptr
 * @pre element != nullptr
 * @pre image_sortie->w == image_entree->w
 * @pre image_sortie->h == image_entree->h
 * @pre image_sortie->w <= TMAX
 * @pre image_sortie->h <= TMAX
 * @pre element->w == element->h
 * @pre element->w % 2 == 1
 * @pre 0 <= couleur_remplissage <= 255
 *
 * @note L'image d'entrée n'est pas modifiée. L'image de sortie doit être préalablement
 *       allouée et de même taille que l'image d'entrée.
 */
void dilatation(const t_Image *image_entree, t_Image *image_sortie, const t_ElementStructurant *element, const unsigned int couleur_remplissage) {
    const int im_w = image_entree->w;
    const int im_h = image_entree->h;

    const int el_w = element->w;
    const int el_h = element->h;

    const int image_sortie_w = image_sortie->w;
    const int image_sortie_h = image_sortie->h;

    assert(image_sortie_w == im_w && "La largeur de l'image d'entrée et de sortie doivent être égale.");
    assert(image_sortie_h == im_h && "La hauteur de l'image d'entrée et de sortie doivent être égale.");
    assert(image_sortie_h <= TMAX && "Les hauteurs des images doivent être <= 800");
    assert(image_sortie_w <= TMAX && "Les largeurs des images doivent être <= 800");
    assert(el_h == el_w && "L'élément structurant doit être carrée.");
    assert(el_h % 2 == 1 && "La taille de l'élément structurant doit être impaire.");
    assert(couleur_remplissage <= 255  && "La valeur de la couleur de remplissage doit respecter : 0 <= s <= 255");


    for (int i = 0; i < im_h; i++) {
        for (int j = 0; j < im_w; j++) {
            if (i == 0 && j == 0) {
                // Coin haut gauche
                if ((image_entree->im[i][j] == element->valeurs[1][1] && image_entree->im[i][j] == 0) ||
                   (image_entree->im[i][j+1] == element->valeurs[1][2] && image_entree->im[i][j+1] == 0) ||
                   (image_entree->im[i+1][j] == element->valeurs[2][1] && image_entree->im[i+1][j] == 0) ||
                   (image_entree->im[i+1][j+1] == element->valeurs[2][2] && image_entree->im[i+1][j+1] == 0)) {
                        image_sortie->im[i][j] = couleur_remplissage;
                   }

            } else if (i == 0 && j == (im_w - 1)) {
                // Coin haut droit
                if ((image_entree->im[i][j] == element->valeurs[1][1] && image_entree->im[i][j] == 0) ||
                   (image_entree->im[i+1][j] == element->valeurs[2][1] && image_entree->im[i+1][j] == 0) ||
                   (image_entree->im[i][j-1] == element->valeurs[1][0] && image_entree->im[i][j-1] == 0) ||
                   (image_entree->im[i+1][j-1] == element->valeurs[2][0] && image_entree->im[i+1][j-1] == 0)) {
                        image_sortie->im[i][j] = couleur_remplissage;
                   }

            } else if (i == (im_h - 1) && j == 0) {
                // Coin bas gauche
                if ((image_entree->im[i][j] == element->valeurs[1][1] && image_entree->im[i][j] == 0) ||
                   (image_entree->im[i][j+1] == element->valeurs[1][2] && image_entree->im[i][j+1] == 0) ||
                   (image_entree->im[i-1][j] == element->valeurs[0][1] && image_entree->im[i-1][j] == 0) ||
                   (image_entree->im[i-1][j+1] == element->valeurs[0][2] && image_entree->im[i-1][j+1] == 0)) {
                        image_sortie->im[i][j] = couleur_remplissage;
                   }
            } else if (i == (im_h - 1) && j == (im_w - 1)) {
                // Coin bas droit
                if ((image_entree->im[i][j] == element->valeurs[1][1] && image_entree->im[i][j] == 0) ||
                   (image_entree->im[i][j-1] == element->valeurs[1][0] && image_entree->im[i][j-1] == 0) ||
                   (image_entree->im[i-1][j] == element->valeurs[0][1] && image_entree->im[i-1][j] == 0) ||
                   (image_entree->im[i-1][j-1] == element->valeurs[0][0] && image_entree->im[i-1][j-1] == 0)) {
                        image_sortie->im[i][j] = couleur_remplissage;
                   }
            }
            else if (i == 0) {
                // Ligne haut
                if ((image_entree->im[i][j] == element->valeurs[1][1] && image_entree->im[i][j] == 0) ||
                    (image_entree->im[i][j+1] == element->valeurs[1][2] && image_entree->im[i][j+1] == 0) ||
                    (image_entree->im[i+1][j] == element->valeurs[2][1] && image_entree->im[i+1][j] == 0) ||
                    (image_entree->im[i+1][j+1] == element->valeurs[2][2] && image_entree->im[i+1][j+1] == 0) ||
                    (image_entree->im[i][j-1] == element->valeurs[1][0] && image_entree->im[i][j-1] == 0) ||
                    (image_entree->im[i+1][j-1] == element->valeurs[2][0] && image_entree->im[i+1][j-1] == 0)) {
                        image_sortie->im[i][j] = couleur_remplissage;
                    }
            } else if (i == (im_h - 1)) {
                // Ligne bas
                if ((image_entree->im[i][j] == element->valeurs[1][1] && image_entree->im[i][j] == 0) ||
                    (image_entree->im[i][j+1] == element->valeurs[1][2] && image_entree->im[i][j+1] == 0) ||
                    (image_entree->im[i][j-1] == element->valeurs[1][0] && image_entree->im[i][j-1] == 0) ||
                    (image_entree->im[i-1][j] == element->valeurs[0][1] && image_entree->im[i-1][j] == 0) ||
                    (image_entree->im[i-1][j-1] == element->valeurs[0][0] && image_entree->im[i-1][j-1] == 0) ||
                    (image_entree->im[i-1][j+1] == element->valeurs[0][2] && image_entree->im[i-1][j+1] == 0)) {
                        image_sortie->im[i][j] = couleur_remplissage;
                    }

            } else if (j == 0) {
                // Ligne gauche
                if ((image_entree->im[i][j] == element->valeurs[1][1] && image_entree->im[i][j] == 0) ||
                    (image_entree->im[i][j+1] == element->valeurs[1][2] && image_entree->im[i][j+1] == 0) ||
                    (image_entree->im[i+1][j] == element->valeurs[2][1] && image_entree->im[i+1][j] == 0) ||
                    (image_entree->im[i+1][j+1] == element->valeurs[2][2] && image_entree->im[i+1][j+1] == 0) ||
                    (image_entree->im[i-1][j] == element->valeurs[0][1] && image_entree->im[i-1][j] == 0) ||
                    (image_entree->im[i-1][j+1] == element->valeurs[0][2] && image_entree->im[i-1][j+1] == 0)) {
                        image_sortie->im[i][j] = couleur_remplissage;
                    }
            } else if (j == (im_w - 1)) {
                //Ligne droit
                if ((image_entree->im[i][j] == element->valeurs[1][1] && image_entree->im[i][j] == 0) ||
                    (image_entree->im[i+1][j] == element->valeurs[2][1] && image_entree->im[i+1][j] == 0) ||
                    (image_entree->im[i][j-1] == element->valeurs[1][0] && image_entree->im[i][j-1] == 0) ||
                    (image_entree->im[i-1][j] == element->valeurs[0][1] && image_entree->im[i-1][j] == 0) ||
                    (image_entree->im[i-1][j-1] == element->valeurs[0][0] && image_entree->im[i-1][j-1] == 0) ||
                    (image_entree->im[i+1][j-1] == element->valeurs[2][0] && image_entree->im[i+1][j-1] == 0)) {
                        image_sortie->im[i][j] = couleur_remplissage;
                    }
            } else {
                // Dans l'image
                if ((image_entree->im[i][j] == element->valeurs[1][1] && image_entree->im[i][j] == 0) ||
                    (image_entree->im[i][j+1] == element->valeurs[1][2] && image_entree->im[i][j+1] == 0) ||
                    (image_entree->im[i+1][j] == element->valeurs[2][1] && image_entree->im[i+1][j] == 0) ||
                    (image_entree->im[i+1][j+1] == element->valeurs[2][2] && image_entree->im[i+1][j+1] == 0) ||
                    (image_entree->im[i][j-1] == element->valeurs[1][0] && image_entree->im[i][j-1] == 0) ||
                    (image_entree->im[i-1][j] == element->valeurs[0][1] && image_entree->im[i-1][j] == 0) ||
                    (image_entree->im[i-1][j-1] == element->valeurs[0][0] && image_entree->im[i-1][j-1] == 0) ||
                    (image_entree->im[i+1][j-1] == element->valeurs[2][0] && image_entree->im[i+1][j-1] == 0) ||
                    (image_entree->im[i-1][j+1] == element->valeurs[0][2] && image_entree->im[i-1][j+1] == 0)) {
                            image_sortie->im[i][j] = couleur_remplissage;
                    }
            }
        }
    }
}

/**
 * @brief Crée et initialise dynamiquement une image.
 *
 * Cette fonction alloue une nouvelle structure `t_Image` sur le tas et initialise
 * tous ses pixels à la valeur de fond spécifiée. L'image créée est de dimensions
 * @p h x @p w et peut être utilisée pour des opérations de traitement d'image.
 *
 * @param h Hauteur de l'image à créer. Doit être inférieure ou égale à TMAX.
 * @param w Largeur de l'image à créer. Doit être inférieure ou égale à TMAX.
 * @param couleur_fond Valeur du pixel de fond pour initialiser l'image.
 *                     Doit être comprise entre 0 et 255 inclus.
 *
 * @return Pointeur vers l'image nouvellement créée et initialisée.
 *         L'image est allouée dynamiquement et doit être libérée avec `delete`
 *         pour éviter les fuites mémoire.
 *
 * @pre h <= TMAX
 * @pre w <= TMAX
 * @pre 0 <= couleur_fond <= 255
 *
 * @note Tous les pixels de l'image sont initialisés à @p couleur_fond.
 * @note La mémoire pour la structure et le tableau interne est allouée sur le tas.
 */
t_Image * createImage(const unsigned int h, const unsigned int w, const unsigned int couleur_fond) {
    assert(h <= TMAX && "Les hauteurs des images doivent être <= 800");
    assert(w <= TMAX && "Les largeurs des images doivent être <= 800");
    assert(couleur_fond <= 255  && "La valeur de la couleur de remplissage doit respecter : 0 <= s <= 255");

    auto image = new t_Image();

    image->h = h;
    image->w = w;

    for (int i = 0; i < image->h; i++)
        for (int j = 0; j < image->w; j++)
            image->im[i][j] = couleur_fond;

    return image;
}

/**
 * @brief Crée et initialise dynamiquement un élément structurant pour les opérations morphologiques.
 *
 * Cette fonction alloue un nouvel élément structurant de dimensions @p h × @p w,
 * initialise toutes ses valeurs à @p couleur_fond, et configure son centre logique
 * en fonction des paramètres @p centreX et @p centreY.
 *
 * L’élément structurant est utilisé dans des opérations telles que la dilatation
 * ou l’érosion. Le centre (centreX, centreY) représente la position qui sera
 * alignée avec le pixel traité lors du parcours de l’image.
 *
 * @param h Hauteur de l’élément structurant. Doit être inférieure ou égale à TMAX.
 * @param w Largeur de l’élément structurant. Doit être inférieure ou égale à TMAX.
 * @param centreX Position du centre sur l’axe horizontal (0 <= centreX < w).
 * @param centreY Position du centre sur l’axe vertical   (0 <= centreY < h).
 * @param couleur_fond Valeur utilisée pour initialiser l’ensemble des cellules
 *                     de l’élément structurant. Doit être comprise entre 0 et 255.
 *
 * @return Pointeur vers l’élément structurant nouvellement créé.
 *         L’objet est alloué dynamiquement et doit être libéré avec `delete`
 *         pour éviter les fuites mémoire.
 *
 * @pre h <= TMAX
 * @pre w <= TMAX
 * @pre 0 <= couleur_fond <= 255
 * @pre centreX < w
 * @pre centreY < h
 *
 * @note Toutes les valeurs de l’élément structurant sont initialisées à @p couleur_fond.
 * @note L’appelant est responsable de la libération de la mémoire avec `delete`.
 */
t_ElementStructurant * createElement(const unsigned int h, const unsigned int w, const unsigned int centreX,
    const unsigned int centreY, const unsigned int couleur_fond) {
    assert(h <= TMAX && "Les hauteurs des images doivent être <= 800");
    assert(w <= TMAX && "Les largeurs des images doivent être <= 800");
    assert(couleur_fond <= 255  && "La valeur de la couleur de remplissage doit respecter : 0 <= s <= 255");
    assert(centreY < h && "L'ordonnée du centre doit être < à la hauteur de l'élément structurant");
    assert(centreX < w && "L'abscisse du centre doit être < à la largeur de l'élément structurant");

    auto element_structurant = new t_ElementStructurant();
    element_structurant->h = h;
    element_structurant->w = w;
    element_structurant->centreX = centreX;
    element_structurant->centreY = centreY;

    for (int i = 0; i < element_structurant->h; i++)
        for (int j = 0; j < element_structurant->w; j++)
            element_structurant->valeurs[i][j] = couleur_fond;

    return element_structurant;
}
