//
// Created by David PROSPÉRIN on 12/11/2025.
//

#ifndef SMP_TP3_OUTILS_H
#define SMP_TP3_OUTILS_H
#include "image.h"

#define BLACK 0
#define WHITE 255

typedef struct {
    int w, h;
    int centreX, centreY;
    t_MatEnt valeurs;
} t_ElementStructurant;

void seuillage(t_Image *image, unsigned int s);
void dilatation(const t_Image *imgIn, t_Image *imgOut, const t_ElementStructurant *element, unsigned int fillColor);

t_Image* createImage(unsigned int h = 50, unsigned int w = 50, unsigned int backgroundColor = WHITE);
t_ElementStructurant* createElement(unsigned int h = 3, unsigned int w = 3, unsigned int centreX = 1, unsigned int centreY = 1, unsigned int backgroundColor = WHITE);
#endif //SMP_TP3_OUTILS_H