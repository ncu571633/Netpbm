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

#include <vector>

class PIC
{
private:
#if 0 
	int **data;		//2D Matrix: data matrix used to store pixels
	int **check; 	//check matrix used to show whether the pixel is checked or not
#endif
	vector<int> data;	//1D Matrix(Array): data matrix used to store pixels
	vector<int> check; 	//check matrix: whether the pixel is checked/visted or not
	int width;          //image width
	int hight;          //image height
	int maxValue;       //PGM Format: max value for black. Normally it is 255 or 65536

public:
    void WriteNetPbm(char *filePath); 
    void ReadNetPbm(char *filePath); 
};

