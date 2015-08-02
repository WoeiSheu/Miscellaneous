#include "PowMatrix.h"

PowMatrix::PowMatrix(int dimension) {
    this->dimension = dimension;            //初始化维度
    matrix = new int[dimension*dimension];  //初始化分配matrix空间
    result = new int[dimension*dimension];  //初始化分配存放结果的矩阵的内存空间
}

PowMatrix::~PowMatrix() {
    delete matrix;
    delete result;
}

void PowMatrix::setMatrix(int* matrix) {    //设置需要自乘的矩阵值,用一维数组形式逐行存放
    int len = dimension*dimension;
    for(int i = 0; i < len; i++) {
        (this->matrix)[i] = matrix[i];
    }
}

int PowMatrix::multiply(int row, int column) {  //result矩阵第row行和matrix矩阵第column列相乘
    int val = 0;
    for(int i = 0, j = 0; i < dimension; i++,j++) {
        val += ( result[row*dimension+i] ) * ( matrix[j*dimension+column] );
    }
    return val;
}

void PowMatrix::pow(int count) {                //矩阵求幂函数
    //以下为建立单位矩阵
    for(int i = 0; i < dimension*dimension; i++) {
        if(i/dimension == i%dimension) {        //判断一维数组模拟矩阵的实际对角线位置
            result[i] = 1;
        } else {
            result[i] = 0;
        }
    }
    //矩阵的count次幂
    for(int k = 0; k < count; k++) {            //第k次自乘
        int* temp = new int[dimension*dimension];
        //以下循环,逐次求出矩阵row行column列的值
        for(int row = 0; row < dimension; row++) {
            for(int column = 0; column < dimension; column++) {
                temp[row*dimension+column] = multiply(row,column);
            }
        }
        //将temp矩阵赋值给result矩阵,即更新result矩阵
        for(int j = 0; j < dimension*dimension; j++) {
            result[j] = temp[j];
        }
    }
}
