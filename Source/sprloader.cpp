#include "sprloader.h"
#include <string.h>


Sprite_Loader::Sprite_Loader ( Sprite *_holder )
{
     spr_loaded = false;
     spr_number = 0;
     holder = _holder;
}

Sprite_Loader::Sprite_Loader ( Sprite *_holder, char *file )
{
     spr_loaded = false;
     spr_number = 0;
     holder = _holder;

     if ( *(file+strlen( file )-1) == '/' )
     {
          LoadFromFolder( file );
     }
     else
     {
          LoadFromFile( file );
     }
}

Sprite_Loader::~Sprite_Loader ( void )
{
     // delete data?
}

void Sprite_Loader::LoadFromFolder ( char *path )
{
     char file[200];
     int i;
     FILE *current;

     for ( i = 1; ; i++ )
     {
          sprintf( file, "%s%d.bmp", path, i );
          
          if ( !LoadFromBMP( file, holder[spr_number], spr_number+1 ) )
          {
               break;
          }
          spr_number++;
     }
     spr_loaded = true;
}

bool Sprite_Loader::LoadFromBMP ( char *path, Sprite &sprite, int id )
{
     FILE *file = fopen(path, "rb");
     if ( !file )
     {
          return false;
     }

     int width, height, x, y;
     unsigned offset, size;
     unsigned char *data, r, g, b;

     /*     
     // Plane, bits count
     unsigned short plane, bits;
     */
     
     fseek( file, 0x0A, SEEK_SET );

     fread( &offset, sizeof( unsigned ), 1, file );
     fread( &size, sizeof( unsigned ), 1, file );
     fread( &width, sizeof( int ), 1, file );
     fread( &height, sizeof( int ), 1, file );
     /*
     fread( &plane, sizeof( unsigned short ), 1, file );
     fread( &bits, sizeof( unsigned short ), 1, file );
     */
     fseek( file, offset, SEEK_SET );

     data = new unsigned char[height*width*4];

     for ( y = 0; y < height; y++ )
     {
          for ( x = 0; x < width; x++ )
          {
               fread( &r, 1, 1, file );
               fread( &g, 1, 1, file );
               fread( &b, 1, 1, file );

               if ( r == 0xFF && g == 0x00 && b == 0xFF ) // MAGENTA!
               {
                    data[(x+y*width)*4+0] = data[(x+y*width)*4+1] = data[(x+y*width)*4+2] = data[(x+y*width)*4+3] = 0x00;
               }
               else
               {
                    data[(x+y*width)*4+0] = r;
                    data[(x+y*width)*4+1] = g;
                    data[(x+y*width)*4+2] = b;
                    data[(x+y*width)*4+3] = 0xFF;
               }
          }
     }

     sprite.data = data;
     sprite.height = height;
     sprite.width = width;
     sprite.id = id;

     fclose ( file );
     return true;
}

void Sprite_Loader::LoadFromFile ( char *file )
{
     // Still need to make my own spr file type, eh?
}
