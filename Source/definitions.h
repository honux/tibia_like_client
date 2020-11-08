#ifndef __DEFINITIONS_H__
#define __DEFINITIONS_H__

     #define SPRITES_NUMBER 5
     #define FONT_TEXTURE_ID 1          /* SPRITE - 1 */

    enum FLAGS
    {
        /* TextBox */
        FLAG_ISPASSWORD     = 2,

        /* Container */
        FLAG_HASCLOSE       = 2,
        FLAG_HASMINIMIZE    = 4,
        FLAG_ISMINIMIZED    = 8,
        FLAG_HASPARENT      = 16

    };

#endif