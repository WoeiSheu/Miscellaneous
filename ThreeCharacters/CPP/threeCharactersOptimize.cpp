#include<iostream>

using namespace std;

struct SMatrix {
    int a1,a2,b1,b2;
    SMatrix(int _a1,int _a2,int _b1,int _b2):
        a1(_a1), a2(_a2), b1(_b1), b2(_b2)
        {}

    void set(int _a1, int _a2, int _b1, int _b2) {
    	a1 = _a1;
        a2 = _a2;
        b1 = _b1;
        b2 = _b2;
    }
};

void MultiplyMatrix(SMatrix &m, SMatrix n) {
    int a1 = m.a1 * n.a1 + m.a2 * n.b1;
    int a2 = m.a1 * n.a2 + m.a2 * n.b2;
    int b1 = m.b1 * n.a1 + m.b2 * n.b1;
    int b2 = m.b1 * n.a2 + m.b2 * n.b2;

    m.set(a1,a2,b1,b2);
}

void MatrixN(SMatrix &m, int n) {
    if(n == 0) {
        m.set(1,0,0,1);
        return;
    }
    if(n == 1) {
        return;
    }
    if(n%2 == 0) {
        MatrixN(m, n/2);
        MultiplyMatrix(m, m);
    } else {
        SMatrix x = m;
        MatrixN(m, n/2);
        MultiplyMatrix(m, m);
        MultiplyMatrix(m, x);
    }
}

int calculateCount(int);
int main(int argc, char** argv) {
    int n;
    cin >> n;

    int accumulation = calculateCount(n);
    cout << accumulation << endl;
    return 0;
}

int calculateCount(int n) {
    int accumulation = 0;
    int dp0 = 3, dp1 = 0;
    SMatrix m(2,1,2,0);
    MatrixN(m, n-1);

    accumulation = 3*(m.a1 + m.a2);
    return accumulation;
}
