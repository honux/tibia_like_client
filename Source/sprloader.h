#ifndef __SPRLOADER_H__
#define __SPRLOADER_H__

#include <windows.h>
#include <stdio.h>
#include <gl/gl.h>

class Sprite;

class Sprite_Loader
{
     bool spr_loaded;
     short spr_number;
     Sprite *holder;

     bool LoadFromBMP ( char *path, Sprite &sprite, int id );
public:
     // Probably 2 types of Constructor (void) and from file/folder
     Sprite_Loader ( Sprite *_holder );
     Sprite_Loader ( Sprite *_holder, char *file );

     ~Sprite_Loader ( void );

     void LoadFromFile ( char *file );
     void LoadFromFolder ( char *path );
};

class Sprite
{
public:
     Sprite() { data = NULL; width = height = id = 0; };
     unsigned char *data;
     GLuint id;
     short width, height;
};

#endif