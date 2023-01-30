#ifndef DEF_OPTIONS
#define DEF_OPTIONS

/*!
 * \file options.hpp
 * \brief Definition of the setting functions
 * \author Baptiste D.
 * \version 1.0
 */


void game(SDL_Surface *screen, Map &map, int posX, int posY, char *const wpName, int speed, char *const playerName);

void erase_highScores();

void edit_map(SDL_Surface *screen, Map &map,int *posX, int *posY, char * const fileWp);

void highScores(SDL_Surface *screens);

void settings(SDL_Surface *screen ,Map &map, char **wpName, int *speed, char **playerName);

void skin(SDL_Surface screen, Map &map);

void speed(SDL_Surface screen, int speed);

void wallPaper(SDL_Surface screen, char ** wpName);

void player(SDL_Surface *screen, char **playerName);





#endif // DEF_OPTIONS
