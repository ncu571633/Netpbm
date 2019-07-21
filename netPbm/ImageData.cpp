#include <cstdlib>  // malloc
#include <cstdio>   // fscanf
#include <cstring>  // memset

#include "ImageData.hpp"

namespace netPbm
{
    ImageData::ImageData(int row, int column)
    {
        this->row = row;
        this->column = column;
    }

    void ImageData::Read(FILE* fp)
    {
        int v = 0;
        for (int i=0; i<this->row; i++)
        {
            for (int j=0; j < this->column; j++)
            {
                fscanf(fp, "%i", &v);
                this->Set(i, j, v);
            }
        }
    }

    void ImageData::Write(FILE* fp)
    {
        for (int i = 0; i < this->row; i++)
        {
            for (int j = 0; j < this->column; j++)
                fprintf(fp, "%d ", this->Get(i, j));
            fprintf(fp, "\n");
        }
    }
    
    void ImageData::Print()
    {
        for (int i = 0; i < this->row; i++)
        {
            for (int j = 0; j < this->column; j++)
                printf("%d ", this->Get(i, j));
            printf("\n");
        }
    }

    ArrayImageDataBase::~ArrayImageDataBase()
    {
        if (this->data == nullptr) 
            return;
        free(this->data);
        this->data = nullptr;
    }

    void ArrayImageDataBase::Reset()
    {
        memset(this->data, 0, size);
    }

    ArrayImageData::ArrayImageData(int row, int column)
        :ArrayImageDataBase(row, column)
    {
        size = row*column*sizeof(int);
        this->data = (int *)malloc(size);
        memset(this->data, 0, size);
    }

    BitArrayImageData::BitArrayImageData(int row, int column)
        :ArrayImageDataBase(row, column)
    {
        size = (row*column / 8) + 1;
        this->data = (int *)malloc(size);
        memset(this->data, 0, size);
    }

    void BitArrayImageData::Set(int i, int j, int v)
    {
        if (v==1)
        {
            this->data[Index(i, j)] |= Mask(i, j);
        }
        else if (v==0)
        {
            this->data[Index(i, j)] &= ~Mask(i, j); 
        }
        else
        {
            assert(0);
        }
    }
    
    MatrixImageData::MatrixImageData(int row, int column)
        :ImageData(row, column)
    {
        this->data = (int **)malloc(row*sizeof(int *));

        for (int i=0; i<row; i++)
        {
            this->data[i]  = (int *)malloc(column* sizeof(int));
            memset(this->data[i], 0, column*sizeof(int));
        }
    }

    MatrixImageData::~MatrixImageData()
    {
        if (this->data == nullptr) 
            return;

        for (int i=0; i<row; i++)
        {
            if (this->data[i] != nullptr)
            {
                free(this->data[i]);
                this->data[i] = nullptr;
            }
        }
        free(this->data);
        this->data = nullptr;
    }

    void MatrixImageData::Reset()
    {
        for (int i=0; i<this->row; i++)
            memset(this->data[i], 0, this->column*sizeof(int));
    }
}
