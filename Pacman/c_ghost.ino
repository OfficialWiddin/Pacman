#define RED     RGB(255, 0,   0)
#define GREEN     RGB(0, 255,   0)
#define BLUE     RGB(0, 0,   255)

#include "assets.h"

class Ghost {
  public:

    //Movement variables.
    bool move_right = true;
    bool move_left = false;
    bool move_up = false;
    bool move_down = false;

    //Movement speed.
    int movement_speed = 2;

    //Movement direction.
    //1 = left, 2 = right, 3 = up, 4 = down
    int move_direction = 0;

    //If pacman has powerup.
    bool can_die = false;

    //Spawn position.
    int posX = 117;
    int posY = 116;

    int previous_index_change = 0;
    int current_index = 0;

    //Save image index.
    int original_index = 0;
    int img_index = 0;
    int changed_index = 0;

    int timer = 0;
    
    void ghost_img_index(int input)
    {
      img_index = input;
      changed_index = input;
      original_index = input;
    }

    void ghost_render(float frame)
    {
      if(frame < 0.5)
      {
        int Yoffset = 8;
        int Xoffset = 8;
        GD.Begin(BITMAPS);
        GD.Vertex2ii(posX-Xoffset,posY-Yoffset,1,changed_index); 
      }
      else
      {
        int Yoffset = 8;
        int Xoffset = 8;
        GD.Begin(BITMAPS);
        GD.Vertex2ii(posX-Xoffset,posY-Yoffset,1,changed_index+1); 
      }
    }


    void ghost_timer()
    {
      if(can_die == true)
      {
        timer += 1;
        if(timer > 150)
        {
          can_die = false;
          timer = 0;
          img_index = original_index;
          changed_index = original_index;
        }
      }
    }
    
    void ghost_move()
    {
      //Calculate the index.
      int xpos = (posX / 8);
      int ypos = (posY / 8);
      int index = (ypos*28+xpos);
      current_index = index;

      //If we walk on the teleport.
      if(my_level[index] == 5) //Teleport
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

      if(bot_level[index] == 6)
      {
        if(previous_index_change != index)
        {
            
          move_right = false;
          move_left = false;
          move_up = false;
          move_down = false;

          int paths = 0;
          int directions[4];
          
          if(bot_level[index-28] != 0)
          {
            //move_up = true;
            directions[paths] = 1;
            paths += 1;  
          }
          
          if(bot_level[index-1] != 0)
          {
            //move_left = true;
            directions[paths] = 2;
            paths += 1;
          }
          
          if(bot_level[index+1] != 0)
          {
            //move_right = true;
            directions[paths] = 3;
            paths += 1;
          }
          
          if(bot_level[index+28] !=0)
          {
            //move_down = true;
            directions[paths] = 4;
            paths += 1;
          }

          
          changed_index = 0;
          int RNG = random(0,paths);


            if(directions[RNG] == 1)
            {
             move_up = true; 
            }
            else if(directions[RNG] == 2)
            {
              move_left = true;
            }
            else if(directions[RNG] == 3)
            {
              move_right = true;
            }
            else if(directions[RNG] == 4)
            {
              move_down = true;
            }



          
        }   
          previous_index_change = index;
       }


      if(move_left == true)
      {
        posX -= movement_speed;
        if(img_index == 22)
        {
          changed_index = img_index + 1;
        }
        else
        {
          changed_index = img_index + 2;
        }
        
      }
      else if(move_right == true)
      {
        posX += movement_speed; 
        if(img_index == 22)
        {
          changed_index = img_index + 1; 
        }
        else
        {
          changed_index = img_index; 
        }
          
     
      }
      else if(move_up == true)
      {
        posY -= movement_speed;
        if(img_index == 22)
        {
          changed_index = img_index + 1;
        }
        else
        {
          changed_index = img_index + 4;
        }
   
      }
      else if(move_down == true)
      {
        posY += movement_speed;
        if(img_index == 22)
        {
          changed_index = img_index + 1;
        }
        else
        {
          changed_index = img_index + 6;
        }
      
      }
    };
};

