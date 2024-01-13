#include <iostream>
#include <fstream>

using namespace  std;

class Mat {
    public:
    int r, c;
    double** mat;
    
    Mat (int r, int c = -1, double** vals = nullptr) {
        this->r = r;
        if (c == -1) this->c = this->r;
        else this->c = c;
        if (vals) mat = vals;
        else {
            mat = new double*[this->r];
            for (int i=0 ; i<this->r; i++) mat[i] = new double[this->c];
        };
    };

    void setVals (double** X) {
        for (int i=0; i<r; i++) 
            for (int j=0; j<c; j++) 
                mat[i][j] = X[i][j];
    };

    void setIdentity() {
        if (r != c) return;
        for (int i=0; i<r; i++) {
            for (int j=0; j<c; j++) {
                if (i==j) mat[i][j] = 1;
                else mat[i][j] = 0;
            };
        };
    };

    void print() {
        for (int i=0; i<r; i++) {
            for (int j=0; j<c; j++) {
                cout << mat[i][j] << " ";
            };
            cout << endl;
        };
        cout << endl;
    };


    Mat* product (Mat* B) {
        if (this->c != B->r) return nullptr;
        Mat* R = new Mat(this->r, B->c);
        for (int i=0; i<this->r; i++) {
            // cout << "hi" << endl;
            for (int j=0; j<B->c; j++) {
                // cout << "hii" << endl;
                R->mat[i][j] = 0;
                for (int k=0; k<B->r; k++) {
                    R->mat[i][j] += this->mat[i][k] * B->mat[k][j];
                };
            };
        };
        return R;
    };


};