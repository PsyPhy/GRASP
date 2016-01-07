//---------------------------------------------------------------------------

#ifndef OpenGLWindowH
#define OpenGLWindowH 

//---------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <mmsystem.h>

#include <gl\gl.h>
#include <gl\glu.h>
#include "../Include/glut.h"

#include "OpenGLColors.h"

#define FONT_DEFAULT "Palatino Linotype" //"Arial Bold"
#define FONT_NCHARS  255
#define FONT_HEIGHT   48
#define FONT_WIDTH    100

class OpenGLWindow

{

private:

  HINSTANCE hInstance;	// Holds The Instance Of The Application
  HDC	    hDC;		    // Private GDI Device Context
  HGLRC     hRC;		    // Permanent Rendering Context

  int       lastInput;

  int       pointerLastX;
  int       pointerLastY;

  int       mouseDeltaX;
  int       mouseDeltaY;

protected:
public:

  HWND      hWnd;	    	// Holds Our Window Handle
  HWND      hParent;
  DWORD		dwExStyle;				// Window Extended Style
  DWORD		dwStyle;				  // Window Style

  int width;
  int height;
    
  int  Colordepth;    // bpp
  bool FullScreen;    // fullscreen flag
  bool Border;
  bool DoubleBuffer;  // double buffer flag
  bool Stereo;        // stereo flag
  unsigned long DisplayFrequency;

  void (*keyboard_callback)( int key );
  bool (*event_callback)( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

  OpenGLWindow( void );
  bool __fastcall Create( HWND parent, char *title, int x, int y, int w, int h );
  void __fastcall SetPosition( int x, int y, int w, int h );
  void __fastcall Destroy( void );
  void __fastcall Activate( void );
  bool __fastcall RunOnce( void );
  void __fastcall Swap( void );
  void __fastcall Clear( int color = WHITE );
  void __fastcall Clear( double r, double g, double b );
  void __fastcall SetKeyboardCallback( void (*callback)( int key ) );
  void __fastcall SetEventCallback( bool (*callback)( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) );

  int __fastcall GetInput( double timeout = -1.0 );
  int __fastcall Sleep( double seconds );

  int __fastcall MouseDeltaX( void );
  int __fastcall MouseDeltaY( void );

	void __fastcall OpenGLWindow::printf(GLuint x, GLuint y, double scale, char* format, ...);

  LRESULT __fastcall WindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

};

//---------------------------------------------------------------------------


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc
OpenGLWindow *CreateMainGlWindow( int argc, char *argv[] );

#endif
