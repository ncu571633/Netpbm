#ifndef __FILEMMAP_HPP__
#define __FILEMMAP_HPP__

#include <cstddef>  // size_t
namespace netPbm
{
    class FileMMap
    {
        public:
            FileMap();
            ~FileMMap();

            char *file;	//start of the file
            char *data; //start of the data
            std::size_t fp;	//file pointer
            std::size_t fileSize;	//file size
            int fd;	//file descriptor
    };

    class FileMMapUtility
    {
        static int getCharBit(unsigned char c, int nBit);
        static int setCharBit(unsigned char c, int nBit, bool value);
        static int intlen(int i);
        static char * f_itoa(int value, char * str, int strSize);
        static int getFileSize(int row, int column);
    };
}

#endif
