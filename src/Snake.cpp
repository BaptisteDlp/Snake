/*!
 * \file Snake.cpp
 * \brief Definition of Snake class
 * \author Baptiste D.
 * \version 1.0
 */


#include<iostream>
#include<fstream>
#include<cstdlib>
#include<SDL/SDL.h>
#include<string>
#include<vector>
#include<ctime>
#include"const.hpp"
#include"Map.hpp"
#include"Snake.hpp"
#include"Input.hpp"


using namespace std;


/*!
 *  \brief Constuctor
 *
 *  Constructor of class Snake
 *
 *  \param fileName : name of the file to load the wallpaper
 *  \param name : name of the user
 *  \param posX : initial x coordinate of the head
 *  \param posY : initial y coordinate of the head
 *  \param speed : speed of the snake
 */
Snake::Snake(char *const fileName, string name, int posX, int posY, int speed){

    Body b;

    wallPaper = SDL_LoadBMP(fileName);

    b.posX = posX;
    b.posY = posY;
    m_snake.push_back(b);
    b.posX = posX+1;
    b.posY = posY;
    m_snake.push_back(b);
    b.posX = posX+2;
    b.posY = posY;
    m_snake.push_back(b);


    m_direction = LEFT;
    m_objectifSet = false;
    m_alive = true;
    m_score = 0;

    m_speed = speed;
    m_timeScore = 0;
    m_setTimeScore = 1500;

    m_player = name;

}




Snake::Snake(Snake const& snake){

    wallPaper = snake.getwall();
    m_alive = snake.getalive();
    m_direction = snake.getdirection();
    m_speed = getspeed();

    m_timeScore = snake.getTimeScore();
    m_score = snake.getscore();
    m_setTimeScore = snake.getsetTimeScore();

    m_objectifSet = snake.getobj();
    m_posObjX  = snake.getobjx();
    m_posObjY = snake.getobjy();


}









Snake::~Snake(){

    SDL_FreeSurface(wallPaper);

}





/*!
 *  \brief event_analysis
 *
 *
 *
 *  \param keepOnGame :
 *  \param screen :
 *  \param pause :
 */
void Snake::event_Analysis(bool *keepOnGame, SDL_Surface *screen, Text & pause){




    Input in;


    in.update_event();

    if(in.getKey(SDLK_ESCAPE)){
        *keepOnGame = false;
    }

    if(in.getKey(SDLK_UP)){
        if(m_direction != UP && m_direction != DOWN){
            m_direction = UP;
        }
        in.setKeyUp(SDLK_UP);
    }

    if(in.getKey(SDLK_DOWN)){
        if(m_direction != DOWN && m_direction != UP){
            m_direction = DOWN;
        }
        in.setKeyUp(SDLK_DOWN);

    }

    if(in.getKey(SDLK_LEFT)){
        if(m_direction != LEFT && m_direction != RIGHT){
            m_direction = LEFT;
        }
        in.setKeyUp(SDLK_LEFT);
    }

    if(in.getKey(SDLK_RIGHT)){
        if(m_direction != RIGHT && m_direction != LEFT){
            m_direction = RIGHT;
        }
        in.setKeyUp(SDLK_RIGHT);

    }
    //=============HOW TO PAUSE THE PROGRAM CORRECTLY==============
    if(in.getKey(SDLK_SPACE)){
        in.setKeyUp(SDLK_SPACE);
        pause.blit_Text(screen,false);
        SDL_Flip(screen);
        while(!(in.getKey((SDLK_SPACE)))){
            in.update_event();
        }
        in.setKeyUp(SDLK_SPACE);
    }
    //============================================================

}






/*!
 *  \brief event_gameOver
 *
 *
 *
 *  \param fileName :
 *  \param screen :
 *  \param map :
 *  \param message :
 *  \parama message2 :
 *  \parama keepOnGame :
 */
void Snake::event_gameOver(char * const fileName, SDL_Surface *screen, Map &map, Text &message, Text &message2, bool keepOnGame){


    bool clic(true);
    SDL_Event event;
    int hs1,hs2,hs3;

    m_objectifSet = false;
    map.setgrid(m_posObjX,m_posObjY,0);

    ifstream stream(fileName);

    string name1,name2,name3;

    if(!stream){
      cout<<"ERROR OPENNING "<<fileName<<endl; exit(1);
    }

    stream>>name1; stream>>hs1;
    stream>>name2; stream>>hs2;
    stream>>name3; stream>>hs3;

    stream.close();


    if(keepOnGame){
        if(m_score>hs1){
            hs3 = hs2; name3 = name2;
            hs2 = hs1; name2 = name1;
            hs1 = m_score; name1 = m_player;

            message2.blit_Text(screen,true);
        }else{
            if(m_score>hs2){
                hs3 = hs2; name3 = name2;
                hs2 = m_score; name2 = m_player;
                message2.blit_Text(screen,true);
            }else{
                if(m_score>hs3){
                    hs3 = m_score; name3 = m_player;
                    message2.blit_Text(screen,true);
                }
            }
        }
        message.blit_Text(screen,false);
        SDL_Flip(screen);

        Input in;


        while(!(in.getKey(SDLK_ESCAPE))){
            in.update_event();
        }

    }

    {
      ofstream stream2(fileName);


	      if(!stream2){
	         cout<<"ERROR OPENNING "<<fileName<<endl; exit(1);
	      }

	       stream2<<name1<<" "<<hs1<<endl;
	       stream2<<name2<<" "<<hs2<<endl;
	       stream2<<name3<<" "<<hs3<<endl;
         stream2.close();
    }



}






/*!
 *  \brief move_Snake
 *
 *  Method used to shift the snake towards its upcoming poisiton
 *
 *  \param map :  map where the snake is living on
 */
void Snake::move_Snake(Map const& map){

    Body previous;
    previous.posX = m_snake[0].posX;
    previous.posY = m_snake[0].posY;


    switch(m_direction){
        case UP:
            m_snake[0].posY --;
            if(m_snake[0].posY == -1)
                m_snake[0].posY = map.getgridH() - 1;
            break;

        case DOWN:
            m_snake[0].posY ++;
            if(m_snake[0].posY == map.getgridH())
                m_snake[0].posY = 0;
            break;

        case LEFT:
            m_snake[0].posX --;
            if(m_snake[0].posX == -1)
                m_snake[0].posX = map.getgridH() -1;
            break;

        case RIGHT:
            m_snake[0].posX ++;
            if(m_snake[0].posX == map.getgridW())
                m_snake[0].posX = 0;
            break;
    }


    for(unsigned int i = 1; i<m_snake.size(); i++){
        Body next;

        next.posX = m_snake[i].posX;
        next.posY = m_snake[i].posY;


        m_snake[i].posX = previous.posX;
        m_snake[i].posY = previous.posY;

        previous.posX = next.posX;
        previous.posY = next.posY;

    }

}






/*!
 *  \brief set_Objectif
 *
 *  Method used to set a new objectif on the map
 *
 *  \param map : map where the snake is living on
 */
void Snake::set_Objectif(Map &map){

    srand(time(0));

    if(!(m_objectifSet)){
        do{
            m_posObjX = rand() % (map.getgridW());
            m_posObjY = rand() % (map.getgridW());
        }while(map.getwall(map.getgrid(m_posObjX,m_posObjY)));
        map.setgrid(m_posObjX,m_posObjY,15);
        m_objectifSet = true;
        m_timeScore = SDL_GetTicks();

    }


}




/*!
 *  \brief eat_Objectif
 *
 *  Method used to update the snake right after eating the objectif
 *
 */
void Snake::eat_Objectif(){

    Body b;
    int time(SDL_GetTicks());

    switch(m_direction){
        case UP:
            if(m_posObjX == m_snake[0].posX && m_posObjY == m_snake[0].posY - 1){
                b.posX = m_posObjX;
                b.posY = m_posObjY;
                m_objectifSet = false;
                vector<Body>::iterator it = m_snake.begin();
                m_snake.insert(it,b);
                if(time - m_timeScore > m_setTimeScore){
                    m_score += LOW_SCORE;
                }else{
                    m_score += MEDIUM_SCORE;
                }
            }
        break;

        case DOWN:
            if(m_posObjX == m_snake[0].posX && m_posObjY == m_snake[0].posY + 1){
                b.posX = m_posObjX;
                b.posY = m_posObjY;
                m_objectifSet = false;
                vector<Body>::iterator it = m_snake.begin();
                m_snake.insert(it,b);
                if(time - m_timeScore > m_setTimeScore){
                    m_score += LOW_SCORE;
                }else{
                    m_score += MEDIUM_SCORE;
                }
            }
        break;

        case LEFT:
            if(m_posObjX == m_snake[0].posX - 1 && m_posObjY == m_snake[0].posY){
                b.posX = m_posObjX;
                b.posY = m_posObjY;
                m_objectifSet = false;
                vector<Body>::iterator it = m_snake.begin();
                m_snake.insert(it,b);
                if(time - m_timeScore > m_setTimeScore){
                    m_score += LOW_SCORE;
                }else{
                    m_score += MEDIUM_SCORE;
                }
            }
        break;

        case RIGHT:
            if(m_posObjX == m_snake[0].posX + 1 && m_posObjY == m_snake[0].posY){
                b.posX = m_posObjX;
                b.posY = m_posObjY;
                m_objectifSet = false;
                vector<Body>::iterator it = m_snake.begin();
                m_snake.insert(it,b);
                if(time - m_timeScore > m_setTimeScore){
                    m_score += LOW_SCORE;
                }else{
                    m_score += MEDIUM_SCORE;
                }
            }
        break;
    }
}








/*!
 *  \brief detection
 *
 *  Method used to know whether the snake will hit a wall during the next time laps
 *  or not
 *
 *  \param map : map where the snake is living on
 */
bool Snake::detection(Map const& map){

    int posX, posY;

    switch(m_direction){
            case UP:
                posX = m_snake[0].posX;
                if(m_snake[0].posY-1 < 0){
                    posY = map.getgridH() - 1;
                }else{
                    posY = m_snake[0].posY - 1;
                }


                if(map.getwall(map.getgrid(posX,posY))){
                    m_alive = false;
                    return true;
                }
                break;

            case DOWN:
                posX = m_snake[0].posX;
                if(m_snake[0].posY+1 == map.getgridH()){
                    posY = 0;
                }else{
                    posY = m_snake[0].posY + 1;
                }


                if(map.getwall(map.getgrid(posX,posY))){
                     m_alive = false;
                     return true;
                }
                break;

            case LEFT:
                posY = m_snake[0].posY;
                if(m_snake[0].posX-1<0){
                    posX = map.getgridW() - 1;
                }else{
                    posX = m_snake[0].posX - 1;
                }


                if(map.getwall(map.getgrid(posX,posY))){
                     m_alive = false;
                     return true;
                }
                break;

            case RIGHT:
                posY = m_snake[0].posY;
                if(m_snake[0].posX+1 == map.getgridW()){
                    posX = 0;
                }else{
                    posX = m_snake[0].posX + 1;
                }


                if(map.getwall(map.getgrid(posX,posY))){
                     m_alive = false;
                     return true;
                }
                break;
    }

    return false;

}




bool Snake::getalive() const{ return m_alive;}
bool Snake::getobj() const { return m_objectifSet;}
int Snake::getspeed() const{ return m_speed;}
int Snake::getscore() const{ return m_score;}
int Snake::getTimeScore() const{ return m_timeScore;}
int Snake::getsetTimeScore() const{ return m_setTimeScore;}
int Snake::getdirection() const{return m_direction;}
int Snake::getobjx() const{ return m_posObjX;}
int Snake::getobjy() const{ return m_posObjY;}
SDL_Surface* Snake::getwall() const{ return wallPaper;}






void Snake::setWallPaper(char *const fileName){

    if(wallPaper != NULL)
        SDL_FreeSurface(wallPaper);
    wallPaper = SDL_LoadBMP(fileName);


}




/*!
 *  \brief clear_Snake
 *
 *  Method used to erase the snake from the map it is living on
 *
 *  \param map : map where the snake is living on
 */
void Snake::clear_Snake(Map &map) const{

    for(int i = 0; i<map.getgridW(); i++){
        for(int j = 0; j<map.getgridH(); j++){
            if(map.getgrid(i,j) != 17 && map.getgrid(i,j) != 0 && map.getgrid(i,j) != 1 && map.getgrid(i,j) != 15){
                map.setgrid(i,j,0);
            }
        }

    }

}






/*!
 *  \brief blit_Snake
 *
 *  Method used to display the snake on the screen
 *
 *  \param map : map where the snake is living on
 */
void Snake::blit_Snake(Map &map) const{

    int xpo,ypo;
    int xp,yp;
    int x,y;
    int xn,yn;
    int xbr, ybr;
    int xbo,ybo;



    switch(m_direction){
        case UP:
            map.setgrid(m_snake[0].posX,m_snake[0].posY,2);
            break;

        case DOWN:
            map.setgrid(m_snake[0].posX,m_snake[0].posY,4);
            break;

        case LEFT:
            map.setgrid(m_snake[0].posX,m_snake[0].posY,6);
            break;

        case RIGHT:
            map.setgrid(m_snake[0].posX,m_snake[0].posY,8);
            break;



    }

    for(unsigned int i = 1; i<m_snake.size()-1; i++){

        xbr = map.getgridW()-1;
        ybr = map.getgridW()-1;

        xbo = 0;
        ybo = 0;

        xp = m_snake[i-1].posX;
        yp = m_snake[i-1].posY;

        x = m_snake[i].posX;
        y = m_snake[i].posY;

        xn = m_snake[i+1].posX;
        yn = m_snake[i+1].posY;


        if(((xp  == x-1 || xp  == x+1) && yp == y && (xn  == x-1 || xn == x+1) && yn == y) ||
           ((xp == xbr || xp == 1) && yp == y && (xn == 1 || xn == xbr) && yn == y) ||
           ((xp == 0 || xp == xbr-1) && yp == y && (xn == 0 || xn == xbr-1) && yn == y)){
            map.setgrid(x,y,3);
        }

        if(((yp  == y-1 || yp  == y+1) && xp == x && (yn  == y-1 || yn  == y+1) && xn == x) ||
          ((yp  == ybr || yp  == 1) && xp == x && (yn  == ybr || yn  == 1) && xn == x) ||
          ((yp  == 0 || yp  == ybr-1) && xp == x && (yn  == 0 || yn  == ybr-1) && xn == x)){

            map.setgrid(x,y,5);
        }

        if((xp  == x-1 && yp == y && xn  == x && yn == y+1) ||
           (xp  == x && yp == y+1 && xn  == x-1 && yn == y) ||
           (xp == xbr && xn == 0 && yp == y && yn == y+1) ||
           (xp == 0 && xn == xbr && yp == y+1 && yn == y) ||
           (xp == x-1 && xn == x && yp == ybr && yn == 0) ||
           (xp == x && xn == x-1 && yp == 0 && yn == ybr)){
            map.setgrid(x,y,7);
        }

        if((xp  == x+1 && yp == y && xn  == x && yn == y+1) ||
           (xp == x && yp == y+1 && xn  == x+1 && yn == y) ||
           (xp == 0 && xn == xbr && yp == y && yn == y+1) ||
           (xp == xbr && xn == 0 && yp == y+1 && yn == y) ||
           (xp == x && xn == x+1 && yp == 0 && yn == ybr) ||
           (xp == x+1 && xn == x && yp == ybr && yn == 0)){
            map.setgrid(x,y,9);
        }

        if((xp  == x-1 && yp == y && xn  == x && yn == y-1) ||
           (xp  == x && yp == y-1 && xn  == x-1 && yn == y) ||
           (xp == x && xn == x-1 && yp == ybr && yn == 0) ||
           (xp == x-1 && xn == x && yp == 0 && yn == ybr) ||
           (xp == 0 && xn == xbr && yp == y-1 && yn == y) ||
           (xp == xbr && xn == 0 && yp == y && yn == y-1)){
            map.setgrid(x,y,11);
        }

        if((xp  == x+1 && yp == y && xn  == x && yn == y-1) ||
           (xp  == x && yp == y-1 && xn  == x+1 && yn == y) ||
           (xp == xbr && xn == 0 && yp == y-1 && yn == y) ||
           (xp == 0 && xn == xbr && yp == y && yn == y-1) ||
           (xp == x && xn == x+1 && yp == ybr && yn == 0) ||
           (xp == x+1 && xn == x && yp == 0 && yn == ybr)){
            map.setgrid(x,y,13);
        }






    }

    x = m_snake[m_snake.size()-1].posX;
    y = m_snake[m_snake.size()-1].posY;

    if(m_snake[m_snake.size()-2].posX  == x && m_snake[m_snake.size()-2].posY == y-1){
        map.setgrid(x,y,10);
    }
    if(m_snake[m_snake.size()-2].posX  == x && m_snake[m_snake.size()-2].posY == y+1){
        map.setgrid(x,y,12);
    }
    if(m_snake[m_snake.size()-2].posY  == y && m_snake[m_snake.size()-2].posX == x-1){
        map.setgrid(x,y,14);
    }
    if(m_snake[m_snake.size()-2].posY  == y && m_snake[m_snake.size()-2].posX == x+1){
        map.setgrid(x,y,16);
    }

}





/*!
 *  \brief blit_wallPaper
 *
 *  Method used to display the wallpaper on the screen
 *
 *  \param screen : Main SDL surface where the the wallpaper has to be displayed on
 *  \param map : map where the snake is living on
 */
void Snake::blit_wallPaper(SDL_Surface *screen, Map const& map) const{

    SDL_Rect position;
    position.x = map.getbeginX();
    position.y = map.getbeginY();
    position.w = map.getgridW() *map.getwidthTile();
    position.h = map.getgridH() *map.getheigthTile();
    SDL_BlitSurface(wallPaper,NULL,screen,&position);

}
