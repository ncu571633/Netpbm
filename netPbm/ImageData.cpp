#include <cstdio>   // fscanf
#include <cstring>  // memset
#include <cstdlib>  // malloc

#include "Matrix.h"

namespace netPbm
{
    ImageData::Init(int row, int column)
    {
        this->row = row;
        this->column = column;
    }

    bool VectorImageData::Init(int row, int column)
    {
        ImageData::Init(row, column);
        this->data = (int *)malloc(row*column*sizeof(int));
        if (this->data == nullptr) 
            return false;
        
        memset(this->data, 0, row*column*sizeof(int));
        return true;
    }

    void VectorImageData::Reset()
    {
        memset(this->data, 0, row*column*sizeof(int));
    }

    void VectorImageData::Destroy()
    {
        if (this->data == nullptr) 
            return;
        free(this->data);
        this->data = nullptr;
    }

    void VectorImageData::Print()
    {
        for(int i=0; i < this->column; i++)
            printf("%d ", this->data[i]);
        printf("\n");
    }

    void VectorImageData::Write(FILE* fp)
    {
        for (int i = 0; i < this->row; i++)
        {
            for (int j = 0; j < this->column; j++)
                fprintf(fp, "%d ", this->data[i*row + j] >= 1 ? 1: 0);
            fprintf(fp, "\n");
        }
    }

    bool MatrixImageData::Init(int row, int column)
    {
        ImageData::Init(row, column);
        this->data = (int **)malloc(row*sizeof(int *));
        if (this->data == nullptr) 
            return false;

        for (int i=0; i<row; i++)
        {
            this->data[i]  = (int *)malloc(column* sizeof(int));
            if (this->data[i] == nullptr)
                return false;
            memset(this->data[i], 0, column*sizeof(int));
        }
        return true;
    }

    void MatrixImageData::Reset()
    {
        for (int i=0; i<this->row; i++)
            memset(this->data[i], 0, this->column*sizeof(int));
    }

    void MatrixImageData::Destroy()
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

    void MatrixImageData::Print()
    {
        printf("row: %d, column: %d.\n", this->row, this->column);
        for (int i=0; i<this->row; i++)
        {
            for(int j=0; j < this->column; j++)
                printf("%d ", this->data[i][j]);
            printf("\n");
        }
    }

    void VectorImageData::Write(FILE* fp)
    {
        for (int i = 0; i < this->row; i++)
        {
            for (int j = 0; j < this->column; j++)
                fprintf(fp, "%d ", this->data[i][j] >= 1 ? 1: 0);
            fprintf(fp, "\n");
        }
    }
}
