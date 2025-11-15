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

    ElementStructurant element1 = {
        .h  = 3,
        .w   = 3,
        .valeurs      = {
                       {255,   0, 255},
                       {0,   0,    0},
                       {255,   0, 255}
                      }
    };

    cout << "=== Dilatation ===" << endl;
    t_Image  image2;
    bool ok2 = false;

    loadPgm("/Users/davidprosperin/CLionProjects/smp_tp3/tp3-images/croix.pgm", &image2, ok2);

    t_Image  image_contour = {
        5,
        5,
        {{255, 255, 255, 255, 255}, {255, 255, 255, 255, 255}, {255, 255, 255, 255, 255}, {255, 255, 255, 255, 255}, {255, 255, 255, 255, 255}}
    };

    dilatation(&image2, &image_contour, &element1);

    string sortieFichier2 = "/Users/davidprosperin/CLionProjects/smp_tp3/tp3-images/croixDilatation.pgm";
    savePgm(sortieFichier2, &image2);

    string sortieFichierContour = "/Users/davidprosperin/CLionProjects/smp_tp3/tp3-images/croixDilatationContour.pgm";
    savePgm(sortieFichierContour, &image_contour);

    return 0;
}