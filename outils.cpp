//
// Created by David PROSPÉRIN on 12/11/2025.
//

#include "outils.h"
#include <cassert>

void seuillage(t_Image *image, int s) {
    int w = image->w;
    int h = image->h;

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            image->im[i][j] = image->im[i][j] < s ? 0 : 255;
        }
    }
}

void dilatation(t_Image *image, t_Image *image_contour, ElementStructurant *element) {
    const int im_w = image->w;
    const int im_h = image->h;

    const int el_w = element->w;
    const int el_h = element->h;

    const int image_contour_w = image_contour->w;
    const int image_contour_h = image_contour->h;

    assert(image_contour_w == im_w && "La largeur de l'image et du contour doivent être égale.");
    assert(image_contour_h == im_h && "La hauteur de l'image et du contour doivent être égale.");
    assert(el_h == el_w && "L'element structurant est carree.");

    const int couleur_contour = 99;

    for (int i = 0; i < im_h; i++) {
        for (int j = 0; j < im_w; j++) {
            if (i == 0 && j == 0) {
                // Coin haut gauche
                if ((image->im[i][j] == element->valeurs[1][1] && image->im[i][j] == 0) ||
                   (image->im[i][j+1] == element->valeurs[1][2] && image->im[i][j+1] == 0) ||
                   (image->im[i+1][j] == element->valeurs[2][1] && image->im[i+1][j] == 0) ||
                   (image->im[i+1][j+1] == element->valeurs[2][2] && image->im[i+1][j+1] == 0)) {
                        image_contour->im[i][j] = couleur_contour;
                   }

            } else if (i == 0 && j == (im_w - 1)) {
                // Coin haut droit
                if ((image->im[i][j] == element->valeurs[1][1] && image->im[i][j] == 0) ||
                   (image->im[i+1][j] == element->valeurs[2][1] && image->im[i+1][j] == 0) ||
                   (image->im[i][j-1] == element->valeurs[1][0] && image->im[i][j-1] == 0) ||
                   (image->im[i+1][j-1] == element->valeurs[2][0] && image->im[i+1][j-1] == 0)) {
                        image_contour->im[i][j] = couleur_contour;
                   }

            } else if (i == (im_h - 1) && j == 0) {
                // Coin bas gauche
                if ((image->im[i][j] == element->valeurs[1][1] && image->im[i][j] == 0) ||
                   (image->im[i][j+1] == element->valeurs[1][2] && image->im[i][j+1] == 0) ||
                   (image->im[i-1][j] == element->valeurs[0][1] && image->im[i-1][j] == 0) ||
                   (image->im[i-1][j+1] == element->valeurs[0][2] && image->im[i-1][j+1] == 0)) {
                        image_contour->im[i][j] = couleur_contour;
                   }
            } else if (i == (im_h - 1) && j == (im_w - 1)) {
                // Coin bas droit
                if ((image->im[i][j] == element->valeurs[1][1] && image->im[i][j] == 0) ||
                   (image->im[i][j-1] == element->valeurs[1][0] && image->im[i][j-1] == 0) ||
                   (image->im[i-1][j] == element->valeurs[0][1] && image->im[i-1][j] == 0) ||
                   (image->im[i-1][j-1] == element->valeurs[0][0] && image->im[i-1][j-1] == 0)) {
                        image_contour->im[i][j] = couleur_contour;
                   }
            }
            else if (i == 0) {
                // Ligne haut
                if ((image->im[i][j] == element->valeurs[1][1] && image->im[i][j] == 0) ||
                    (image->im[i][j+1] == element->valeurs[1][2] && image->im[i][j+1] == 0) ||
                    (image->im[i+1][j] == element->valeurs[2][1] && image->im[i+1][j] == 0) ||
                    (image->im[i+1][j+1] == element->valeurs[2][2] && image->im[i+1][j+1] == 0) ||
                    (image->im[i][j-1] == element->valeurs[1][0] && image->im[i][j-1] == 0) ||
                    (image->im[i+1][j-1] == element->valeurs[2][0] && image->im[i+1][j-1] == 0)) {
                        image_contour->im[i][j] = couleur_contour;
                    }
            } else if (i == (im_h - 1)) {
                // Ligne bas
                if ((image->im[i][j] == element->valeurs[1][1] && image->im[i][j] == 0) ||
                    (image->im[i][j+1] == element->valeurs[1][2] && image->im[i][j+1] == 0) ||
                    (image->im[i][j-1] == element->valeurs[1][0] && image->im[i][j-1] == 0) ||
                    (image->im[i-1][j] == element->valeurs[0][1] && image->im[i-1][j] == 0) ||
                    (image->im[i-1][j-1] == element->valeurs[0][0] && image->im[i-1][j-1] == 0) ||
                    (image->im[i-1][j+1] == element->valeurs[0][2] && image->im[i-1][j+1] == 0)) {
                        image_contour->im[i][j] = couleur_contour;
                    }

            } else if (j == 0) {
                // Ligne gauche
                if ((image->im[i][j] == element->valeurs[1][1] && image->im[i][j] == 0) ||
                    (image->im[i][j+1] == element->valeurs[1][2] && image->im[i][j+1] == 0) ||
                    (image->im[i+1][j] == element->valeurs[2][1] && image->im[i+1][j] == 0) ||
                    (image->im[i+1][j+1] == element->valeurs[2][2] && image->im[i+1][j+1] == 0) ||
                    (image->im[i-1][j] == element->valeurs[0][1] && image->im[i-1][j] == 0) ||
                    (image->im[i-1][j+1] == element->valeurs[0][2] && image->im[i-1][j+1] == 0)) {
                        image_contour->im[i][j] = couleur_contour;
                    }
            } else if (j == (im_w - 1)) {
                //Ligne droit
                if ((image->im[i][j] == element->valeurs[1][1] && image->im[i][j] == 0) ||
                    (image->im[i+1][j] == element->valeurs[2][1] && image->im[i+1][j] == 0) ||
                    (image->im[i][j-1] == element->valeurs[1][0] && image->im[i][j-1] == 0) ||
                    (image->im[i-1][j] == element->valeurs[0][1] && image->im[i-1][j] == 0) ||
                    (image->im[i-1][j-1] == element->valeurs[0][0] && image->im[i-1][j-1] == 0) ||
                    (image->im[i+1][j-1] == element->valeurs[2][0] && image->im[i+1][j-1] == 0)) {
                        image_contour->im[i][j] = couleur_contour;
                    }
            } else {
                // Dans l'image
                if ((image->im[i][j] == element->valeurs[1][1] && image->im[i][j] == 0) ||
                    (image->im[i][j+1] == element->valeurs[1][2] && image->im[i][j+1] == 0) ||
                    (image->im[i+1][j] == element->valeurs[2][1] && image->im[i+1][j] == 0) ||
                    (image->im[i+1][j+1] == element->valeurs[2][2] && image->im[i+1][j+1] == 0) ||
                    (image->im[i][j-1] == element->valeurs[1][0] && image->im[i][j-1] == 0) ||
                    (image->im[i-1][j] == element->valeurs[0][1] && image->im[i-1][j] == 0) ||
                    (image->im[i-1][j-1] == element->valeurs[0][0] && image->im[i-1][j-1] == 0) ||
                    (image->im[i+1][j-1] == element->valeurs[2][0] && image->im[i+1][j-1] == 0) ||
                    (image->im[i-1][j+1] == element->valeurs[0][2] && image->im[i-1][j+1] == 0)) {
                            image_contour->im[i][j] = couleur_contour;
                    }
            }
        }
    }
}