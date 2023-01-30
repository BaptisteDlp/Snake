/*!
 * \file option.cpp
 * \brief Definition of all the setting functions
 * \author Baptiste D.
 * \version 1.0
 */


#include<iostream>
#include<fstream>
#include<ctime>
#include<string>
#include<stdio.h>
#include<stdlib.h>
#include<SDL/SDL.h>
#include<SDL/SDL_image.h>
#include"const.hpp"
#include"Map.hpp"
#include"Snake.hpp"
#include"Text.hpp"
#include"Input.hpp"



using namespace std;





/*!
 *  \brief game
 *
 *  This function corresponds to the main routine to play the game
 *
 *  \param screen : SDL SDL_Surface where the game takes place
 *  \param map : map where the snake is living on
 *  \param posX : initial x-coordinate of the snake (head)
 *  \param posY : initial y-coordinate of the snake (head)
 *  \param wpName : name of the wallpaper used for the game
 *  \param speed :  speed of the snake
 *  \param playerName : pseudo of the user
 */
void game(SDL_Surface *screen, Map &map, int posX, int posY, char *const wpName, int speed, char *const playerName){

    bool keepOnGame(true);
    SDL_Event event;
    int previousTime(0), time(0);


    Snake snake(wpName,playerName,posX,posY,speed);


    TTF_Font *police1 = TTF_OpenFont("data/fonts/GOTHIC.TTF",22);

    Text score("Score: 0",police1,255,255,255,true,0,map.getbeginY()+ map.getgridH()*map.getheigthTile());
    Text gameOver("Game Over",police1,255,255,255,true,screen->w/2,map.getbeginY()+ map.getgridH()*map.getheigthTile());
    Text pause("PAUSE",police1,255,255,255,true,screen->w/2,map.getbeginY()+ map.getgridH()*map.getheigthTile());
    Text hS("New HighScore",police1, 255,255,255,true,screen->w/2,screen->h/2);


    char message[20];

    while(keepOnGame && snake.getalive()){


        time = SDL_GetTicks();
        if(time - previousTime > snake.getspeed())
            snake.event_Analysis(&keepOnGame,screen, pause);

        if(time - previousTime > snake.getspeed()){
            if(!(snake.detection(map))){
                snake.set_Objectif(map);
                snake.clear_Snake(map);
                snake.eat_Objectif();
                if(snake.getobj()){
                    snake.move_Snake(map);
                }else{
                    sprintf(message,"Score: %d",snake.getscore());
                    score.update_Message(message);
                }

                snake.blit_Snake(map);


                SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
                score.blit_Text(screen,false);
                snake.blit_wallPaper(screen,map);
                map.print_Map(screen);

            }
            previousTime = time;
        }

        SDL_Flip(screen);

    }

    snake.event_gameOver("data/highScores/highScores.txt",screen,map,gameOver,hS,keepOnGame);



    TTF_CloseFont(police1);





}





/*!
 *  \brief erase_highScores
 *
 *  Function used to reset the highScores file when the map is re-edited
 *
 */
void erase_highScores(){

  ofstream os("data/highScores/highScores.txt");

  if(!os){
    cout<<"ERROR OPENNING data/highScores/highScores.txt"<<endl; exit(1);
  }

  for(int i = 0; i < 3; i++)
    os<<"Admin 0"<<endl;

  os.close();

}






/*!
 *  \brief edit_map
 *
 *  This fucntion aims at editing a new map for the game
 *
 *  \param screen : SDL SDL_Surface where the game takes place
 *  \param map : map where the snake is living on
 *  \param posX : initial x-coordinate of the snake (head)
 *  \param posY : initial y-coordinate of the snake (head)
 *  \param fileWp: name of the wallpaper used for the game
 */
void edit_map(SDL_Surface *screen, Map &map, int *posX, int *posY, char * const fileWp){


    SDL_Event event;
    bool stopSnake(false), stop(false);
    int actualObject(17);

    SDL_Surface *wp;
    wp = SDL_LoadBMP(fileWp);
    SDL_Rect posWp;
    posWp.x = 0; posWp.y = 0;

    TTF_Font *police = TTF_OpenFont("data/fonts/GOTHIC.TTF",22);
    Text message("Wall",police,255,0,0,true,0,map.getbeginY()+ map.getgridH()*map.getheigthTile());
    Text message2("Snake",police,255,255,255,true,screen->w/2+20,map.getbeginY()+ map.getgridH()*map.getheigthTile());


    for(int i = 0; i<map.getgridW(); i++){
        for(int j = 0; j<map.getgridH(); j++){
            map.setgrid(i,j,0);
        }
    }



    Input in;

    while(!(stop) && !(in.getQuit())){
        in.update_event();

        if(in.getKey(SDLK_ESCAPE) && stopSnake){
            stop = true;
        }

        if(in.getKey(SDLK_LEFT)){
            actualObject = 17;
            message.setColor(255,0,0);
            message2.setColor(255,255,255);
            in.setKeyUp(SDLK_LEFT);
        }

        if(in.getKey(SDLK_RIGHT)){
            actualObject = 6;
            message2.setColor(255,0,0);
            message.setColor(255,255,255);
            in.setKeyUp(SDLK_RIGHT);
        }

       if(in.getMouseButton(SDL_BUTTON_LEFT) && in.getMouseY()<map.getheigthTile()*map.getgridH()){
            if(actualObject == 17){
                map.setgrid(in.getMouseX()/map.getwidthTile(),in.getMouseY()/map.getheigthTile(),17);
            }
            if(actualObject == 6 && !(stopSnake)){
                map.setgrid(in.getMouseX()/map.getwidthTile(),in.getMouseY()/map.getheigthTile(),6);
                map.setgrid(in.getMouseX()/map.getwidthTile()+1,in.getMouseY()/map.getheigthTile(),3);
                map.setgrid(in.getMouseX()/map.getwidthTile()+2,in.getMouseY()/map.getheigthTile(),14);
                stopSnake = true;
                *posX = in.getMouseX()/map.getwidthTile();
                *posY = in.getMouseY()/map.getheigthTile();
            }

       }

       if(in.getMouseButton(SDL_BUTTON_RIGHT) && in.getMouseY()<map.getheigthTile()*map.getgridH()){
            if(map.getgrid(in.getMouseX()/map.getwidthTile(),in.getMouseY()/map.getheigthTile()) == 17){
                map.setgrid(in.getMouseX()/map.getwidthTile(),in.getMouseY()/map.getheigthTile(),0);
            }
            if(stopSnake){
                if(map.getgrid(in.getMouseX()/map.getwidthTile(),in.getMouseY()/map.getheigthTile()) == 6){
                    map.setgrid(in.getMouseX()/map.getwidthTile(),in.getMouseY()/map.getheigthTile(),0);
                    map.setgrid(in.getMouseX()/map.getwidthTile()+1,in.getMouseY()/map.getheigthTile(),0);
                    map.setgrid(in.getMouseX()/map.getwidthTile()+2,in.getMouseY()/map.getheigthTile(),0);
                }

                if(map.getgrid(in.getMouseX()/map.getwidthTile(),in.getMouseY()/map.getheigthTile()) == 3){
                    map.setgrid(in.getMouseX()/map.getwidthTile(),in.getMouseY()/map.getheigthTile(),0);
                    map.setgrid(in.getMouseX()/map.getwidthTile()-1,in.getMouseY()/map.getheigthTile(),0);
                    map.setgrid(in.getMouseX()/map.getwidthTile()+1,in.getMouseY()/map.getheigthTile(),0);
                }

                if(map.getgrid(in.getMouseX()/map.getwidthTile(),in.getMouseY()/map.getheigthTile()) == 14){
                    map.setgrid(in.getMouseX()/map.getwidthTile(),in.getMouseY()/map.getheigthTile(),0);
                    map.setgrid(in.getMouseX()/map.getwidthTile()-1,in.getMouseY()/map.getheigthTile(),0);
                    map.setgrid(in.getMouseX()/map.getwidthTile()-2,in.getMouseY()/map.getheigthTile(),0);
                }

                stopSnake = false;
            }
        }


        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
        SDL_BlitSurface(wp, NULL, screen ,&posWp);
        map.print_Map(screen);
        message.blit_Text(screen,false);
        message2.blit_Text(screen,false);
        SDL_Flip(screen);



    }


    TTF_CloseFont(police);
    SDL_FreeSurface(wp);

    erase_highScores();//do not forget to reset highScores


}





/*!
 *  \brief highScores
 *
 *  This fucntion displays the highScores  menu
 *
 *  \param screen : SDL SDL_Surface where the game takes place
 */
void highScores(SDL_Surface *screen){

    bool keep(true);
    SDL_Event event;
    TTF_Font *police = TTF_OpenFont("data/fonts/GOTHIC.TTF",22);

    ifstream stream("data/highScores/highScores.txt");

    if(!stream){
      cout<<"ERROR OPENNING data/highScores/highScores.txt"<<endl; exit(1);
    }

    string line;

    getline(stream,line);
    char * message1 = new char [line.length()+1];
    strcpy (message1, line.c_str());


    getline(stream,line);
    char * message2 = new char [line.length()+1];
    strcpy (message2, line.c_str());


    getline(stream,line);
    char * message3 = new char [line.length()+1];
    strcpy (message3, line.c_str());


    stream.close();

    Text tHs("HighScores",police,0,255,0,false,screen->w/2,screen->h/6);
    Text tHs1(message1,police,255,255,255,false,screen->w/2,screen->h/3);
    Text tHs2(message2,police,255,255,255,false,screen->w/2,screen->h/2);
    Text tHs3(message3,police,255,255,255,false,screen->w/2,2*screen->h/3);


    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));



    Input in;
    tHs.blit_Text(screen,true);
    tHs1.blit_Text(screen,true);
    tHs2.blit_Text(screen,true);
    tHs3.blit_Text(screen,true);
    SDL_Flip(screen);

    while(!(in.getKey(SDLK_ESCAPE)) && !(in.getQuit())){
        in.update_event();
    }

    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));


    TTF_CloseFont(police);
    delete [] message1;
    delete [] message2;
    delete [] message3;


}





/*!
 *  \brief player
 *
 *  This fucncion displays the playerName menu
 *
 *  \param screen : SDL surface where the game takes place
 *  \param playerName: pseudo of the user
 */
void player(SDL_Surface *screen, char **playerName){


    int player(1);


    TTF_Font *police = TTF_OpenFont("data/fonts/GOTHIC.TTF",22);
    Text message("Settings",police,0,255,0,false,screen->w/2,screen->h/6);
    Text message2("Choose player",police,255,255,255,false,screen->w/2,screen->h/3);
    Text tadmin("Admin",police,255,0,0,false,screen->w/2,screen->h/2);
    Text tguest("Guest",police,255,255,255,false,screen->w/2,2*screen->h/3);




    Input in;

    while(!(in.getKey(SDLK_ESCAPE)) && !(in.getQuit())){
        in.update_event();


        if(in.getKey(SDLK_UP)){
            tadmin.setColor(255,0,0);
            tguest.setColor(255,255,255);
            player = 1;
            in.setKeyUp(SDLK_UP);
        }

        if(in.getKey(SDLK_DOWN)){
            tguest.setColor(255,0,0);
            tadmin.setColor(255,255,255);
            player = 2;
            in.setKeyUp(SDLK_DOWN);
        }



        if(in.getKey(SDLK_RETURN)){
            switch(player){
                case 1:
                    *playerName = "Admin";
                    break;
                case 2:
                    *playerName = "Guest";
                    break;
            }
            in.setKeyUp(SDLK_RETURN);
            in.setQuit();
        }


        SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format, 0, 0, 0));
        tadmin.blit_Text(screen,true);
        tguest.blit_Text(screen,true);
        message.blit_Text(screen,true);
        message2.blit_Text(screen,true);
        SDL_Flip(screen);


    }



    TTF_CloseFont(police);




}










/*!
 *  \brief skin
 *
 *  This fucncion displays the sin setting menu
 *
 *  \param screen : SDL surface where the game takes place
 *  \param map : map where the snake is living on
 */
void skin(SDL_Surface *screen, Map &map){


    int skin(1);


    TTF_Font *police = TTF_OpenFont("data/fonts/GOTHIC.TTF",22);
    Text message("Settings",police,0,255,0,false,screen->w/2,screen->h/6);
    Text message2("Choose skin",police,255,255,255,false,screen->w/2,screen->h/3);
    Text tskin1("Skin1",police,255,0,0,false,screen->w/2,screen->h/2);
    Text tskin2("Skin2",police,255,255,255,false,screen->w/2,2*screen->h/3);




    Input in;

    while(!(in.getKey(SDLK_ESCAPE)) && !(in.getQuit())){
        in.update_event();


        if(in.getKey(SDLK_UP)){
            tskin1.setColor(255,0,0);
            tskin2.setColor(255,255,255);
            skin = 1;
            in.setKeyUp(SDLK_UP);
        }

        if(in.getKey(SDLK_DOWN)){
            tskin2.setColor(255,0,0);
            tskin1.setColor(255,255,255);
            skin = 2;
            in.setKeyUp(SDLK_DOWN);
        }



        if(in.getKey(SDLK_RETURN)){
            switch(skin){
                case 1:
                    map.setTileMap("data/skins/Snake.bmp",63,72,204);
                    //tskin1.setColor(0,0,255);
                    break;
                case 2:
                    map.setTileMap("data/skins/Snake2.bmp",63,72,204);
                    //tskin2.setColor(0,0,255);
                    break;
            }
            in.setKeyUp(SDLK_RETURN);
            in.setQuit();
        }


        SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format, 0, 0, 0));
        tskin1.blit_Text(screen,true);
        tskin2.blit_Text(screen,true);
        message.blit_Text(screen,true);
        message2.blit_Text(screen,true);
        SDL_Flip(screen);


    }



    TTF_CloseFont(police);

}








/*!
 *  \brief speed
 *
 *  This fucncion displays the speed setting menu
 *
 *  \param screen : SDL surface where the game takes place
 *  \param speed : speed of the snake
 */
void speed(SDL_Surface *screen, int *speed){


    TTF_Font *police = TTF_OpenFont("data/fonts/GOTHIC.TTF",22);
    Text tSettings("Settings",police,0,255,0,false,screen->w/2,screen->h/6);
    Text message("Choose speed",police,255,255,255,false,screen->w/2,screen->h/3);
    Text message2("High",police,255,255,255,false,screen->w/2,screen->h/2);
    Text message3("Medium",police,255,255,255,false,screen->w/2,2*screen->h/3);
    Text message4("Low",police,255,255,255,false,screen->w/2,5*screen->h/6);


    int pos(2);
    Input in;

    while(!(in.getKey(SDLK_ESCAPE)) && !(in.getQuit())){
        in.update_event();

        if(in.getKey(SDLK_UP)){
            if(pos>1)
                pos--;
            in.setKeyUp(SDLK_UP);
        }

        if(in.getKey(SDLK_DOWN)){
            if(pos<3)
                pos++;
            in.setKeyUp(SDLK_DOWN);
        }


        if(in.getKey(SDLK_RETURN)){
            in.setKeyUp(SDLK_RETURN);
            switch(pos){
                case 1:
                   *speed = LOW_SPEED;
                    break;

                case 2:
                    *speed = MEDIUM_SPEED;
                    break;

                case 3:
                    *speed = HIGH_SPEED;
                    break;
            }
            in.setQuit();
        }



        SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format, 0, 0, 0));
        switch(pos){
            case 1:
                message2.setColor(255,0,0);
                message3.setColor(255,255,255);
                message4.setColor(255,255,255);
                break;

            case 2:
                message3.setColor(255,0,0);
                message2.setColor(255,255,255);
                message4.setColor(255,255,255);
                break;

            case 3:
                message4.setColor(255,0,0);
                message2.setColor(255,255,255);
                message3.setColor(255,255,255);
                break;
        }

        tSettings.blit_Text(screen,true);
        message.blit_Text(screen,true);
        message2.blit_Text(screen,true);
        message3.blit_Text(screen,true);
        message4.blit_Text(screen,true);
        SDL_Flip(screen);


    }

    TTF_CloseFont(police);

}











/*!
 *  \brief wallPaper
 *
 *  This fucncion displays the wallPaper setting menu
 *
 *  \param screen : SDL surface where the game takes place
 *  \param wpName : name of the WallPaper
 */
void wallPaper(SDL_Surface *screen, char **wpName){


    int wall(1);


    TTF_Font *police = TTF_OpenFont("data/fonts/GOTHIC.TTF",22);
    Text message("Settings",police,0,255,0,false,screen->w/2,screen->h/6);
    Text message2("Choose wallPaper",police,255,255,255,false,screen->w/2,screen->h/3);
    Text tgrass("Grass",police,255,0,0,false,screen->w/2,screen->h/2);
    Text tsand("Sand",police,255,255,255,false,screen->w/2,2*screen->h/3);




    Input in;

    while(!(in.getKey(SDLK_ESCAPE)) && !(in.getQuit())){
        in.update_event();


        if(in.getKey(SDLK_UP)){
            tgrass.setColor(255,0,0);
            tsand.setColor(255,255,255);
            wall = 1;
            in.setKeyUp(SDLK_UP);
        }

        if(in.getKey(SDLK_DOWN)){
            tsand.setColor(255,0,0);
            tgrass.setColor(255,255,255);
            wall = 2;
            in.setKeyUp(SDLK_DOWN);
        }



        if(in.getKey(SDLK_RETURN)){
            switch(wall){
                case 1:
                    *wpName = "data/wallPaper/grass1515.bmp";
                    //tgrass.setColor(0,0,255);
                    break;
                case 2:
                    *wpName = "data/wallPaper/sand1515.bmp";
                     //tsand.setColor(0,0,255);
                    break;
            }
            in.setKeyUp(SDLK_RETURN);
            in.setQuit();
        }


        SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format, 0, 0, 0));
        tgrass.blit_Text(screen,true);
        tsand.blit_Text(screen,true);
        message.blit_Text(screen,true);
        message2.blit_Text(screen,true);
        SDL_Flip(screen);


    }



    TTF_CloseFont(police);
}








/*!
 *  \brief setting
 *
 *  This fucncion displays the main setting menu
 *
 *  \param screen : SDL surface where the game takes place
 *  \param map :  map where the snake is living on
 *  \param wpName : name of the wallPaper
 *  \param spd : speed of the snake
 *  \param playerName: pseudo of the user
 */
void settings(SDL_Surface *screen, Map &map, char ** wpName, int *spd, char **playerName){


    TTF_Font *police = TTF_OpenFont("data/fonts/GOTHIC.TTF",22);
    Text tSettings("Settings",police,0,255,0,false,screen->w/2,screen->h/6);
    Text message("Player",police,255,255,255,false,screen->w/2,screen->h/3);
    Text message2("Skin",police,255,255,255,false,screen->w/2,screen->h/2);
    Text message3("WallPaper",police,255,255,255,false,screen->w/2,2*screen->h/3);
    Text message4("Speed",police,255,255,255,false,screen->w/2,5*screen->h/6);


    int pos(1);
    Input in;

    while(!(in.getKey(SDLK_ESCAPE)) && !(in.getQuit())){
        in.update_event();

        if(in.getKey(SDLK_UP)){
            if(pos>1)
                pos--;
            in.setKeyUp(SDLK_UP);
        }

        if(in.getKey(SDLK_DOWN)){
            if(pos<4)
                pos++;
            in.setKeyUp(SDLK_DOWN);
        }


        if(in.getKey(SDLK_RETURN)){
            in.setKeyUp(SDLK_RETURN);
            switch(pos){
                case 1:
                    player(screen,&(*playerName));
                    break;

                case 2:
                    skin(screen,map);
                    break;

                case 3:
                    wallPaper(screen,&(*wpName));
                    break;

                case 4:
                    speed(screen,&(*spd));
                    break;
            }
        }



        SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format, 0, 0, 0));
        switch(pos){
            case 1:
                message.setColor(255,0,0);
                message2.setColor(255,255,255);
                message3.setColor(255,255,255);
                message4.setColor(255,255,255);
                break;

            case 2:
                message2.setColor(255,0,0);
                message.setColor(255,255,255);
                message3.setColor(255,255,255);
                message4.setColor(255,255,255);
                break;

            case 3:
                message3.setColor(255,0,0);
                message.setColor(255,255,255);
                message2.setColor(255,255,255);
                message4.setColor(255,255,255);
                break;

            case 4:
                message4.setColor(255,0,0);
                message.setColor(255,255,255);
                message2.setColor(255,255,255);
                message3.setColor(255,255,255);
                break;
        }

        tSettings.blit_Text(screen,true);
        message.blit_Text(screen,true);
        message2.blit_Text(screen,true);
        message3.blit_Text(screen,true);
        message4.blit_Text(screen,true);
        SDL_Flip(screen);


    }



    TTF_CloseFont(police);


}
