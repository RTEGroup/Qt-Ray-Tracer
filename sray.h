#ifndef SRAY_H
#define SRAY_H

#include <QWidget>


#include "sVector.h"
#include "sphere.h"
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <vector>
#include <iostream>
#include <cassert>

#define MAX_RAY_DEPTH 5

namespace Ui {
class sRay;
}

class sRay : public QWidget
{
    Q_OBJECT

public:
    explicit sRay(QWidget *parent = 0);
    ~sRay();
    void init();
    int render();

private:
    std::vector<Sphere> spheres;

    Ui::sRay *ui; //THIS POINTER IS A FRIGGING GODSEND

    float mix(const float &a, const float &b, const float &mix);

    Vec3f trace(const Vec3f &rayorig, const Vec3f &raydir, const std::vector<Sphere> &spheres, const int &depth);




    ///TODO - Uncomplicate the Function Name
    QString Vec3fToQString(int index, char coordinate);

private slots:

    void comboBoxActive();

    void changeLineEdit(int index);

    void renderClicked();

};

#endif // SRAY_H
