//
// Created by David PROSPÉRIN on 12/11/2025.
//

#include "outils.h"
#include <cassert>

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

void dilatation(const t_Image *image_entree, t_Image *image_sortie, const ElementStructurant *element, const unsigned int couleur_remplissage) {
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

t_Image * createImageFilledBy(const unsigned int h, const unsigned int w, const unsigned int couleur_fond) {
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
