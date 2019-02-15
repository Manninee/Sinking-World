#include "game_saving.hh"
#include "gameboard.hh"
#include "gamestate.hh"
#include "player.hh"

namespace Student
{

game_saving::game_saving(std::shared_ptr< GameBoard > board_ptr,
                         std::shared_ptr< GameState > state_ptr,
                         std::vector<std::shared_ptr<Student::Player>> players):
    board_ptr_(board_ptr), state_ptr_(state_ptr),players_(players)
{

}

unsigned int Student::game_saving::save()
{
    return  0;
}

}


