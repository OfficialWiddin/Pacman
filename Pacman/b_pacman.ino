#define PIN_X 10
#define PIN_Y 15

#define RED     RGB(255, 0,   0)
#define GREEN     RGB(0, 255,   0)
#define BLUE     RGB(0, 0,   255)

#include "map.h"
  
#include "assets.h"


//Rotating images.
static void rotate_64_64(uint16_t a)
{
  GD.cmd_translate(F16(8),F16(8));
  GD.cmd_rotate(a);
  GD.cmd_translate(F16(-8), F16(-8));
}


class Pacman {
  public:

    //Movement variables.
    bool move_right = true;
    bool move_left = true;
    bool move_up = true;
    bool move_down = true;

    //If power up is active.
    bool power_up = false;

    //Movement speed.
    int movement_speed = 2;
    
    //Movement direction.
    //1 = left, 2 = right, 3 = up, 4 = down
    int move_direction = 0;

    //Pacmans score.
    int score = 0;

    //Start position.
    int posX = 113;
    int posY = 188;

    int current_index = 0;
    int health = 1;

    //Because it renders from the top left corner.
    int Yoffset = 8;
    int Xoffset = 8;
    
    void pac_render(float frame)
    {
      int deg = 0;
      //Rotate image depending on direction.
      if(move_direction == 1)
      {
        deg = 180;
      }
      else if(move_direction == 2)
      {
        deg = 0;
      }
      else if(move_direction == 3)
      {
        deg = 270;
      }
      else if(move_direction == 4)
      {
        deg = 90;
      }

      //Jump between two images depending on the frame, "animation".
      if(frame < 0.33)
      {
        GD.Begin(BITMAPS);
        rotate_64_64(DEGREES(deg));
        GD.cmd_setmatrix();
        GD.Vertex2ii(posX-Xoffset,posY-Yoffset,1,0);
        rotate_64_64(DEGREES(-deg));
        GD.cmd_setmatrix();
      }
      else if (frame > 0.33 && frame < 0.66)
      {
        GD.Begin(BITMAPS);
        rotate_64_64(DEGREES(deg));
        GD.cmd_setmatrix();
        GD.Vertex2ii(posX-Xoffset,posY-Yoffset,1,0+1);
        rotate_64_64(DEGREES(-deg));
        GD.cmd_setmatrix();
        
      }
      else if(frame > 0.66)
      {
        GD.Begin(BITMAPS);
        rotate_64_64(DEGREES(deg));
        GD.cmd_setmatrix();
        GD.Vertex2ii(posX-Xoffset,posY-Yoffset,1,0+2);
        rotate_64_64(DEGREES(-deg));
        GD.cmd_setmatrix();
      }
    }
    
    void pac_move()
    {
      //Calculate the players index.
      int xpos = (posX / 8);
      int ypos = (posY / 8);
      int index = (ypos*28 + xpos);
      current_index = index;

      //Check available directions.
      if(bot_level[index+1] == 0)
      {
        move_right = false;
      }
      if(my_level[index-1] == 0)
      {
        move_left = false;
      }
      if(my_level[index+28] == 0)
      {
        move_down = false;
      }
      if(my_level[index-28] == 0)
      { 
        move_up = false;
      }

      //If we walk on a pellet.
      if(my_level[index] == 2)
      {
        my_level[index] = 1;
        score += 10;
      }
      //If we walk on a powerup.
      else if(my_level[index] == 3)
      {
        power_up = true;
        GD.play(PIANO,60);
        my_level[index] = 1;
      }
      //If we walk on a teleport.
      else if(my_level[index] == 5)
      {
        if(index == 419) //Right to Left side.
        {
          posX = 10;
          posY = 117;
        }
        else if(index == 392) //Left to Right side.
        {
          posX = 215;
          posY = 117;
        }
      }


      //Joystick movement.
      if (analogRead(PIN_X) < 15 && move_left == true)
      {
        move_direction = 1;
      }
      else if (analogRead(PIN_X) > 950 && move_right == true)
      {
        move_direction = 2;
      }
      else if (analogRead(PIN_Y) > 950 && move_up == true)
      {
        move_direction = 3;
      }
      else if (analogRead(PIN_Y) < 15 && move_down == true)
      {
        move_direction = 4;
      }

      if (move_direction == 1 && move_left == true)
      {
        posX -= movement_speed;
      }
      else if (move_direction == 2 && move_right == true)
      {
        posX += movement_speed;
      }
      else if (move_direction == 3 && move_up == true)
      {
        posY -= movement_speed;
      }
      else if (move_direction == 4 && move_down == true)
      {
        posY += movement_speed;
      }

      //Reset
      move_right = true;
      move_left = true;
      move_up = true;
      move_down = true;
      
    }
};

