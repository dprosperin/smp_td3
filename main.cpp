#include <iostream>
#include "chargesauve.h"
#include  "image.h"
#include "outils.h"

using namespace std;

int main() {
    cout << "=== Binarisation de l'image  ===" << endl;
    auto image_kodie512x512 = createImage();
    bool ok = false;

    int seuil = 25;

    loadPgm("/Users/davidprosperin/CLionProjects/smp_tp3/tp3-images/kodie512x512.pgm", image_kodie512x512, ok);
    cout << ok << endl;

    if (ok) {
        seuillage(image_kodie512x512, seuil);
    }

    string sortieFichier = "/Users/davidprosperin/CLionProjects/smp_tp3/tp3-images/kodie512x512seuil"
                     + to_string(seuil)
                     + ".pgm";

    savePgm(sortieFichier, image_kodie512x512);
    delete image_kodie512x512;

    cout << "=== Dilatation ===" << endl;

    auto element1 = createElement(3, 3);

    /* Motif de l'element structurant:
        255,   0, 255
        0,     0,   0
        255,   0, 255
    */
    element1->valeurs[0][1] = 0;
    element1->valeurs[1][1] = 0;
    element1->valeurs[2][1] = 0;
    element1->valeurs[1][0] = 0;
    element1->valeurs[1][2] = 0;

    auto image2 = createImage();
    bool ok2 = false;

    loadPgm("/Users/davidprosperin/CLionProjects/smp_tp3/tp3-images/kodie512x512seuil20.pgm", image2, ok2);

    auto image_contour = createImage(image2->h, image2->w);

    dilatation(image2, image_contour, element1, 99);

    string sortieFichierContour = "/Users/davidprosperin/CLionProjects/smp_tp3/tp3-images/kodie512x512seuil20ContourVariable.pgm";
    savePgm(sortieFichierContour, image_contour);

    delete element1;
    delete image_contour;
    delete image2;

    return 0;
}
