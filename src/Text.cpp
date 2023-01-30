/*!
 * \file Text.cpp
 * \brief Definition of Text class
 * \author Baptiste D.
 * \version 1.0
 */

#include<iostream>
#include<cassert>
#include"Text.hpp"

using namespace std;



/*!
 *  \brief Constuctor
 *
 *  Constructor of class Text
 *
 *  \param message : message we would like to display on the screen
 *  \param police : style of police for the message
 *  \param colR : red pixel for the color (between 0 and 256)
 *  \param colG : green pixel for the color (between 0 and 256)
 *  \param colB : blue pixel for the color (between 0 and 256)
 *  \param bold : true if we want the message to be bold, false otherwise
 *  \param posX : x coordinate of the upper left corner of the message
 *  \param posY : y coordinate of the upper left corner of the message
 */
Text::Text(char *const message, TTF_Font* police ,int colR, int colG, int colB, bool bold, int posX, int posY){

  assert(colR >= 0 && colR <= 256);
  assert(colG >= 0 && colG <= 256);
  assert(colB >= 0 && colB <= 256);
  m_message = message;
  m_police = police;
  m_color = {colR,colG,colB};



  if(bold);
  TTF_SetFontStyle(m_police, TTF_STYLE_BOLD);

  m_position.x = posX;
  m_position.y = posY;



  m_surface = TTF_RenderText_Solid(m_police,m_message,m_color);

  m_middlePosition.x =  m_surface->w/2;
  m_middlePosition.y =  m_surface->h/2;

}


Text::~Text(){

    SDL_FreeSurface(m_surface);

}



int Text::getmiddlePosx() const{return m_middlePosition.x;};
int Text::getmiddlePosy() const{return m_middlePosition.y;};




/*!
 *  \brief update_Message
 *
 *  Method used to change the message
 *
 *  \param message : message we would like to display on the screen
 */
void Text::update_Message(char *const message){

    SDL_FreeSurface(m_surface);
    m_message = message;
    m_surface = TTF_RenderText_Solid(m_police,m_message,m_color);
    m_middlePosition.x =  m_surface->w/2;
    m_middlePosition.y =  m_surface->h/2;



}



/*!
 *  \brief update_Message
 *
 *  Method used to display the message on the screen
 *
 *  \param screen : SDL surface where we want to display to message
 *  \param middle : true if the want the message to be centered, false otherwise
 */
void Text::blit_Text(SDL_Surface *screen, bool middle){

    if(middle){
        SDL_Rect position;
        position.x = m_position.x-m_middlePosition.x;
        position.y = m_position.y-m_middlePosition.y;
        SDL_BlitSurface(m_surface, NULL, screen ,&position);
    }else{
        SDL_BlitSurface(m_surface, NULL, screen ,&m_position);
    }

}



/*!
 *  \brief setColor
 *
 *  Method used to change the color of the message
 *
 *  \param colR : red pixel for the color (between 0 and 256)
 *  \param colG : green pixel for the color (between 0 and 256)
 *  \param colB : blue pixel for the color (between 0 and 256)
 */
void Text::setColor(int colR, int colG, int colB){


  assert(colR >= 0 && colR <= 256);
  assert(colG >= 0 && colG <= 256);
  assert(colB >= 0 && colB <= 256);

  m_color = {colR,colG,colG};
  SDL_FreeSurface(m_surface);
  m_surface = TTF_RenderText_Solid(m_police,m_message,m_color);

}
