#include "sray.h"
#include "ui_sray.h"

sRay::sRay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::sRay)
{
    ui->setupUi(this);

    //Limits the Input to Floating Point Numbers with one decimal point
    ui->lineEdit->setValidator(new QDoubleValidator(-100, 100, 1, this));
    ui->lineEdit_2->setValidator(new QDoubleValidator(-100, 100, 1, this));
    ui->lineEdit_3->setValidator(new QDoubleValidator(-100, 100, 1, this));
}

sRay::~sRay()
{
    delete ui;
}


//Initializes the Spheres.
void sRay::init()
{

    ///TODO - Dynamically get input for all the features of the spheres.
    ///TODO - Add a way to alter the number of spheres being drawn

    spheres.push_back(Sphere(Vec3f( 0.0, -10004, -20), 2,Vec3f(1.00, 0.32, 0.36), 0, 0.0));
    spheres.push_back(Sphere(Vec3f( 0.0,      0, -25),     2, Vec3f(1.00, 0.32, 0.36), 0, 0.5));//red
    spheres.push_back(Sphere(Vec3f( 8.0,     -4, -25),     2, Vec3f(0.90, 0.76, 0.46), 1, 0.0));//yellow
    spheres.push_back(Sphere(Vec3f( 8.0,      4, -25),     2, Vec3f(0.65, 0.77, 0.97), 1, 0.0));//blue
    spheres.push_back(Sphere(Vec3f(-8.0,      4, -25),     2, Vec3f(0.90, 0.90, 0.90), 1, 0.0));//black
    spheres.push_back(Sphere(Vec3f(-8.0,     -4, -25), 2 ,Vec3f(0.00, 1.00, 0.00), 1, 0.0));//green
    spheres.push_back(Sphere(Vec3f( 0.0,     20, -30),     3, Vec3f(0.00, 0.00, 0.00), 0, 0.0, Vec3f(3)));//light

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
///TODO - Get Dynamic input for image width and height
int sRay::render()
{
    unsigned width = 1920, height = 1080;
    Vec3f *image = new Vec3f[width * height], *pixel = image;
    float invWidth = 1 / float(width), invHeight = 1 / float(height);
    float fov = 30, aspectratio = width / float(height);
    float angle = tan(M_PI * 0.5 * fov / 180.);

    for (unsigned y = 0; y < height; ++y)
    {
        for (unsigned x = 0; x < width; ++x, ++pixel)
        {
            float xx = (2 * ((x + 0.5) * invWidth) - 1) * angle * aspectratio;
            float yy = (1 - 2 * ((y + 0.5) * invHeight)) * angle;
            Vec3f raydir(xx, yy, -1);
            raydir.normalize();
            *pixel = trace(Vec3f(0), raydir, spheres, 0);
        }
    }

    std::ofstream ofs("./render_qt2.ppm", std::ios::out | std::ios::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (unsigned i = 0; i < width * height; ++i)
    ofs << (unsigned char)(std::min(float(1), image[i].x) * 255) << (unsigned char)(std::min(float(1), image[i].y) * 255) << (unsigned char)(std::min(float(1), image[i].z) * 255);

    ofs.close();
    delete [] image;

    return 1;
}


//Slot for when the Combo Box is Active
void sRay::comboBoxActive()
{

    //Gets the current active index of the Combo Box and stores it in an integer variable
    int index = ui->comboBox->currentIndex();

    //Calls the Slot that updates the Line Edit boxes
    changeLineEdit(index);
}

//Function to convert Vec3 to QString based on the coordinated
QString sRay::Vec3fToQString(int index, char coordinate)
{
    if(coordinate=='x')

        /* Refers to the X coordinate of the center of the Sphere present
        in the "index" position of the vector. Thank you OOPS :)*/

        return(QString::number(spheres[index].center.x));

    else if(coordinate=='y')
        return(QString::number(spheres[index].center.y));
    else
        return(QString::number(spheres[index].center.z));
}

//Slot to set the values for the Line Edit Widgets
void sRay::changeLineEdit(int index)
{

            ui->lineEdit->setText(Vec3fToQString(index, 'x'));
            ui->lineEdit_2->setText(Vec3fToQString(index, 'y'));
            ui->lineEdit_3->setText(Vec3fToQString(index, 'z'));
}

void sRay::renderClicked()
{
    int finish=render(); // Generates a warning. For the timebeing.

    ///TODO - Get this abomination working
    //while(finish!=1)
    //{
    //  ui->pushButton->setEnabled(false);
    //}

    ///TODO - Get dynamic input for the file name and location

}



