#ifndef __IMAGEDATA_HPP__
#define __IMAGEDATA_HPP__

#include <cassert>  // assert

namespace netPbm
{
    class ImageData
    {
        public:
            ImageData(int row, int column);
            void Read(FILE* fp);
            void Write(FILE* fp);
            void Print();
            
            virtual ~ImageData(){}
            virtual void Reset() = 0;
            virtual int* operator[] (int i) = 0;
            virtual int Get(int i, int j) = 0;
            virtual void Set(int i, int j, int v) = 0;

            int row;
            int column;
    };

    // implement Matrix by using a 1-D array 
    class ArrayImageDataBase: public ImageData
    {
        protected:
            int* data;
            int size;
        public:
            ArrayImageDataBase(int row, int column)
                :ImageData(row, column) {}
            virtual ~ArrayImageDataBase();
            void Reset();
    };

    // implement Matrix by using a 1D int array, each element is a bit
    // [i][j]:  (i*column + j)
    class ArrayImageData: public ArrayImageDataBase
    {
        public:
            ArrayImageData(int row, int column);
            
            virtual int* operator[] (int i) { return &(this->data[i*column]); }
            virtual int Get(int i, int j) { return this->data[i*column + j]; }
            virtual void Set(int i, int j, int v) { this->data[i*column + j] = v; }
    };

    // implement Matrix by using a bit array, each element is a bit
    // [i][j]:  (i*column + j) / 8 int
    //          (i*column + j) % 8 bit
    class BitArrayImageData: public ArrayImageDataBase
    {
        public:
            BitArrayImageData(int row, int column);
            
            inline int Get(int i, int j) { return (this->data[Index(i, j)] & Mask(i, j)) >> Bit(i, j); }
            void Set(int i, int j, int v);

        private:
            virtual int* operator[] (int i) { assert(0); }
            // sizeof(int) == 4
            inline int Index(int i, int j) { return (i*column+j) / (8 * sizeof(int)); }
            inline int Bit(int i, int j) { return (i*column+j) % (8 * sizeof(int)); }
            inline int Mask(int i, int j) { return 1 << Bit(i, j); }
    };

    // implement Matrix by using a 2-D vector
    class MatrixImageData: public ImageData
    {
        private:
            int** data;
        public:
            MatrixImageData(int row, int column);
            ~MatrixImageData();
            void Reset();

            inline int* operator[] (int i) { return this->data[i]; }
            inline int Get(int i, int j) { return this->data[i][j]; }
            inline void Set(int i, int j, int v) { this->data[i][j] = v; }
    };
}
#endif
