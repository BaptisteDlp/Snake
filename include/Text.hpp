#ifndef DEF_TEXT
#define DEF_TEXT

/*!
 * \file Text.hpp
 * \brief Definition of Text class
 * \author Baptiste D.
 * \version 1.0
 */

#include<SDL/SDL_ttf.h>



/*!
 * \class Text
 * \brief class that allow to display text message on the screen
 * 
 */
class Text{

    public:
        Text(char*const message, TTF_Font * police ,int colR, int colG, int colB, bool bold, int posX, int posY);
        ~Text();

        void blit_Text(SDL_Surface *screen, bool middle);
        void update_Message(char *const message);

        int getmiddlePosx() const;
        int getmiddlePosy() const;

        void setColor(int colR, int colG, int colB);


    private:
        TTF_Font *m_police; /*< Style of police (ttf_font) */
        SDL_Color m_color; /*< Color of the font */
        SDL_Surface *m_surface; /*< SDL surface that corresponds to the message to display */
        SDL_Rect m_position; /*< coordinates of the upper left corner of the rectangle */
        SDL_Rect m_middlePosition; /*< Middle coordinates of the rectangle */
        char * m_message; /*< Message that we would like to display on the screen */


};




#endif // DEF_TEXT
