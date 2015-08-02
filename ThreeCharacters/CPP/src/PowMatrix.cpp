#include "PowMatrix.h"

PowMatrix::PowMatrix(int dimension) {
    this->dimension = dimension;            //��ʼ��ά��
    matrix = new int[dimension*dimension];  //��ʼ������matrix�ռ�
    result = new int[dimension*dimension];  //��ʼ�������Ž���ľ�����ڴ�ռ�
}

PowMatrix::~PowMatrix() {
    delete matrix;
    delete result;
}

void PowMatrix::setMatrix(int* matrix) {    //������Ҫ�Գ˵ľ���ֵ,��һά������ʽ���д��
    int len = dimension*dimension;
    for(int i = 0; i < len; i++) {
        (this->matrix)[i] = matrix[i];
    }
}

int PowMatrix::multiply(int row, int column) {  //result�����row�к�matrix�����column�����
    int val = 0;
    for(int i = 0, j = 0; i < dimension; i++,j++) {
        val += ( result[row*dimension+i] ) * ( matrix[j*dimension+column] );
    }
    return val;
}

void PowMatrix::pow(int count) {                //�������ݺ���
    //����Ϊ������λ����
    for(int i = 0; i < dimension*dimension; i++) {
        if(i/dimension == i%dimension) {        //�ж�һά����ģ������ʵ�ʶԽ���λ��
            result[i] = 1;
        } else {
            result[i] = 0;
        }
    }
    //�����count����
    for(int k = 0; k < count; k++) {            //��k���Գ�
        int* temp = new int[dimension*dimension];
        //����ѭ��,����������row��column�е�ֵ
        for(int row = 0; row < dimension; row++) {
            for(int column = 0; column < dimension; column++) {
                temp[row*dimension+column] = multiply(row,column);
            }
        }
        //��temp����ֵ��result����,������result����
        for(int j = 0; j < dimension*dimension; j++) {
            result[j] = temp[j];
        }
    }
}
