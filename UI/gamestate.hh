#ifndef GAMESTATE_HH
#define GAMESTATE_HH

#include "igamestate.hh"

#include <QObject>

/**
 * @file
 * @brief Tiedostossa määritellään GameState-luokka.
 */

namespace Student {

/**
 * @brief Toteuttaa GameState-luokan.
 */
class GameState : public QObject, public Common::IGameState
{
    Q_OBJECT
public:
    /**
     * @brief Tyhjä rakentaja
     */
    GameState();

    /**
     * @brief Palauttaa pelin tilan Common::GamePhase-enumeraattorina.
     * @return
     */
    Common::GamePhase currentGamePhase() const override;

    /**
     * @brief Palauttaa senhetkisen pelaajan kokonaislukuna.
     * @return
     */
    int currentPlayer() const override;

    /**
     * @brief Siirtää pelin tilaa yhdellä eteenpäin.
     * @param next_phase
     */
    void changeGamePhase(Common::GamePhase next_phase) override;

    /**
     * @brief Asettaa seuraavan pelaajan vuoroon.
     * @param next_player
     */
    void changePlayerTurn(int next_player) override;

signals:
    void phase_changed();
    void turn_changed();

private:
    /**
     * @brief Pelin tila. Oletuksena Common::MOVEMENT.
     */
    Common::GamePhase phase_;

    /**
     * @brief Vuorossa oleva pelaaja. Oletuksena 0.
     */
    int current_player_;
};

}

#endif // GAMESTATE_HH
