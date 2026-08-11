/*
 * Copyright (c) 2010 Nicolas George
 * Copyright (c) 2011 Stefano Sabatini
 * Copyright (c) 2026 Arithai Yang
 */
//toolbox.c ArithAI Yang 2025.01.17
//Purpose: Graphics Draw ToolBox
//畫面,畫線 fill_yuv_image AVFrame *pict; frame_index 
#define _XOPEN_SOURCE 600 /* for usleep */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
extern "C" {
  #include <libavcodec/avcodec.h>
  #include <libavformat/avformat.h>
  #include <libavfilter/buffersink.h>
  #include <libavfilter/buffersrc.h>
  #include <libavutil/opt.h>
  #include <libavutil/imgutils.h>
  #include <libavutil/frame.h>
}

#define CLIP(X) ( (X) > 255 ? 255 : (X) < 0 ? 0 : X)

// RGB -> YUV
#define RGB2Y(R, G, B) CLIP(( (  66 * (R) + 129 * (G) +  25 * (B) + 128) >> 8) +  16)
#define RGB2U(R, G, B) CLIP(( ( -38 * (R) -  74 * (G) + 112 * (B) + 128) >> 8) + 128)
#define RGB2V(R, G, B) CLIP(( ( 112 * (R) -  94 * (G) -  18 * (B) + 128) >> 8) + 128)

// YUV -> RGB
#define C(Y) ( (Y) - 16  )
#define D(U) ( (U) - 128 )
#define E(V) ( (V) - 128 )

#define YUV2R(Y, U, V) CLIP(( 298 * C(Y)              + 409 * E(V) + 128) >> 8)
#define YUV2G(Y, U, V) CLIP(( 298 * C(Y) - 100 * D(U) - 208 * E(V) + 128) >> 8)
#define YUV2B(Y, U, V) CLIP(( 298 * C(Y) + 516 * D(U)              + 128) >> 8)

// RGB -> YCbCr
#define CRGB2Y(R, G, B) CLIP((19595 * R + 38470 * G + 7471 * B ) >> 16)
#define CRGB2Cb(R, G, B) CLIP((36962 * (B - CLIP((19595 * R + 38470 * G + 7471 * B ) >> 16) ) >> 16) + 128)
#define CRGB2Cr(R, G, B) CLIP((46727 * (R - CLIP((19595 * R + 38470 * G + 7471 * B ) >> 16) ) >> 16) + 128)

// YCbCr -> RGB
#define CYCbCr2R(Y, Cb, Cr) CLIP( Y + ( 91881 * Cr >> 16 ) - 179 )
#define CYCbCr2G(Y, Cb, Cr) CLIP( Y - (( 22544 * Cb + 46793 * Cr ) >> 16) + 135)
#define CYCbCr2B(Y, Cb, Cr) CLIP( Y + (116129 * Cb >> 16 ) - 226 )

#define WHITEY 255
#define WHITEU 128
#define WHITEV 128
#define BLACKY 0
#define BLACKU 128
#define BLACKV 128
#define REDY 76
#define REDU 84
#define REDV 255
#define GREENY 149
#define GREENU 43
#define GREENV 21
#define BLUEY 29
#define BLUEU 255
#define BLUEV 107
#define YELLOWY 255
#define YELLOWU 0
#define YELLOWV 148
//淡粉紅
#define PURPLEY 180
#define PURPLEU 170
#define PURPLEV 181
//青色
#define CYANY 178
#define CYANU 171 
#define CYANV 0
    
#define GLOBAL_WIDTH  640*6
#define GLOBAL_HEIGHT 360*6

typedef struct STPOINT {
  unsigned short x;  //0..3840-1
  unsigned short y;  //0..2160-1
  unsigned short nr; //0..65535
  int w;
} ST_POINT;
typedef struct STPONTLIST {
  ST_POINT pt;
  struct STPONTLIST *next;
} ST_POINT_LIST;
extern ST_POINT_LIST *ptHead,*ptTail;
void ptSet(unsigned short x,unsigned short y,int weight);
void ptFree();
void ptListAll();
#define maxFirst 32
extern ST_POINT_LIST *ptFirst,*ptFirstG;
void ptGetFirst();
void ptGetFirstG();
#include <string.h>
#include <math.h>
extern "C" {
  #include <libavutil/avassert.h>
  #include <libavutil/channel_layout.h>
  #include <libavutil/opt.h>
  #include <libavutil/mathematics.h>
  #include <libavutil/timestamp.h>
  #include <libavcodec/avcodec.h>
  #include <libavformat/avformat.h>
  #include <libswscale/swscale.h>
  #include <libswresample/swresample.h>
}
#define STREAM_DURATION   10.0
#define STREAM_FRAME_RATE 25 /* 25 images/s */
#define STREAM_PIX_FMT    AV_PIX_FMT_YUV420P /* default pix_fmt */
#define SCALE_FLAGS SWS_BICUBIC
#include <stdbool.h>
#define MAX_JUMP 2
extern AVFrame *filt_frame;
extern int frameWidth,frameHeight;
extern unsigned char *Ybefore;
extern unsigned char *Ubefore;
extern unsigned char *Vbefore;
extern unsigned char *Ynow;
extern unsigned char *Unow;
extern unsigned char *Vnow;
extern unsigned char *Ydiffnow;
extern unsigned char *Udiffnow;
extern unsigned char *Vdiffnow;
extern unsigned char *Yref;
extern unsigned char *Uref;
extern unsigned char *Vref;
extern unsigned char *Rnow;
extern unsigned char *Gnow;
extern unsigned char *Bnow;
extern int Ylinesize,Ulinesize,Vlinesize;
extern bool marginalV(int x,int y);
extern int HXYR[60][60];
extern int HXYG[60][60];
extern int HXYB[60][60];
extern int HXYY[60][60];
extern int HXYV[30][30];
extern int HXYU[30][30];
extern int EX[3840];
extern int EY[3840];
void calc_64x36(AVFrame *pict, int frame_index,
                int width, int height);
                //邊緣,界,端點,起點,終點
#define edgeSpace      1               //左右上下邊緣不算
#define maxDeltaSpace  1               //最長不是線的空白
#define minPointNrLine 6               //最少線的點數
typedef struct STLINE {
  short c0;  //0..3840-1 =coordinate
  short c1;  //0..65535
  int w;     //點數
} ST_LINE;
typedef struct STLINELIST {
  ST_LINE L;
  struct STLINELIST *next;
} ST_LINE_LIST;
extern ST_LINE_LIST *lnHead;
void initLineList(int width,int height);
void freeLineList();
#define MAX_JUMP_LINE 12
bool marginalLine(int x,int y);
//(x0,y0 ...x,y,... x1,y1)
#define ratioS  1.33333
#define ratioT -1.33333
#define MAX_JUMP_SLOPE 12
typedef struct STSLOPE { //Slope A,B
  short y0;
  short c0;  //0..3840-1 =coordinate
  short c1;  //0..65535
  int w;     //點數
} ST_SLOPE;
typedef struct STSLOPELIST {
  ST_SLOPE s;
  struct STSLOPELIST *next;
} ST_SLOPE_LIST;
extern ST_SLOPE_LIST *slHead;
void initSlopeList(int width,int height);
void freeSlopeList();
#define sign(x) (x>0?1:-1)
#define maxDeltaSlopeSpace 1
extern bool marginalSlope(int x,int y,int ystart,float ratio,int width,int height);
//XYZST
//Xfi X frame index
//PQR R rect
extern int wp,
    Xfi,XposLine,Xc0,Xc1,Xw,
    Yfi,YposLine,Yc0,Yc1,Yw,
    Zfi,ZposLine,Zc0,Zc1,Zw,
    Sfi,SposSlope,Sc0,Sc1,Sy0,Sw,
    Tfi,TposSlope,Tc0,Tc1,Ty0,Tw
    ;
void FindXYZSTLineList(AVFrame *pict, int frame_index,
                    int width, int height);
extern int Xmax,Xleft,Xright,Ymax,Yup,Ydown;
//#define MAXNR_PT 64
//extern int HX[GLOBAL_WIDTH],HY[GLOBAL_HEIGHT];
extern int HX[GLOBAL_HEIGHT/2],HY[GLOBAL_WIDTH/2];
void calc_histogram(AVFrame *pict, int frame_index,
                          int width, int height);

extern void calc_nb(int x,int y);
extern void calc_ref(int frame_index,int x,int y);
extern int rn[3][3],gn[3][3],bn[3][3];
extern int rb[3][3],gb[3][3],bb[3][3];
extern int re,ge,be,ra,ga,ba;
extern int imaxG,imaxB,imaxR;
#define a(v) (v<128? 0:255)  
void fill_yuv_image(AVFrame *pict, int frame_index,
                    int width, int height)
{
    int x, y, i, r,g,b;
    int x2, y2;
    int xs60,ys60;           
    int Y0,U0,V0,Y1,U1,V1,R0,G0,B0,G1,R1,B1;
#if 1
    int Xmid=(Xleft+Xright)/2;
    int Ymid=(Yup+Ydown)/2;
    Xmid = Xmid;
    Ymid = Ymid;
#endif    
    r=0;b=0;r=r;b=b;
    R0=0;B0=0;R1=0;B1=0;R0=R0;B0=B0;R1=R1;B1=B1;
    xs60=width/60;
    ys60=height/60;
    xs60=xs60;ys60=ys60;
    i = frame_index; i = i;
#if 1
    printf("%s(%d),h,w=(%d,%d),(%d,%d,%d),(%d,%d,%d)\n",
           __FILE__,__LINE__,height,width, 
           pict->linesize[0],pict->linesize[1],pict->linesize[2],
           filt_frame->linesize[0],filt_frame->linesize[1],filt_frame->linesize[2]
        );  
#endif     
  //printf("%s(%d) (%d,%d) (%X,%X))\n",__FILE__,__LINE__,xs60,ys60,ptFirst,ptFirstG);
#if 0
    /* Y */
    for (y = 0; y < height; y++)
        for (x = 0; x < width; x++)
            pict->data[0][y * pict->linesize[0] + x] = x + y + i * 3;
    /* Cb and Cr */
    for (y = 0; y < height / 2; y++) {
        for (x = 0; x < width / 2; x++) {
            pict->data[1][y * pict->linesize[1] + x] = 128 + y + i * 2;
            pict->data[2][y * pict->linesize[2] + x] = 64 + x + i * 5;
        }
    }
#endif
//follow below for 樹葉
#if 0
    calc_ref(frame_index,width,height);
    printf("%s(%d) (%d,%d) (%X,%X))\n",__FILE__,__LINE__,xs60,ys60,ptFirst,ptFirstG);
    for (y = 0; y < height; y++) {
      y30=y/ys60;
      y30=y30;
      y2=y/2;
      if(y2==Sy0) {
//      exit(0);
      }
      for (x = 0; x < width; x++) {
        x30=x/xs60;
        x30=x30;
        x2=x/2;
        pos = y * Ylinesize+ x; 
        posU=y2 * Ulinesize + x2; 
        posV=y2 * Vlinesize + x2; 
      //if(frame_index) 
      //  printf("%s(%d)(%4d,%4d) (%6d),%X\n",__FILE__,__LINE__,y,x,HXYG[x30][y30],ptFirstG);
        R1=Rnow[pos];G1=Gnow[pos];B1=Bnow[pos];
        #if 0
        Y1=Ynow[pos];
        U1=Unow[posU];
        V1=Vnow[posV];
        #endif
        #if 1
        Y1=Ydiffnow[pos];
        U1=Udiffnow[posU];
        V1=Vdiffnow[posV];
        #endif
        pos2 = y * pict->linesize[0]+ x; 
        pos2U=y2 * pict->linesize[1] + x2; 
        pos2V=y2 * pict->linesize[2] + x2; 
        if(Yref[pos]==GREENY && Uref[posU]==GREENU && Vref[posV]==GREENV) {
          pict->data[0][pos2]  = GREENY;  //BLUE
          pict->data[1][pos2U] = GREENU; 
          pict->data[2][pos2V] = GREENV;
        }
        else if(Yref[pos]==BLACKY && Uref[posU]==BLACKU && Vref[posV]==BLACKV) {
          pict->data[0][pos2]  = BLACKY;  //BLUE
          pict->data[1][pos2U] = BLACKU; 
          pict->data[2][pos2V] = BLACKV;
        }
        else if(Yref[pos]==PURPLEY && Uref[posU]==PURPLEU && Vref[posV]==PURPLEV) {
          pict->data[0][pos2]  = PURPLEY;  //BLUE
          pict->data[1][pos2U] = PURPLEU; 
          pict->data[2][pos2V] = PURPLEV;
        }
        #if 0
        else if(x>Xmid-10&&x<Xmid+10&&y>Ymid-10&&y<Ymid+10) {
          pict->data[0][pos2]  = BLUEY;  //BLUE
          pict->data[1][pos2U] = BLUEU; 
          pict->data[2][pos2V] = BLUEV; 
        }
        #endif
        #if 0
        else if(x>Xmid-10&&x<Xmid+10&&y>Ymid-10&&y<Ymid+10) {
          pict->data[0][pos2]  = BLACKY; //0x0;  
          pict->data[1][pos2U] = BLACKU; //0xFF;
          pict->data[2][pos2V] = BLACKV; //0x0;
        }
        #endif
        #if 0
        else if( (y2 >= YposLine && y2 < (YposLine+5) && x2>=Yc0 && x2<=Yc1) 
              || (x2 >= XposLine && x2 < (XposLine+5) && y2>=Xc0 && y2<=Xc1)
              || (x2 >= ZposLine && x2 < (ZposLine+5) && y2>=Zc0 && y2<=Zc1)
            ) {
//        printf("%s(%d)(%4d,%4d)\n",__FILE__,__LINE__,y,x);
          pict->data[0][pos]  = 0xFF; //YELLOW
          pict->data[1][posU] = 0x00;
          pict->data[2][posV] = 0xFF;
        }
        #endif
        #if 0
        else if(marginalV(x2,y2)) {
          pict->data[0][pos]  = PURPLEY; //0x0;  //BLACK
          pict->data[1][posU] = PURPLEU; //0x33;
          pict->data[2][posV] = PURPLEV; //0x88;
        }
        #endif 
        #if 0
        else if(marginalV(x2,y2)) { //為何邊黃
          pict->data[0][pos]  = 0x0;  
          pict->data[1][posU] = 0x33;
          pict->data[2][posV] = 0x88;
        }
        #endif
        #if 0
        else if(HXYV[x30/2][y30/2] >= ptFirst->pt.w) {
          pict->data[0][pos]  = 0x0;   //GREEN
          pict->data[1][posU] = 0x22;  //correlated line 
          pict->data[2][posV] = 0x66;
        }
        #endif
        #if 0
        else if(HXYV[x30/2][y30/2] >= ptFirst->pt.w) {
          pict->data[0][pos]  = GREENY;   //GREEN
          pict->data[1][posU] = GREENU;  //correlated line 
          pict->data[2][posV] = GREENV;
        }
        #endif
        #if 0
        else if(HXYV[x30/2][y30/2] >= ptFirst->pt.w ) {
          pict->data[0][pos]  = BLUEY;   //BLUE
          pict->data[1][posU] = BLUEU;
          pict->data[2][posV] = BLUEV;
        }
        #endif
        #if 0
        else if(HXYG[x30][y30] >= ptFirstG->pt.w ) {
//        printf("%s(%d)(%4d,%4d) (%6d,%6d) <======\n",__FILE__,__LINE__,y,x,HXYG[x30][y30],ptFirstG->pt.w);
          pict->data[0][pos]  = REDY;   
          pict->data[1][posU] = REDU;
          pict->data[2][posV] = REDV;
          //128,148 WHITEY also YELLOWY 255
          #define z(s,t) ((Yref[(t) * Ylinesize+ (s)]==WHITEY)?0:1) 
          if(y30>5 && y30<55 && x30>5 && x30 <55) {
          
            if((y-y30*ys60)==ys60/2) { //only for corner
              for (x0=x+xs60;x0<width-xs60;x0++) {
                if( ( z(x0-1,y-1)+z(x0,y-1)+z(x0+1,y-1)
                     +z(x0-1,y)  +z(x0,y  )+z(x0+1,y  )
                     +z(x0-1,y+1)+z(x0,y+1)+z(x0+1,y+1))
                   >=4) {
                  Yref[y * Ylinesize+ x0]  =YELLOWY;
                  Uref[y2* Ulinesize+ x0/2]=YELLOWU;
                  Vref[y2* Vlinesize+ x0/2]=YELLOWV;
//                printf("%s(%d)(%4d,%4d)-%4d,(%4d,%4d))\n",__FILE__,__LINE__,y,x,x0,xs60,ys60);
                  break;
                }
              }  
              for (x0=x-xs60;x0>=xs60;x0--) {
                if( ( z(x0-1,y-1)+z(x0,y-1)+z(x0+1,y-1)
                     +z(x0-1,y)  +z(x0,y  )+z(x0+1,y  )
                     +z(x0-1,y+1)+z(x0,y+1)+z(x0+1,y+1))
                   >=4) {
                  Yref[y * Ylinesize+ x0]  =YELLOWY;
                  Uref[y2* Ulinesize+ x0/2]=YELLOWU;
                  Vref[y2* Vlinesize+ x0/2]=YELLOWV;
                  break;
                }
              }
            }

            if((x-x30*xs60)==xs60/2) {
              for (y0=y+ys60;y0<height-ys60;y0++) {
                if( ( z(x-1,y0-1)+z(x,y0-1)+z(x+1,y0-1)
                     +z(x-1,y0)  +z(x,y0  )+z(x+1,y0  )
                     +z(x-1,y0+1)+z(x,y0+1)+z(x+1,y0+1))
                   >=4) {
                  Yref[y0 * Ylinesize+  x] =YELLOWY;
                  Uref[y0/2* Ulinesize+ x2]=YELLOWU;
                  Vref[y0/2* Vlinesize+ x2]=YELLOWV;
//                printf("%s(%d)(%4d,%4d)-%4d,(%4d,%4d))\n",__FILE__,__LINE__,y,x,x0,xs60,ys60);
                  break;
                }
              }  
           
              for (y0=y-ys60;y0>=ys60;y0--) {
                if( ( z(x-1,y0-1)+z(x,y0-1)+z(x+1,y0-1)
                     +z(x-1,y0)  +z(x,y0  )+z(x+1,y0  )
                     +z(x-1,y0+1)+z(x,y0+1)+z(x+1,y0+1))
                   >=4) {
                  Yref[y0 * Ylinesize+  x] =YELLOWY;
                  Uref[y0/2* Ulinesize+ x2]=YELLOWU;
                  Vref[y0/2* Vlinesize+ x2]=YELLOWV;
                  break;
                }
              } 
            }
 
          }  
        }
        #endif  
        else {
          #if 0
     //   printf("%s(%d)(%4d,%4d)\n",__FILE__,__LINE__,y,x);
          if(Rnow[pos]==imaxR && Gnow[pos]==imaxG && Bnow[pos]==imaxB) {
            pict->data[0][pos2]  = CYANY;
            pict->data[1][pos2U] = CYANU;
            pict->data[2][pos2V] = CYANV;          
          }
          else 
          #endif     
          {
            pict->data[0][pos2]  = Yref[pos ];
            pict->data[1][pos2U] = Uref[posU];
            pict->data[2][pos2V] = Vref[posV];
          }
          #if 0
          pict->data[0][pos2]  = Y1;
          pict->data[1][pos2U] = U1;
          pict->data[2][pos2V] = V1;
          #endif
        }
      }    
   // printf("%s(%d)(%4d,%4d)(%5d,%5d,%5d)\n",__FILE__,__LINE__,y,x,pos,posU,posV);
    }
    #if 1 
    printf("%s(%d)[%4d],(%d,%d,%d)(%d,%d,%d)(%d,%d,%d)(%d,%d,%d,%d)(%d,%d,%d,%d)\n",
    __FILE__,__LINE__,
    frame_index,XposLine,Xc0,Xc1,YposLine,Yc0,Yc1,ZposLine,Zc0,Zc1,
    SposSlope,Sc0,Sc1,Sy0,TposSlope,Tc0,Tc1,Ty0);
    #endif
    printf("%s(%d)\n",__FILE__,__LINE__);
    #if 0
    for (x0 = Sc0; x0 < Sc1; x0++) {
//      printf("%s(%d)\n",__FILE__,__LINE__);
      y0=(int)(Sy0+x0*ratioS);
      for (y=0;y<5;y++) {
        if(y0<(height/2-1)) {
//        printf("%s(%d)(%4d,%4d)\n",__FILE__,__LINE__,x0,y0);
          pict->data[0][2*y0 *      pict->linesize[0] + 2*x0]   = BLUEY;
          pict->data[0][2*y0 *      pict->linesize[0] + 2*x0+1] = BLUEY;
          pict->data[0][(2*y0+1) *  pict->linesize[0] + 2*x0]   = BLUEY;
          pict->data[0][(2*y0+1) *  pict->linesize[0] + 2*x0+1] = BLUEY;
          pict->data[1][y0 * pict->linesize[1] + x0] = BLUEU;
          pict->data[2][y0 * pict->linesize[2] + x0] = BLUEV;
          y0++;
        }  
      }          
    }
    #endif
    #if 0
    for (x0 = Tc0; x0 < Tc1; x0++) {
//    printf("%s(%d)\n",__FILE__,__LINE__);
      y0=(int)(Ty0+x0*ratioT);
      for (y=0;y<5;y++) {
        if(y0>0) {
//        printf("%s(%d)(%4d,%4d)\n",__FILE__,__LINE__,x0,y0);
          pict->data[0][2*y0 *      pict->linesize[0] + 2*x0]   = BLUEY;
          pict->data[0][2*y0 *      pict->linesize[0] + 2*x0+1] = BLUEY;
          pict->data[0][(2*y0+1) *  pict->linesize[0] + 2*x0]   = BLUEY;
          pict->data[0][(2*y0+1) *  pict->linesize[0] + 2*x0+1] = BLUEY;
          pict->data[1][y0 * pict->linesize[1] + x0] = BLUEU;
          pict->data[2][y0 * pict->linesize[2] + x0] = BLUEV;
          y0--;
        }  
      }          
    }
    #endif 
#endif
#if 0
    Xmid=800;Ymid=800;
    for (y = 0; y < height; y++)
      for (x = 0; x < width; x++) {
//       if(x>Xmax-10&&x<Xmax+10&&y>Ymax-10&&y<Ymax+10) {
         if(x>Xmid-100&&x<Xmid+100&&y>Ymid-100&&y<Ymid+100) {
           pict->data[0][y * pict->linesize[0] + x] = 0x0;
         }
         else {
           pict->data[0][y * pict->linesize[0] + x] = 0x0;
         }  
      }    
    /* Cb and Cr */
    for (y = 0; y < height/2; y++) {
       for (x = 0; x < width/2; x++) {  
//       if(x>Xmax/2-10&&x<Xmax/2+10&&y>Ymax/2-10&&y<Ymax/2+10) {
         if(x>Xmid/2-100&&x<Xmid/2+100&&y>Ymid/2-100&&y<Ymid/2+100) {
              pict->data[1][y * pict->linesize[1] + x] = 0;
              pict->data[2][y * pict->linesize[2] + x] = 0xFF;
          }
          else {
              pict->data[1][y * pict->linesize[1] + x] = 0x0;
              pict->data[2][y * pict->linesize[2] + x] = 0x0;
          }    
       }
    }
#endif     
#if 0
    if (frame_index > 0) {
      printf("fill_yuv_image %s(%d)\n",__FILE__,__LINE__);
      for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
          pict->data[0][y * pict->linesize[0] + x] = abs(Ybefore[y * Ylinesize + x]-
                                                     filt_frame->data[0][y * Ylinesize + x]);
        }
      }  
      for (y = 0; y < height/2; y++) {
        for (x = 0; x < width/2; x++) {
          pict->data[1][y * pict->linesize[1] + x] = abs(Ubefore[y * Ulinesize + x]-
                                                     filt_frame->data[1][y * Ulinesize + x]);
          pict->data[2][y * pict->linesize[2] + x] = abs(Vbefore[y * Vlinesize + x]-
                                                     filt_frame->data[2][y * Vlinesize + x]);          
        }
      }        
    }
    else {
      printf("fill_yuv_image %s(%d)\n",__FILE__,__LINE__);
      for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
          pict->data[0][y * pict->linesize[0] + x] = filt_frame->data[0][y * filt_frame->linesize[0] + x];
        }
      }  
      for (y = 0; y < height/2; y++) {
        for (x = 0; x < width/2; x++) {
          pict->data[1][y * pict->linesize[1] + x] = filt_frame->data[1][y * filt_frame->linesize[1] + x];
          pict->data[2][y * pict->linesize[2] + x] = filt_frame->data[2][y * filt_frame->linesize[2] + x];          
        }
      }        
    }
#endif
#if 1
    if (frame_index>0) {
      printf("fill_yuv_image %s(%d),%3d,(%4d,%4d,%4d)\n",__FILE__,__LINE__,frame_index,
             Ylinesize,Ulinesize,Vlinesize);
      for (y = 0; y < height; y++) {
        y2 = y/2;
        for (x = 0; x < width; x++) {
          x2 = x/2;
          g = abs(Ybefore[y * Ylinesize + x]-Ydiffnow[y * Ylinesize + x]);
          
          Y0 = Ybefore[y *   Ylinesize + x];
          U0 = Ubefore[y2 *  Ulinesize + x2];
          V0 = Vbefore[y2 *  Vlinesize + x2];
//        Y1 = Ydiffnow[y *  Ylinesize + x];
//        U1 = Udiffnow[y2 * Ulinesize + x2];
//        V1 = Vdiffnow[y2 * Vlinesize + x2];
          Y1 = Ynow[y *  Ylinesize + x];
          U1 = Unow[y2 * Ulinesize + x2];
          V1 = Vnow[y2 * Vlinesize + x2];
          G0 = YUV2G(Y0,U0,V0); if(G0<200) G0=0; else G0=255;
          G1 = YUV2G(Y1,U1,V1); if(G1<200) G1=0; else G1=255;
          
          g = abs(G0-G1);
          
          if(g > 250) {
            pict->data[0][y *  pict->linesize[0] + x]  = BLACKY;                
            pict->data[1][y2 * pict->linesize[1] + x2] = BLACKU;
            pict->data[2][y2 * pict->linesize[2] + x2] = BLACKV;
          }  
          else {
            pict->data[0][y *  pict->linesize[0] + x]  = WHITEY;                
            pict->data[1][y2 * pict->linesize[1] + x2] = WHITEU;
            pict->data[2][y2 * pict->linesize[2] + x2] = WHITEV;
          }  
        }
      }  
    }
    else {
      printf("fill_yuv_image %s(%d),%3d\n",__FILE__,__LINE__,frame_index);
      for (y = 0; y < height; y++) {
     //   printf("fill_yuv_image %s(%d) (%4d,%4d),%4d,%4d,%4d\n",__FILE__,__LINE__,x,y,
     //          Ylinesize,Ulinesize,Vlinesize);
        y2 = y/2;
        for (x = 0; x < width; x++) {
          x2 = x/2;
          pict->data[0][y *  pict->linesize[0] + x]  = Ydiffnow[y *  Ylinesize + x];
          pict->data[1][y2 * pict->linesize[1] + x2] = Udiffnow[y2 * Ulinesize + x2];
          pict->data[2][y2 * pict->linesize[2] + x2] = Vdiffnow[y2 * Vlinesize + x2];          
        }
      }        
    }
#endif
#if 0
    for (y = 0; y < height; y++)  {
        y2 = y/2;
        for (x = 0; x < width; x++) {
            x2 = x/2;
#if 0       
            pict->data[0][pict->linesize[0] + x]       = filt_frame->data[0][y *  filt_frame->linesize[0] + x];
            pict->data[1][pict->linesize[1] + x2]      = filt_frame->data[1][y2 * filt_frame->linesize[1] + x2];
            pict->data[2][pict->linesize[2] + x2]      = filt_frame->data[2][y2 * filt_frame->linesize[2] + x2];
#else
            pict->data[0][y *  pict->linesize[0] + x]  = Ydiffnow[y *  Ylinesize + x];
            pict->data[1][y2 * pict->linesize[1] + x2] = Udiffnow[y2 * Ulinesize + x2];
            pict->data[2][y2 * pict->linesize[2] + x2] = Vdiffnow[y2 * Vlinesize + x2];
#endif
        }
    }       
#endif    
#if 0
    for (y = 0; y < height; y++)  {
        y2 = y/2;
        for (x = 0; x < width; x++) {
            x2 = x/2;
            r = Rnow[y *  pict->linesize[0] + x];
            g = Gnow[y *  pict->linesize[0] + x];
            b = Bnow[y *  pict->linesize[0] + x];
            Y0 = RGB2Y(r,g,b);
            U0 = RGB2U(r,g,b);
            V0 = RGB2V(r,g,b);            
            pict->data[0][y *  pict->linesize[0] + x]  = Y0;
            pict->data[1][y2 * pict->linesize[1] + x2] = U0;
            pict->data[2][y2 * pict->linesize[2] + x2] = V0;
        }
    }       
#endif
#if 0
    printf("%s(%d)\n",__FILE__,__LINE__);
    calc_ref(frame_index,width,height);
    printf("%s(%d) (%4d,%4d,%4d)\n",__FILE__,__LINE__,frame_index,width,height);
    for (y = 0; y < height; y++)  {
        y2 = y/2;
        for (x = 0; x < width; x++) {
            x2 = x/2;
 //         printf("%s(%d),(%4d,%4d,%4d,%4d))\n",__FILE__,__LINE__,x,y,x2,y2);
            pict->data[0][y *  pict->linesize[0] + x]  = Yref[y *   Ylinesize + x];
            pict->data[1][y2 * pict->linesize[1] + x2] = Uref[y2 *  Ulinesize + x2];
            pict->data[2][y2 * pict->linesize[2] + x2] = Vref[y2 *  Vlinesize + x2];
        }
    }       
#endif 
#if 0
    if (frame_index>0) {
      printf("fill_yuv_image %s(%d),%3d,(%4d,%4d,%4d)\n",__FILE__,__LINE__,frame_index,
             Ylinesize,Ulinesize,Vlinesize);
      for (y = 0; y < height; y++) {
        y2 = y/2;
        for (x = 0; x < width; x++) {
          x2 = x/2;
          calc_nb(x,y);
          
          R1 = rn[1][1]; G1 = gn[1][1]; B1 = bn[1][1];
          R0 = rb[1][1]; G0 = gb[1][1]; B0 = bb[1][1];
          #if 1
          pict->data[0][y *  pict->linesize[0] + x]  = RGB2Y(R1,G1,B1);
          pict->data[1][y2 * pict->linesize[1] + x2] = RGB2U(R1,G1,B1);
          pict->data[2][y2 * pict->linesize[2] + x2] = RGB2V(R1,G1,B1);
          #endif
          #if 0
          pict->data[0][y *  pict->linesize[0] + x]  = RGB2Y(R0,G0,B0);
          pict->data[1][y2 * pict->linesize[1] + x2] = RGB2U(R0,G0,B0);
          pict->data[2][y2 * pict->linesize[2] + x2] = RGB2V(R0,G0,B0);
          #endif          
          #if 1
          g=abs(a(G0)-a(G1));
#if 0
          if (ge>8)
          printf("%s(%d) (%4d,%4d),(%4d,%4d),(%4d,%4d),(%4d,%4d),%4d\n",
                 __FILE__,__LINE__,y,x,gb[1][1],gn[1][1],G0,G1,g,ga,Y1);                       
#endif      
         if(ge> 8 && (abs(a(G1)-a(G0)) > 8 || abs(a(B1)-a(B0)) > 8)  ) { //變動且是邊界
            pict->data[0][y *  pict->linesize[0] + x]  = BLACKY;                
            pict->data[1][y2 * pict->linesize[1] + x2] = BLACKU;
            pict->data[2][y2 * pict->linesize[2] + x2] = BLACKV;
          }  
          else {
            pict->data[0][y *  pict->linesize[0] + x]  = WHITEY;                
            pict->data[1][y2 * pict->linesize[1] + x2] = WHITEU;
            pict->data[2][y2 * pict->linesize[2] + x2] = WHITEV;
          }
          #endif
        }  
      }
    }
    else {
      printf("fill_yuv_image %s(%d),%3d\n",__FILE__,__LINE__,frame_index);
      for (y = 0; y < height; y++) {
     //   printf("fill_yuv_image %s(%d) (%4d,%4d),%4d,%4d,%4d\n",__FILE__,__LINE__,x,y,
     //          Ylinesize,Ulinesize,Vlinesize);
        y2 = y/2;
        for (x = 0; x < width; x++) {
          x2 = x/2;
          #if 0 
          pos = y *  pict->linesize[0] + x;
          R1 = Rnow[pos]; G1 = Gnow[pos]; B1 = Bnow[pos];
          pict->data[0][y *  pict->linesize[0] + x]  = RGB2Y(R1,G1,B1);
          pict->data[1][y2 * pict->linesize[1] + x2] = RGB2U(R1,G1,B1);
          pict->data[2][y2 * pict->linesize[2] + x2] = RGB2V(R1,G1,B1);
          #endif
          #if 1
          pict->data[0][y *  pict->linesize[0] + x]  = Ydiffnow[y *  Ylinesize + x];
          pict->data[1][y2 * pict->linesize[1] + x2] = Udiffnow[y2 * Ulinesize + x2];
          pict->data[2][y2 * pict->linesize[2] + x2] = Vdiffnow[y2 * Vlinesize + x2];
          #endif          
        }
      }        
    }
#endif
    
}

int savePicture(AVFrame *pFrame, char *out_name) {
//printf("%s(%4d) %X\n",__FILE__,__LINE__,pFrame);
  int width = pFrame->width;
  int height = pFrame->height;
  AVCodecContext *pCodeCtx = NULL;
  AVFormatContext *pFormatCtx = avformat_alloc_context();
  pFormatCtx->oformat = av_guess_format("mjpeg", NULL, NULL);
// ????????AVIOContext
  if (avio_open(&pFormatCtx->pb, out_name, AVIO_FLAG_READ_WRITE) < 0) {
    printf("Couldn't open output file.");
    return -1;
  }
// ?????stream
  AVStream *pAVStream = avformat_new_stream(pFormatCtx, 0); 
  if (pAVStream == NULL) {
    return -1;
  }
  AVCodecParameters *parameters = pAVStream->codecpar;
  parameters->codec_id = pFormatCtx->oformat->video_codec;
  parameters->codec_type = AVMEDIA_TYPE_VIDEO;
  parameters->format = AV_PIX_FMT_YUVJ420P;
  parameters->width = pFrame->width;
  parameters->height = pFrame->height;
  AVCodec *pCodec = (AVCodec *)avcodec_find_encoder(pAVStream->codecpar->codec_id); //?????
  if (!pCodec) {
    printf("Could not find encoder\n");
    return -1;
  }
  pCodeCtx = avcodec_alloc_context3(pCodec); //?AVCodecContext????
  if (!pCodeCtx) {
    fprintf(stderr, "Could not allocate video codec context\n");
    exit(1);
  }
  if ((avcodec_parameters_to_context(pCodeCtx, pAVStream->codecpar)) < 0) {
    fprintf(stderr, "Failed to copy %s codec parameters to decoder context\n",
    av_get_media_type_string(AVMEDIA_TYPE_VIDEO));
    return -1;
  }
  pCodeCtx->time_base = (AVRational) {1, 25};
  if (avcodec_open2(pCodeCtx, pCodec, NULL) < 0) { //?????
    printf("Could not open codec.");
   return -1;
  }
  int ret = avformat_write_header(pFormatCtx, NULL);
  if (ret < 0) {
    printf("write_header fail\n");
    return -1;
  }
  int y_size = width * height;
//Encode
// ?AVPacket????????
  AVPacket pkt;
  av_new_packet(&pkt, y_size * 3);
// ????
  ret = avcodec_send_frame(pCodeCtx, pFrame);
  if (ret < 0) {
    printf("Could not avcodec_send_frame.");
    return -1;
  }
// ???????
  ret = avcodec_receive_packet(pCodeCtx, &pkt);
  if (ret < 0) {
    printf("Could not avcodec_receive_packet");
    return -1;
  }
  ret = av_write_frame(pFormatCtx, &pkt);
  if (ret < 0) {
    printf("Could not av_write_frame");
    return -1;
  }
  av_packet_unref(&pkt);
//Write Trailer
  av_write_trailer(pFormatCtx);
  avcodec_close(pCodeCtx);
  avio_close(pFormatCtx->pb);
  avformat_free_context(pFormatCtx);
  return 0;
}
// a wrapper around a single output AVStream
typedef struct OutputStream {
    AVStream *st;
    AVCodecContext *enc;
    // pts of the next frame that will be generated
    int64_t next_pts;
    int samples_count;
    AVFrame *frame;
    AVFrame *tmp_frame;
    AVPacket *tmp_pkt;
    float t, tincr, tincr2;
    struct SwsContext *sws_ctx;
    struct SwrContext *swr_ctx;
} OutputStream;
void copyyuvimage(AVFrame *pict, int frame_index,
                    int width, int height) {
  int x,y,x2,y2,Y0;
  printf("copy_yuv_image %s(%d),(%d,%d)\n",__FILE__,__LINE__,filt_frame->linesize[0],pict->linesize[0]);
  for (y = 0; y < height; y++) {
    for (x = 0; x < width; x++) {
      x2=x/2;
      y2=y/2;
      if(x2==y2) {
        pict->data[0][2*y2     * pict->linesize[0] + 2*x2]=BLACKY;
        pict->data[0][2*y2     * pict->linesize[0] + 2*x2+1]=BLACKY;
        pict->data[0][(2*y2+1) * pict->linesize[0] + 2*x2]=BLACKY;
        pict->data[0][(2*y2+1) * pict->linesize[0] + 2*x2+1]=BLACKY;
      }
      else {
        Y0 = filt_frame->data[0][y * filt_frame->linesize[0] + x];
        if(Y0<32) {
          pict->data[0][y * pict->linesize[0] + x] = BLACKY;
        }
        else {
          pict->data[0][y * pict->linesize[0] + x] = Y0;
        }
      } 
    }
  }  
  for (y = 0; y < height/2; y++) {
    for (x = 0; x < width/2; x++) {
      pict->data[1][y * pict->linesize[1] + x] = filt_frame->data[1][y * filt_frame->linesize[1] + x];
      pict->data[2][y * pict->linesize[2] + x] = filt_frame->data[2][y * filt_frame->linesize[2] + x];          
      if(x==y) {
        pict->data[1][y * pict->linesize[1] + x] =BLACKU;
        pict->data[2][y * pict->linesize[2] + x] =BLACKV;
      }
    }
  }     
}
AVFrame *getvideoframe(OutputStream *ost)
{
  AVCodecContext *c = ost->enc;
  printf("%s(%d) %lld\n",__FILE__,__LINE__,ost->next_pts);
//check if we want to generate more frames
  if (av_compare_ts(ost->next_pts, c->time_base,
                   STREAM_DURATION, (AVRational){ 1, 1 }) > 0)
    return NULL;
 printf("%s(%d)\n",__FILE__,__LINE__);
//when we pass a frame to the encoder, it may keep a reference to it
//internally; make sure we do not overwrite it here */
  if (av_frame_make_writable(ost->frame) < 0)
    exit(1);
  printf("%s(%d)\n",__FILE__,__LINE__);
  if (c->pix_fmt != AV_PIX_FMT_YUV420P) {
  //as we only generate a YUV420P picture, we must convert it
  //to the codec pixel format if needed
    if (!ost->sws_ctx) {
      ost->sws_ctx = sws_getContext(c->width, c->height,
                                    AV_PIX_FMT_YUV420P,
                                    c->width, c->height,
                                    c->pix_fmt,
                                    SCALE_FLAGS, NULL, NULL, NULL);
      if (!ost->sws_ctx) {
        fprintf(stderr,
                "Could not initialize the conversion context\n");
                exit(1);
        }
      }
      printf("%s(%d) (%d,%d)\n",__FILE__,__LINE__,c->width, c->height);
//    copy_yuv_image(ost->frame, ost->next_pts, c->width, c->height);
      sws_scale(ost->sws_ctx, (const uint8_t * const *) ost->tmp_frame->data,
                ost->tmp_frame->linesize, 0, c->height, ost->frame->data,
                  ost->frame->linesize);
    } else {
//    printf("%s(%d)%" PRIu64 ",(%d,%d),%X\n",__FILE__,__LINE__,
//           ost->next_pts,c->width, c->height,ost->tmp_frame);
      copyyuvimage(ost->frame, ost->next_pts, c->width, c->height);
    }
    ost->frame->pts = ost->next_pts++;
    return ost->frame;
}
int savePicture(AVFrame *pFrame, char *out_name);
static int writeframejpg(AVFormatContext *fmt_ctx, AVCodecContext *c,
                         AVStream *st, AVFrame *frame, AVPacket *pkt,int frame_index)
{
    int ret;
    char imgbuf[256];
  //printf("%s(%d) %X,%X,%X\n",__FILE__,__LINE__,c,frame,pkt);
    if(frame==0) return 1;
  //send the frame to the encoder
    ret = avcodec_send_frame(c, frame);
  //printf("%s(%d) %X,%X,%X\n",__FILE__,__LINE__,c,frame,pkt);
    if (ret < 0) {
        fprintf(stderr, "Error sending a frame to the encoder: %s\n",
                av_err2str(ret));
        exit(1);
    }
    while (ret >= 0) {
    //printf("%s(%d) %X,%X,%X\n",__FILE__,__LINE__,c,frame,pkt);
      ret = avcodec_receive_packet(c, pkt);
    //printf("write_frame %s(%d) %d ret=%d,%d,%d\n",__FILE__,__LINE__,frame->data[0][2],ret,AVERROR(EAGAIN),AVERROR_EOF);
      if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
        break;
      }    
      else if (ret < 0) {
        fprintf(stderr, "Error encoding a frame: %s\n", av_err2str(ret));
        exit(1);
      }
    //rescale output packet timestamp values from codec to stream timebase
      av_packet_rescale_ts(pkt, c->time_base, st->time_base);
      pkt->stream_index = st->index;
    //Write the compressed frame to the media file.
    //log_packet(fmt_ctx, pkt);

      snprintf(imgbuf, sizeof(imgbuf), "img/x%03d.jpg",frame_index); 
//    savePicture(filt_frame, imgbuf);
      savePicture(frame, imgbuf);
      printf("fname=%s\n",imgbuf);
      if (0) {
        printf("pkt->size=%d\n",pkt->size);
        FILE *f = fopen("output.jpg", "wb");
        fwrite(pkt->data, 1, pkt->size, f);
        fclose(f);
        exit(0);
     }
#if 0
    ret = av_interleaved_write_frame(fmt_ctx, pkt);
//  ret = av_write_frame(fmt_ctx, pkt);
  //pkt is now blank (av_interleaved_write_frame() takes ownership of
  //its contents and resets pkt), so that no unreferencing is necessary.
  //This would be different if one used av_write_frame(). */
    if (ret < 0) {
      fprintf(stderr, "Error while writing output packet: %s\n", av_err2str(ret));
      exit(1);
    }
#endif
  }

  return ret == AVERROR_EOF ? 1 : 0;
}
int writejpgframe(AVFormatContext *oc, OutputStream *ost)
{
//return write_frame_jpg(oc, ost->enc, ost->st, getvideoframe(ost), ost->tmp_pkt, ost->next_pts);
  return writeframejpg(oc, ost->enc, ost->st, ost->frame, ost->tmp_pkt, ost->next_pts);
}
static AVFrame *allocframe(enum AVPixelFormat pix_fmt, int width, int height)
{
  AVFrame *frame;
  int ret;
  frame = av_frame_alloc();
  if (!frame)
    return NULL;
  frame->format = pix_fmt;
  frame->width  = width;
  frame->height = height;
//allocate the buffers for the frame data
  ret = av_frame_get_buffer(frame, 0);
  if (ret < 0) {
    fprintf(stderr, "Could not allocate frame data.\n");
    exit(1);
  }
  return frame;
}
void openvideo(AVFormatContext *oc, const AVCodec *codec,
               OutputStream *ost, AVDictionary *opt_arg)
{
  int ret;
  AVCodecContext *c = ost->enc;
  AVDictionary *opt = NULL;
  av_dict_copy(&opt, opt_arg, 0);
//open the codec
  ret = avcodec_open2(c, codec, &opt);
  av_dict_free(&opt);
  if (ret < 0) {
    fprintf(stderr, "Could not open video codec: %s\n", av_err2str(ret));
    exit(1);
  }
//allocate and init a re-usable frame
  ost->frame = allocframe(c->pix_fmt, c->width, c->height);
  if (!ost->frame) {
    fprintf(stderr, "Could not allocate video frame\n");
    exit(1);
  }
//printf("%s(%d) %X,%d,%d\n",__FILE__,__LINE__,ost->frame,c->pix_fmt,AV_PIX_FMT_YUV420P);
//If the output format is not YUV420P, then a temporary YUV420P
//picture is needed too. It is then converted to the required
//output format.
  ost->tmp_frame = NULL;
  if (c->pix_fmt != AV_PIX_FMT_YUV420P) {
    ost->tmp_frame = allocframe(AV_PIX_FMT_YUV420P, c->width, c->height);
    if (!ost->tmp_frame) {
      fprintf(stderr, "Could not allocate temporary video frame\n");
      exit(1);
    }
  //printf("%s(%d) %X\n",__FILE__,__LINE__,ost->tmp_frame);
  }
//copy the stream parameters to the muxer
  ret = avcodec_parameters_from_context(ost->st->codecpar, c);
  if (ret < 0) {
    fprintf(stderr, "Could not copy the stream parameters\n");
    exit(1);
  }
} 
static void addstream(OutputStream *ost, AVFormatContext *oc,
  const AVCodec **codec,enum AVCodecID codec_id,AVCodecContext *dec_ctx)
{
  AVCodecContext *c;
  AVChannelLayout stereo_layout = AV_CHANNEL_LAYOUT_STEREO;
  int i;
//find the encoder 
  *codec = avcodec_find_encoder(codec_id);
  if (!(*codec)) {
    fprintf(stderr, "Could not find encoder for '%s'\n",
            avcodec_get_name(codec_id));
    exit(1);
  }
  ost->tmp_pkt = av_packet_alloc();
  if (!ost->tmp_pkt) {
    fprintf(stderr, "Could not allocate AVPacket\n");
    exit(1);
  }
  ost->st = avformat_new_stream(oc, NULL);
  if (!ost->st) {
    fprintf(stderr, "Could not allocate stream\n");
    exit(1);
  }
  ost->st->id = oc->nb_streams-1;
  c = avcodec_alloc_context3(*codec);
  if (!c) {
    fprintf(stderr, "Could not alloc an encoding context\n");
    exit(1);
  }
  ost->enc = c;
  switch ((*codec)->type) {
    case AVMEDIA_TYPE_AUDIO:
      c->sample_fmt  = (*codec)->sample_fmts ?
            (*codec)->sample_fmts[0] : AV_SAMPLE_FMT_FLTP;
      c->bit_rate    = 64000;
      c->sample_rate = 44100;
      if ((*codec)->supported_samplerates) {
        c->sample_rate = (*codec)->supported_samplerates[0];
        for (i = 0; (*codec)->supported_samplerates[i]; i++) {
          if ((*codec)->supported_samplerates[i] == 44100)
            c->sample_rate = 44100;
        }
      }
      av_channel_layout_copy(&c->ch_layout, &stereo_layout);
//    av_channel_layout_copy(&c->ch_layout, &(AVChannelLayout)AV_CHANNEL_LAYOUT_STEREO);
      ost->st->time_base = (AVRational){ 1, c->sample_rate };
      break;
    case AVMEDIA_TYPE_VIDEO:
      c->codec_id = codec_id;
      c->bit_rate = 400000;
//Resolution must be a multiple of two. */
      #if 1
//    c->width    = GLOBAL_HEIGHT;   //2160; //352;
//    c->height   = GLOBAL_WIDTH;    //3840; //288;
//    c->width    = GLOBAL_WIDTH;    //2160; //352;
//    c->height   = GLOBAL_HEIGHT;   //3840; //288;
      c->width    = dec_ctx->width;    //2160; //352;
      c->height   = dec_ctx->height;   //3840; //288;
      printf("%s(%d) %d,%d\n",__FILE__,__LINE__,c->width,c->height);
      #else
      c->width    = 352;
      c->height   = 288;
      #endif
      // timebase: This is the fundamental unit of time (in seconds) in terms
      // of which frame timestamps are represented. For fixed-fps content,
      // timebase should be 1/framerate and timestamp increments should be
      // identical to 1. */
      ost->st->time_base = (AVRational){ 1, STREAM_FRAME_RATE };
      c->time_base       = ost->st->time_base;
      c->gop_size      = 12; /* emit one intra frame every twelve frames at most */
      c->pix_fmt       = STREAM_PIX_FMT;
      if (c->codec_id == AV_CODEC_ID_MPEG2VIDEO) {
      //just for testing, we also add B-frames
        c->max_b_frames = 2;
      }
      if (c->codec_id == AV_CODEC_ID_MPEG1VIDEO) {
      //Needed to avoid using macroblocks in which some coeffs overflow.
      //This does not happen with normal video, it just happens here as
      //the motion of the chroma plane does not match the luma plane.
        c->mb_decision = 2;
      }
      break;
    default:
        break;
  }
  //Some formats want stream headers to be separate.
  if (oc->oformat->flags & AVFMT_GLOBALHEADER)
    c->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
}
void copyFrame_now(int frame_index);
int getframe(const char *filename, int frame_index)
{
  int ret=0;
  AVPacket *packet;
  AVFrame *frame;
  static AVFormatContext *fmt_ctx;
  const AVOutputFormat *fmt;
  AVFormatContext *oc;
  const AVCodec *video_codec;
  AVCodecContext *dec_ctx;
  AVCodec *dec;
  int video_stream_index;
  char filter_descr[sizeof("scale=3840:2160,transpose=clock")];
  OutputStream video_st = { 0 };
  AVFilterContext *buffersink_ctx;
  AVFilterContext *buffersrc_ctx;
  AVFilterGraph *filter_graph;
  AVDictionary *opt = NULL;
  frame = av_frame_alloc();
  filt_frame   = av_frame_alloc();
  packet = av_packet_alloc();
  if (!frame || !filt_frame || !packet) {
    fprintf(stderr, "Could not allocate frame or packet\n");
    exit(1);
  }
//if ((ret = open_input_file(argv[1])) < 0) goto end;
//open_input_file start
  if ((ret = avformat_open_input(&fmt_ctx, filename, NULL, NULL)) < 0) {
    av_log(NULL, AV_LOG_ERROR, "Cannot open input file\n");
    return ret;
  }
  if ((ret = avformat_find_stream_info(fmt_ctx, NULL)) < 0) {
    av_log(NULL, AV_LOG_ERROR, "Cannot find stream information\n");
    return ret;
  }
//select the video stream
  ret = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, (const AVCodec**)&dec, 0);
  if (ret < 0) {
    av_log(NULL, AV_LOG_ERROR, "Cannot find a video stream in the input file\n");
    return ret;
  }
  printf("%s(%4d)\n",__FILE__,__LINE__);
  video_stream_index = ret;
//create decoding context 
  dec_ctx = avcodec_alloc_context3(dec);
  if (!dec_ctx) return AVERROR(ENOMEM);
  avcodec_parameters_to_context(dec_ctx, fmt_ctx->streams[video_stream_index]->codecpar);
//init the video decoder 
  if ((ret = avcodec_open2(dec_ctx, dec, NULL)) < 0) {
    av_log(NULL, AV_LOG_ERROR, "Cannot open video decoder\n");
    return ret;
  }
//open_input_file start finish
//sprintf(filter_descr,"scale=%d:%d,transpose=clock",GLOBAL_WIDTH,GLOBAL_HEIGHT);
//sprintf(filter_descr,"scale=%d:%d",GLOBAL_WIDTH,GLOBAL_HEIGHT);
  sprintf(filter_descr,"scale=iw:ih");
//if ((ret = init_filters(filter_descr)) < 0)
//  goto end;

//init_filters start
  const AVFilter *buffersrc  = avfilter_get_by_name("buffer");
  const AVFilter *buffersink = avfilter_get_by_name("buffersink");
  AVFilterInOut *outputs = avfilter_inout_alloc();
  AVFilterInOut *inputs  = avfilter_inout_alloc();
  AVRational time_base = fmt_ctx->streams[video_stream_index]->time_base;
//enum AVPixelFormat pix_fmts[] = { AV_PIX_FMT_GRAY8, AV_PIX_FMT_NONE };
//enum AVPixelFormat pix_fmts[] = { AV_PIX_FMT_RGB24, AV_PIX_FMT_NONE };
//enum AVPixelFormat pix_fmts[] = { AV_PIX_FMT_MONOWHITE , AV_PIX_FMT_NONE };
//enum AVPixelFormat pix_fmts[] = { AV_PIX_FMT_MONOBLACK , AV_PIX_FMT_NONE };
  enum AVPixelFormat pix_fmts[] = { STREAM_PIX_FMT, AV_PIX_FMT_NONE };
  filter_graph = avfilter_graph_alloc();
  if (!outputs || !inputs || !filter_graph) {
    ret = AVERROR(ENOMEM);
    goto end;
  }
//buffer video source: the decoded frames from the decoder will be inserted here. 
  char argstr[256];
  snprintf(argstr, sizeof(argstr),
    "video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:pixel_aspect=%d/%d",
    dec_ctx->width, dec_ctx->height, dec_ctx->pix_fmt,
    time_base.num, time_base.den,
  dec_ctx->sample_aspect_ratio.num, dec_ctx->sample_aspect_ratio.den);
  ret = avfilter_graph_create_filter(&buffersrc_ctx, buffersrc, "in",
                                     argstr, NULL, filter_graph);
  if (ret < 0) {
    av_log(NULL, AV_LOG_ERROR, "Cannot create buffer source\n");
    goto end;
  }
//buffer video sink: to terminate the filter chain.
  ret = avfilter_graph_create_filter(&buffersink_ctx, buffersink, "out",
                                     NULL, NULL, filter_graph);
  if (ret < 0) {
    av_log(NULL, AV_LOG_ERROR, "Cannot create buffer sink\n");
    goto end;
  }
//ret = av_opt_set_int_list(buffersink_ctx, "pix_fmts", pix_fmts,
//                          AV_PIX_FMT_NONE, AV_OPT_SEARCH_CHILDREN);
  ret = av_opt_set_int_list(buffersink_ctx, "pix_fmts", pix_fmts,
                            AV_PIX_FMT_NONE, AV_OPT_SEARCH_CHILDREN);
  if (ret < 0) {
    av_log(NULL, AV_LOG_ERROR, "Cannot set output pixel format\n");
    goto end;
  }
//Set the endpoints for the filter graph. The filter_graph will
//be linked to the graph described by filters_descr.
//The buffer source output must be connected to the input pad of
//the first filter described by filters_descr; since the first
//filter input label is not specified, it is set to "in" by
//default.
  outputs->name       = av_strdup("in");
  outputs->filter_ctx = buffersrc_ctx;
  outputs->pad_idx    = 0;
  outputs->next       = NULL;
//The buffer sink input must be connected to the output pad of
//the last filter described by filters_descr; since the last
//filter output label is not specified, it is set to "out" by
//default.
  inputs->name       = av_strdup("out");
  inputs->filter_ctx = buffersink_ctx;
  inputs->pad_idx    = 0;
  inputs->next       = NULL;
  if ((ret = avfilter_graph_parse_ptr(filter_graph, filter_descr,
                                  &inputs, &outputs, NULL)) < 0)
    goto end;
  if ((ret = avfilter_graph_config(filter_graph, NULL)) < 0)
    goto end;
end:
  avfilter_inout_free(&inputs);
  avfilter_inout_free(&outputs);
  printf("video_size=%dx%d:pix_fmt=%d:pixel_aspect=%d/%d\n",
          dec_ctx->width, dec_ctx->height, dec_ctx->pix_fmt,
          dec_ctx->sample_aspect_ratio.num, dec_ctx->sample_aspect_ratio.den);
//init_filter finish

//allocate the output media context
  avformat_alloc_output_context2(&oc, NULL, NULL, "img/tmp.mp4");
  if (!oc) {
    printf("Could not deduce output format from file extension: using MPEG.\n");
    avformat_alloc_output_context2(&oc, NULL, "mpeg", filename);
  }
  if (!oc) return 1;
  fmt = oc->oformat;
//void add_stream(OutputStream *ost, AVFormatContext *oc,
//                const AVCodec **codec,enum AVCodecID codec_id)
  addstream(&video_st, oc, &video_codec, fmt->video_codec,dec_ctx);
  openvideo(oc, video_codec, &video_st, opt);
/*	
  {  
    AVFormatContext *fmt_ctx = NULL;
    const AVDictionaryEntry *tag = NULL;
	if ((ret = avformat_open_input(&fmt_ctx, argv[1], NULL, NULL)))
      return ret;
    if ((ret = avformat_find_stream_info(fmt_ctx, NULL)) < 0) {
      av_log(NULL, AV_LOG_ERROR, "Cannot find stream information\n");
      return ret;
    }
    while ((tag = av_dict_iterate(fmt_ctx->metadata, tag)))
      printf("%s=%s\n", tag->key, tag->value);
    avformat_close_input(&fmt_ctx);
  }  
  {
    double theta = 0.0;
    int32_t *displaymatrix = NULL;
    AVFrameSideData *sd = av_frame_get_side_data(frame, AV_FRAME_DATA_DISPLAYMATRIX);
    if (sd)
      displaymatrix = (int32_t *)sd->data;
    if (!displaymatrix)
      displaymatrix = (int32_t *)av_stream_get_side_data(is->video_st, AV_PKT_DATA_DISPLAYMATRIX, NULL);
  }  
  {
    int len =0;
    int iloopcount = 0;
    AVProbeData probeData = {0};
	 probeData.buf_size = len < 4096 ? len : 4096;
	 probeData.filename = "stream";
	 probeData.buf = av_mallocz(probeData.buf_size + AVPROBE_PADDING_SIZE);
	 memcpy(probeData.buf, lpData, probeData.buf_size);
  }
  */
//mux begin
//sdl_main(argc, argv);
//printf("exit from sdl_exit.\n");
//mux end
//read all packets
  while (1) {
    if ((ret = av_read_frame(fmt_ctx, packet)) < 0)
      break;
    if (packet->stream_index == video_stream_index) {
      ret = avcodec_send_packet(dec_ctx, packet);
      if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Error while sending a packet to the decoder\n");
        break;
      }
      while (ret >= 0) {
        ret = avcodec_receive_frame(dec_ctx, frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
          break;
        } else if (ret < 0) {
          av_log(NULL, AV_LOG_ERROR, "Error while receiving a frame from the decoder\n");
          goto end;
        }
        frame->pts = frame->best_effort_timestamp;
        //push the decoded frame into the filtergraph
        if (av_buffersrc_add_frame_flags(buffersrc_ctx, frame, AV_BUFFERSRC_FLAG_KEEP_REF) < 0) {
          av_log(NULL, AV_LOG_ERROR, "Error while feeding the filtergraph\n");
          break;
        }
//pull filtered frames from the filtergraph
        while (1) {
          ret = av_buffersink_get_frame(buffersink_ctx, filt_frame);
          printf("%s(%d),%3d,%3d\n",__FILE__,__LINE__,
                 frame->data[0][2],filt_frame->data[0][2]);
          if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            printf("%s(%d)\n",__FILE__,__LINE__);                   
            break;
          } 
          if (ret < 0) {
            printf("%s(%d),%3d\n",__FILE__,__LINE__,
            filt_frame->data[0][2]);                   
            goto end;
          }  
//        display_frame(filt_frame, buffersink_ctx->inputs[0]->time_base);
#if 1
          //select the stream to encode
          getvideoframe(&video_st);
          printf("%s(%d),%" PRIu64 " %d\n",__FILE__,__LINE__,video_st.next_pts, frame_index);
          if (video_st.next_pts==frame_index) {
            copyFrame_now(video_st.next_pts);
          //calc_edge(video_st.next_pts,video_st.enc->width, video_st.enc->height);t_pts);
            writejpgframe(oc, &video_st);
            goto end_loop;
            break;
          }                 
         //copyFramebefore();
#endif
//        av_frame_unref(filt_frame);
        }
//      av_frame_unref(frame);
      }
    }
//  av_packet_unref(packet);
  }
end_loop:
  printf("%s(%d)\n",__FILE__,__LINE__);
  //Close each codec.
//close_stream(oc, &video_st);
  avcodec_free_context(&video_st.enc);
  av_frame_free(&video_st.frame);
  av_frame_free(&video_st.tmp_frame);
  av_packet_free(&video_st.tmp_pkt);
  sws_freeContext(video_st.sws_ctx);
  swr_free(&video_st.swr_ctx);
#if 1
  av_frame_unref(filt_frame);
  av_frame_unref(frame);
  av_packet_unref(packet);   
//free(Ybefore);free(Ubefore);free(Vbefore);
//free(Ydiffnow);free(Udiffnow);free(Vdiffnow);
#endif
//avfilter_graph_free(&filter_graph);  //2025/4/1 by calc_matrix ?
  avcodec_free_context(&dec_ctx);
  avformat_close_input(&fmt_ctx);
  av_frame_free(&frame);
  av_frame_free(&filt_frame);
  av_packet_free(&packet);
  if (ret < 0 && ret != AVERROR_EOF) {
    fprintf(stderr, "Error occurred: %s\n", av_err2str(ret));
    exit(1);
  }
//now1 = time(NULL);
//printf("diff=%llu\n",now1-now0);
  return 1;
//exit(0);
}

AVFormatContext *format_ctx = NULL;
AVCodecContext *codec_ctx = NULL;
const AVCodec *codec = NULL;
AVPacket *packet = NULL;
AVFrame *frame = NULL;
AVFrame *getFrame(const char *filename) {
   packet = av_packet_alloc();
   frame  = av_frame_alloc();
  // 1. Open the input file
  if (avformat_open_input(&format_ctx, filename, NULL, NULL) < 0) {
    fprintf(stderr, "Could not open source file %s\n", filename);
    return NULL;
  }
  // 2. Find stream information
  if (avformat_find_stream_info(format_ctx, NULL) < 0) {
    fprintf(stderr, "Could not find stream information\n");
    return NULL;
  }
  // 3. Find the video stream (JPG is treated as a single-frame video stream)
  int stream_index = -1;
  for (unsigned int i = 0; i < format_ctx->nb_streams; i++) {
    if (format_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
      stream_index = i;
      break;
    }
  }
  if (stream_index == -1) {
    fprintf(stderr, "Could not find video stream in file\n");
    return NULL;
  }
  // 4. Find decoder for the JPG stream
  AVCodecParameters *codec_params = format_ctx->streams[stream_index]->codecpar;
  codec = avcodec_find_decoder(codec_params->codec_id);
  if (!codec) {
    fprintf(stderr, "Failed to find decoder\n");
    return NULL;
  }
  // 5. Allocate and initialize codec context
  codec_ctx = avcodec_alloc_context3(codec);
  if (avcodec_parameters_to_context(codec_ctx, codec_params) < 0) {
    fprintf(stderr, "Failed to copy codec params to context\n");
    return NULL;
  }
  if (avcodec_open2(codec_ctx, codec, NULL) < 0) {
    fprintf(stderr, "Failed to open codec\n");
    return NULL;
  }
  // 6. Read frame from stream
  int frame_finished = 0;
  while (av_read_frame(format_ctx, packet) >= 0) {
    if (packet->stream_index == stream_index) {
    //Send the packet to the decoder
      if (avcodec_send_packet(codec_ctx, packet) >= 0) {
      //Receive the decoded frame
        int ret = avcodec_receive_frame(codec_ctx, frame);
        if (ret == 0) {
          frame_finished = 1;
          printf("Successfully decoded frame!\n");
          printf("Width: %d, Height: %d, Pixel Format: %d\n", 
                 frame->width, frame->height, frame->format);
          break;
        }
      }
    }
    av_packet_unref(packet);
  }
  //Clean up
#if 0
  av_frame_free(&frame);
  av_packet_free(&packet);
  avcodec_free_context(&codec_ctx);
  avformat_close_input(&format_ctx);
#endif
  if (!frame_finished) {
    fprintf(stderr, "Failed to decode frame from JPG\n");
    return NULL;
  }
  return frame;
}
void freeAll(void) {
  if(frame != NULL) av_frame_free(&frame);
  if(packet != NULL) av_packet_free(&packet);
  if(codec_ctx != NULL) avcodec_free_context(&codec_ctx);
  if(format_ctx != NULL) avformat_close_input(&format_ctx);
  frame = NULL;
  packet = NULL;
  codec_ctx = NULL;
  format_ctx = NULL;
}
void testToolBox2(int x,int y) {
  int x0=x;
  int y0=y;
  int Y0;
  freeAll();
  AVFrame *aframe=getFrame("../VID20260802132623/x0095.jpg");
  printf("%s(%4d)\n",__FILE__,__LINE__);
  Y0=frame->data[0][y0 * frame->linesize[0] + x0];
  for (y = 0; y < codec_ctx->height; y++) {
    for (x = 0; x < codec_ctx->width; x++) {
      if( abs(frame->data[0][y * frame->linesize[0] + x]-Y0) > 20 )
      {
        frame->data[0][y * frame->linesize[0] + x]     = BLACKY;
        frame->data[1][y/2 * frame->linesize[1] + x/2] = BLACKU;
        frame->data[2][y/2 * frame->linesize[2] + x/2] = BLACKV; 
      }  
    }
  }
  savePicture(aframe, (char*)"../VID20260802132623/tmp.jpg");
  aframe=aframe;
}
void getYUV(int frame_index,int x,int y,int *Y,int *U,int *V) {
  char fname[256];
  sprintf(fname,"../VID20260802132623/x%04d.jpg",frame_index);
  printf("%s(%4d) (%4d,%4d)\n",__FILE__,__LINE__,x,y);
  AVFrame *aframe=getFrame(fname);
  if(x>=0 && y>0 && x<codec_ctx->width && y<codec_ctx->height) {
    *Y=frame->data[0][y * frame->linesize[0] + x];
    *U=frame->data[1][y/2 * frame->linesize[1] + x/2];
    *V=frame->data[2][y/2 * frame->linesize[2] + x/2];
  }
  else {
    printf("%s(%4d) (%4d,%4d)\n",__FILE__,__LINE__,x,y);
  }
  aframe=aframe;
  freeAll();
}
void loadtmp(void) {
  printf("%s(%4d)\n",__FILE__,__LINE__);
}
static AVFrame *get_video_frame(OutputStream *ost,const char *fDirectory, const char cprefix)
{
  AVCodecContext *c = ost->enc;
  printf("%s(%d)\n",__FILE__,__LINE__);
  //check if we want to generate more frames
  if (av_compare_ts(ost->next_pts, c->time_base,
                    STREAM_DURATION, (AVRational){ 1, 1 }) > 0)
        return NULL;
  //when we pass a frame to the encoder, it may keep a reference to it
  //internally; make sure we do not overwrite it here */
  if (av_frame_make_writable(ost->frame) < 0)
    exit(1);
  if (c->pix_fmt != AV_PIX_FMT_YUV420P) {
  //as we only generate a YUV420P picture, we must convert it
  //to the codec pixel format if needed */
    if (!ost->sws_ctx) {
      ost->sws_ctx = sws_getContext(c->width, c->height,
                                    AV_PIX_FMT_YUV420P,
                                    c->width, c->height,
                                    c->pix_fmt,
                                    SCALE_FLAGS, NULL, NULL, NULL);
      if (!ost->sws_ctx) {
        fprintf(stderr,
                "Could not initialize the conversion context\n");
        exit(1);
      }
    }
    printf("%s(%d) (%d,%d)\n",__FILE__,__LINE__,c->width, c->height);
    fill_yuv_image(ost->tmp_frame, ost->next_pts, c->width, c->height);
    sws_scale(ost->sws_ctx, (const uint8_t * const *) ost->tmp_frame->data,
              ost->tmp_frame->linesize, 0, c->height, ost->frame->data,
              ost->frame->linesize);
  } else {
    int frame_index=ost->next_pts;
    printf("%s(%d)%" PRIu64 ",(%d,%d)\n",__FILE__,__LINE__,
           ost->next_pts,c->width, c->height); 
    char fname[256];
    sprintf(fname,"%s/%c%04d.jpg",fDirectory,cprefix,frame_index+1);
    freeAll();
    ost->frame=getFrame(fname);
#if 0
    int width=c->width;
    int height=c->height;
  //fill_yuv_image(ost->frame, ost->next_pts, c->width, c->height);
    AVFrame *pict=ost->frame;
    int x,y,x2,y2;
    printf("fill_yuv_image %s(%d),%3d,%4d\n",__FILE__,__LINE__,frame_index,
            frame2->linesize[0]);
    for (y = 0; y < height; y++) {
    //printf("fill_yuv_image %s(%d) (%4d,%4d),%4d,%4d,%4d\n",__FILE__,__LINE__,x,y,
    //Ylinesize,Ulinesize,Vlinesize);
      y2 = y/2;
      for (x = 0; x < width; x++) {
        x2 = x/2;
        pict->data[0][y *  pict->linesize[0] + x]  = frame2->data[0][y *   frame2->linesize[0] + x] ;
        pict->data[1][y2 * pict->linesize[1] + x2] = frame2->data[1][y2 *  frame2->linesize[1] + x2] ;
        pict->data[2][y2 * pict->linesize[2] + x2] = frame2->data[2][y2 *  frame2->linesize[2] + x2] ;          
      }
    } 
#endif  
    printf("%s(%d)\n",__FILE__,__LINE__);
  }
  ost->frame->pts = ost->next_pts++;
  return ost->frame;
}
static int write_frame(AVFormatContext *fmt_ctx, AVCodecContext *c,
                       AVStream *st, AVFrame *frame, AVPacket *pkt,int frame_index)
{
  int ret;
//printf("%s(%d) %X,%X,%X\n",__FILE__,__LINE__,c,frame,pkt);
  if(frame==0) return 1;
// send the frame to the encoder
  ret = avcodec_send_frame(c, frame);
//printf("%s(%d) %X,%X,%X\n",__FILE__,__LINE__,c,frame,pkt);
  if (ret < 0) {
    fprintf(stderr, "Error sending a frame to the encoder: %s\n",
            av_err2str(ret));
    exit(1);
  }
  while (ret >= 0) {
  //printf("%s(%d) %X,%X,%X\n",__FILE__,__LINE__,c,frame,pkt);
    ret = avcodec_receive_packet(c, pkt);
    //printf("write_frame %s(%d) %d ret=%d,%d,%d\n",__FILE__,__LINE__,frame->data[0][2],ret,AVERROR(EAGAIN),AVERROR_EOF);
    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
      break;
    }    
    else if (ret < 0) {
      fprintf(stderr, "Error encoding a frame: %s\n", av_err2str(ret));
      exit(1);
    }
    //rescale output packet timestamp values from codec to stream timebase */
    av_packet_rescale_ts(pkt, c->time_base, st->time_base);
    pkt->stream_index = st->index;
    //Write the compressed frame to the media file. 
    //log_packet(fmt_ctx, pkt);
    ret = av_interleaved_write_frame(fmt_ctx, pkt);
    //ret = av_write_frame(fmt_ctx, pkt);
    //pkt is now blank (av_interleaved_write_frame() takes ownership of
    //its contents and resets pkt), so that no unreferencing is necessary.
    //This would be different if one used av_write_frame(). */
    if (ret < 0) {
      fprintf(stderr, "Error while writing output packet: %s\n", av_err2str(ret));
      exit(1);
    }
  }
  return ret == AVERROR_EOF ? 1 : 0;
}
static int write_video_frame(AVFormatContext *oc, OutputStream *ost,const char *fDirectory,const char cprefix)
{
  return write_frame(oc, ost->enc, ost->st, get_video_frame(ost,fDirectory,cprefix), ost->tmp_pkt, ost->next_pts);
}
#if 0
static void close_stream(AVFormatContext *oc, OutputStream *ost)
{
  if(ost->enc) avcodec_free_context(&ost->enc);
  if(ost->frame) av_frame_free(&ost->frame);
  if(ost->tmp_frame) av_frame_free(&ost->tmp_frame);
  if(ost->tmp_pkt) av_packet_free(&ost->tmp_pkt);
  if(ost->sws_ctx) sws_freeContext(ost->sws_ctx);
  if(ost->swr_ctx) swr_free(&ost->swr_ctx);
}
#endif
int generateMP4(const char *path,const char cprefix) {
  int encode_video = 0, encode_audio = 0;
////////////////////////////////////////////////////////////////////
  int ret=0;
  AVPacket *packet;
  AVFrame *frame;
  static AVFormatContext *fmt_ctx;
  const AVOutputFormat *fmt;
  AVFormatContext *oc;
  const AVCodec *video_codec;
  AVCodecContext *dec_ctx;
  AVCodec *dec;
  int video_stream_index;
  char filter_descr[sizeof("scale=3840:2160,transpose=clock")];
  OutputStream video_st = { 0 };
  AVFilterContext *buffersink_ctx;
  AVFilterContext *buffersrc_ctx;
  AVFilterGraph *filter_graph;
  AVDictionary *opt = NULL;
  char filename[256];
  sprintf(filename,"%s.mp4",path+3);printf("==>%s\n",filename);
  frame = av_frame_alloc();
  filt_frame   = av_frame_alloc();
  packet = av_packet_alloc();
  if (!frame || !filt_frame || !packet) {
    fprintf(stderr, "Could not allocate frame or packet\n");
    exit(1);
  }
//if ((ret = open_input_file(argv[1])) < 0) goto end;
//open_input_file start
  if ((ret = avformat_open_input(&fmt_ctx, filename, NULL, NULL)) < 0) {
    av_log(NULL, AV_LOG_ERROR, "Cannot open input file\n");
    return ret;
  }
  if ((ret = avformat_find_stream_info(fmt_ctx, NULL)) < 0) {
    av_log(NULL, AV_LOG_ERROR, "Cannot find stream information\n");
    return ret;
  }
//select the video stream
  ret = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, (const AVCodec**)&dec, 0);
  if (ret < 0) {
    av_log(NULL, AV_LOG_ERROR, "Cannot find a video stream in the input file\n");
    return ret;
  }
  printf("%s(%4d)\n",__FILE__,__LINE__);
  video_stream_index = ret;
//create decoding context 
  dec_ctx = avcodec_alloc_context3(dec);
  if (!dec_ctx) return AVERROR(ENOMEM);
  avcodec_parameters_to_context(dec_ctx, fmt_ctx->streams[video_stream_index]->codecpar);
//init the video decoder 
  if ((ret = avcodec_open2(dec_ctx, dec, NULL)) < 0) {
    av_log(NULL, AV_LOG_ERROR, "Cannot open video decoder\n");
    return ret;
  }
//open_input_file start finish
//sprintf(filter_descr,"scale=%d:%d,transpose=clock",GLOBAL_WIDTH,GLOBAL_HEIGHT);
//sprintf(filter_descr,"scale=%d:%d",GLOBAL_WIDTH,GLOBAL_HEIGHT);
  sprintf(filter_descr,"scale=iw:ih");
//if ((ret = init_filters(filter_descr)) < 0)
//  goto end;

//init_filters start
  const AVFilter *buffersrc  = avfilter_get_by_name("buffer");
  const AVFilter *buffersink = avfilter_get_by_name("buffersink");
  AVFilterInOut *outputs = avfilter_inout_alloc();
  AVFilterInOut *inputs  = avfilter_inout_alloc();
  AVRational time_base = fmt_ctx->streams[video_stream_index]->time_base;
//enum AVPixelFormat pix_fmts[] = { AV_PIX_FMT_GRAY8, AV_PIX_FMT_NONE };
//enum AVPixelFormat pix_fmts[] = { AV_PIX_FMT_RGB24, AV_PIX_FMT_NONE };
//enum AVPixelFormat pix_fmts[] = { AV_PIX_FMT_MONOWHITE , AV_PIX_FMT_NONE };
//enum AVPixelFormat pix_fmts[] = { AV_PIX_FMT_MONOBLACK , AV_PIX_FMT_NONE };
  enum AVPixelFormat pix_fmts[] = { STREAM_PIX_FMT, AV_PIX_FMT_NONE };
  filter_graph = avfilter_graph_alloc();
  if (!outputs || !inputs || !filter_graph) {
    ret = AVERROR(ENOMEM);
    goto end;
  }
//buffer video source: the decoded frames from the decoder will be inserted here. 
  char argstr[256];
  snprintf(argstr, sizeof(argstr),
    "video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:pixel_aspect=%d/%d",
    dec_ctx->width, dec_ctx->height, dec_ctx->pix_fmt,
    time_base.num, time_base.den,
  dec_ctx->sample_aspect_ratio.num, dec_ctx->sample_aspect_ratio.den);
  ret = avfilter_graph_create_filter(&buffersrc_ctx, buffersrc, "in",
                                     argstr, NULL, filter_graph);
  if (ret < 0) {
    av_log(NULL, AV_LOG_ERROR, "Cannot create buffer source\n");
    goto end;
  }
//buffer video sink: to terminate the filter chain.
  ret = avfilter_graph_create_filter(&buffersink_ctx, buffersink, "out",
                                     NULL, NULL, filter_graph);
  if (ret < 0) {
    av_log(NULL, AV_LOG_ERROR, "Cannot create buffer sink\n");
    goto end;
  }
//ret = av_opt_set_int_list(buffersink_ctx, "pix_fmts", pix_fmts,
//                          AV_PIX_FMT_NONE, AV_OPT_SEARCH_CHILDREN);
  ret = av_opt_set_int_list(buffersink_ctx, "pix_fmts", pix_fmts,
                            AV_PIX_FMT_NONE, AV_OPT_SEARCH_CHILDREN);
  if (ret < 0) {
    av_log(NULL, AV_LOG_ERROR, "Cannot set output pixel format\n");
    goto end;
  }
//Set the endpoints for the filter graph. The filter_graph will
//be linked to the graph described by filters_descr.
//The buffer source output must be connected to the input pad of
//the first filter described by filters_descr; since the first
//filter input label is not specified, it is set to "in" by
//default.
  outputs->name       = av_strdup("in");
  outputs->filter_ctx = buffersrc_ctx;
  outputs->pad_idx    = 0;
  outputs->next       = NULL;
//The buffer sink input must be connected to the output pad of
//the last filter described by filters_descr; since the last
//filter output label is not specified, it is set to "out" by
//default.
  inputs->name       = av_strdup("out");
  inputs->filter_ctx = buffersink_ctx;
  inputs->pad_idx    = 0;
  inputs->next       = NULL;
  if ((ret = avfilter_graph_parse_ptr(filter_graph, filter_descr,
                                  &inputs, &outputs, NULL)) < 0)
    goto end;
  if ((ret = avfilter_graph_config(filter_graph, NULL)) < 0)
    goto end;
end:
  avfilter_inout_free(&inputs);
  avfilter_inout_free(&outputs);
  printf("video_size=%dx%d:pix_fmt=%d:pixel_aspect=%d/%d\n",
          dec_ctx->width, dec_ctx->height, dec_ctx->pix_fmt,
          dec_ctx->sample_aspect_ratio.num, dec_ctx->sample_aspect_ratio.den);
//init_filter finish

//allocate the output media context
  avformat_alloc_output_context2(&oc, NULL, NULL, "img/tmp.mp4");
  if (!oc) {
    printf("Could not deduce output format from file extension: using MPEG.\n");
    avformat_alloc_output_context2(&oc, NULL, "mpeg", filename);
  }
  if (!oc) return 1;
  fmt = oc->oformat;
//void add_stream(OutputStream *ost, AVFormatContext *oc,
//                const AVCodec **codec,enum AVCodecID codec_id)
  addstream(&video_st, oc, &video_codec, fmt->video_codec,dec_ctx);
  openvideo(oc, video_codec, &video_st, opt);

  strcpy(filename,"output.mp4");
  av_dump_format(oc, 0, filename, 1);
  //open the output file, if needed
  if (!(fmt->flags & AVFMT_NOFILE)) {
    ret = avio_open(&oc->pb, filename, AVIO_FLAG_WRITE);
    if (ret < 0) {
      fprintf(stderr, "Could not open '%s': %s\n", filename,
      av_err2str(ret));
      return 1;
    }
  }
  //Write the stream header, if any.
  ret = avformat_write_header(oc, &opt);
  while (1) {
    if ((ret = av_read_frame(fmt_ctx, packet)) < 0)
      break;
    if (packet->stream_index == video_stream_index) {
      ret = avcodec_send_packet(dec_ctx, packet);
      if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Error while sending a packet to the decoder\n");
        break;
      }
      while (ret >= 0) {
        ret = avcodec_receive_frame(dec_ctx, frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
          break;
        } else if (ret < 0) {
          av_log(NULL, AV_LOG_ERROR, "Error while receiving a frame from the decoder\n");
          goto end;
        }
        frame->pts = frame->best_effort_timestamp;
        //push the decoded frame into the filtergraph 
        if (av_buffersrc_add_frame_flags(buffersrc_ctx, frame, AV_BUFFERSRC_FLAG_KEEP_REF) < 0) {
          av_log(NULL, AV_LOG_ERROR, "Error while feeding the filtergraph\n");
          break;
        }
        //pull filtered frames from the filtergraph
        while (1) {
          ret = av_buffersink_get_frame(buffersink_ctx, filt_frame);
          printf("%s(%d),%3d,%3d,%3d,%3d\n",__FILE__,__LINE__,
            frame->data[0][2],filt_frame->data[0][2],
            encode_video,encode_audio);
          if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            printf("%s(%d),%3d,%3d\n",__FILE__,__LINE__,
              encode_video,encode_audio);                   
            break;
          } 
          if (ret < 0) {
            printf("%s(%d),%3d,%3d,%3d\n",__FILE__,__LINE__,
            filt_frame->data[0][2],encode_video,encode_audio);                   
            goto end;
          }  
//        display_frame(filt_frame, buffersink_ctx->inputs[0]->time_base);
#if 1
          printf("%s(%d) %lld\n",__FILE__,__LINE__,video_st.next_pts);
          encode_video = !write_video_frame(oc, &video_st, path, cprefix);
        //copyFramebefore();
#endif
//        av_frame_unref(filt_frame);
        }
//      av_frame_unref(frame);
      }
    }
  }
  av_write_trailer(oc);
//Close each codec. 
//close_stream(oc, &video_st);
  if (!(fmt->flags & AVFMT_NOFILE))
  //Close the output file. 
    avio_closep(&oc->pb);
  //free the stream 
  if(oc) avformat_free_context(oc);
  if (ret < 0 && ret != AVERROR_EOF) {
    fprintf(stderr, "Error occurred: %s\n", av_err2str(ret));
    exit(1);
  }    
#if 1
  if(filt_frame) {
     av_frame_unref(filt_frame);
     av_frame_free(&filt_frame);
  }
  if(frame) {
     av_frame_unref(frame);
     av_frame_free(&frame);
  }
  if(packet) {
    av_packet_unref(packet);   
    av_packet_free(&packet);
  }
#endif
//avfilter_graph_free(&filter_graph);  //2025/4/1 by calc_matrix ?
  if(dec_ctx) avcodec_free_context(&dec_ctx);
  if(fmt_ctx) avformat_close_input(&fmt_ctx);
  return 1;
}

