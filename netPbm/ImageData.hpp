#ifndef __MATRIX_HPP__
#define __MATRIX_HPP__

namespace netPbm
{
    class ImageData
    {
        protected:
            int row;
            int column;
        public:
            virtual bool Init(int row, int column);
            virtual void Reset() = 0;
            virtual void Destroy() = 0;
            virtual void Print() = 0;
            virtual void Write() = 0;
    };

    // implement Matrix by using a 1-D vector
    class VectorImageData: public ImageData
    {
        private:
            int* data;
        public:
            bool Init(int row, int column);
            void Reset() = 0;
            void Destroy() = 0;
            void Print() = 0;
    };

    // implement Matrix by using a 2-D vector
    class MatrixImageData: public ImageData
    {
        private:
            int** data;
        public:
            bool Init(int row, int column);
            void Reset() = 0;
            void Destroy() = 0;
            void Print() = 0;
    };
}
#endif
