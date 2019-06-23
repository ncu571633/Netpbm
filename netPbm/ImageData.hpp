#ifndef __MATRIX_HPP__
#define __MATRIX_HPP__

namespace netPbm
{
    class ImageData
    {
        public:
            ImageData(int row, int column);
            virtual ~ImageData(){}
            virtual void Reset() = 0;
            virtual void Print() = 0;
            virtual void Write(FILE* fp) = 0;
            virtual void Read(FILE* fp) = 0;

            int row;
            int column;
    };

    // implement Matrix by using a 1-D vector
    class VectorImageData: public ImageData
    {
        private:
            int* data;
        public:
            VectorImageData(int row, int column);
            ~VectorImageData();
            void Reset();
            void Print();
            void Write(FILE* fp);
            void Read(FILE* fp);
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
            void Print();
            void Write(FILE* fp);
            void Read(FILE* fp);
    };
}
#endif
