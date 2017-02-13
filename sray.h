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
    //sRay():width(1920), height(1080), n(0), index(0) {}
    ~sRay();
    int flag=0;
    void init();
    int render();
    int width, height;
    int n;
    int index;
    QStringList sphereEntries;
    std::vector<Sphere> spheres;

private:

    Ui::sRay *ui; //THIS POINTER IS A FRIGGING GODSEND

    float mix(const float &a, const float &b, const float &mix);

    Vec3f trace(const Vec3f &rayorig, const Vec3f &raydir, const std::vector<Sphere> &spheres, const int &depth);

private slots:

    void comboBoxActive();
    void changeLineEdit(int index);
    void renderClicked();
    void on_lineEdit_4_textChanged(const QString &arg1);
    void on_lineEdit_5_textChanged(const QString &arg1);
    void on_lineEdit_15_textChanged(const QString &arg1);
    void on_lineEdit_textChanged(const QString &arg1);
    void on_lineEdit_2_textChanged(const QString &arg1);
    void on_lineEdit_3_textChanged(const QString &arg1);
    void on_lineEdit_6_textChanged(const QString &arg1);
    void on_lineEdit_7_textChanged(const QString &arg1);
    void on_lineEdit_8_textChanged(const QString &arg1);
    void on_lineEdit_14_textChanged(const QString &arg1);
    void on_lineEdit_13_textChanged(const QString &arg1);
    void on_lineEdit_12_textChanged(const QString &arg1);
    void on_lineEdit_9_textChanged(const QString &arg1);
    void on_lineEdit_11_textChanged(const QString &arg1);
    void on_lineEdit_10_textChanged(const QString &arg1);
};

#endif // SRAY_H
