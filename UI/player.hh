#ifndef PLAYER_HH
#define PLAYER_HH

#include "iplayer.hh"
#include "player_info.hh"
#include "pawn.hh"
#include "gpawn.hh"
#include "constants.hh"

#include <string>
#include <QString>
#include <vector>

/**
 * @file
 * @brief Tiedostossa määritellään Player-luokka
 */

namespace Student
{

/**
 * @brief Toteuttaa Player-luokan.
 */
class Player : public Common::IPlayer
{
public:
    /**
     * @brief Tyhjä rakentaja
     * @param player-info-structi, jossa tiedot pelaajan alustamiseen.
     */
    explicit Player(player_info player);

    /**
     * @brief Palauttaa pelaajan id:n.
     * @return
     */
    int getPlayerId() const override;

    /**
     * @brief Asettaa jäljellä olevien liikkeiden määrän.
     * @param actionsLeft
     */
    void setActionsLeft(unsigned int actionsLeft) override;

    /**
     * @brief Palauttaa jäljellä olevien liikkeiden määrän.
     * @return
     */
    unsigned int getActionsLeft() const override;

    /**
     * @brief Luo pelaajalle uuden nappula.
     * @param pawn_id: uuden nappulan globaali tunniste.
     */
    void add_pawn(std::shared_ptr<Common::Pawn> pawn);

    /**
     * @brief Palauttaa älykkään osoittimen pelinappula-id:tä
     * vastaavaan nappulaan.
     * @param pawnId
     * @return
     */
    std::shared_ptr<Common::Pawn> get_pawn(int pawnId) const;

    /**
     * @brief Palauttaa pelaajan värin.
     * @return
     */
    QString get_color() const;

    /**
     * @brief Palauttaa pelaajan pelinappular vectorissa.
     * @return
     */
    std::vector<std::shared_ptr<Common::Pawn>>* get_pawn_vector();

    /**
     * @brief Palauttaa pelaajan nimen.
     * @return
     */
    QString get_name();

    /**
     * @brief Poistaa pelinappulan pelajalta.
     * @param pawn_id: Nappulan id.
     */
    void remove_pawn(int pawn_id);

    /**
     * @brief Lisää pelaajalle pisteen.
     */
    void add_point();

    /**
     * @brief Palauttaa pelaajan pistetilanteen.
     * @return
     */
    int get_points() const;

private:

    /**
     * @brief Pelaajan väri QStringinä.
     */
    QString color_;

    /**
     * @brief Pelaajan globaali tunniste.
     */
    unsigned int player_id_;

    /**
     * @brief Pelaajan nimi
     */
    QString name_;

    /**
     * @brief Liikkeiden lukumäärä, joka pelaajalla on käytettävissä kuluvan
     * vuoron aikana.
     */
    unsigned int actions_left_;

    /**
     * @brief Pelaajan pistetilanne.
     */
    int points_;

    /**
     * @brief Vektori, jossa pelaajan nappulat sijaitsevat.
     */
    std::vector<std::shared_ptr<Common::Pawn>> pawn_vctr_;

};
}
#endif // PLAYER_HH
