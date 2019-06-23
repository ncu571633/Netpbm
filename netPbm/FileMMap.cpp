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
    //map a file to the memory
    FileMMap::FileMMap(const std::string& filePath, char type)
    {
        this->file = nullptr;
        this->data = nullptr;
        this->fp = 0;
        this->fileSize = 0;
        this->fd = 0;

        //open file
        switch (ctype)
        {
            case ('r'):
                this->fd = open(filePath.c_str(), O_RDONLY);
                break;
            case ('w'):
                /* Open a file for writing.
                 *  - Creating the file if it doesn't exist.
                 *  - Truncating it to 0 size if it already exists. (not really needed)
                 * Note: "O_WRONLY" mode is not sufficient when mmaping.
                 */
                this->fd = open(DataFileName, O_RDWR | O_CREAT | O_TRUNC, (mode_t)0600);
                break;
            default:
                perror("error argument");
        }

        if ((*fmmap)->fd == -1)
        {
            perror ("open");
        }

        //preparation for mmap
        struct stat sb;
        switch (type)
        {
            case ('r'):
                if (fstat (this->fd, &sb) == -1)
                {
                    perror ("fstat");
                }

                if (!S_ISREG (sb.st_mode))
                {
                    perror("not a file");
                }
                (*fmmap)->fileSize = sb.st_size;

                (*fmmap)->file = (char *)mmap (0, sb.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, (*fmmap)->fd, 0);
                if ((*fmmap)->file == MAP_FAILED)
                {
                    perror ("mmap");
                    if(sb.st_size == 0 && ctype == 'r')
                        fprintf(stderr, "Read NULL file.\n");
                    exit(ERROR_openFile);
                }
                break;
            case ('w'):
                {
                    /* Stretch the file size to the size of the (mmapped) array of ints */
                    //pbm image size Binary
                    (*fmmap)->fileSize = getFileSize(pic);
                    //pgm image
                    if(pic->maxValue != 1)
                        (*fmmap)->fileSize ++;
                    (*fmmap)->fp = lseek((*fmmap)->fd, (*fmmap)->fileSize-1, SEEK_SET);
                    if ((*fmmap)->fp == -1)
                    {
                        close((*fmmap)->fd);
                        perror("Error calling lseek() to 'stretch' the file");
                    }
                    /* Something needs to be written at the end of the file to
                     * have the file actually have the new size.
                     * Just writing an empty string at the current file position will do.
                     * Note:   - The current position in the file is at the end of the stretched
                     *    file due to the call to lseek().
                     *  - An empty string is actually a single '\0' character, so a zero-byte
                     *    will be written at the last byte of the file.  */
                    this->fp = write((*fmmap)->fd, "", 1);
                    if (this->fp != 1)
                    {
                        close((*fmmap)->fd);
                        perror("Error writing last byte of the file");
                    }

                    (*fmmap)->file = mmap (0, (*fmmap)->fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, (*fmmap)->fd, 0);
                    if ((*fmmap)->file == MAP_FAILED)
                    {
                        perror ("mmap");
                    }
                }
                break;
            default:
                perror("error type");
        }
    }

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
