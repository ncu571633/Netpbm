#include "FileMMap.hpp"

#include <cstring>  // memset
#include <cmath>  // ceil 
#include <cstdio>// snprintf

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
namespace netPbm
{

    FileMMap::~FileMMap()
    {
        if(this->file != NULL)
        {
            if (munmap (this->file, this->fileSize) == -1)
            {
                perror ("munmap");
            }
        }
        if(this->fd>0)
        {
            if ( close(this->fd) == -1 )
            {
                perror ("close");
            }
        }
    }

    int FileMMapUtility::getCharBit(unsigned char srcChar, int nBit) 
    { 
        if((nBit<0)||(nBit>7))
            return   -1;
        return   ((srcChar&(1<<nBit))>>nBit); 
    } 

    int FileMMapUtility::setCharBit(unsigned char srcChar, int nBit, bool value)
    {
        if(	((nBit<0)||(nBit>7))
                ||	((value!=true)&&(value!=false))
          )
            return -1;
        srcChar = (srcChar|(value<<nBit));
        return srcChar;
    }

    int FileMMapUtility::intlen(int i)
    {
        return (int)(log10(i)+1);
    }

    //convert int to string
    char * FileMMapUtility::f_itoa(int value, char * str, int strSize)
    {
        memset(str, '\0', strSize);
        snprintf(str, strSize, "%d", value);
        return str;
    }

    int FileMMapUtility::getFileSize(int row, int column)
    {
        return (ceil(((double)column)/8)*row + intlen(row) + intlen(column) + 5) 
            * sizeof(char);
    }
}
