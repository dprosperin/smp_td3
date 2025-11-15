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

void seuillage(t_Image *image, int s);
void dilatation(t_Image *image, t_Image *image_contour, ElementStructurant *element);

#endif //SMP_TP3_OUTILS_H