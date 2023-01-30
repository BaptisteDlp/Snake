#ifndef DEF_SNAKE
#define DEF_SNAKE


/*!
 * \file Snake.hpp
 * \brief Definition of Snake class
 * \author Baptiste D.
 * \version 1.0
 */


#include<vector>
#include"Text.hpp"


/*!
 * \struc TileCharacteristic
 * \brief Properties of a tile that characterize the snake
 *
 */
typedef struct{
  SDL_Surface *tileCharacter; /*!< tile_file.bmp */
  SDL_Rect rectCharacter; /*!< Define the tile dimension */
  SDL_Rect position; /*!<Position of the character within the grid */
  int widthTile; /*!< Width of the tile within the tileSet (nb of pixel)*/
  int heigthTile; // Heigth of the tile within the tileSet (nb of pixel)*/
}TileCharacteritic;


/*!
 * \struc Body
 * \brief Position of on part of the Snake within the grid
 *
 */
typedef struct{
  int posX;
  int posY;
}Body;




enum{UP, DOWN, LEFT, RIGHT};
enum{HEADN, HEADS, HEADO, HEADE, TAILN, TAILS, TAILO, TAILE, BODYH, BODYV, NO, NE, SO, SE, OBJ};




/*! \class Snake
 * \brief class that models a Snake on a 2-dimmensional map
 *
 *  This class aims at modelling a snake on a 2 dimmensionnal map that will be
 *  generated with a tileSet. Several other methods usefull for the game itself are also
 *  implemented in this class.
 */
class Snake{

public:
  Snake(char *const fileName, std::string name, int posX, int posY, int speed);
  Snake(Snake const& snake);
  ~Snake();


  void event_Analysis(bool *keepOnGame, SDL_Surface *screen, Text & pause);
  void event_gameOver(char* const fileName,SDL_Surface *screen, Map &map1, Text &message, Text &message2, bool keepOnGame);
  void move_Snake(Map const& map1);

  void set_Objectif(Map &map1);
  void eat_Objectif();

  bool detection(Map const& map1);

  bool getalive() const;
  bool getobj() const;
  int getspeed() const;
  int getscore() const;
  int getTimeScore() const;
  int getsetTimeScore() const;
  int getdirection() const;
  int getobjx() const;
  int getobjy() const;
  SDL_Surface* getwall() const;


  void setWallPaper(char *const fileName);
  void setPosXY(int posX, int posY);

  void clear_Snake(Map &map1) const;

  void blit_Snake(Map &map1) const;
  void blit_Objectif(SDL_Surface *screen) const;
  void blit_wallPaper(SDL_Surface *screen, Map const& map1) const;
  void blit_Score(SDL_Surface *screen, Map const& map1);



private:

  SDL_Surface *wallPaper;/*< SDL Surface that corresponds to the wallpaper (background of the game) */
  int m_score; /*< Score obtained by the user */
  int m_timeScore; /*< */
  int m_setTimeScore; /*< */

  int m_direction; /*< Direction of the snake, either UP, DOWN, LEFT or RIGHT*/
  bool m_alive; /*< true if the snake is alive, false otherwise( and thus the game ends */
  int m_speed; /*< Speed of the snake */

  bool m_objectifSet; /*< true or false whether the objectif is set or not */
  int m_posObjX; /*< x-coordinate of the objectif within the grid */
  int m_posObjY; /*< y-coordinate of the objectif within the grid */

  std::string m_player; /*< Pseudo of the user */

  std::vector<Body> m_snake; /*< Vector that contains the body parts of the snake */




};





#endif // DEF_SNAKE
