#include "pairedcoordinates.h"

PairedCoordinates::PairedCoordinates(int x1, int y1, int x2, int y2)
{
    this->x1 = x1;
    this->x2 = x2;
    this->y1 = y1;
    this->y2 = y2;
}
/**
 * @brief PairedCoordinates::getX1
 * @return x1
 */
int PairedCoordinates::getX1(){
    return x1;
}
/**
 * @brief PairedCoordinates::getX2
 * @return x2
 */
int PairedCoordinates::getX2(){
    return x2;
}
/**
 * @brief PairedCoordinates::getY1
 * @return y1
 */
int PairedCoordinates::getY1(){
    return y1;
}
/**
 * @brief PairedCoordinates::getY2
 * @return y2
 */
int PairedCoordinates::getY2(){
    return y2;
}
