#include <EEPROM.h>
#include <SPI.h>
#include <GD2.h>

#include "assets.h"
#include "map.h"

//Defined colors.
#define RED     RGB(255, 0,   0)
#define GREEN     RGB(0, 255,   0)
#define YELLOW    RGB(255,255,0)
#define BLUE     RGB(0, 0,   255)
#define WHITE  RGB(255,255,255)

//Frame.
float frame = 0;

//Create pacman.
Pacman pacman;

//Create container for ghosts.
Ghost container_ghost[4];

//Timer for powerup.
int powerup_timer = 0;
bool once = false;
  
void setup() 
{
  //Runs once.
  GD.begin();
  LOAD_ASSETS();
  Serial.begin(9600);

  int first_img = 14;
  int second_img = 26;
  int third_img = 38;
  int fourth_img = 46;

  //Create four ghosts.
  for(int i = 0; i < 4; i++)
  {
    Ghost ghost;
    container_ghost[i] = ghost;
  }

  //Give them all different images.
  container_ghost[0].ghost_img_index(14);
  container_ghost[1].ghost_img_index(26);
  container_ghost[2].ghost_img_index(38);
  container_ghost[3].ghost_img_index(46);
}

void loop() 
{
  GD.Clear();
  GD.Begin(BITMAPS);
  
  //Load map.
  GD.Vertex2ii(0,0,0);

  //Display current score.
  char buf[50];
  sprintf(buf, "Score: %d", pacman.score);
  GD.cmd_text(350, 80, 24, OPT_CENTER, buf);

  //If pacman collected all pellets == WIN
  if(pacman.score >= 2400)
  {
    GD.play(COWBELL,60);
    pacman.health = 999;
    GD.cmd_text(340, 136, 31, OPT_CENTER, "YOU WON.");
  }

  //If pacman ate a power up.
  if(pacman.power_up == true)
  {
    powerup_timer += 1;
    
    if(powerup_timer > 150)
    {
      
      pacman.power_up = false;
      once = false;
      powerup_timer = 0;
      
      for(int i = 0; i < 4; i++)
      {
        container_ghost[i].can_die = false;
        container_ghost[i].timer = 0;
        container_ghost[i].img_index = container_ghost[i].original_index;
      }
      
    }
    else if(once == false)
    {
      for(int i = 0; i < 4; i++)
      {
        container_ghost[i].img_index = 22;
        container_ghost[i].can_die = true;
        container_ghost[i].timer = 0;
      }
      once = true; 
    }
  }

      

  //If ghost collides with pacman.
  for(int i = 0; i < 4; i++)
  {

    container_ghost[i].ghost_move();
    container_ghost[i].ghost_render(frame);
    container_ghost[i].ghost_timer();
    
    if(container_ghost[i].current_index == pacman.current_index and pacman.power_up == false)
    {
      pacman.health -= 1;
      break;
    }
    else if(container_ghost[i].current_index == pacman.current_index and pacman.power_up == true and container_ghost[i].can_die == true)
    {
      container_ghost[i].posX = 117;
      container_ghost[i].posY = 116;
      container_ghost[i].can_die = false;
      container_ghost[i].img_index = container_ghost[i].original_index;
      container_ghost[i].timer = 0;
      break;
    }
    else if(container_ghost[i].current_index == pacman.current_index and pacman.power_up == true and container_ghost[i].can_die == false)
    {
      pacman.health -= 1;
      break;
    }
  }
  


  //Render all the pellets and powerups.
  GD.Begin(RECTS);
  for(int a = 0; a < 31; a++)
  {
    for(int i = 0; i < 28; i++)
    {
      if (my_level[a*28+i] == 0)
      {
       //GD.ColorRGB(RED);
       //GD.Vertex2ii(i*8.2,a*8.2);  
       //GD.Vertex2ii(i*8.2,a*8.2);
      }
      else if(my_level[a*28+i] == 2) //Points.
      {
        GD.ColorRGB(YELLOW);
        GD.Vertex2ii(i*8.2,a*8.2);  
        GD.Vertex2ii(i*8.2,a*8.2);
      }
      else if(my_level[a*28+i] == 3) //Powerup.
      {
        GD.ColorRGB(GREEN);
        GD.Vertex2ii(i*8.2,a*8.2);  
        GD.Vertex2ii(i*8.2+3,a*8.2+3);
      }
      else if(my_level[a*28+i] == 5) //Teleport.
      {
        //GD.ColorRGB(RED);
        //GD.Vertex2ii(i*8.2,a*8.2);  
        //GD.Vertex2ii(i*8.2,a*8.2);
      }
      
      
    }
  }
  
  GD.ColorRGB(WHITE);
  
  //If pacman is alive, else dead.
  if(pacman.health > 0)
  {
      pacman.pac_render(frame);
      pacman.pac_move();

  }
  else
  {
    GD.cmd_text(340, 136, 31, OPT_CENTER, "YOU DIED.");
  }

  //Increase frame.
  frame += 0.05;
  if (frame > 1)
  {
    frame = 0;
  }
  
  delay(16);
  GD.swap();
}
