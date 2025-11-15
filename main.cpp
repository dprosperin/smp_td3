#include <iostream>
#include "chargesauve.h"
#include  "image.h"
#include "outils.h"

using namespace std;

int main() {
    // TODO: Plus de 3 structures ça plante
    /*
    t_Image  image;
    bool ok = false;

    int seuil = 25;

    loadPgm("/Users/davidprosperin/CLionProjects/smp_tp3/tp3-images/kodie512x512.pgm", &image, ok);
    cout << ok << endl;

    if (ok) {
        seuillage(&image, seuil);
    }

    cout << "=== Binarisation de l'image  ===" << endl;
    string sortieFichier = "/Users/davidprosperin/CLionProjects/smp_tp3/tp3-images/kodie512x512seuil"
                     + to_string(seuil)
                     + ".pgm";

    savePgm(sortieFichier, &image);*/


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

    cout << "=== Dilatation ===" << endl;
    auto image2 = createImage();
    bool ok2 = false;

    loadPgm("/Users/davidprosperin/CLionProjects/smp_tp3/tp3-images/kodie512x512seuil20.pgm", image2, ok2);

    auto image_contour = createImage(image2->h, image2->w);

    dilatation(image2, image_contour, element1, 99);

    //string sortieFichier2 = "/Users/davidprosperin/CLionProjects/smp_tp3/tp3-images/kodie512x512seuil20.pgm";
    //savePgm(sortieFichier2, &image2);

    string sortieFichierContour = "/Users/davidprosperin/CLionProjects/smp_tp3/tp3-images/kodie512x512seuil20Contour.pgm";
    savePgm(sortieFichierContour, image_contour);

    delete element1;
    delete image_contour;
    delete image2;

    return 0;
}
