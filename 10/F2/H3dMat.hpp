#include "Mat.hpp"
#include "Vec.hpp"
#include <iomanip>

class H3dMat : public Mat {
    public:
    H3dMat (double** vals = nullptr) : Mat(4, 4, vals) {
        if (!vals) this->setIdentity();
    };
};

class H3dPoint : public Mat {
    public:
    H3dPoint (Point* p) : Mat(4, 1) {
        this->mat[0][0] = p->x;
        this->mat[1][0] = p->y;
        this->mat[2][0] = p->z;
        this->mat[3][0] = 1;
    };

    void print (ostream& out = cout) {
        out << fixed << setprecision(7);
        out << mat[0][0] << " " << mat[1][0] << " " << mat[2][0] << endl;
    };
};