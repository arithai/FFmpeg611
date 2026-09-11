#ifndef STTD
//t00fi.txt       ;frame_index
//(12,34)(1,2,3)(4,5,6);          (px,py)(Y.U.V)(R,G,B) t0
//(11,22) (55,66)-(77,88);         t1  x    x
//(11,22)-(33,44) (55,66)-(77,88); t2   x  x
//(11,22)-(33,44) (55,66)-(77,88); t3    o x x
//(11,22)-(33,44) (55,66)-(77,88); t4   x  x
//(11,22)-(33,44) (55,66)-(77,88); t5   x  x
//(11,22)(1,2,3)(4,5,6)          ; t6 u
typedef struct ST_T {
  int x0,y0;
  unsigned char Y0,U0,V0,R0,G0,B0;
  int y1i,y1j,x1s,y1s,x1f,y1f; //x(0,2159)
  int x2i,y2i,x2j,x2s,y2s,x2f,y2f; //y(0,3839)
  int x3i,x3p,y3p,x3s,y3s,x3f,y3f;
  int x4i,y4i,x4s,y4s,x4f,y4f;
  int x5i,y5i,x5s,y5s,x5f,y5f;
  int x6,y6;
  unsigned char Y6,U6,V6,R6,G6,B6;
  bool isUpdate;
} STT;
extern STT stt; //define in SDLmain.cpp
void savestt(int picID);
#endif
#define STTD 1

