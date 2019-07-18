#include <cstdlib>

#include "NetPbmImage.hpp"

namespace netPbm
{
    NetPbmImage::~NetPbmImage()
    {
        if(this->data)
        {
            delete(this->data);
        }
        this->data = nullptr;
    }
    
    void NetPbmImageASCII::Read(void *fp)
    {
        this->ReadNetPbmHead(fp);
        this->data->Read((FILE *)fp);
    }

    /* SKIP COMMENT */
    /* This function skips past a comment in a file. The comment */
    /* begins with a '#' character and ends with a newline character. */
    /* The function returns EOF if there's an error. */
    int NetPbmImageASCII::SkipNetPbmComment (void *p)
    {
        FILE *fp = (FILE*)p;
        int i;
        while ((i = getc(fp)) == '\n' && i != EOF);
        ungetc(i, fp);
        if ((i = getc(fp)) == '#')
            while ((i = getc(fp)) != '\n' && i != EOF);
        return (ungetc(i, fp));
    }

    /* READ PGM HEADER */
    /* This function reads the header of a NetPbm image. */
    /* The dimensions are returned as arguments. */
    /* The return value is false if there's an error. */
    bool NetPbmImageASCII::ReadNetPbmHead(void *p)
    {
        FILE* fp = (FILE*)p;
        
        // read row and column
        int column = 0, row = 0;
        bool ret = !(SkipNetPbmComment(fp) == EOF
            || SkipNetPbmComment(fp) == EOF
            || fscanf(fp, "%d", &column) != 1
            || SkipNetPbmComment(fp) == EOF
            || fscanf(fp, "%d", &row) != 1);

        this->data = new 
#if MATRIXTYPE 
            VectorImageData(row, column);
#else
            MatrixImageData(row, column);
#endif
        return ret;
    }
    
    bool P2_PGM_ASCII::ReadNetPbmHead(void *fp)
    {
        NetPbmImageASCII::ReadNetPbmHead(fp);
        
        // P2, P3, P5, P6 pgm format
        return fscanf((FILE*)fp, "%d", &(this->maxValue)) == 1;
    }

    bool P1_PBM_ASCII::Write(const std::string& filePath)
    {
        FILE *fp = fopen(filePath.c_str(), "wt");
        if(!fp)
        {
            return false;
        }
                
        //acsii pbm "P1" format
        fprintf(fp, "P1\n");
        fprintf(fp, "%d %d\n", this->data->column, this->data->row);
        this->data->Write(fp);
        fclose(fp);

        return true;
    }

    bool P2_PGM_ASCII::Write(const std::string& filePath)
    {
        FILE *fp = fopen(filePath.c_str(), "wt");
        if(!fp)
        {
            return false;
        }

        //"P2" format
        fprintf(fp, "P2\n");
        fprintf(fp, "%d %d %d\n", this->data->column, this->data->row, this->maxValue);
        this->data->Write(fp);
        fclose(fp);

        return true;
    }

    // entry point: read netPbm image into memory
    NetPbmImage* ReadNetPbmImage(const std::string& filePath)
    {
        FILE *fp = fopen(filePath.c_str(), "r");
        if(!fp)
        {
            return nullptr;
        }

        //read the head of the netpbm pic
        char c;
        fscanf(fp, "%c", &c);
        if(c != 'P')
        {
            return nullptr;
        }

        fscanf(fp, "%c", &c);
        NetPbmImage* ret = nullptr;
        switch(c)
        {
            case '1'://pbm ASCII
                ret = new P1_PBM_ASCII();
                ret->Read(fp);
                fclose(fp);
                break;
            case '2'://pgm ASCII
                ret = new P2_PGM_ASCII();
                ret->Read(fp);
                fclose(fp);
                break;
            case '4'://pbm RAW
                fclose(fp);
                ret = ReadNetPbmRAWImage(filePath);
            default:
                // not implemented at this time
                fclose(fp);
                return nullptr;
        }

        return ret;
    }

}
