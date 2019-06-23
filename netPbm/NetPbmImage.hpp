#ifndef __NETPBMIMAGE_HPP__
#define __NETPBMIMAGE_HPP__
/*
definition of PBM and PGM images
Magic Number for netpbm image
   "P1" (ASCII codes 80 and 49): PBM ASCII black+white
   "P2" (ASCII codes 80 and 50): PGM ASCII grayscale
   "P3" (ASCII codes 80 and 51): PPM ASCII colored
   "P4" (ASCII codes 80 and 52): PBM RAW black + white
   "P5" (ASCII codes 80 and 53): PGM RAW grayscale
   "P6" (ASCII codes 80 and 54): PPM RAW colored 
*/

#include <string>
#include <cstdio>

#include "ImageData.hpp"

// 0: use 1D array to store image
// 1: use 2D array to store image
#define MATRIXTYPE 0 

namespace netPbm
{
    class NetPbmImage
    {
        protected:
            ImageData* data;
            int maxValue;       //PGM Format: max value for black. Normally it is 255 or 65536

        public:
            virtual ~NetPbmImage();
    
            virtual bool ReadNetPbmHead(FILE *fp) = 0;
            virtual void Read(FILE* fp) = 0;
            virtual bool Write(const std::string filePath) = 0; 
    };

    class GrayColorNetPbmImageASCII: public NetPbmImage
    {
        public:
            bool ReadNetPbmHead(FILE *fp);
            void Read(FILE* fp);
    };

    class P1_PBM_ASCII: public NetPbmImage
    {
        public:
            bool Write(const std::string filePath); 
    };

    class P2_PGM_ASCII: public GrayColorNetPbmImageASCII
    {
        public:
            bool Write(const std::string filePath); 
    };

    class P4_PBM_RAW()
    {

    }

    NetPbmImage* ReadNetPbmImage(const std::string filePath);
}

#endif
