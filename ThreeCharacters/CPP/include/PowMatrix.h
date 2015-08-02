#ifndef POWMATRIX_H
#define POWMATRIX_H

class PowMatrix {
    public:
        PowMatrix(int dimension);   //构造函数
        virtual ~PowMatrix();       //析构函数
        void setMatrix(int* matrix);    //设置矩阵
        int multiply(int row,int column);   //两数组对应相乘
        void pow(int count);        //求幂函数
        int* getResult() {          //得到结果
            return result;
        }
    private:
        int dimension;      //矩阵维度
        int* matrix;        //需要求幂的初始矩阵
        int* result;        //求幂之后存放结果的矩阵
};

#endif // POWMATRIX_H
