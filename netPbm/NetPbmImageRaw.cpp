#include <stdlib.h>	//itoa

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

namespace netPbm
{
    int NetPbmImageRAW::SkipNetPbmComment (void *p)
    {
        FILEMMAP* fmmap = (FILEMMAP*)p;

        while(	((fmmap->file[fmmap->fp] == '\n') || (fmmap->file[fmmap->fp] == ' '))
                &&	fmmap->fp<fmmap->fileSize
             )
            fmmap->fp++;
        if(fmmap->file[fmmap->fp] == '#')
            while((fmmap->file[fmmap->fp] != '\n') && fmmap->fp<fmmap->fileSize)
                fmmap->fp++;
        while(	((fmmap->file[fmmap->fp] == '\n') || (fmmap->file[fmmap->fp] == ' '))
                &&	fmmap->fp<fmmap->fileSize
             )
            fmmap->fp++;

        return true;
    }

    bool NetPbmImageASCII::ReadNetPbmHead(void *p)
    {
        FILEMMAP* fp = (FILEMMAP*)p;
        
        int row = 0, column = 0;
        SkipNetPbmCommentMMAP(fp);
        int column = atoi(&fp->file[fp->fp]);
        fmmap->fp += FileMMapUtility::intlen(column);
        
        SkipNetPbmCommentMMAP(fp);
        int row = atoi(&fp->file[fp->fp]);
        fp->fp += FileMMapUtility::intlen(nrows);
        SkipNetPbmCommentMMAP(fp);

        //start of data
        fp->data = fp->file + fp->fp;
        
        this->data = new 
#if MATRIXTYPE 
            VectorImageData(row, column);
#else
            MatrixImageData(row, column);
#endif
    }

    bool NetPbmImageRAW::Read(void* p)
    {
        this->ReadNetPbmHead(p);

        FILEMMAP* fmmap = (FILEMMAP*)p;
        for(int i=0; i < row; i++)
        {
            for(int j=0; j < column; j++)
            {
                int index = i * (column/8+1) + j/8;
                this->data[i*row + j]= getCharBit(fmmap->data[index], 7-j%8);
            }
        }

        return true;
    }

    bool NetPbmImageRAW::Write(const std::string& filePath)
    {
        FileMMap* fp = new FileMMap(filePath, 'r');

        fmmap->fp = 0;
        strcpy(fmmap->file, "P4\n");
        fmmap->fp += 3;

        char ctemp[FILENAMEBUFFERSIZE];
        strcpy(&fmmap->file[fmmap->fp], f_itoa(pic->width, ctemp, FILENAMEBUFFERSIZE));
        fmmap->fp += intlen(pic->width);
        fmmap->file[fmmap->fp++] = ' ';

        strcpy(&fmmap->file[fmmap->fp], f_itoa(pic->hight, ctemp, FILENAMEBUFFERSIZE));
        fmmap->fp += intlen(pic->hight);
        fmmap->file[fmmap->fp++] = ' ';

        fmmap->data = fmmap->file + fmmap->fp;
        int i, j;
        for(i=0; i < this->data->row; i++)
        {
            for(j=0; j < this->data->column; j++)
            {
                if(this->data->index(i, j) == 1)
                {
                    int index = i * ((this->data->column-1)/8+1) + j/8;
                    fmmap->data[index] = setCharBit(fmmap->data[index], 7-j%8, picdata(i, j));
                }
            }
        }
        DestroyFILEMMAP(&fmmap);
    }

    NetPbmImage* ReadNetPbmRAWImage(const std::string& filePath)
    { 
        FileMMap* fp = new FileMMap(filePath, 'r');
        //File magic number is at the beginning of the file
        if( fp->file[0] != 'P')
            return nullptr;
        fp->fp += 2;	//"PX"
        this->SkipNetPbmComment(fp);
        
        NetPbmImage* ret = nullptr;
        switch( fp->file[1] )
        {
            case '1':
            case '2':
            case '3':
                perror("Should not reach here.\n"); 
                return false;
            
            case '4':	//P4 PBM Binary RAW
                ret = new NetPbmImageRaw();
                ret->Read(fp);

                this->maxValue = 1;

                break;
            
            case '5':
            case '6':
                perror("not implemented.\n");
                return false;

            default:
                perror("unknown type.\n");
                return false;
        }

        delete(fp);

        return ret;

    }
}
