#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image_write.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <stdio.h>
void __assert_func(const char *file, int line, const char *func, const char *failedexpr) {
    // You can customize this to print to stderr or a logging system
    // Or just leave it empty if you want to silence the error
};

//#include <stdlib.h> 
//#include <string.h>
#include <wchar.h>
#include <math.h>
#include <cstring>
#include <vector>
#define MAX_LINE_LENGTH 256
struct Button {
  SDL_Rect rect;
  bool isPressed;
  char fstr[MAX_LINE_LENGTH];
};
bool isMouseOver(int mx, int my, const SDL_Rect& rect) {
  return (mx >= rect.x && mx <= rect.x + rect.w &&
          my >= rect.y && my <= rect.y + rect.h);
}
#include <unistd.h>
SDL_Point pt[3][20];

SDL_Point ptClick;
SDL_Point predictClick;
int ptClickn=-1;
int nPt[3];
FILE *ptFile=NULL;
char fDirectory[MAX_LINE_LENGTH];
char ptfname[MAX_LINE_LENGTH];
int picSN[20];
int NpicSN;
char buffer[MAX_LINE_LENGTH];
void initfDirectory(void) {
  int len;
  strcpy(ptfname,"data.txt"); 
  strcpy(fDirectory,"../VID");
  if (access(fDirectory, F_OK) == 0) {
  }
  else {
    fDirectory[0]=0;
  }
  if (access(ptfname, F_OK) == 0) {
    ptFile = fopen(ptfname,"rt");
    // Read line-by-line until fgets returns NULL
    while (fgets(buffer, sizeof(buffer), ptFile) != NULL) {
      len=strlen(buffer);
      if(buffer[len-1]==0x0A) buffer[len-1]=0;
      if (access(buffer, F_OK) == 0) {
        strcpy(fDirectory,buffer);
        break;
      }  
    }
    if(ptFile!=NULL) fclose(ptFile); 
    ptFile=NULL;
  }
}
void initpicSN(void) {
  int i,x,j;
  char snstr[6];
  if(fDirectory[0]==0) {
    printf("initpicSN no Directory\n");
    return;
  }
  sprintf(ptfname,"%s/data.txt",fDirectory);  
  if (access(ptfname, F_OK) == 0) {
    // file exists
    ptFile = fopen(ptfname,"rt");

    // Read line-by-line until fgets returns NULL
    i=0;
    NpicSN=0;
    while (fgets(buffer, sizeof(buffer), ptFile) != NULL) {
      j=strlen(buffer);if(buffer[j-1]==0x0A) buffer[j-1]=0x0;
      x=atoi(buffer);
      sprintf(snstr,"%d",x);
//    printf("serial=[%4d],[%s],%02X\n",x,buffer,buffer[j-1]);
      if (!memcmp(buffer,snstr,strlen(buffer))) {
        picSN[i]=x;
        i++;
        NpicSN=i;
      }
      if(i>15) break;
    }
    if(ptFile!=NULL) fclose(ptFile); 
    ptFile=NULL;
    printf("ptfname=%s,NpicSN=%d\n",ptfname,NpicSN);
  }
}
void initPoint(int picID) {
  char coor[6];
  int totp=0;
  int x,y;
  char *split;
  ptClick.x = -1;
  ptClick.y = -1;
  sprintf(ptfname,"%s/point%d.txt",fDirectory,picSN[picID]);  
  if (access(ptfname, F_OK) == 0) {
    // file exists
    ptFile = fopen(ptfname,"rt");

    // Read line-by-line until fgets returns NULL
    while (fgets(buffer, sizeof(buffer), ptFile) != NULL) {
        split=strstr(buffer,",");
        if(split!=NULL && split!=buffer) {
          memcpy(coor,buffer,split-buffer);coor[split-buffer]=0;
          x=atoi(coor);
          strcpy(coor,split+1);
          y=atoi(coor);
          pt[picID][totp].x=x; pt[picID][totp].y=y;
          printf("%s,(picID=%d),[%d],x=%4d,y=%4d\n", ptfname,picID,totp,pt[picID][totp].x, pt[picID][totp].y);
          totp++;
          if(totp>15) break;
        } 
    }
    if(ptFile!=NULL) fclose(ptFile); 
    ptFile=NULL;
    nPt[picID] = totp;
  } else {
    // file doesn't exist
    ptFile = fopen(ptfname,"wt");
    if(ptFile!=NULL) fclose(ptFile); 
    ptFile=NULL;
    nPt[picID] = 0;
  }
}
int getPoint(int picID,int x,int y) {
  int n;
  //search
  for(n=0;n<nPt[picID];n++) {
    if(x==pt[picID][n].x && y==pt[picID][n].y) {
      return n;
    }
  }
  return -1;
}
int putPoint(int picID,int x,int y) {
  int n=nPt[picID],i;
  char buffer[MAX_LINE_LENGTH];
  char ptfname[256];
  if(getPoint(picID,x,y)==-1) 
  {
    pt[picID][n].x=x;pt[picID][n].y=y;
    sprintf(ptfname,"%s/point%d.txt",fDirectory,picSN[picID]);  
    n++;if(n>15) n=15; nPt[picID]=n; 
    ptFile = fopen(ptfname,"wt");
    for(i=0;i<nPt[picID];i++) {
        sprintf(buffer,"%d,%d\n", pt[picID][i].x, pt[picID][i].y);
        fputs(buffer, ptFile);
    }
    fclose(ptFile);

    return n;
  }
  return n;
}
int nowpicID=0;
void DrawCircle(SDL_Renderer *renderer, int32_t centreX, int32_t centreY, int32_t radius) {
    const int32_t diameter = (radius * 2);
    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    while (x >= y) {
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

        if (error <= 0) {
            ++y;
            error += ty;
            ty += 2;
        }
        if (error > 0) {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}

// Configure button specs: X, Y, Width, Height
Button myButton[6]= { { {  40, 50, 40, 40 }, false, "" },
                      { {  90, 50, 40, 40 }, false, "" },
                      { { 140, 50, 40, 40 }, false, "" },
                      { { 190, 50, 40, 40 }, false, "" },
                      { { 240, 50, 40, 40 }, false, "" },
                      { { 290, 50, 40, 40 }, false, "" },
                    };
const int width  = 2160;
const int height = 3840;
//Screen dimension constants
const int SCREEN_WIDTH = 972;
const int SCREEN_HEIGHT = 576;
// Set up source rectangle (e.g., cropping a 400x300 chunk starting at x=50, y=50)
//SDL_Rect srcRect = {1200, 1000, 1056, 594};
SDL_Rect srcRect   = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
// Set up destination rectangle (same size for true 1:1 "real" size)
SDL_Rect srcMRect  = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
SDL_Rect destRect  = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}; 
SDL_Rect destMRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}; 

void Draw4K(SDL_Surface* surface,SDL_Renderer* renderer0, int yid) {
    SDL_Rect img_rect2;
    img_rect2.x    = 0;
    img_rect2.y    = 0;
    img_rect2.w    = width;
    img_rect2.h    = height;
    // Create a hidden window & 4K software renderer
    SDL_Surface* surface3 = IMG_Load(myButton[yid].fstr);
//  SDL_Window* window2 = SDL_CreateWindow("4K Circle", 0, 0, width, height, SDL_WINDOW_HIDDEN);
    SDL_Window* window2 = SDL_CreateWindow("4K Circle", 0, 0, width, height, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer2 = SDL_CreateRenderer(window2, -1, SDL_RENDERER_SOFTWARE);
    SDL_Texture* texture2 = SDL_CreateTextureFromSurface(renderer2, surface3);
    SDL_Surface* surface2 = SDL_ConvertSurfaceFormat(surface3, SDL_PIXELFORMAT_ARGB8888, 0);
    
    // Set background to black
//  SDL_SetRenderDrawColor(renderer2, 255, 255, 255, 255);
//  SDL_RenderClear(renderer2);

//  SDL_RenderCopy(renderer2, texture, &img_rect2, &img_rect2);
//  SDL_RenderPresent(renderer2);

    // Draw a white circle in the center with a radius of 500 pixels
    printf("%d\n",__LINE__);
//    SDL_SetRenderDrawColor(renderer2, 255, 255, 255, 255);
//    SDL_RenderClear(renderer2);
    SDL_RenderCopy(renderer2, texture2, NULL, &img_rect2); 
//  SDL_RenderPresent(renderer2);    //present renderer
//    SDL_SetRenderDrawColor(renderer2, 0,  0, 0, 255);
//    DrawCircle(renderer2, 100, 100, 100); 

        int n=nPt[0];
        if(n>10) n=10;
        for(int i=0;i<n;i++) {
//        circleColor(renderer, pt[nowpicID][i].x, pt[nowpicID][i].y, 50, 0xFF0000FF);
          SDL_SetRenderDrawColor(renderer2, 0, 0, 255, 255); 
          DrawCircle(renderer2, pt[nowpicID][i].x-srcRect.x, pt[nowpicID][i].y-srcRect.y, 400);
        }
  
    SDL_RenderPresent(renderer2);    //present renderer
    SDL_Delay(500);
//  SDL_RenderPresent(renderer2);
    SDL_RenderReadPixels(renderer2, NULL,surface2->format->format, surface2->pixels, surface2->pitch);
    SDL_RenderClear(renderer2); 

    // Allocate buffer for 4K pixels
//  std::vector<Uint32> pixels(width * height);
//  SDL_LockSurface(surface);
    // Calculate the total size in bytes (pitch * height)
    // Assuming 32-bit (4 bytes) pixels, we divide by 4 to get the vector size
    size_t pixelCount = (surface2->w * surface2->h) ;
    printf("%d,%lld,%d,%d,%d\n",__LINE__,pixelCount,surface2->pitch,surface2->h,surface2->w);  
    // Create the vector and directly copy the pixel memory
    Uint32* pixelsData = static_cast<Uint32*>(surface2->pixels);
    std::vector<Uint32> pixels2(pixelsData, pixelsData + pixelCount);
//  std::vector<Uint32> pixels(pixels2);
    // Unlock surface when done
//  SDL_RenderReadPixels(renderer2, NULL, SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch);

    // Read the pixels from the renderer into memory
//  SDL_RenderReadPixels(renderer2, NULL, SDL_PIXELFORMAT_ARGB8888, pixels.data(), width * sizeof(Uint32));

    // Save as 4K JPG (quality: 90)
//  stbi_write_jpg("output_4k.jpg", width, height, 4, pixels.data(), 90);
    sprintf(ptfname,"%s/y%04d.jpg",fDirectory,yid);  
//  stbi_write_jpg(ptfname, width, height, 4, pixels.data(), 90);
    stbi_write_jpg(ptfname, width, height, 4, pixels2.data(), 90);
//  SDL_UnlockSurface(surface);

    // Clean up
    SDL_DestroyRenderer(renderer2);
    SDL_DestroyWindow(window2);
}
int dfvmux3diff_main(int argc, char **argv);
void getver(wchar_t *pDest, int size, const wchar_t *fixstr);
#include <string>
std::string WStringToString(const std::wstring& wstr);
void mProduct(double x,double y);
int getframe(const char *filename, int frame_index);

//The window we'll be rendering to
SDL_Window* gWindow = NULL;
//The window renderer
SDL_Renderer* gRenderer = NULL;
//Globally used font
TTF_Font* gFont = NULL;
//Scene textures
std::string PromptText = "Enter Text:"; 
//Texture wrapper class
class LTexture
{
  public:
  //Initializes variables
	LTexture();
  //Deallocates memory
	~LTexture();
  //Loads image at specified path
	bool loadFromFile( std::string path );
	#if defined(SDL_TTF_MAJOR_VERSION)
	//Creates image from font string
	bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
	#endif
	//Deallocates texture
	void free();
	//Set color modulation
	void setColor( Uint8 red, Uint8 green, Uint8 blue );
	//Set blending
	void setBlendMode( SDL_BlendMode blending );
	//Set alpha modulation
	void setAlpha( Uint8 alpha );
	//Renders texture at given point
	void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );
	//Gets image dimensions
	int getWidth();
	int getHeight();
  private:
  //The actual hardware texture
	SDL_Texture* mTexture;
	//Image dimensions
	int mWidth;
	int mHeight;
};
LTexture gPromptTextTexture;
LTexture gInputTextTexture;
LTexture::LTexture()
{
//Initialize
  mTexture = NULL;
  mWidth = 0;
  mHeight = 0;
}
LTexture::~LTexture()
{
//Deallocate
  free();
}
bool LTexture::loadFromFile( std::string path )
{
//Get rid of preexisting texture
  free();
//The final texture
  SDL_Texture* newTexture = NULL;
//Load image at specified path
  SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
  if( loadedSurface == NULL )
  {
    printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
  }
  else
  {
  //Color key image
    SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );
  //Create texture from surface pixels
    newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
    if( newTexture == NULL )
	{
	  printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
	}
	else
	{
	//Get image dimensions
	  mWidth = loadedSurface->w;
	  mHeight = loadedSurface->h;
	}
	//Get rid of old loaded surface
	SDL_FreeSurface( loadedSurface );
  }
  //Return success
  mTexture = newTexture;
  return mTexture != NULL;
}
#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
//Get rid of preexisting texture
  free();
//Render text surface
  SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
  if( textSurface != NULL )
  {
  //Create texture from surface pixels
    mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
    if( mTexture == NULL )
	{
	  printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
	}
	else
	{
	//Get image dimensions
	  mWidth = textSurface->w;
	  mHeight = textSurface->h;
	}
	//Get rid of old surface
	SDL_FreeSurface( textSurface );
  }
  else
  {
    printf( "(%4d)(%s), Unable to render text surface! SDL_ttf Error: %s\n", __LINE__,textureText.c_str(),TTF_GetError() );
  }
  //Return success
  return mTexture != NULL;
}
#endif
void LTexture::free()
{
  //Free texture if it exists
  if( mTexture != NULL )
  {
	SDL_DestroyTexture( mTexture );
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
  }
}
void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
//Modulate texture rgb
  SDL_SetTextureColorMod( mTexture, red, green, blue );
}
void LTexture::setBlendMode( SDL_BlendMode blending )
{
//Set blending function
  SDL_SetTextureBlendMode( mTexture, blending );
}
void LTexture::setAlpha( Uint8 alpha )
{
//Modulate texture alpha
  SDL_SetTextureAlphaMod( mTexture, alpha );
}
void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
//Set rendering space and render to screen
  SDL_Rect renderQuad = { x, y, mWidth, mHeight };
//Set clip rendering dimensions
  if( clip != NULL )
  {
	renderQuad.w = clip->w;
	renderQuad.h = clip->h;
  }
//Render to screen
  SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}
int LTexture::getWidth()
{
  return mWidth;
}
int LTexture::getHeight()
{
  return mHeight;
}
bool loadMedia()
{
//Loading success flag
  bool success = true;
//Open the font
//gFont = TTF_OpenFont( "32_text_input_and_clipboard_handling/lazy.ttf", 28 );
//this opens a font style and sets a size
//TTF_Font* gFont = TTF_OpenFont("FreeSans.ttf", 24);
  gFont = TTF_OpenFont("m.ttf", 25);
//TTF_Font* gFont = TTF_OpenFont( "C:/Windows/Fonts/kaiu.ttf", 25 );//???, ????C:/Windows/Fonts????
//this is the color in rgb format,
//maxing out all would give you the color white,
//and it will be your text's color
//gFont = TTF_OpenFont( "lazy.ttf", 28 );
  if( gFont == NULL )
  {
	printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
	success = false;
  }
  else
  {
  //Render the prompt
	SDL_Color textColor = { 0, 0, 0, 0xFF };
	if( !gPromptTextTexture.loadFromRenderedText( PromptText, textColor ) )
	{
  	  printf( "Failed to render prompt text!\n" );
	  success = false;
	}
  }
  return success;
}
void testToolBox2(int x,int y);
void loadtmp(void);
void getYUV(int frame_index,int x,int y,int *Y,int *U,int *V);
//matrix computation
#include "matrix.h"
int sdl_main(int argc, char* argv[]) {
  int i;
  wchar_t verstr[128] = {0};
//Print Chinese string using wide characters
//wprintf(L"你好，世界！\n");
  getver(verstr, sizeof(verstr), L"arithai.com/dfvmux3diff 葉綠素生技");
  wprintf_s(L"verstr=%ws\n",verstr);
//wprintf(L"===葉綠素生技\n");
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL Init Failed: %s\n", SDL_GetError());
    return 1;
  }
 
  initfDirectory();
  printf("Get directory [%s]<======\n",fDirectory);
    
  initpicSN();
  if(NpicSN<3) {
    printf("No pictures are assigned.\n");exit(0);
  } 
  for(i=0;i<NpicSN;i++) {
    printf("picSN[%4d]=%04d\n",i,picSN[i]);
  }
  predictClick.x=-1;predictClick.y=-1;

  initPoint(0);
//putPoint(0,7,7);
  initPoint(1);
  initPoint(2);

//Initialize SDL_image for JPG
  int flags = IMG_INIT_JPG;
  if ((IMG_Init(flags) & flags) != flags) {
    printf("IMG Init Failed: %s\n", IMG_GetError());
    SDL_Quit();
  }

  std::string utf8Title = WStringToString(verstr);
//Create Window and Renderer
//gWindow = SDL_CreateWindow(utf8Title.c_str(), SDL_WINDOWPOS_CENTERED, 
//    SDL_WINDOWPOS_CENTERED, 972, 576, SDL_WINDOW_SHOWN);
  gWindow = SDL_CreateWindow( utf8Title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
      SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
//SDL_Window* window = SDL_CreateWindow("Real Size JPG", 0, 0, width, height, SDL_WINDOW_SHOWN);
//SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_SOFTWARE);
//gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
//Load JPG into a surface
  sprintf(myButton[0].fstr,"%s/x%04d.jpg",fDirectory,picSN[0]);
  sprintf(myButton[1].fstr,"%s/x%04d.jpg",fDirectory,picSN[1]);
  sprintf(myButton[2].fstr,"%s/x%04d.jpg",fDirectory,picSN[2]);
  SDL_Surface* surface = IMG_Load(myButton[0].fstr);
  if (!surface) {
    printf("Failed to load image: %s\n", IMG_GetError());
    IMG_Quit();
    SDL_Quit();
    return 1;
  }
//Convert surface to texture
  SDL_Rect img_rect;
#if 1
  SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer, surface);
#else
  SDL_Texture* texture = SDL_CreateTexture(gRenderer, 
      SDL_PIXELFORMAT_RGBA8888, 
      SDL_TEXTUREACCESS_STREAMING, 
      surface->w, surface->h);
    
  SDL_LockTexture(texture, &img_rect, &surface->pixels, &surface->pitch);
//paint into surface pixels
  SDL_UnlockTexture(texture);
#endif
  SDL_QueryTexture(texture, NULL, NULL, &img_rect.w, &img_rect.h);
  img_rect.x=0;img_rect.y=0;
  printf("(%3d) (%4d,%4d,%4d,%4d)\n",__LINE__,img_rect.x, img_rect.y,img_rect.w, img_rect.h);
//SDL_FreeSurface(surface); // We don't need the surface anymore
     
  if (TTF_Init() < 0)
  {
    printf("Couldn't initialize SDL TTF: %s\n", SDL_GetError());
    exit(1);
  }
  if( !loadMedia() )
  {
	printf( "Failed to load media!\n" );
    exit(1);
  }
  SDL_Color White = {255, 255, 255, 255};
//wchar_t MyString[]=L"arithai.com 葉綠素生技 V2026.07.22     .01!";
  wchar_t *MyString=verstr;
  int MyStringLengh = wcslen(MyString); 
  char DecToHex[20]; 
  Uint16 PrintMyString[MyStringLengh];
//setlocale(LC_ALL, "en_US.utf8");
//swprintf(MyString, sizeof MyString/sizeof *MyString,
//         L"arithai.com 葉綠素生技 V'%s'.01", __DATE__); 
  for (int i = 0; i < MyStringLengh; i++) {
//itoa(MyString[i], DecToHex, 16);  
  snprintf(DecToHex, sizeof(DecToHex), "%02X", MyString[i]); 
//wchar MyString to utf5 DecToHex
    PrintMyString[i]= strtol(DecToHex,NULL,16);
  }
  PrintMyString[MyStringLengh]={0};
//wprintf(L"[%s]中\n",MyString);
 
//as TTF_RenderText_Solid could only be used on
//SDL_Surface then you have to create the surface first
  SDL_Surface* surfaceMessage =
//TTF_RenderText_Solid(gFont, "arithai.com v1.1!", White); 
//TTF_RenderText_Blended(gFont, "arithai.com 葉綠素生技 v1.1!", White);
  TTF_RenderUNICODE_Blended(gFont, PrintMyString, White);
//TTF_RenderUNICODE_Solid( gFont, PrintMyString, White );
//now you can convert it into a texture
  SDL_Texture* Message = SDL_CreateTextureFromSurface(gRenderer, surfaceMessage);

  SDL_Rect Message_rect; //create a rect
  Message_rect.x = SCREEN_WIDTH-600;  //controls the rect's x coordinate 
  Message_rect.y = SCREEN_HEIGHT-125; // controls the rect's y coordinte
//Message_rect.w = 200; // controls the width of the rect
//Message_rect.h = 200; // controls the height of the rect
  Message_rect.w = surfaceMessage->w; // controls the width of the rect
  Message_rect.h = surfaceMessage->h; // controls the height of the rect

//(0,0) is on the top left of the window/screen,
//think a rect as the text's box,
//that way it would be very simple to understand

//Now since it's a texture, you have to put RenderCopy
//in your game loop area, the area where the whole code executes

//you put the renderer's name first, the Message,
//the crop size (you can ignore this if you don't want
//to dabble with cropping), and the rect which is the size
//and coordinate of your texture

  int zoomFactorX = 18; // Pixels to scale per scroll
  int zoomFactorY = 32; // Pixels to scale per scroll
  int offset_x = 0, offset_y = 0;
  bool is_dragging = false;
  bool is_zooming = false;
  bool is_ClickingOnly = false;
  bool is_leftclick = false;
  char rightclickcount = 0;
//Rendering loop 
  int quit = 0;
  int is_extending = 0;
  SDL_Event e;
  int result = 0;
  char cmd[10]={0};
  char *cmdp=cmd;
  int frame_index = 2;
  int cursorn=0;
  std::string cursors[2]={"_","|"};

  //Set text color as black
  SDL_Color textColor = { 0, 0, 0, 0xFF };
  //The current input text.
  std::string inputText = " ";
  gInputTextTexture.loadFromRenderedText( inputText.c_str(), textColor );
  inputText = "";
  //Enable text input
  SDL_StartTextInput();

  Uint64 startTick = SDL_GetTicks();
  Uint64 finalTick = SDL_GetTicks();
  while (!quit) {
	//The rerender text flag
	bool renderText = false;
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
        quit = 1;
      }
      else if (e.type == SDL_KEYDOWN) {
        #if 0				
        typedef struct {
          Uint8 scancode;
          SDLKey sym;
          SDLMod mod;
          Uint16 unicode; unused
        } SDL_keysym;
        #endif
        printf("%3d %02X,%02X,%02X,%04X\n",__LINE__,e.key.keysym.scancode,e.key.keysym.sym,
               e.key.keysym.mod,e.key.keysym.unused);
      //Triggered exactly when a key is pressed down
      //Check which specific key was pressed
        switch (e.key.keysym.sym) {
          case SDLK_BACKSPACE:
            if(inputText.length() > 0) {
			//lop off character
			  inputText.pop_back();
			  renderText = true;
            }
            printf("BACKSPACE pressed!\n");
            break;
          case SDLK_c:
            if(SDL_GetModState() & KMOD_CTRL) {
              SDL_SetClipboardText( inputText.c_str() );
            }
            else if(SDL_GetModState() & KMOD_SHIFT) {
            }
            else {
              if(e.key.keysym.mod==0x2000)
                inputText += toupper(e.key.keysym.sym);
              else
                inputText += e.key.keysym.sym;
   			  renderText = true;
              printf("%4d+,Ctrl-c pressed!\n",__LINE__);
            }
            printf("Ctrl-c pressed!\n");
            break;
          case SDLK_v:
            if(SDL_GetModState() & KMOD_CTRL) {
			//Copy text from temporary buffer
 			  char* tempText = SDL_GetClipboardText();
			  inputText = tempText;
			  SDL_free( tempText );
  			  renderText = true;
            }
            else if(SDL_GetModState() & KMOD_SHIFT) {
            }
            else {
              if(e.key.keysym.mod==0x2000)
                inputText += toupper(e.key.keysym.sym);
              else
                inputText += e.key.keysym.sym;
   			  renderText = true;
              printf("%4d+,Ctrl-v pressed!\n",__LINE__);
            }
            printf("%4d,Ctrl-v pressed!\n",__LINE__);
            break;
          case SDLK_ESCAPE:
            printf("Escape key pressed! Exiting...\n");
            quit =1;
            break;
          case SDLK_UP:
            printf("Up arrow pressed!\n");
            break;
          case SDLK_SPACE:
            inputText += e.key.keysym.sym;
			renderText = true;
            printf("%4d+,Space pressed!\n",__LINE__);
            break;
          case SDLK_RETURN:
            if(inputText=="cal") {
              mProduct(1.1,2.2);
              PromptText = "mProduct done!";
              printf("Enter Return pressed![%s]!\n",inputText.c_str());
			  renderText = true;
            } //show YUV 2026.08.10
            else if(inputText=="test") {
              int x0=ptClick.x;
              int y0=ptClick.y;
              if(ptClick.x<0) x0=0;
              if(ptClick.y<0) y0=0;
              testToolBox2(x0,y0);
              surface = IMG_Load("../VID20260802132623/tmp.jpg");
              texture = SDL_CreateTextureFromSurface(gRenderer, surface);
              PromptText = "testToolBox2 done!";
              printf("Enter Return pressed![%s]!\n",inputText.c_str());
			  renderText = true;
            }
            else if(inputText=="loadtmp") {
              loadtmp();
              surface = IMG_Load("../VID20260802132623/tmp.jpg");
              texture = SDL_CreateTextureFromSurface(gRenderer, surface);
              PromptText = "loadtmp done!";
              printf("Enter Return pressed![%s]!\n",inputText.c_str());
			  renderText = true;
            }
            else if(inputText=="getYUV") {
#define CLIP(X) ( (X) > 255 ? 255 : (X) < 0 ? 0 : X)
// YUV -> RGB
#define C(Y) ( (Y) - 16  )
#define D(U) ( (U) - 128 )
#define E(V) ( (V) - 128 )
#define YUV2R(Y, U, V) CLIP(( 298 * C(Y)              + 409 * E(V) + 128) >> 8)
#define YUV2G(Y, U, V) CLIP(( 298 * C(Y) - 100 * D(U) - 208 * E(V) + 128) >> 8)
#define YUV2B(Y, U, V) CLIP(( 298 * C(Y) + 516 * D(U)              + 128) >> 8)
              if(ptClick.x>=0 && ptClick.y>=0) {
                char Prompt[256];
                int frame_index0=picSN[nowpicID];
                int x0=ptClick.x;
                int y0=ptClick.y;
                int Y0,U0,V0;
                getYUV(frame_index0,x0,y0,&Y0,&U0,&V0);
                sprintf(Prompt,"pt [%4d,%4d]=[%3d,%3d,%3d],[%3d,%3d,%3d]",x0,y0,Y0,U0,V0,
                  YUV2R(Y0,U0,V0),YUV2G(Y0,U0,V0),YUV2B(Y0,U0,V0));
                PromptText = Prompt;
                printf("Enter Return pressed![%s][%s]!\n",inputText.c_str(),Prompt);
			    renderText = true;
              }
              else {
                printf("please select\n");
              }
            }
            else {
              printf("Enter Return pressed!(%s)\n",inputText.c_str());
            }
            break;
          default:
            if(SDL_GetModState() & KMOD_SHIFT) {
              printf("%4d shift,<<%c>>\n",__LINE__,e.key.keysym.sym);
            }
            else {
              if(e.key.keysym.mod==0 || 
                (e.key.keysym.mod==2 && e.key.keysym.sym>0) ) {
			    inputText += e.key.keysym.sym;
			    renderText = true;
                printf("%4d+,<<%c>>\n",__LINE__,e.key.keysym.sym);
                break;
              }
              else if(e.key.keysym.mod==0x2000) { //CAPS
			    inputText += toupper(e.key.keysym.sym);
			    renderText = true;
                printf("%4d+,<<%c>>\n",__LINE__,e.key.keysym.sym);
                break;
              }
            }
            char *cmdn;
            int n;
            printf("%4d,<<%c>>\n",__LINE__,e.key.keysym.sym);
            if(e.key.keysym.sym=='a') {
#if 0
  vector_t *u1=(vector_t *)ivector_new(pt[nowpicID][0].x,pt[nowpicID][0].y);
  vector_t *u2=(vector_t *)ivector_new(pt[nowpicID][1].x,pt[nowpicID][1].y);
  vector_t *xv1=(vector_t *)ivector6_new(0,0.1,0.3,0.2,0,0.8);
  matrix_t *A=(matrix_t *)matrix_from_vectors(u1,u2);
  matrix_t *b=(matrix_t *)SolveB_from_Ax(A,xv1);
  vector_t *u3=(vector_t *)ivector_new(pt[nowpicID][2].x,pt[nowpicID][2].y);
  vector_t *xv3=(vector_t *)matrix_mult_vector(b, u3);
  vector_print(xv3);
  vector_t *xv4=(vector_t *)matrix_mult_vector(b, u1);
  vector_print(xv4);
  vector_t *xv5=(vector_t *)matrix_mult_vector(b, u2);
  vector_print(xv5);
  printf("=================\n");
  lr_main();
#endif
//u1->v1 u2->v2 u3->v3
  vector_t *u1=ivector3_new(pt[0][0].x,pt[0][0].y,1.0);
  vector_t *u2=ivector3_new(pt[0][1].x,pt[0][1].y,1.0);
  vector_t *u3=ivector3_new(pt[0][2].x,pt[0][2].y,1.0);
  vector_t *v1=ivector3_new(pt[1][0].x,pt[1][0].y,1.0);  ////<== row vector 1
  vector_t *v2=ivector3_new(pt[1][1].x,pt[1][1].y,1.0);  ////<== row vector 2
  vector_t *v3=ivector3_new(pt[1][2].x,pt[1][2].y,1.0);  ////<== row vector 3
  matrix_t *BM=computeBM_from_UV(u1,u2,u3,v1,v2,v3);
  predictClick.x=-1;predictClick.y=-1;
  if(BM!=NULL) {
    printf("BM ");matrix_print(BM);
    vector_t *u4=ivector3_new(pt[0][3].x,pt[0][3].y,1.0);
    vector_t *v4=matrix_mult_vector(BM,u4);
    printf("va ");vector_print(v4);
    predictClick.x=(int)v4->data[0];predictClick.y=(int)v4->data[1];
    printf("%s(%4d) %s (%4d,%4d),(%4d,%4d)\n",__FILE__,__LINE__,argv[1],
         srcRect.x,srcRect.y,predictClick.x-srcRect.x,predictClick.y-srcRect.y);
  }
  else {
    printf("Please select other points.\n");
  }

            }
            else if(e.key.keysym.sym=='z') {
              printf("please press a key...\n");

              while (true) {
                char ch = getchar();
                if (ch == 0x0A) {
                  *cmdp=0;printf("ch==0x0A,%s\n",cmd);
                  if(!memcmp(cmd,"load",4)) {
                    cmdn = cmd+5;
                    n = atoi(cmdn);
                    frame_index = n;
                    printf("load %d\n",n);
                    myButton[4].isPressed = true;
                  }
                  cmd[0]=0;cmdp=cmd;
                  break;
                }
                else if (ch != EOF) {
                //printf("%c",ch);
                  if (ch == 'q') break; // Exit loop if 'q' is pressed
                  cmdp[0]=ch;cmdp++;
                }
                usleep(10000); // Sleep for 10ms to prevent 100% CPU usage
              }
              printf("exit press a key...\n");
            }
            break;
        } //switch
      }
// Capture committed text from keyboard or IME
      else if (e.type == SDL_TEXTINPUT) {
         printf("%s(%3d) TEXTINPUT %s\n",__FILE__,__LINE__,e.text.text);
 	    //Not copy or pasting
		if( !( SDL_GetModState() & KMOD_CTRL && ( e.text.text[ 0 ] == 'c' || e.text.text[ 0 ] == 'C' || e.text.text[ 0 ] == 'v' || e.text.text[ 0 ] == 'V' ) ) )
		{
		//Append character
		  inputText += e.text.text;
		  renderText = true;
          printf("%3d+ %c\n",__LINE__,e.text.text[0]);
		}
        else {
		  inputText += e.text.text;
		  renderText = true;
          printf("%3d+ %c\n",__LINE__,e.text.text[0]);
	    }
      }
      // Optional: Capture composition changes (IME pre-edit state)
      else if (e.type == SDL_TEXTEDITING) {
        printf("Editing: TEXTEDITING %s (cursor at %d)\n", 
          e.text.text, e.edit.start);
      }
      else if (e.type == SDL_MOUSEWHEEL) {
        is_dragging = false;
        is_zooming = true;
        is_extending = 0;
      //SDL_QueryTexture(texture, NULL, NULL, &img_rect.w, &img_rect.h);
      //Adjust dimensions on mouse wheel
        if (e.wheel.y > 0) { // Scroll Up (Zoom In)
          destRect.w += zoomFactorX;
          destRect.h += zoomFactorY;
          destRect.x -= zoomFactorX / 2; // Center the zoom
          destRect.y -= zoomFactorY / 2;
        } else if (e.wheel.y < 0) { // Scroll Down (Zoom Out)
          destRect.w -= zoomFactorX;
          destRect.h -= zoomFactorY;
          destRect.x += zoomFactorX / 2;
          destRect.y += zoomFactorY / 2;
        }
      //srcRect.x = img_rect.x;
      //srcRect.y = img_rect.y;
//Remapping if zoom out 2026.7.4
        if(srcRect.x>0 && srcRect.y>0 && destRect.x>0 && destRect.y>0 && destRect.h>48) {
          printf("**[%4d,%4d,%4d,%4d],z=(%4d,%4d,%4d),s=(%4d,%4d,%4d,%4d),d=(%4d,%4d,%4d,%4d)\n",
                 img_rect.x, img_rect.y, img_rect.w, img_rect.h,  
                 zoomFactorX,zoomFactorY,e.wheel.y,
                 srcRect.x, srcRect.y, srcRect.w, srcRect.h,
                 destRect.x, destRect.y, destRect.w, destRect.h
                );
          srcMRect.x=srcRect.x-srcRect.x*destRect.x/972;
          srcMRect.y=srcRect.y-srcRect.y*destRect.y/576;
          srcMRect.w=972*srcRect.w/destRect.w;
          srcMRect.h=576*srcRect.h/destRect.h;
          if(srcMRect.h>3840) {
            srcMRect.x=0;
            srcMRect.y=0;
            srcMRect.w=2160;
            srcMRect.h=3840;

            destMRect.x=324;
            destMRect.y=0;
            destMRect.w=324;
            destMRect.h=576;               
          }  
          else {
            destMRect.x=0;
            destMRect.y=0;
            destMRect.w=972;
            destMRect.h=576;               
          }
          is_extending = 1;
        }
        SDL_QueryTexture(texture, NULL, NULL, &img_rect.w, &img_rect.h);
        printf(">>[%4d,%4d,%4d,%4d],z=(%4d,%4d,%4d),s=(%4d,%4d,%4d,%4d),d=(%4d,%4d,%4d,%4d)\n",
               img_rect.x, img_rect.y, img_rect.w, img_rect.h,  
               zoomFactorX, zoomFactorY,e.wheel.y,
               srcMRect.x, srcMRect.y, srcMRect.w, srcMRect.h,
               destRect.x, destRect.y, destRect.w, destRect.h
        );
      }
      else if (e.type == SDL_MOUSEBUTTONDOWN) {
        int mouse_x, mouse_y;
        is_zooming = false;
        is_ClickingOnly = false;
        Uint32 mouseState = SDL_GetMouseState(&mouse_x, &mouse_y);
        if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
          is_leftclick = true;
          rightclickcount=0;
        }
      //right click cancel it
        if (mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
          is_leftclick = false;
          rightclickcount++;
        }
        printf("(%4d) mx=%4d,my=%4d,ix=%4d,iy=%4d,iw=%4d,ih=%4d\n",
               __LINE__,mouse_x, mouse_y,
               img_rect.x,img_rect.y,img_rect.w,img_rect.h);
        if (false) {
        }
        else if (isMouseOver(mouse_x, mouse_y, myButton[5].rect)) {
          myButton[5].isPressed = true;
        }
        else if (isMouseOver(mouse_x, mouse_y, myButton[4].rect)) {
          myButton[4].isPressed = true;
        }
        else if (isMouseOver(mouse_x, mouse_y, myButton[0].rect)) {
          myButton[0].isPressed = true;
          printf("Button Clicked!\n");
          surface = IMG_Load(myButton[0].fstr);
          texture = SDL_CreateTextureFromSurface(gRenderer, surface);
          nowpicID=0;
        }
        else if (isMouseOver(mouse_x, mouse_y, myButton[1].rect)) {
          myButton[1].isPressed = true;
          printf("Button Clicked!\n");
          surface = IMG_Load(myButton[1].fstr);
          texture = SDL_CreateTextureFromSurface(gRenderer, surface);
          nowpicID=1;
        }
        else if (isMouseOver(mouse_x, mouse_y, myButton[2].rect)) {
          myButton[2].isPressed = true;
          printf("Button Clicked!\n");
          surface = IMG_Load(myButton[2].fstr);
          texture = SDL_CreateTextureFromSurface(gRenderer, surface);                     
          nowpicID=2;
        }
        else if (isMouseOver(mouse_x, mouse_y, myButton[3].rect)) {
          myButton[3].isPressed = true;
          Draw4K(surface,gRenderer,0);
          Draw4K(surface,gRenderer,1);
          Draw4K(surface,gRenderer,2);
          nowpicID=-1;
        }
        else {
          if (mouse_x >= img_rect.x && mouse_x <= img_rect.x + img_rect.w &&
              mouse_y >= img_rect.y && mouse_y <= img_rect.y + img_rect.h) {
            is_dragging = true;
            offset_x = mouse_x - img_rect.x;
            offset_y = mouse_y - img_rect.y;
#ifdef DEBUG
            printf("%d mx=%4d,my=%4d,ix=%4d,iy=%4d,iw=%4d,ih=%4d\n",
                   __LINE__,mouse_x, mouse_y,
                   img_rect.x,img_rect.y,img_rect.w,img_rect.h);
#endif
          }
          is_ClickingOnly = true;
        }    
      } else if (e.type == SDL_MOUSEBUTTONUP) {
        if(is_ClickingOnly) {
          int mouse_x, mouse_y;
          int x,y,dx,dy;
          SDL_GetMouseState(&mouse_x, &mouse_y);
          x = mouse_x - img_rect.x;   
          y = mouse_y - img_rect.y;

          if(ptClick.x==-1 && ptClick.y==-1) {
            ptClick.x = x;
            ptClick.y = y;
          }
          else {
            if(is_leftclick) {
            // Left click action here
              dx = x-ptClick.x;
              dy = y-ptClick.y;
              if(sqrt(dx*dx+dy*dy)<50) {
                printf("(%3d) x=%3d,y=%3d,px=%3d,py=%3d,now=%3d,pic=%3d\n",__LINE__,x,y,
                ptClick.x,ptClick.y,nowpicID,picSN[nowpicID]); 
                putPoint(nowpicID, ptClick.x, ptClick.y);
                printf("%d Click,%3d,%4d,%4d,%4d,%4d,%4d,%4d\n",__LINE__,nowpicID, mouse_x, mouse_y,
                     img_rect.x,img_rect.y,img_rect.w,img_rect.h);
              }
              else {
                ptClick.x=x;  ptClick.y=y;
              }
            }
            else {
              if(rightclickcount==1) {
                ptClickn=-1;
                for(i=0;i<nPt[nowpicID];i++) {
                  dx = x-pt[nowpicID][i].x;
                  dy = y-pt[nowpicID][i].y;
                  if(sqrt(dx*dx+dy*dy)<=7) {
                    ptClickn=i;
                    break;
                  }
                }
                printf("%3d,Click point %d,(%d,%d),(%d,%d))\n",__LINE__,
                        ptClickn,x,y,
                        mouse_x - img_rect.x,mouse_y - img_rect.y);
                if(ptClickn==-1) rightclickcount=0; 
              }
              else if(rightclickcount>=2){
                int ptClickn2=-1;
                char buffer[MAX_LINE_LENGTH];
                char ptfname[256];
                for(i=0;i<nPt[nowpicID];i++) {
                  dx = x-pt[nowpicID][i].x;
                  dy = y-pt[nowpicID][i].y;
                  if(sqrt(dx*dx+dy*dy)<=7) {
                    ptClickn2=i;
                    break;
                  }
                }
                printf("%3d,Click point (%d,%d),(%d,%d),(%d,%d))\n",__LINE__,
                        ptClickn,ptClickn2,x,y,
                        mouse_x - img_rect.x,mouse_y - img_rect.y);                
              //delete ptClickn2
                if(ptClickn2>=0 && ptClickn2==ptClickn) {
                  if(ptClickn2<nPt[nowpicID]-1) {                 
                    for(i=ptClickn2;i<nPt[nowpicID]-1;i++) {
                      pt[nowpicID][i].x=pt[nowpicID][i+1].x;
                      pt[nowpicID][i].y=pt[nowpicID][i+1].y;
                    }
                  }
                  nPt[nowpicID]=nPt[nowpicID]-1;

                  sprintf(ptfname,"%s/point%d.txt",fDirectory,picSN[nowpicID]);   
                  ptFile = fopen(ptfname,"wt");
                  for(i=0;i<nPt[nowpicID];i++) {
                    sprintf(buffer,"%d,%d\n", pt[nowpicID][i].x, pt[nowpicID][i].y);
                    fputs(buffer, ptFile);
                  }
                  fclose(ptFile);
                }
                rightclickcount=0;ptClickn=-1;ptClickn2=-1;
              }
            }
          }
        }

        is_dragging = false;
        if(  img_rect.x<=0 && (img_rect.x+img_rect.w)>=0
           &&img_rect.y<=0 && (img_rect.y+img_rect.h)>=0 ) {
          srcRect.x = -img_rect.x;
          srcRect.y = -img_rect.y;
        //srcRect.w = img_rect.w;
        //srcRect.h = img_rect.h;
          zoomFactorX = 18;
          zoomFactorY = 32;
#ifdef DEBUG
          printf("%d,sx=%4d,sy=%4d,ix=%4d,iy=%4d,iw=%4d,ih=%4d\n",
                 __LINE__,srcRect.x, srcRect.y,
                 img_rect.x,img_rect.y,img_rect.w,img_rect.h);
#endif
          if (e.button.button == SDL_BUTTON_LEFT) {
            myButton[0].isPressed = false; // Reset state when released
            myButton[1].isPressed = false; // Reset state when released
            myButton[2].isPressed = false; // Reset state when released
          }
        }
      } else if (e.type == SDL_MOUSEMOTION) {
        if (is_dragging) {
          int mouse_x, mouse_y;
          SDL_GetMouseState(&mouse_x, &mouse_y);
          img_rect.x = mouse_x - offset_x;
          img_rect.y = mouse_y - offset_y;
#ifdef DEBUG
          printf("%d,mx=%4d,my=%4d,ix=%4d,iy=%4d,iw=%4d,ih=%4d\n",
                 __LINE__,mouse_x, mouse_y,
                 img_rect.x,img_rect.y,img_rect.w,img_rect.h);
#endif
        }
      }
    } //SDL_KEYDOWN,SDL_QUIT

    if (myButton[4].isPressed) {
      char imgbuf[256];
      printf("%s(%4d) %s\n",__FILE__,__LINE__,argv[1]);
      myButton[4].isPressed = false;          
      getframe(argv[1], frame_index);
      snprintf(imgbuf, sizeof(imgbuf), "img/x%03d.jpg",frame_index); 
      surface = IMG_Load(imgbuf);
      texture = SDL_CreateTextureFromSurface(gRenderer, surface);          
    }

    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gRenderer);
    // Draw the cropped portion
    if(is_zooming) 
    {
      if(is_extending==1) {
  //    printf("is_extending..(%4d,%4d,%4d,%4d)\n",destMRect.x,destMRect.y,destMRect.w,destMRect.h);
        SDL_RenderCopy(gRenderer, texture, &srcMRect, &destMRect);            
      }
      else {
        SDL_RenderCopy(gRenderer, texture, &srcRect, &destRect);
      }
    }
    else { 
      SDL_RenderCopy(gRenderer, texture, NULL, &img_rect);
    }      
    if (myButton[0].isPressed) {
      SDL_SetRenderDrawColor(gRenderer, 46, 204, 113, 255); // Green click accent
    } else {
      SDL_SetRenderDrawColor(gRenderer, 52, 152, 219, 255); // Blue normal mode
    }
    SDL_RenderFillRect(gRenderer, &myButton[0].rect);
    if (myButton[1].isPressed) {
      SDL_SetRenderDrawColor(gRenderer, 46, 204, 113, 255); // Green click accent
    } else {
      SDL_SetRenderDrawColor(gRenderer, 52, 152, 219, 255); // Blue normal mode
    }
    SDL_RenderFillRect(gRenderer, &myButton[1].rect);
    if (myButton[2].isPressed) {
      SDL_SetRenderDrawColor(gRenderer, 46, 204, 113, 255); // Green click accent
    } else {
      SDL_SetRenderDrawColor(gRenderer, 52, 152, 219, 255); // Blue normal mode
    }
    SDL_RenderFillRect(gRenderer, &myButton[2].rect);
    if (myButton[3].isPressed) {
      SDL_SetRenderDrawColor(gRenderer, 46, 204, 113, 255); // Green click accent
    } else {
      SDL_SetRenderDrawColor(gRenderer, 52, 152, 219, 255); // Blue normal mode
    }
    SDL_RenderFillRect(gRenderer, &myButton[3].rect);

    SDL_SetRenderDrawColor(gRenderer, 52, 152, 219, 255); // Blue normal mode
    SDL_RenderFillRect(gRenderer, &myButton[4].rect);
    SDL_SetRenderDrawColor(gRenderer,255, 0, 0, 255); // Blue normal mode
    SDL_RenderFillRect(gRenderer, &myButton[5].rect);
    if (myButton[5].isPressed) {
      quit = 0;
      myButton[5].isPressed = false;
      printf("call dfvmux3diff_main..\n");

      result = dfvmux3diff_main(argc, argv);
      result = result; 
      printf("Quit dfvmux3diff_main..\n");
    }

    if(ptClick.x>=0 && ptClick.y>=0) {
   // printf("%s(%4d) %s (%4d,%4d)\n",__FILE__,__LINE__,argv[1],ptClick.y,ptClick.y);
      SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 255); 
      DrawCircle(gRenderer, ptClick.x-srcRect.x, ptClick.y-srcRect.y, 10);
    } 

//DrawCircle
    int n=nPt[nowpicID];
    if(n>10) n=10;
    for(int i=0;i<n;i++) {
//    circleColor(gRenderer, pt[nowpicID][i].x, pt[nowpicID][i].y, 50, 0xFF0000FF);
      if(ptClickn==i) {
        SDL_SetRenderDrawColor(gRenderer, 0,   0, 255, 255); 
      }
      else {
        SDL_SetRenderDrawColor(gRenderer, 255, 0,   0, 255); 
      }  
      DrawCircle(gRenderer, pt[nowpicID][i].x-srcRect.x, pt[nowpicID][i].y-srcRect.y, 10);
    }

    if(predictClick.x!=-1 && predictClick.y!=-1) 
    {
      SDL_SetRenderDrawColor(gRenderer, 255,  0,  255, 255);
      DrawCircle(gRenderer, predictClick.x-srcRect.x, predictClick.y-srcRect.y, 10);
    }

	//Rerender text if needed
    finalTick = SDL_GetTicks();
	if( renderText || (finalTick-startTick)>2000)
	{
      std::string inputTexts;
	  //Text is not empty
	  if( inputText != "" ) {
        inputTexts = inputText+cursors[cursorn];
      }
      else {
        inputTexts = " "+cursors[cursorn];
      }  
	  //Render new text
	  gInputTextTexture.loadFromRenderedText( inputTexts.c_str(), textColor );
      gPromptTextTexture.loadFromRenderedText( PromptText.c_str(), textColor );
      cursorn++;
      cursorn=cursorn&0x0001;
      startTick = finalTick;
	}
	//Clear screen
//	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
//	SDL_RenderClear( gRenderer );
    //Render text textures
	gPromptTextTexture.render( 30, //( SCREEN_WIDTH - gPromptTextTexture.getWidth() ) / 2, 
                              SCREEN_HEIGHT-2*gPromptTextTexture.getHeight()-10 );
	gInputTextTexture.render( 30, //( SCREEN_WIDTH - gInputTextTexture.getWidth() ) / 2, 
                              SCREEN_HEIGHT-gPromptTextTexture.getHeight()-5 );


    SDL_RenderCopy(gRenderer, Message, NULL, &Message_rect);
    SDL_RenderPresent(gRenderer);
  }
//LOOPexit:
  //Clean up
  SDL_FreeSurface(surfaceMessage);
  SDL_DestroyTexture(Message);
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(gRenderer);
  SDL_DestroyWindow(gWindow);
  IMG_Quit();
  SDL_Quit();
  TTF_Quit();

  return 0;
}

