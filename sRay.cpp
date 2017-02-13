#include "sray.h"
#include <QDesktopWidget>
#include "ui_sray.h"
#include <qpixmap.h>

sRay::sRay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::sRay)
{
    //QDesktopWidget d;

    ui->setupUi(this);
    //this->resize(d.geometry().width(), d.geometry().height());

    //this->setWindowState(Qt::WindowFullScreen);
    n=6;
    width=1920;
    height=1080;
    index=1;

    ui->label_21->setStyleSheet("QLabel { background-color : white}");

    ui->lineEdit->setValidator(new QDoubleValidator(-100, 100, 1, this));
    ui->lineEdit_2->setValidator(new QDoubleValidator(-100, 100, 1, this));
    ui->lineEdit_3->setValidator(new QDoubleValidator(-100, 100, 1, this));
    ui->lineEdit_4->setValidator(new QIntValidator(0, 2000, this));
    ui->lineEdit_5->setValidator(new QIntValidator(0, 2000, this));
    ui->lineEdit_6->setValidator(new QDoubleValidator(-100, 100, 1, this));
    ui->lineEdit_7->setValidator(new QDoubleValidator(-100, 100, 1, this));
    ui->lineEdit_8->setValidator(new QDoubleValidator(-100, 100, 1, this));
    ui->lineEdit_9->setValidator(new QDoubleValidator(-100, 100, 1, this));
    ui->lineEdit_10->setValidator(new QDoubleValidator(-100, 100, 1, this));
    ui->lineEdit_11->setValidator(new QDoubleValidator(-100, 100, 1, this));
    ui->lineEdit_12->setValidator(new QDoubleValidator(-100, 100, 1, this));
    ui->lineEdit_13->setValidator(new QDoubleValidator(-100, 100, 1, this));
    ui->lineEdit_14->setValidator(new QDoubleValidator(-100, 100, 1, this));
    ui->lineEdit_15->setValidator(new QIntValidator(1, 100, this));

}

sRay::~sRay()
{
    delete ui;
}

void sRay::init()
{

    //UNCOMMENT THESE DEFAULT VALUES FOR DEBUGGING. SET No.of Spheres TO 7

    spheres.push_back(Sphere(Vec3f( 0.0, -10004, -20), 2,Vec3f(1.00, 0.32, 0.36), 0, 0.0));
    spheres.push_back(Sphere(Vec3f( 0.0,      0, -25),     2, Vec3f(1.00, 0.32, 0.36), 0, 0.5));//red
    spheres.push_back(Sphere(Vec3f( 8.0,     -4, -25),     2, Vec3f(0.90, 0.76, 0.46), 1, 0.0));//yellow
    spheres.push_back(Sphere(Vec3f( 8.0,      4, -25),     2, Vec3f(0.65, 0.77, 0.97), 1, 0.0));//blue
    spheres.push_back(Sphere(Vec3f(-8.0,      4, -25),     2, Vec3f(0.90, 0.90, 0.90), 1, 0.0));//black
    spheres.push_back(Sphere(Vec3f(-8.0,     -4, -25), 2 ,Vec3f(0.00, 1.00, 0.00), 1, 0.0));//green
    spheres.push_back(Sphere(Vec3f( 0.0,     20, -30),     3, Vec3f(0.00, 0.00, 0.00), 0, 0.0, Vec3f(3)));


    //COMMENT THE FOLLOWING IF YOU DON'T WANT TO ENTER ALL THOSE DAMN ATTRIBUTES WHILE DEBUGGING
    //for(int i=0;i<n;i++)
    //   spheres.push_back(Sphere());
}


float sRay::mix(const float &a, const float &b, const float &mix)
{
    return b * mix + a * (1 - mix);
}

//Function to trace the rays
Vec3f sRay::trace(const Vec3f &rayorig, const Vec3f &raydir, const std::vector<Sphere> &spheres, const int &depth)
{
    float tnear = INFINITY;
    const Sphere* sphere = NULL;

    for (unsigned i = 0; i < spheres.size(); ++i)
    {
        float t0 = INFINITY, t1 = INFINITY;
        if (spheres[i].intersect(rayorig, raydir, t0, t1))
        {
            if (t0 < 0) t0 = t1;

            if (t0 < tnear)
            {
                tnear = t0;
                sphere = &spheres[i];
            }
        }
    }

    if (!sphere)
        return Vec3f(2);

    Vec3f surfaceColor = 0;

    Vec3f phit = rayorig + raydir * tnear;
    Vec3f nhit = phit - sphere->center;

    nhit.normalize();

    float bias = 1e-4;
    bool inside = false;

    if (raydir.dot(nhit) > 0)
        nhit = -nhit, inside = true;

    if ((sphere->transparency > 0 || sphere->reflection > 0) && depth < MAX_RAY_DEPTH)
    {
        float facingratio = -raydir.dot(nhit);
        float fresneleffect = mix(pow(1 - facingratio, 3), 1, 0.1);

        Vec3f refldir = raydir - nhit * 2 * raydir.dot(nhit);
        refldir.normalize();
        Vec3f reflection = trace(phit + nhit * bias, refldir, spheres, depth + 1);
        Vec3f refraction = 0;

        if (sphere->transparency)
        {
            float ior = 1.1, eta = (inside) ? ior : 1 / ior;
            float cosi = -nhit.dot(raydir);
            float k = 1 - eta * eta * (1 - cosi * cosi);
            Vec3f refrdir = raydir * eta + nhit * (eta *  cosi - sqrt(k));
            refrdir.normalize();
            refraction = trace(phit - nhit * bias, refrdir, spheres, depth + 1);
        }

        surfaceColor = (reflection * fresneleffect + refraction * (1 - fresneleffect) * sphere->transparency) * sphere->surfaceColor;
    }

    else
    {

       for (unsigned i = 0; i < spheres.size(); ++i)
       {
            if (spheres[i].emissionColor.x > 0)
            {

                Vec3f transmission = 1;
                Vec3f lightDirection = spheres[i].center - phit;
                lightDirection.normalize();

                for (unsigned j = 0; j < spheres.size(); ++j)
                {
                    if (i != j)
                    {
                        float t0, t1;

                        if (spheres[j].intersect(phit + nhit * bias, lightDirection, t0, t1))
                        {
                            transmission = 0;
                            break;
                        }
                    }
                }
                surfaceColor += sphere->surfaceColor * transmission * std::max(float(0), nhit.dot(lightDirection)) * spheres[i].emissionColor;
            }
        }
    }

    return surfaceColor + sphere->emissionColor;
}


//Function to Render the final .ppm image

///TODO - Use libpng to render .png instead of .ppm images
int sRay::render()
{

    Vec3f *image = new Vec3f[width * height], *pixel = image;
    float invWidth = 1 / float(width), invHeight = 1 / float(height);
    float fov = 30, aspectratio = width / float(height);
    float angle = tan(M_PI * 0.5 * fov / 180.);

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x, ++pixel)
        {
            float xx = (2 * ((x + 0.5) * invWidth) - 1) * angle * aspectratio;
            float yy = (1 - 2 * ((y + 0.5) * invHeight)) * angle;
            Vec3f raydir(xx, yy, -1);
            raydir.normalize();
            *pixel = trace(Vec3f(0), raydir, spheres, 0);
        }
    }

    std::ofstream ofs("./img_render.ppm", std::ios::out | std::ios::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (int i = 0; i < width * height; ++i)
    ofs << (unsigned char)(std::min(float(1), image[i].x) * 255) << (unsigned char)(std::min(float(1), image[i].y) * 255) << (unsigned char)(std::min(float(1), image[i].z) * 255);

    ofs.close();
    delete [] image;

    return 1;


}


//Slot for when the Combo Box is Active
void sRay::comboBoxActive()
{

    //Gets the current active index of the Combo Box and stores it in an integer variable
    index = ui->comboBox->currentIndex();

    //Calls the Slot that updates the Line Edit boxes
    changeLineEdit(index);

}

void sRay::changeLineEdit(int index)
{
    ui->lineEdit->setText(QString::number(spheres[index].center.x));
    ui->lineEdit_2->setText(QString::number(spheres[index].center.y));
    ui->lineEdit_3->setText(QString::number(spheres[index].center.z));
    ui->lineEdit_6->setText(QString::number(spheres[index].surfaceColor.x));
    ui->lineEdit_7->setText(QString::number(spheres[index].surfaceColor.y));
    ui->lineEdit_8->setText(QString::number(spheres[index].surfaceColor.z));
    ui->lineEdit_14->setText(QString::number(spheres[index].emissionColor.x));
    ui->lineEdit_13->setText(QString::number(spheres[index].emissionColor.y));
    ui->lineEdit_12->setText(QString::number(spheres[index].emissionColor.z));
    ui->lineEdit_9->setText(QString::number(spheres[index].radius));
    ui->lineEdit_11->setText(QString::number(spheres[index].transparency));
    ui->lineEdit_10->setText(QString::number(spheres[index].reflection));
}

void sRay::renderClicked()
{

    int success=render();


    if(success==1)
    {
        //File Path is hard coded for now
        QPixmap pix("C:/Ray Tracer/Qt-Img/Qt-Ray-Tracer/img_render.ppm");
        ui->label_21->setPixmap(pix);
    }

}




void sRay::on_lineEdit_4_textChanged(const QString &arg1)
{
    width = arg1.toFloat();
}

void sRay::on_lineEdit_5_textChanged(const QString &arg1)
{
    height = arg1.toFloat();
}

void sRay::on_lineEdit_15_textChanged(const QString &arg1)
{
    spheres.clear();

    ui->comboBox->clear();

    n = arg1.toInt();

    ui->comboBox->setMaxCount(n);

    sphereEntries.clear();

    for(int i=0;i<n;i++)
    {
        sphereEntries.append("Sphere " + QString::number(i+1));
    }

    ui->comboBox->addItems(sphereEntries);

    init();
}

void sRay::on_lineEdit_textChanged(const QString &arg1)
{

    spheres[index].center.x = arg1.toFloat();
}

void sRay::on_lineEdit_2_textChanged(const QString &arg1)
{

    spheres[index].center.y = arg1.toFloat();
}

void sRay::on_lineEdit_3_textChanged(const QString &arg1)
{

    spheres[index].center.z = arg1.toFloat();
}

void sRay::on_lineEdit_6_textChanged(const QString &arg1)
{

    spheres[index].surfaceColor.x=arg1.toFloat();
}

void sRay::on_lineEdit_7_textChanged(const QString &arg1)
{

    spheres[index].surfaceColor.y=arg1.toFloat();
}

void sRay::on_lineEdit_8_textChanged(const QString &arg1)
{

    spheres[index].surfaceColor.z=arg1.toFloat();
}

void sRay::on_lineEdit_14_textChanged(const QString &arg1)
{

    spheres[index].emissionColor.x=arg1.toFloat();
}

void sRay::on_lineEdit_13_textChanged(const QString &arg1)
{

    spheres[index].emissionColor.y=arg1.toFloat();
}

void sRay::on_lineEdit_12_textChanged(const QString &arg1)
{

    spheres[index].emissionColor.z=arg1.toFloat();
}

void sRay::on_lineEdit_9_textChanged(const QString &arg1)
{

    spheres[index].radius = arg1.toFloat();
    spheres[index].radius2 = spheres[index].radius * spheres[index].radius;
}


void sRay::on_lineEdit_11_textChanged(const QString &arg1)
{

    spheres[index].transparency = arg1.toFloat();
}

void sRay::on_lineEdit_10_textChanged(const QString &arg1)
{

    spheres[index].reflection = arg1.toFloat();
}

