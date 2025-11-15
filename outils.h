//
// Created by David PROSPÉRIN on 12/11/2025.
//

#ifndef SMP_TP3_OUTILS_H
#define SMP_TP3_OUTILS_H
#include "image.h"
typedef struct {
    int w, h;
    int centreX, centreY;
    t_MatEnt valeurs;
} ElementStructurant;

void seuillage(t_Image *image, unsigned int s);
void dilatation(const t_Image *image_entree, t_Image *image_sortie, const ElementStructurant *element, unsigned int couleur_remplissage);

t_Image* createImageFilledBy(unsigned int h = 50, unsigned int w = 50, unsigned int couleur_fond = 255);

#endif //SMP_TP3_OUTILS_H