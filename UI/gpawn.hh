#ifndef GPAWN_HH
#define GPAWN_HH

#include "constants.hh"
#include "cubecoordinate.hh"
#include "gamestate.hh"

#include <QGraphicsPolygonItem>
#include <QGraphicsSceneDragDropEvent>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsView>
#include <QPainter>
#include <QCursor>
#include <QApplication>
#include <QDrag>
#include <QWidget>
#include <QMimeData>

namespace Student {

/**
 * @brief Toteuttaa pelinappulan graaffisen puolen
 */
class GPawn : public QGraphicsPolygonItem
{
public:
    /**
     * @brief Luo graaffisen pelinappulan
     * @param playerId: Pelaajan id
     * @param pawnId: Nappulan id
     * @param gamestate: Osoitin pelintilanetta seuraavaan olioon
     * @param view_scale: Osoitin ruudun skaalausta seuraavaan muuttujaan
     * @param coordinates: Nappulan koordinaatit
     * @param parent: Osoitin pelinappulan graaffisen olion vanhempaan
     * @post Poikkeustakuu: Perus
     */
    explicit GPawn(int playerId, int pawnId,
          std::shared_ptr<Student::GameState> gamestate, double* view_scale,
          Common::CubeCoordinate coordinates = Common::CubeCoordinate(0,0,0),
          QGraphicsItem *parent = nullptr);

    /**
     * @brief Asetetaan pelinappulalle koordinaatit
     * @param new_coord: Uudet koordinaatit
     * @post Poikkeustakuu: Nothrow
     */
    void set_coordinate(Common::CubeCoordinate new_coord);

    /**
     * @brief Palauttaa pelinappulan id:n
     * @return Pelinnappulan id
     * @post Poikkeustakuu: Nothrow
     */
    int get_pawn_id() const;

protected:

    /**
     * @brief Tarkistaa voiko nappulaa painaa ja hyväksyy tai hylkää
     *        painalluksen
     * @param event: Tapahtuman tiedot
     * @post Poikkeustakuu: Nothrow
     */
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    /**
     * @brief Aloittaa nappulan raahaustapahtuman
     * @details Luo hiiren alle kuvan raahattavasta nappulasta ja värjää
     *          nappualan raahauksen ajaksi oranssiksi
     * @param event: Tapahtuman tiedot
     * @post Poikkeustakuu: Perus
     */
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    /**
     * @brief Vaihtaa kursorin oikeaksi kun hiiri vapautetaan
     * @param event: Tapahtuman tiedot
     * @post Poikkeustakuu: Nothrow
     */
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:

    /**
     * @brief coordinates_: Nappulan koordinaatit
     */
    Common::CubeCoordinate coordinates_;

    /**
     * @brief gamestate_: Osoitin pelin tilanteeseen
     */
    std::shared_ptr<Student::GameState> gamestate_;

    /**
     * @brief view_scale_: Osoitin ruudun skaalauksen suuruutta seuraavaan lukuun
     */
    double* view_scale_;

    /**
     * @brief playerid_: Pelaajan id
     */
    int playerid_;

    /**
     * @brief pawnid_: Pelinappulan id
     */
    int pawnid_;
};

}
#endif // GPAWN_HH
