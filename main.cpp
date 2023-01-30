/**
 * \file main.cpp
 * \brief Runs Snake Game.
 * \author Baptiste D.
 * \version 1.0
 * \date June 24th 2019
 *
 * Program that runs the Game
 *
 */




#include<iostream>
#include<fstream>
#include<ctime>
#include<string>
#include<stdio.h>
#include<stdlib.h>
#include<SDL/SDL.h>
#include<SDL/SDL_image.h>
#include"Map.hpp"
#include"Snake.hpp"
#include"Text.hpp"
#include"Input.hpp"
#include"options.hpp"




using namespace std;





int main(int argc, char *argv[]){

    SDL_Surface *screen = NULL;
    int pos(1);
    int posX(0),posY(0);

    TTF_Init();


    SDL_Init(SDL_INIT_VIDEO);
    if(SDL_Init(SDL_INIT_VIDEO) == -1){
        fprintf(stderr,"ERROR INITIALISATION SDL: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }


    int beginX(0),beginY(0);
    Map map("data/maps/mapSnake.txt",beginX,beginY); //Map loading
    map.clear_TileSetColorsKey(63,72,204);



    SDL_WM_SetIcon(SDL_LoadBMP("data/skins/objectif.bmp"),NULL);



    screen = SDL_SetVideoMode((map.getgridW() + beginX*2) * map.getwidthTile(), (map.getgridH()+2) * map.getheigthTile(), 32, SDL_HWSURFACE| SDL_DOUBLEBUF );
    if(screen == NULL){
        fprintf(stderr,"CAN NOT LOAD SCREEN: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }




    SDL_WM_SetCaption("Snake", NULL);



    TTF_Font *police = TTF_OpenFont("data/fonts/SNAP____.TTF",30);
    TTF_Font *police2 = TTF_OpenFont("data/fonts/SNAP____.TTF",20);
    TTF_Font *police3 = TTF_OpenFont("data/fonts/GOTHIC.TTF",15);



    Text tSnake("Snake",police,0,255,0,true,screen->w/2,screen->h/3);
    Text tOption("Game",police2,0,255,255,true,screen->w/2,screen->h/2);
    Text tname("author: Baptiste Delaporte",police3,0,0,255,false,screen->w/2,5*screen->h/6);
    Text tversion("V-1.0",police3,0,0,255,false,screen->w/2,5.5*screen->h/6);



    char *wpName = "data/wallPaper/grass1515.bmp"; //Iniial wallPaper
    char *playerName = "Admin"; //Initial user
    int speed = 80; //Initial speed


    Input in;



    while(!(in.getKey(SDLK_ESCAPE)) && !(in.getQuit())){
        in.update_event();

        if(in.getKey(SDLK_UP)){
            pos++;
            if(pos == 5)
                pos = 1;
            in.setKeyUp(SDLK_UP);
        }

        if(in.getKey(SDLK_DOWN)){
            pos--;
            if(pos == 0)
                pos = 4;
             in.setKeyUp(SDLK_DOWN);
        }


        if(in.getKey(SDLK_RETURN)){
            in.setKeyUp(SDLK_RETURN);
            switch(pos){
                case 1:
                    game(screen,map,posX,posY,wpName,speed,playerName);
                    break;

                case 2:
                    edit_map(screen,map,&posX,&posY,wpName);
                    break;

                case 3:
                    highScores(screen);
                    break;

                case 4:
                    settings(screen,map,&wpName,&speed,&playerName);
                    break;
            }
        }


        switch(pos){
            case 1:
                tOption.update_Message("Game");
                break;
            case 2:
                tOption.update_Message("Edit Map");
                break;
            case 3:
                tOption.update_Message("High Scores");
                break;
            case 4:
                tOption.update_Message("Settings");
                break;
        }

        SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format, 0, 0, 0));
        tSnake.blit_Text(screen,true);
        tOption.blit_Text(screen,true);
        tname.blit_Text(screen,true);
        tversion.blit_Text(screen,true);
        SDL_Flip(screen);


    }

    TTF_CloseFont(police);
    TTF_CloseFont(police2);
    TTF_CloseFont(police3);
    TTF_Quit();
    SDL_Quit();

    erase_highScores(); //do not forget to erase_highScores


    return EXIT_SUCCESS;

}
