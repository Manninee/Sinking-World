#include "gamestate.hh"

#include <QDebug>

namespace Student {

GameState::GameState():
    phase_(Common::GamePhase::MOVEMENT),
    current_player_(0)
{

}

Common::GamePhase Student::GameState::currentGamePhase() const
{
    return phase_;
}

int GameState::currentPlayer() const
{
    return current_player_;
}

void GameState::changeGamePhase(Common::GamePhase next_phase)
{
    phase_ = next_phase;
    emit phase_changed();
}

void GameState::changePlayerTurn(int next_player)
{
    current_player_ = next_player;
    emit turn_changed();
}

}
