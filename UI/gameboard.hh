#ifndef GAMEBOARD_HH
#define GAMEBOARD_HH

#include "igameboard.hh"
#include "hexagon.hh"
#include "hex.hh"
#include "cubecoordinate.hh"
#include "player.hh"
#include "gamestate.hh"
#include "transport.hh"
#include "boat.hh"
#include "actor.hh"
#include "gameexception.hh"
#include "pawn.hh"

#include <map>
#include <QObject>

namespace Student
{
    using hexes_map_type = std::map<Common::CubeCoordinate,
                           std::shared_ptr<Common::Hex>>;

/**
 * @brief Toimii pelilaudan ja toimijoiden tallennuspaikkana
 */
class GameBoard : public QObject, public Common::IGameBoard
{
    Q_OBJECT
public:

    /**
     * @brief Alustaa pohja tallennettaville tiedoille ja
     * luo nappulat pelaajille
     * @post Poikkeustakuu: Perus
     */
    GameBoard();

    /**
     * @brief Asettaa vektorin pelaajiin ja antaa pelaajille nappulat
     * @param players Pelaajien osoittimia sisältävä vektori
     * @post Poikkeustakuu: Perus
     */
    void set_player_pointer_vector(
            std::vector<std::shared_ptr<Player>> players);

    /**
     * @brief Asettaa Gameboard luokan vaatiman gamestate-olion.
     * @param gamestateOsoitin pelin tilannetta seuraavaan olioon
     * @post Poikkeustakuu: Perus
     */
    void set_gamestate_ptr(std::shared_ptr<GameState> gamestate);

    /**
     * @copydoc Common::IGameBoard::addHex()
     */
    void addHex(std::shared_ptr<Common::Hex> new_hex) override;

    /**
     * @copydoc Common::IGameBoard::getHex()
     */
    std::shared_ptr<Common::Hex> getHex(Common::CubeCoordinate hex_coordinate)
                                        const override;

    /**
     * @brief Hakee koko pelilaudan osoittimen
     * @return Osoitin koko pelilaudan ruudustoon
     * @post Poikkeustakuu: Nothrow
     */
    hexes_map_type* get_map();

    /**
     * @brief Hakee kaikkien transporttien osoittimet
     * @return Osoittin mappiin jossa on transporttien osoittimet
     * @post Poikkeustakuu: Nothrow
     */
    std::map<int, std::shared_ptr<Common::Transport>>* get_transport_map();

    /**
     * @brief Hakee kaikkien actoreiden osoittimet
     * @return Osoitin mappiin jossa on actorien osoittimet
     * @post Poikkeustakuu: Nothrow
     */
    std::map<int, std::shared_ptr<Common::Actor>>* get_actor_map();

    /**
     * @brief Hakee transportin osoittimen
     * @param transport_id: Transportin id
     * @return Osoitin transporttiin jos transport löytyy muuten nullptr
     * @post Poikkeustakuu: Perus
     */
    std::shared_ptr<Common::Transport> get_transport(int transport_id) const;

    /**
     * @brief Hakee osoittimen nappulaan
     * @param pawn_id: Nappulan id
     * @return Osoitin nappulaan jos napppula löytyy muuten nullptr
     * @post Poikkeustakuu: Perus
     */
    std::shared_ptr<Common::Pawn> get_pawn(int pawn_id) const;

    /**
     * @copydoc Common::IGameBoard::checkTileOccupation()
     */
    int checkTileOccupation(Common::CubeCoordinate hex_coordinate) const override;

    /**
     * @copydoc Common::IGameBoard::sWaterTile()
     */
    bool isWaterTile(Common::CubeCoordinate hex_coordinate) const override;

    /**
     * @copydoc Common::IGameBoard::addPawn()
     */
    void addPawn(int playerId, int pawnId, Common::CubeCoordinate coord) override;

    /**
     * @copydoc Common::IGameBoard::addPawn()
     */
    void addPawn(int playerId, int pawnId) override;

    /**
     * @copydoc Common::IGameBoard::movePawn()
     */
    void movePawn(int pawnId, Common::CubeCoordinate pawnCoord) override;

    /**
     * @copydoc Common::IGameBoard::removePawn()
     */
    void removePawn(int pawnId) override;

    /**
     * @copydoc Common::IGameBoard::moveActor()
     */
    void moveActor(int actorId, Common::CubeCoordinate actorCoord) override;

    /**
     * @copydoc Common::IGameBoard::removeActor()
     */
    void removeActor(int actorId) override;

    /**
     * @copydoc Common::IGameBoard::addActor()
     */
    void addActor(std::shared_ptr<Common::Actor> actor,
                  Common::CubeCoordinate actorCoord) override;

    /**
     * @copydoc Common::IGameBoard::addTransport()
     */
    void addTransport(std::shared_ptr<Common::Transport> transport,
                      Common::CubeCoordinate coord) override;

    /**
     * @copydoc Common::IGameBoard::moveTransport()
     */
    void moveTransport(int id, Common::CubeCoordinate coord) override;

    /**
     * @copydoc Common::IGameBoard::removeTransport()
     */
    void removeTransport(int id) override;

    /**
     * @brief Tekee actorien toiminnasta johtuvat transporttien, nappuloiden ja
     *        actoreiden poistot
     * @param actor_id: Toimivan actorin id
     * @post Poikkeustakuu: Perus
     */
    void use_actor(int actor_id);

signals:

    /**
     * @brief Ilmoittaa uuden transportin luomisesta
     * @param transport: Luodun transportin id
     * @param coord: Luodun transportin koordinaatit
     */
    void transport_created(std::shared_ptr<Common::Transport> transport,
                           Common::CubeCoordinate coord);

    /**
     * @brief Ilmoittaa uuden actorin luomisesta
     * @param type: Uuden actorin tyyppi
     * @param id: Uuden actorin  id
     * @param coordinates: Uuden actorin koordinaatit
     */
    void new_actor(int type, int id, Common::CubeCoordinate coordinates);

    /**
     * @brief Ilmoittaa actorin poistamisesta
     * @param id: Poistetun actorin id
     */
    void remove_g_actor(int id);

    /**
     * @brief Ilmoittaa vortexin pyörimistarpeen
     * @param id: Vortexin id
     */
    void spin_vortex(int id);

    /**
     * @brief Ilmoittaa nappulan poistosta
     * @param id: Poistetun nappulan id
     */
    void remove_pawn(int id);

    /**
     * @brief Ilmoittaa transportin poistosta
     * @param transport_id: Poistetun transportin id
     * @param coord: Poistetun transportin koordinaatit
     */
    void remove_transport(int transport_id, Common::CubeCoordinate coord);

private:

    /**
     * @brief hex_map_: Pelilaudan ruudusto
     */
    hexes_map_type hex_map_;

    /**
     * @brief actor_struct_: sisältää kaikki actorit
     */
    std::map<int, std::shared_ptr<Common::Actor>> actor_map_;

    /**
     * @brief player_vector_: Sisältää osoittimet pelaajiin
     */
    std::vector<std::shared_ptr<Student::Player>> player_vector_;

    /**
     * @brief transport_map_: Sisältää osoittimet transportteihin
     */
    std::map<int, std::shared_ptr<Common::Transport>> transport_map_;

    /**
     * @brief gamestate_: Osoitin pelin tilannetta seuraavaan olioon
     */
    std::shared_ptr<Student::GameState> gamestate_;

    /**
     * @brief pawn_map_: Sisältää nappulat avaimena nappulan id
     */
    std::map<int, std::shared_ptr<Common::Pawn>> pawn_map_;
};

}
#endif // GAMEBOARD_HH
