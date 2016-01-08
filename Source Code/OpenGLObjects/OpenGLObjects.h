/***************************************************************************/

#ifndef OpenGLObjectsH
#define OpenGLObjectsH 

//---------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <mmsystem.h>
#include <math.h>
#include <float.h>

#include "../Useful/Useful.h"
#include "../VectorsMixin/VectorsMixin.h"

#include <gl\gl.h>
#include <gl\glu.h>
#include "../Include/glaux.h"

#include "OpenGLWindows.h"
#include "OpenGLColors.h"
#include "OpenGLUseful.h"
#include "OpenGLTextures.h"

/* 
 * Default values for spheres, cylinders, etc. 
 */
#define DEFAULT_STACKS       32
#define DEFAULT_SLICES       32
#define DEFAULT_RADIUS       10.0
#define DEFAULT_HEIGHT       20.0
#define DEFAULT_WIDTH        10.0
#define DEFAULT_THICKNESS     1.0
#define DEFAULT_LENGTH       25.0


/***************************************************************************/

/*
 * The basic class of OpenGL graphics objects.
 * Contains all the common properties, such as the
 * position, orientation, color, texture, etc.
 */

class OpenGLObject : public VectorsMixin {

private:

protected:

public:

  OpenGLObject( void );

  void __fastcall PrepDraw();
  virtual void __fastcall Draw();
  void __fastcall FinishDraw();

  virtual void __fastcall SetPosition( const Vector3 p );
  virtual void __fastcall SetPosition( double x, double y, double z );
  void __fastcall GetPosition( Vector3 p );

  virtual void __fastcall SetOrientation( Matrix3x3 m );
  virtual void __fastcall SetOrientation( double angle, const Vector3 axis );
  virtual void __fastcall SetOrientation( double roll, double pitch, double yaw );
  void __fastcall GetOrientation( Matrix3x3 m );

  void __fastcall SetAttitude( Matrix3x3 m );
  void __fastcall SetAttitude( double angle, const Vector3 axis );
  void __fastcall SetAttitude( double roll, double pitch, double yaw );
  void __fastcall GetAttitude( Matrix3x3 m );

  virtual void __fastcall SetOffset( const Vector3 p );
  virtual void __fastcall SetOffset( double x, double y, double z );
  void __fastcall GetOffset( Vector3 p );
 
  void __fastcall SetColor( GLfloat c[4] );
  void __fastcall SetColor( float r, float g, float b, float a = 1.0 );
  void __fastcall SetColor( int index );

  void __fastcall SetTexture( Texture *texture );
  void __fastcall SetTexture( Texture *texture, double repeat );
  void __fastcall SetTexture( Texture *texture, double repeat_x, double repeat_y );

  void __fastcall Enable( void );
  void __fastcall Disable( void );
  bool __fastcall IsEnabled( void );


  bool    enabled;
  Vector3  position;
  Matrix3x3  orientation;
  double  gl_position[3];
  double  gl_orientation[16];

  Vector3  offset;
  Matrix3x3  attitude;
  double  gl_offset[3];
  double  gl_attitude[16];

  GLfloat color[4];
  Texture *texture;
  GLfloat umag, vmag;

  GLint   list;

};

/***************************************************************************/

/*
 * A collection of OpenGL graphics objects that are drawn together.
 */

class Assembly : public OpenGLObject {

private:

  OpenGLObject  *component[MAX_COMPONENTS];
  int components;

protected:

public:

  Assembly( void );
  ~Assembly( void );
  void __fastcall AddComponent( OpenGLObject *c );
  void __fastcall RemoveComponent( OpenGLObject *c );
  virtual void __fastcall Draw();

};

/***************************************************************************/

/*
 * Some basic 3D shapes.
 */

//---------------------------------------------------------------------------

class Box : public OpenGLObject {

private:

protected:

public:

  double width;
  double height;
  double length;

  Box( double width = DEFAULT_WIDTH, double height = DEFAULT_WIDTH, double length = DEFAULT_WIDTH );
  void __fastcall Draw();

};

//---------------------------------------------------------------------------

class WindowFrame : public OpenGLObject {

private:

protected:

public:

  double height;
  double width;
  double thickness;

  WindowFrame( double width = DEFAULT_WIDTH, double length = DEFAULT_WIDTH, double thickness = DEFAULT_THICKNESS );
  void __fastcall Draw();

};

//---------------------------------------------------------------------------

class Slab : public OpenGLObject {

private:

protected:

public:

  double length;
  double width;
  double thickness;

  Slab( double width = DEFAULT_WIDTH, double length = DEFAULT_WIDTH, double thickness = DEFAULT_THICKNESS );
  void __fastcall Draw();

};

//---------------------------------------------------------------------------

class Patch : public OpenGLObject {

private:

protected:

public:

  double length;
  double width;

  Patch( double width = DEFAULT_WIDTH, double length = DEFAULT_WIDTH );
  void __fastcall Draw();

};

//---------------------------------------------------------------------------

class Bar : public OpenGLObject {

private:

protected:

public:

  double length;
  double width;
  double thickness;
  bool   capped;

  Bar( double length = DEFAULT_LENGTH, double width = DEFAULT_THICKNESS, double height = DEFAULT_THICKNESS, bool cap = false );
  void __fastcall Draw();

};

//---------------------------------------------------------------------------

class Extrusion : public OpenGLObject {

private:

protected:

public:

  double length;
  double vertex[MAX_VERTICES][2];
  int    vertices;

  Extrusion( double length, double vertex[][2], int n );
  void __fastcall Draw();

};

//---------------------------------------------------------------------------

class Sphere : public OpenGLObject {

private:

protected:

public:

  double radius;
  int    slices;
  int    stacks;
  GLUquadric *quad;

  Sphere( double radius = DEFAULT_RADIUS, int slices = DEFAULT_SLICES, int stacks = DEFAULT_STACKS );
	void SetRadius( double radius );
  void __fastcall Draw();

};

//---------------------------------------------------------------------------

class Ellipsoid : public OpenGLObject {

private:

  double x_distortion;
  double y_distortion;
  double z_distortion;
  double max_radius;

protected:

public:

  double x_radius;
  double y_radius;
  double z_radius;

  int    slices;
  int    stacks;
  GLUquadric *quad;

  Ellipsoid( double x_radius, double y_radius, double z_radius,
    int slices = DEFAULT_SLICES, int stacks = DEFAULT_STACKS );

  Ellipsoid( double xy_radius, double z_radius,
    int slices = DEFAULT_SLICES, int stacks = DEFAULT_STACKS );

  void __fastcall Draw();

};
//---------------------------------------------------------------------------

class Disk : public OpenGLObject {

private:

protected:

public:

  double inner_radius;
  double outer_radius;
  int    slices;
  int    loops;
  GLUquadric *quad;

  Disk( double outer_radius = DEFAULT_RADIUS, double inner_radius = 0.0, 
    int slices = DEFAULT_SLICES, int loops = DEFAULT_STACKS );

  void __fastcall Draw();
  void SetRadius( double outer_radius, double inner_radius = 0.0 );

};

//---------------------------------------------------------------------------

class Cylinder : public OpenGLObject {

private:

protected:

public:

  double top_radius;
  double bottom_radius;
  double height;

  int    slices;
  int    stacks;
  GLUquadric *quad;

  Cylinder( double top_radius = DEFAULT_RADIUS, double bottom_radius = DEFAULT_RADIUS, 
              double height = DEFAULT_HEIGHT, 
              int slices = DEFAULT_SLICES, int stacks = DEFAULT_STACKS );

  void __fastcall Draw();
  void __fastcall SetRadius( double top_radius, double bottom_radius = NaN );
  void __fastcall SetHeight( double height );

};

//---------------------------------------------------------------------------

class Frustrum : public OpenGLObject {

private:

  void __fastcall draw_polygons( void );
  void __fastcall draw_end( void );

protected:

public:

  bool multi_color;

  double near_vertex[3];
  double far_vertex[3];
  
  Frustrum( double near_vertex[3], double far_vertex[3] );

  void __fastcall Draw();

};

//---------------------------------------------------------------------------

class Circle : public OpenGLObject {

private:

protected:

public:

  double radius;
  int    slices;
  int    loops;

  Circle( double radius = DEFAULT_RADIUS, 
    int slices = DEFAULT_SLICES, int loops = DEFAULT_STACKS );

  void __fastcall Draw();

};

//---------------------------------------------------------------------------

// A rectangle with a circular hole in it
class Hole : public OpenGLObject {

private:

protected:

public:

  double  radius;
  double  width;
  double  height;

  Hole( double radius = DEFAULT_RADIUS, double width = 1.0, double height = 1.0 );
  void SetRadius( double value );

  void __fastcall Draw();

};

//---------------------------------------------------------------------------

enum { FLOOR = 0, CEILING, LEFT_WALL, RIGHT_WALL, FRONT_WALL, BACK_WALL };

class Room : public OpenGLObject {

private:

protected:

public:

  double width;
  double height;
  double length;

  Texture wall_texture[6];
  int     wall_color[6];

  void Box( double width = DEFAULT_WIDTH, double height = DEFAULT_WIDTH, double length = DEFAULT_WIDTH );

  void SetColor( int surface, int color );
  void SetColors( int colors[6] );
  void SetTexture( int surface, Texture *texture );
  void SetTextures( int colors[6] );

  void __fastcall Draw();

};



#endif
