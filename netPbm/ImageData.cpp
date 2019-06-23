#include <cstdio>   // fscanf
#include <cstring>  // memset
#include <cstdlib>  // malloc

#include "ImageData.hpp"

namespace netPbm
{
    ImageData::ImageData(int row, int column)
    {
        this->row = row;
        this->column = column;
    }

    VectorImageData::VectorImageData(int row, int column)
        :ImageData(row, column)
    {
        this->data = (int *)malloc(row*column*sizeof(int));
        memset(this->data, 0, row*column*sizeof(int));
    }

    VectorImageData::~VectorImageData()
    {
        if (this->data == nullptr) 
            return;
        free(this->data);
        this->data = nullptr;
    }

    void VectorImageData::Reset()
    {
        memset(this->data, 0, row*column*sizeof(int));
    }

    void VectorImageData::Print()
    {
        for(int i=0; i < this->column; i++)
            printf("%d ", this->data[i]);
        printf("\n");
    }

    void VectorImageData::Write(FILE* fp)
    {
        int index = 0;
        for (int i = 0; i < this->row; i++)
        {
            for (int j = 0; j < this->column; j++)
                fprintf(fp, "%d ", this->data[index++]);
            fprintf(fp, "\n");
        }
    }

    void VectorImageData::Read(FILE* fp)
    {
        int index = 0;
        for (int i=0; i<this->row; i++)
        {
            for (int j=0; j < this->column; j++)
            {
                fscanf(fp, "%i", &(this->data[index++]));
            }
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

    void MatrixImageData::Write(FILE* fp)
    {
        for (int i = 0; i < this->row; i++)
        {
            for (int j = 0; j < this->column; j++)
                fprintf(fp, "%d ", this->data[i][j]);
            fprintf(fp, "\n");
        }
    }

    void MatrixImageData::Read(FILE* fp)
    {
        for (int i=0; i<this->row; i++)
        {
            for (int j=0; j < this->column; j++)
                fscanf(fp, "%i", &(this->data[i][j]));
        }
    }
}
