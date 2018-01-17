#define PIN_X 10
#define PIN_Y 15

#define RED     RGB(255, 0,   0)
#define GREEN     RGB(0, 255,   0)
#define BLUE     RGB(0, 0,   255)

#include "map.h"
#include "assets.h"

//Function for rotating images.
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

    //Check the state of powerup.
    bool power_up = false;

    //Movement speed of Pacman.
    int movement_speed = 2;
    
    //Movement direction.
    //1 = left, 2 = right, 3 = up, 4 = down
    int move_direction = 0;

    //Score.
    int score = 0;

    //Pacman start position.
    int posX = 113;
    int posY = 188;
    
    //Index in the map.
    int current_index = 0;

    //Total healthpoints.
    int health = 1;

    //Offset because it renders from the top left corner.
    int Yoffset = 8;
    int Xoffset = 8;
    
    //Render Pacman.
    void pac_render(float frame)
    {
      //Rotate image depending on direction.
      int deg = 0;
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

      //Jump between two images depending on the frame, To make an "animation".
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
    
    //Pacman movement.
    void pac_move()
    {
      //Calculate the players index in the map.
      int xpos = (posX / 8);
      int ypos = (posY / 8);
      int index = (ypos*28 + xpos);
      current_index = index;

      //Check available walkable directions.
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

      if(my_level[index] == 2) //If we walk on a pellet.
      {
        my_level[index] = 1;
        score += 10;
      }
      else if(my_level[index] == 3) //If we walk on a powerup.
      {
        power_up = true;
        GD.play(PIANO,60);
        my_level[index] = 1;
      }
      else if(my_level[index] == 5) //If we walk on a teleport.
      {
        if(index == 419) //Teleport from right to left side.
        {
          posX = 10;
          posY = 117;
        }
        else if(index == 392) //Teleport from left to right side.
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

      //Reset the movement.
      move_right = true;
      move_left = true;
      move_up = true;
      move_down = true;
      
    }
};

