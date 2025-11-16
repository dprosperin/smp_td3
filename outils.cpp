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
 * @param imgIn Pointeur vers l'image d'entrée (const) ne devant pas être modifiée.
 * @param imgOut Pointeur vers l'image de sortie qui recevra le résultat.
 *                     Doit avoir les mêmes dimensions que @p image_entree.
 * @param element Pointeur vers l'élément structurant utilisé pour la dilatation.
 *                Doit être carré et de taille impaire.
 * @param fillColor Valeur du pixel utilisé pour "allumer" les positions
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
void dilatation(const t_Image *imgIn, t_Image *imgOut, const t_ElementStructurant *element, const unsigned int fillColor = BLACK) {
    const int imgInWidth = imgIn->w;
    const int imgInHeight = imgIn->h;

    const int elementWidth = element->w;
    const int elementHeight = element->h;

    const int imgOutWidth = imgOut->w;
    const int imgOutHeight = imgOut->h;

    assert(imgOutWidth == imgInWidth && "La largeur de l'image d'entrée et de sortie doivent être égale.");
    assert(imgOutHeight == imgInHeight && "La hauteur de l'image d'entrée et de sortie doivent être égale.");
    assert(imgOutHeight <= TMAX && "Les hauteurs des images doivent être <= 800");
    assert(imgOutWidth <= TMAX && "Les largeurs des images doivent être <= 800");
    assert(elementHeight == elementWidth && "L'élément structurant doit être carrée.");
    assert(elementHeight % 2 == 1 && "La taille de l'élément structurant doit être impaire.");
    assert(fillColor <= 255  && "La valeur de la couleur de remplissage doit respecter : 0 <= s <= 255");

    for (int imgY = 0; imgY < imgInHeight; imgY++) {
        for (int imgX = 0; imgX < imgInWidth; imgX++) {
            bool overlap = false;
            for (int elementY = 0; elementY < elementHeight && !overlap; elementY++) {
                for (int elementX = 0; elementX < elementWidth && !overlap; elementX++) {
                    const int pixelX = imgX + elementX - element->centreX,
                            pixelY = imgY + elementY - element->centreY;

                    if (pixelX >= 0 && pixelX < imgInWidth &&
                        pixelY >= 0 && pixelY < imgInHeight) {
                        const unsigned int pixel = imgIn->im[pixelY][pixelX],
                                           el = element->valeurs[elementY][elementX];

                        if (pixel == el && pixel == BLACK)
                            overlap = true;
                    }
                }
            }
            if (overlap)
                imgOut->im[imgY][imgX] = fillColor;
        }
    }
}

/**
 * @brief Applique une opération d'érosion morphologique sur une image binaire.
 *
 * L'érosion est une opération de morphologie mathématique permettant de réduire
 * la taille des objets présents dans une image. Pour chaque position de l'élément
 * structurant déplacé sur l'image d'entrée, le pixel correspondant au centre de
 * l'élément structurant dans l'image de sortie n'est conservé (rempli par
 * @p fillColor) que si l'ensemble de l'élément structurant est entièrement contenu
 * dans la forme de l'image. Autrement dit, si une seule cellule active de
 * l'élément structurant chevauche un pixel de fond (valeur 0) dans l'image
 * d'entrée, le pixel central est supprimé dans l'image érodée.
 *
 * Cette opération a pour effet :
 * - de "rétrécir" les objets,
 * - d'éliminer de petits détails ou protubérances,
 * - de séparer des objets faiblement connectés,
 * - de filtrer le bruit isolé.
 *
 * @param imgIn   Pointeur vers l'image d'entrée, non modifiée par la fonction.
 * @param imgOut  Pointeur vers l'image de sortie, préalablement allouée et de même
 *                dimensions que l'image d'entrée.
 * @param element Pointeur vers l'élément structurant (doit être carré et de taille impaire).
 * @param fillColor Valeur du pixel utilisée lorsque l'érosion conserve un point (0 à 255).
 *
 * @pre imgOut->w == imgIn->w
 * @pre imgOut->h == imgIn->h
 * @pre imgOut->w <= TMAX
 * @pre imgOut->h <= TMAX
 * @pre element->w == element->h
 * @pre element->w % 2 == 1
 * @pre 0 <= fillColor <= 255
 *
 * @note L'image d'entrée n'est jamais modifiée. L'image de sortie doit être
 *       allouée avant l'appel. Seuls les pixels répondant strictement au critère
 *       d'érosion sont définis dans imgOut ; les autres doivent être initialisés
 *       par l'appelant si nécessaire.
 */
void erosion(const t_Image *imgIn, t_Image *imgOut, const t_ElementStructurant *element, unsigned int fillColor = BLACK) {
    const int imgInWidth = imgIn->w;
    const int imgInHeight = imgIn->h;

    const int elementWidth = element->w;
    const int elementHeight = element->h;

    const int imgOutWidth = imgOut->w;
    const int imgOutHeight = imgOut->h;

    assert(imgOutWidth == imgInWidth && "La largeur de l'image d'entrée et de sortie doivent être égale.");
    assert(imgOutHeight == imgInHeight && "La hauteur de l'image d'entrée et de sortie doivent être égale.");
    assert(imgOutHeight <= TMAX && "Les hauteurs des images doivent être <= 800");
    assert(imgOutWidth <= TMAX && "Les largeurs des images doivent être <= 800");
    assert(elementHeight == elementWidth && "L'élément structurant doit être carrée.");
    assert(elementHeight % 2 == 1 && "La taille de l'élément structurant doit être impaire.");
    assert(fillColor <= 255  && "La valeur de la couleur de remplissage doit respecter : 0 <= s <= 255");

    for (int imgY = 0; imgY < imgInHeight; imgY++) {
        for (int imgX = 0; imgX < imgInWidth; imgX++) {
            bool overlap = true;
            for (int elementY = 0; elementY < elementHeight && overlap; elementY++) {
                for (int elementX = 0; elementX < elementWidth && overlap; elementX++) {
                    const int pixelX = imgX + elementX - element->centreX,
                            pixelY = imgY + elementY - element->centreY;

                    if (pixelX >= 0 && pixelX < imgInWidth &&
                        pixelY >= 0 && pixelY < imgInHeight) {
                        const unsigned int pixel = imgIn->im[pixelY][pixelX],
                                           el = element->valeurs[elementY][elementX];

                        if (el == BLACK)
                            overlap = (pixel == BLACK);
                        }
                }
            }
            if (overlap)
                imgOut->im[imgY][imgX] = fillColor;
        }
    }
}

/**
 * @brief Réalise une ouverture morphologique sur une image binaire.
 *
 * L’ouverture est une opération de morphologie mathématique composée d’une
 * érosion suivie d’une dilatation avec le même élément structurant.
 * Cette opération permet de supprimer le bruit, d’éliminer de petites
 * irrégularités ou objets isolés, tout en conservant la forme générale
 * des structures plus grandes.
 *
 * Concrètement :
 * - L’érosion réduit les objets et supprime les détails isolés.
 * - La dilatation qui suit restaure approximativement la taille des objets,
 *   mais sans réintroduire les petites imperfections supprimées.
 *
 * L’ouverture est particulièrement utile pour filtrer les artefacts
 * de petite taille tout en préservant la géométrie des objets principaux.
 *
 * @param imgIn   Pointeur vers l'image d'entrée (non modifiée).
 * @param imgOut  Pointeur vers l'image de sortie qui contiendra le résultat.
 *                Doit être de mêmes dimensions que @p imgIn.
 * @param element Pointeur vers l'élément structurant utilisé pour l’ouverture.
 *                Doit être carré et de taille impaire.
 * @param fillColor Valeur utilisée pour remplir les pixels conservés
 *                  lors de l'érosion et de la dilatation (0 à 255).
 *                  Par défaut : @c BLACK.
 *
 * @pre imgOut->w == imgIn->w
 * @pre imgOut->h == imgIn->h
 * @pre imgOut->w <= TMAX
 * @pre imgOut->h <= TMAX
 * @pre element->w == element->h
 * @pre element->w % 2 == 1
 * @pre 0 <= fillColor <= 255
 *
 * @note Une image temporaire est allouée pour stocker le résultat de l’érosion.
 *       Cette image est libérée automatiquement avant la fin de la fonction.
 * @note L’image d’entrée n’est jamais modifiée.
 */
void ouverture(const t_Image *imgIn, t_Image *imgOut, const t_ElementStructurant *element, unsigned int fillColor = BLACK) {
    const int imgInWidth = imgIn->w;
    const int imgInHeight = imgIn->h;

    const int elementWidth = element->w;
    const int elementHeight = element->h;

    const int imgOutWidth = imgOut->w;
    const int imgOutHeight = imgOut->h;

    assert(imgOutWidth == imgInWidth && "La largeur de l'image d'entrée et de sortie doivent être égale.");
    assert(imgOutHeight == imgInHeight && "La hauteur de l'image d'entrée et de sortie doivent être égale.");
    assert(imgOutHeight <= TMAX && "Les hauteurs des images doivent être <= 800");
    assert(imgOutWidth <= TMAX && "Les largeurs des images doivent être <= 800");
    assert(elementHeight == elementWidth && "L'élément structurant doit être carrée.");
    assert(elementHeight % 2 == 1 && "La taille de l'élément structurant doit être impaire.");
    assert(fillColor <= 255  && "La valeur de la couleur de remplissage doit respecter : 0 <= s <= 255");

    auto imgEroded = createImage(imgIn->h, imgIn->w, WHITE);

    erosion(imgIn, imgEroded, element, fillColor);

    dilatation(imgEroded, imgOut, element, fillColor);

    delete imgEroded;
}

void fermeture(const t_Image *imgIn, t_Image *imgOut, const t_ElementStructurant *element, unsigned int fillColor = BLACK) {
    const int imgInWidth = imgIn->w;
    const int imgInHeight = imgIn->h;

    const int elementWidth = element->w;
    const int elementHeight = element->h;

    const int imgOutWidth = imgOut->w;
    const int imgOutHeight = imgOut->h;

    assert(imgOutWidth == imgInWidth && "La largeur de l'image d'entrée et de sortie doivent être égale.");
    assert(imgOutHeight == imgInHeight && "La hauteur de l'image d'entrée et de sortie doivent être égale.");
    assert(imgOutHeight <= TMAX && "Les hauteurs des images doivent être <= 800");
    assert(imgOutWidth <= TMAX && "Les largeurs des images doivent être <= 800");
    assert(elementHeight == elementWidth && "L'élément structurant doit être carrée.");
    assert(elementHeight % 2 == 1 && "La taille de l'élément structurant doit être impaire.");
    assert(fillColor <= 255  && "La valeur de la couleur de remplissage doit respecter : 0 <= s <= 255");

    auto imgDilated = createImage(imgIn->h, imgIn->w, WHITE);

    dilatation(imgIn, imgDilated, element, fillColor);

    erosion(imgDilated, imgOut, element, fillColor);

    delete imgDilated;
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
 * @param backgroundColor Valeur du pixel de fond pour initialiser l'image.
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
t_Image * createImage(const unsigned int h, const unsigned int w, const unsigned int backgroundColor) {
    assert(h <= TMAX && "Les hauteurs des images doivent être <= 800");
    assert(w <= TMAX && "Les largeurs des images doivent être <= 800");
    assert(backgroundColor <= 255  && "La valeur de la couleur de remplissage doit respecter : 0 <= s <= 255");

    auto image = new t_Image();

    image->h = h;
    image->w = w;

    for (int i = 0; i < image->h; i++)
        for (int j = 0; j < image->w; j++)
            image->im[i][j] = backgroundColor;

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
 * @param backgroundColor Valeur utilisée pour initialiser l’ensemble des cellules
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
    const unsigned int centreY, const unsigned int backgroundColor) {
    assert(h <= TMAX && "Les hauteurs des images doivent être <= 800");
    assert(w <= TMAX && "Les largeurs des images doivent être <= 800");
    assert(backgroundColor <= 255  && "La valeur de la couleur de remplissage doit respecter : 0 <= s <= 255");
    assert(centreY < h && "L'ordonnée du centre doit être < à la hauteur de l'élément structurant");
    assert(centreX < w && "L'abscisse du centre doit être < à la largeur de l'élément structurant");

    auto element_structurant = new t_ElementStructurant();
    element_structurant->h = h;
    element_structurant->w = w;
    element_structurant->centreX = centreX;
    element_structurant->centreY = centreY;

    for (int i = 0; i < element_structurant->h; i++)
        for (int j = 0; j < element_structurant->w; j++)
            element_structurant->valeurs[i][j] = backgroundColor;

    return element_structurant;
}
