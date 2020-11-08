#ifndef __ECOLORH__
#define __ECOLORH__

struct ECOLOR
{
     ECOLOR(float _r = 1.0, float _g = 1.0, float _b = 1.0, float _a = 1.0)
     {
          r 	= _r;
          g 	= _g;
          b 	= _b;
          a 	= _a;
     }

     bool operator==(const ECOLOR s)
     {
          return (r == s.r && g == s.g && b == s.b && a == s.a);
     }

     bool operator!=(const ECOLOR s)
     {
          return (r != s.r || g != s.g || b != s.b || a != s.a);
     }

     float r, g, b, a;
};

#endif