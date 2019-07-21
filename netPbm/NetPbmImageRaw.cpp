#include "FileMMap.hpp"

#include <cstring>  //strcpy

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

namespace netPbm
{
    ImageData* NetPbmImageRAW::InitImageData(int row, int column)
    {
        return new ArrayImageData(row, column);
    }

    int NetPbmImageRAW::SkipNetPbmComment (void *p)
    {
        FileMMap* fp = (FileMMap*)p;

        while(	((fp->file[fp->fp] == '\n') || (fp->file[fp->fp] == ' '))
                &&	fp->fp<fp->fileSize
             )
            fp->fp++;
        if(fp->file[fp->fp] == '#')
            while((fp->file[fp->fp] != '\n') && fp->fp<fp->fileSize)
                fp->fp++;
        while(	((fp->file[fp->fp] == '\n') || (fp->file[fp->fp] == ' '))
                &&	fp->fp<fp->fileSize
             )
            fp->fp++;

        return true;
    }

    bool NetPbmImageRAW::ReadNetPbmHead(void *p)
    {
        FileMMap* fp = (FileMMap*)p;
        
        SkipNetPbmComment(fp);
        int column = atoi(&fp->file[fp->fp]);
        fp->fp += FileMMapUtility::intlen(column);
        
        SkipNetPbmComment(fp);
        int row = atoi(&fp->file[fp->fp]);
        fp->fp += FileMMapUtility::intlen(row);
        SkipNetPbmComment(fp);

        //start of data
        fp->data = fp->file + fp->fp;
        
        this->data = InitImageData(row, column); 
        return true;
    }

    void NetPbmImageRAW::Read(void* p)
    {
        this->ReadNetPbmHead(p);

        FileMMap* fp = (FileMMap*)p;
        int row = this->getRow(), column = this->getColumn();
        for(int i=0; i < row; i++)
        {
            for(int j=0; j < column; j++)
            {
                int index = i * (column/8+1) + j/8;
                this->data->Set(i, j, FileMMapUtility::getCharBit(fp->data[index], 7-j%8));
            }
        }
    }

    bool NetPbmImageRAW::Write(const std::string& filePath)
    {
        FileMMap* fp = new FileMMap(filePath, 'w', this);
        fp->fp = 0;
        
        strcpy(fp->file, "P4\n");
        fp->fp += 3;

        int row = this->getRow(), column = this->getColumn();
        strcpy(&fp->file[fp->fp], std::to_string(column).c_str());
        fp->fp += FileMMapUtility::intlen(column);
        fp->file[fp->fp++] = ' ';

        strcpy(&fp->file[fp->fp], std::to_string(row).c_str());
        fp->fp += FileMMapUtility::intlen(row);
        fp->file[fp->fp++] = ' ';

        fp->data = fp->file + fp->fp;
        for(int i=0; i < row; i++)
        {
            for(int j=0; j < column; j++)
            {
                if(this->data->Get(i, j) == 1)
                // if(this->data[i][j] == 1)
                {
                    int index = i * ((column-1)/8+1) + j/8;
                    fp->data[index] = FileMMapUtility::setCharBit(fp->data[index], 7-j%8, 1);
                }
            }
        }
        
        delete(fp);
        return true;
    }

    NetPbmImage* ReadNetPbmRAWImage(const std::string& filePath)
    { 
        FileMMap* fp = new FileMMap(filePath, 'r');
        //File magic number is at the beginning of the file
        if( fp->file[0] != 'P')
            return nullptr;
        fp->fp += 2;	//"PX"
        
        NetPbmImage* ret = nullptr;
        switch( fp->file[1] )
        {
            case '1':
            case '2':
            case '3':
                perror("Should not reach here.\n"); 
            
            case '4':	//P4 PBM Binary RAW
                ret = new NetPbmImageRAW();
                ret->Read(fp);
                break;
            
            case '5':
            case '6':
                perror("not implemented.\n");

            default:
                perror("unknown type.\n");
        }

        delete(fp);

        return ret;
    }
}
