#include <iostream>
#include "chargesauve.h"
#include  "image.h"
#include "outils.h"
#include <cassert>

using namespace std;

int main() {
    cout << "=== Binarisation de l'image  ===" << endl;
    auto image_kodie = createImage();
    bool ok = false;

    int seuil = 50;

    loadPgm("/Users/davidprosperin/CLionProjects/smp_tp3/tp3-images/kodie512x512.pgm", image_kodie, ok);
    cout << ok << endl;

    if (ok) {
        seuillage(image_kodie, seuil);
    }

    string sortieFichier = "/Users/davidprosperin/CLionProjects/smp_tp3/tp3-images/kodie512x512seuil"
                     + to_string(seuil)
                     + ".pgm";

    savePgm(sortieFichier, image_kodie);
    delete image_kodie;

    cout << "=== Chargement de l'image kodie512x512seuil50 ===" << endl;

    auto image_kodie_seuil50 = createImage();
    bool ok2 = false;

    loadPgm("/Users/davidprosperin/CLionProjects/smp_tp3/tp3-images/kodie512x512seuil50.pgm", image_kodie_seuil50, ok2);

    assert(ok2 && "Problème lors du chargement de l'image kodie512x512seuil50.pgm");

    cout << "=== Dilatation ===" << endl;

    auto element3x3 = createElement(3, 3);

    /* Motif de l'element structurant:
        255,   0, 255
        0,     0,   0
        255,   0, 255
    */
    element3x3->valeurs[0][1] = BLACK;
    element3x3->valeurs[1][1] = BLACK;
    element3x3->valeurs[2][1] = BLACK;
    element3x3->valeurs[1][0] = BLACK;
    element3x3->valeurs[1][2] = BLACK;

    auto image_contour = createImage(image_kodie_seuil50->h, image_kodie_seuil50->w);

    dilatation(image_kodie_seuil50, image_contour, element3x3, BLACK);

    string sortieFichierContour = "/Users/davidprosperin/CLionProjects/smp_tp3/tp3-images/kodie512x512seuil50dilatation3x3.pgm";
    savePgm(sortieFichierContour, image_contour);

    cout << "==== Dilatation avec un élément structurant de taille 5x5 ==== " << endl;

    auto element5x5 = createElement(5, 5);
    auto image_kodie_dilatation5x5 = createImage(image_kodie_seuil50->h, image_kodie_seuil50->w);

    /* Motif de l'element structurant:
        255,   255,   0,   255,   255,
        255,   255,   0,   255,   255,
          0,     0,   0,     0,     0,
        255,   255,   0,   255,   255,
        255,   255,   0,   255,   255,
    */

    element5x5->valeurs[0][2] = BLACK;
    element5x5->valeurs[1][2] = BLACK;
    element5x5->valeurs[2][2] = BLACK;
    element5x5->valeurs[3][2] = BLACK;
    element5x5->valeurs[4][2] = BLACK;

    element5x5->valeurs[2][0] = BLACK;
    element5x5->valeurs[2][1] = BLACK;
    element5x5->valeurs[2][3] = BLACK;
    element5x5->valeurs[2][4] = BLACK;

    dilatation(image_kodie_seuil50, image_kodie_dilatation5x5, element5x5, BLACK);

    savePgm("/Users/davidprosperin/CLionProjects/smp_tp3/tp3-images/kodie512x512seuil50dilatation5x5.pgm", image_kodie_dilatation5x5);

    cout << "=== Erosion élément structurant 3x3 ===" << endl;

    auto image_kodie_erosion3x3 = createImage(image_kodie_seuil50->h, image_kodie_seuil50->w);

    erosion(image_kodie_seuil50, image_kodie_erosion3x3, element3x3, BLACK);

    savePgm("/Users/davidprosperin/CLionProjects/smp_tp3/tp3-images/kodie512x512seuil50erosion3x3.pgm", image_kodie_erosion3x3);

    cout << "=== Ouverture élément structurant 3x3 ===" << endl;

    auto image_kodie_ouverture3x3 = createImage(image_kodie_seuil50->h, image_kodie_seuil50->w);

    ouverture(image_kodie_seuil50, image_kodie_ouverture3x3, element3x3, BLACK);

    savePgm("/Users/davidprosperin/CLionProjects/smp_tp3/tp3-images/kodie512x512seuil50ouverture3x3.pgm",
            image_kodie_ouverture3x3);

    delete element3x3;
    delete image_contour;
    delete image_kodie_seuil50;
    delete element5x5;
    delete image_kodie_erosion3x3;
    delete image_kodie_ouverture3x3;

    return 0;
}
