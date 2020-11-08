#pragma once

struct Vector
{
     int x, y;
     Vector (void) { x = y = 0; };
     Vector (int _x) { x = y = _x; };
     Vector (int _x, int _y) { x = _x; y = _y; };
     Vector (const Vector &data) { x = data.x; y = data.y; };

     void Set( int _x, int _y ) { x = _x; y = _y; };
     void Set( int _x ) { x = y = _x; };

     bool operator==(Vector cmp)
     {
          if ( x == cmp.x && y == cmp.y )
          {
               return true;
          }
          else
          {
               return false;
          }
     }

     Vector operator+(Vector sum)
     {
          return Vector( x+sum.x, y+sum.y );
     }

     Vector operator-(Vector sum)
     {
          return Vector( x-sum.x, y-sum.y );
     }

     Vector operator =(const Vector &temp)
     {
         this->x = temp.x;
         this->y = temp.y;
         return *this;
     }

     bool operator >=(const Vector &temp)
     {
        return (this->x >= temp.x && this->y >= temp.y);
     }

     bool operator <=(const Vector &temp)
     {
         return (this->x <= temp.x && this->y <= temp.y);
     }

     bool operator >(const Vector &temp)
     {
        return (this->x > temp.x && this->y > temp.y);
     }

     bool operator <(const Vector &temp)
     {
         return (this->x < temp.x && this->y < temp.y);
     }
};

struct Vector2
{
     double start_x, start_y, end_x, end_y;
     Vector2() { start_x = start_y = end_x = end_y = 0; };
     Vector2( double _x, double _y ) { start_x = _x; start_y = _y; end_x = end_y = 0; };
     Vector2( double s_x, double s_y, double e_x, double e_y )
     { 
          start_x = s_x; 
          start_y = s_y; 
          end_x = e_x; 
          end_y = e_y; 
     };

     void SetStart( double _x, double _y ) { start_x = _x; start_y = _y; };
     void SetEnd( double _x, double _y ) { end_x = _x; end_y = _y; };
     void Set( double s_x, double s_y, double e_x, double e_y ) { start_x = s_x; start_y = s_y; end_x = e_x; end_y = e_y; };

     bool operator==(Vector2 cmp)
     {
          if ( start_x == cmp.start_x && start_y == cmp.start_y && end_x == cmp.end_x && end_y == cmp.end_y )
          {
               return true;
          }
          else
          {
               return false;
          }
     }
};