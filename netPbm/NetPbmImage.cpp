#include "stdio.h"
#include "stdlib.h"

#include "NetPbmImage.hpp"

//*******************************************************
// Read and write the pgm file, store the image into a matrix
//*******************************************************
void NetPbmImage::WriteData(FILE *fp)
{

}

void NetPbmImage::WriteNetPbm(char *filePath)
{
	FILE *fp = fopen(filePath, "wt");
	if(!fp)
	{
        throw "Can't open file " + filePath;
	}
	
	switch (pic->maxValue != 1)
	{
	case 0://acsii pbm "P1" format
        fprintf(fp, "P1\n");
	    fprintf(fp, "%d %d\n", this.width, this.hight);
		break;
	case 1:
        fprintf(fp, "P2\n");
	    fprintf(fp, "%d %d %d\n", this.width, this.hight, this.maxValue);
		break;
	case 2:	//RAW pbm "P4" format
		WriteNetPbmMMap(DataFileName, pic);
		break;
	}

    for (int i = 0; i < this.hight; i++)
    {
        for (int j = 0; j < this.width; j++)
            fprintf(fp, "%d ", picdata(i, j)>=1?1:0);
        fprintf(fp, "\n");
    }
	fclose(fp);
}

/* SKIP COMMENT */
/* This function skips past a comment in a file. The comment */
/* begins with a '#' character and ends with a newline character. */
/* The function returns EOF if there's an error. */
int SkipNetPbmComment(FILE *fp)
{
	int i;
	while ((i = getc(fp)) == '\n' && i != EOF);
	ungetc(i, fp);
	if ((i = getc(fp)) == '#')
		while ((i = getc(fp)) != '\n' && i != EOF);
	return(ungetc(i, fp));
}

/* READ PGM HEADER */
/* This function reads the header of a PGM image. */
/* The dimensions are returned as arguments. */
/* This function ensures that there's no more than 8 bpp. */
/* The return value is false if there's an error. i*/
bool ReadNetPbmHead(FILE *fp, int *ncols, int *nrows, int *maxval)
{
//	char filetype[3];
	if (	SkipNetPbmComment(fp) == EOF
//	        || fscanf(fp, "%2s", filetype) != 1
//	        || strcmp(filetype, "P2")
	        || SkipNetPbmComment(fp) == EOF
	        || fscanf(fp, "%d", ncols) != 1
	        || SkipNetPbmComment(fp) == EOF
	        || fscanf(fp, "%d", nrows) != 1
//	        || SkipComment(fp) == EOF
//	        || fscanf(fp, "%d", maxval) != 1
//	        || (*maxval) > 255
	   )
		return false;
	//"P1" pbm format
	if(*maxval==1)
		return true;
	//"P2" or "P3" pgm format
	if(	SkipNetPbmComment(fp) == EOF
		|| fscanf(fp, "%d", maxval) != 1
	)
		return false;
	return true;
}

void NetPbmImage::ReadNetPbm(char *filePath)
{
	FILE *fp = fopen(DataFileName, "r");
	if(!fp)
	{
        throw "Can't open file " + filePath;
	}
	
    //read the head of the netpbm pic
	char ctemp[bufferSize];
	fscanf(dfp, "%c", ctemp);
	if(ctemp[0] != 'P')
	{
        throw "Can't open file " + filePath;
	}
	
	fscanf(dfp, "%c", ctemp);
	int nrows=0, ncols=0, maxval=0;
	switch(ctemp[0])
	{
		case '1'://pbm ASCII
			maxval = 1;
			if ( ReadNetPbmHead(dfp, &ncols, &nrows, &maxval)== false)
			{
				sprintf(ctemp, "readPGMFile: ReadDataFile: File %s header error.", DataFileName);
				printErrorMsg(ERROR_openFile, ctemp);
			}
			InitPIC(pic, ncols, nrows, maxval);
			int i, j;
			for (i=0; i<(*pic)->hight; i++)
				for (j=0; j<(*pic)->width; j++)
					fscanf(dfp, "%i", &(picdatastar(i, j)));
			fclose(dfp);
			break;
		case '2'://pgm ASCII
			if ( ReadNetPbmHead(dfp, &ncols, &nrows, &maxval)== false)
			{
				sprintf(ctemp, "readPGMFile: ReadDataFile: File %s header error.", DataFileName);
				printErrorMsg(ERROR_openFile, ctemp);
			}
			InitPIC(pic, ncols, nrows, maxval);
			for (i=0; i<(*pic)->hight; i++)
				for (j=0; j<(*pic)->width; j++)
					fscanf(dfp, "%i", &picdatastar(i, j));
			fclose(dfp);
			break;
		case '3'://ppm ASCII
			if ( ReadNetPbmHead(dfp, &ncols, &nrows, &maxval)== false)
			{
				sprintf(ctemp, "readPGMFile: ReadDataFile: File %s header error.", DataFileName);
				printErrorMsg(ERROR_openFile, ctemp);
			}
			InitPIC(pic, ncols, nrows, maxval);
			ExtendPIC(*pic);
			for (i=0; i<(*pic)->hight; i++)
				for (j=0; j<(*pic)->width; j++)
				{
					fscanf(dfp, "%i %i %i", &picdatastar(i, j), &picdatastar2(i, j), &picdatastar3(i, j));
				}
			fclose(dfp);
			break;
		case '4'://pbm RAW
			fclose(dfp);
			ReadNetPbmMMap(DataFileName, pic);
			break;
		default:
			fclose(dfp);
			fprintf(stderr, "Can't read P%c type RAW file.\n", ctemp[0]);
			exit(0);
	}
}

