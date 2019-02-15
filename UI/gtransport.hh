#ifndef GBOAT_HH
#define GBOAT_HH

#include "constants.hh"
#include "cubecoordinate.hh"
#include "gamestate.hh"

#include <QGraphicsPolygonItem>
#include <QGraphicsSceneDragDropEvent>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QCursor>
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QPainter>
#include <string>

namespace Student {

/**
 * @brief Toteuttaa transporttien graaffisen puolen
 */
class GTransport : public QObject, public QGraphicsPolygonItem
{
    Q_OBJECT

public:
    /**
     * @brief Luo graaffisen transportin
     * @param transport_id: Transporttin id
     * @param type: Transportin tyyppi
     * @param gamestate: Osoitin pelintilanetta seuraavaan olioon
     * @param moving_actor: Osoitin muuttujaan jossa on tietona pyörityksessä
     *                      liikuteltavan olion tyyppi
     * @param view_scale: Osoitin ruudun skaalausta seuraavaan muuttujaan
     * @param coordinates: Transportin koordinaatit
     * @param parent: Osoitin transportin graaffisen olion vanhempaan
     * @post Poikkeustakuu: Perus
     */
    explicit GTransport(int transport_id, std::string type,
                        std::shared_ptr<Student::GameState> gamestate,
                        std::string* moving_actor, double* view_scale,
                        Common::CubeCoordinate coordinates =
                            Common::CubeCoordinate(0,0,0),
                        QGraphicsItem *parent = nullptr);

    /**
     * @brief Asettaa uuden koordinaatin veneelle
     * @param new_coord: Uudet koordinaatit
     * @post Poikkeustakuu: Nothrow
     */
    void set_coordinate(Common::CubeCoordinate new_coord);

    /**
     * @brief Palauttaa veneen id:n
     * @return Veneen id
     * @post Poikkeustakuu: Nothrow
     */
    int get_transport_id() const;

signals:

    /**
     * @brief Ilmoittaa mikä nappula on tiputettu transporttiin
     * @param pawn_id: Tiputetun nappulan id
     * @param transport_id: Tiputuksen kohteena olevan transportin id
     * @param origin: Nappulan lähtökoordinaatit
     * @param target: Nappulan tiputuskokoordinaatit
     */
    void pawn_dropped(int pawn_id, int transport_id,
                      Common::CubeCoordinate origin,
                      Common::CubeCoordinate target);

protected:

    /**
     * @brief Tarkistaa voiko nappulaa painaa ja hyväksyy tai hylkää
     *        painalluksen
     * @param event: Tapahtuman tiedot
     * @post Poikkeustakuu: Nothrow
     */
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    /**
     * @brief Aloittaa transportin raahaustapahtuman
     * @details Luo hiiren alle kuvan raahattavasta transportista ja värjää
     *          transportin raahauksen ajaksi oranssiksi
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

    /**
     * @brief Hyväksyy raahauksen tulemisen
     * @param event: Tapahtuman tiedot
     * @post Poikkeustakuu: Nothrow
     */
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;

    /**
     * @brief Ottaa vastaan nappuloiden tiputuksen
     * @details Ottaa vastaan nappulan tiputuksen ja lähettää siitä tiedon
     *          signaalilla
     * @param event: Tapahtuman tiedot
     * @post Poikkeustakuu: Perus
     */
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;

private:

    /**
     * @brief coordinates_: Transportin koordinaatit
     */
    Common::CubeCoordinate coordinates_;

    /**
     * @brief gamestate_: Osoitin pelintilanetta seuraavaan olioon
     */
    std::shared_ptr<Student::GameState> gamestate_;

    /**
     * @brief boat_id_: Transportin id
     */
    int transport_id_;

    /**
     * @brief type_: Transportin tyyppi
     */
    std::string type_;

    /**
     * @brief moving_actor_: Osoitin muuttujaan jossa on tietona pyörityksessä
     *                       liikuteltavan olion tyyppi
     */
    std::string* moving_actor_;

    /**
     * @brief view_scale_: Osoitin ruudun skaalauksen suuruutta seuraavaan lukuun
     */
    double* view_scale_;
};

}
#endif // GBOAT_HH
