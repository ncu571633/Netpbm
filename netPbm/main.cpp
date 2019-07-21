#include "NetPbmImage.hpp"

int main()
{
    netPbm::NetPbmImage* p = netPbm::ReadNetPbmImage("resource/p1.pbm");
    p->Write("p1.pbm");
    delete(p);
    
    p = netPbm::ReadNetPbmImage("resource/p2.pgm");
    p->Write("p2.pgm");
    delete(p);
    
    p = netPbm::ReadNetPbmImage("resource/p4.pbm");
    p->Write("p4.pbm");
    delete(p);
    
    return 1;
}
