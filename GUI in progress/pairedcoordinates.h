#ifndef PAIREDCOORDINATES_H
#define PAIREDCOORDINATES_H


class PairedCoordinates
{
public:
    PairedCoordinates(int x1, int y1, int x2, int y2);
    int getX1();
    int getX2();
    int getY1();
    int getY2();

private:
    int x1,x2,y1,y2;
};

#endif // PAIREDCOORDINATES_H
