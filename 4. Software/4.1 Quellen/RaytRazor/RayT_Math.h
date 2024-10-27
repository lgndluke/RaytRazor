//beinhaltet mathematische formeln für raytracer kalkulationen

#ifndef RayT_Math_h
#define RayT_Math_h

//Definieren von oft verwendeten variablen
namespace mathdef{
    const double PI = 3.14159;
}

//vector construktor
class Rayt_vec{

private:
int x,y,z;

public:
Rayt_vec(){};
int getx(){};
int gety(){};
int getz(){};
double dist_to_vec(Rayt_vec){};

};


//Ray construktor
class RayT_ray{

private:
Rayt_vec e; //start vector
Rayt_vec d; //direction vector

public:
Rayt_vec calculate(double t){}; //calc P = e+t*d

};
//Kreuz produkt
Rayt_vec xprod(Rayt_vec vec1, Rayt_vec vec2){};

//Calculate camera alignment
Rayt_vec calc_camU(Rayt_vec W){};
Rayt_vec calc_camV(Rayt_vec W,Rayt_vec U){};

//Calculate v&u
double calc_Rayv(Rayt_vec ray,int nx,int ny,int xpos,int ypos){};
double calc_Rayu(Rayt_vec ray,int nx,int ny,int xpoy,int ypos){};

//creates a ray using calc_Rayv & -u and camera alignments
RayT_ray CreateRay(){};

//check if hit a surface (returns distance of viable hit and checks for different types of surfaces e.g triangels,spheres...)
double hit_scan(RayT_ray ray){};
//^still needs to get a surface passed

//create a normal vector from a surface bounce
Rayt_vec calc_normVec(Rayt_vec pd1, Rayt_vec pd2){};
//^needs to be passed a surface Rayt_vec is temporary (or else it gives me compiler errors)

//create a reflection vector from an incomming ray
RayT_ray calc_reflecRay(Rayt_vec normVec, RayT_ray OrigRay){};
#endif 
//RayT_Math_h