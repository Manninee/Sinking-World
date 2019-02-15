#ifndef HEXAGON_HH
#define HEXAGON_HH

#include "constants.hh"
#include "cubecoordinate.hh"
#include "gamestate.hh"

#include <QPainter>
#include <QGraphicsPolygonItem>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QObject>

namespace Student
{

/**
 * @brief Toteuttaa graaffisen pelilaudan ruudun
 */
class Hexagon : public QObject, public QGraphicsPolygonItem
{
    Q_OBJECT

public:
    /**
     * @brief Luo ja alustaa graaffisen ruudun
     * @param coordinates: Ruudun koordinaatit
     * @param gamestate: Osoitin pelintilanetta seuraavaan olioon
     * @param parent: Osoitin ruudun vanhempaan
     * @post Poikkeustakuu: Perus
     */
    Hexagon(Common::CubeCoordinate coordinates,
            std::shared_ptr<Student::GameState> gamestate,
            QGraphicsItem *parent = nullptr);

signals:

    /**
     * @brief Ilmoittaa että ruutua on painettu
     * @param coord: Ruudun koordinaatit
     */
    void clicked(Common::CubeCoordinate coord);

    /**
     * @brief Lähetään nappulan id:n sen alkuperäisen koordinaatin ja
     *        tiputuksen kohteena olevan ruudun koordinaatin
     * @param pawn_id: Tiputetun nappulan id
     * @param origin: Nappulan lähtökoordinaatit
     * @param target: Nappulan tiputuskokoordinaatit
     */
    void pawn_dropped(int pawn_id,
                      Common::CubeCoordinate origin,
                      Common::CubeCoordinate target);

    /**
     * @brief Ilmoittaa actorin tiputtamisesta
     * @param actor_id: Tiputettavan actorin id
     * @param origin: Actorin lähtökoordinaatit
     * @param target: Actorin tiputuskokoordinaatit
     */
    void actor_dropped(int actor_id, Common::CubeCoordinate origin,
                       Common::CubeCoordinate target);

    /**
     * @brief Ilmoittaa transportin tiputuksesta
     * @param transport_id: Transportin id
     * @param origin: Transportin lähtökoordinaatit
     * @param target: Transportin tiputuskoordinaatit
     */
    void transport_dropped(int transport_id,
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
     * @brief coordinates_: Ruudun koordinaatit
     */
    Common::CubeCoordinate coordinates_;

    /**
     * @brief gamestate_: Osoitin pelin tilanteeseen
     */
    std::shared_ptr<Student::GameState> gamestate_;
};

}

#endif // HEXAGON_HH
