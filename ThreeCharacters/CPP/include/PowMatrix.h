#ifndef POWMATRIX_H
#define POWMATRIX_H

class PowMatrix {
    public:
        PowMatrix(int dimension);   //���캯��
        virtual ~PowMatrix();       //��������
        void setMatrix(int* matrix);    //���þ���
        int multiply(int row,int column);   //�������Ӧ���
        void pow(int count);        //���ݺ���
        int* getResult() {          //�õ����
            return result;
        }
    private:
        int dimension;      //����ά��
        int* matrix;        //��Ҫ���ݵĳ�ʼ����
        int* result;        //����֮���Ž���ľ���
};

#endif // POWMATRIX_H
