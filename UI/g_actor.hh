#ifndef G_ACTOR_HH
#define G_ACTOR_HH

#include "constants.hh"
#include "cubecoordinate.hh"
#include "gamestate.hh"
#include "gameexception.hh"

#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QGraphicsSceneDragDropEvent>
#include <QObject>
#include <QCursor>
#include <QApplication>
#include <QPixmap>
#include <QDrag>
#include <QMimeData>
#include <QWidget>
#include <QDebug>

/**
 * @file
 * @brief Toteuttaa luokan g_agtor
 */

namespace Student
{

/**
 * @brief Toteuttaa pelilaudalla näkyvän toimijan.
 */
class g_actor : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:

    /**
     * @brief Rakentaja
     * @param parent QObject-tyyppinen osoitin vanhempaan
     * @param parent_item QGraphicsItem-tyyppinen osoitin vanhempaan
     * @param type Toimijan tyyppi enum-arvona
     * @param id Toimijan yksilöivä tunnus
     * @param coordinates Toimijan sijainti alustetaan koordinaatteihin (0,0,0)
     * @param gamestate Osoitin pelin tilaa ylläpitävään olioon.
     */
    explicit g_actor(QObject *parent = nullptr,
          QGraphicsItem* parent_item = nullptr,  double* view_scale = nullptr,
          ACTOR_TYPES type = KRAKEN, int id = 0,
          std::string* moving_actor = nullptr,
          Common::CubeCoordinate coordinates = Common::CubeCoordinate(0,0,0),
          std::shared_ptr<Student::GameState> gamestate = nullptr);

    /**
     * @brief Asettaa toimijalle QGraphicsItem-tyyppisen vanhemman.
     * @param parent Uusi vanhempi
     */
    void set_parent_item(QGraphicsItem* parent);

    /**
     * @brief Asettaa uuden sijainnin toimijalle
     * @param coordinates Uusi sijainti
     */
    void set_coordinates(Common::CubeCoordinate coordinates);

    /**
     * @brief get_actor_type
     * @return Toimijan tyyppi enumeraattorina
     */
    int get_actor_type();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    /**
     * @brief parent_: Osoitin vanhempaan, eli siihen ruutuun, jossa ollaan.
     */
    QGraphicsItem* parent_;

    /**
     * @brief Kuva, joka näkyy toimijan ulkomuotona pelaajalle.
     */
    QPixmap picture_;

    /**
     * @brief Toimijan tyyppi enumeraattorina.
     */
    ACTOR_TYPES type_;

    /**
     * @brief Toimijan yksilöivä tunniste kokonaislukuna.
     */
    int id_;

    std::string* moving_actor_;

    /**
     * @brief Toimijan sijainti pelilaudalla.
     */
    Common::CubeCoordinate coordinates_;

    /**
     * @brief Osoitin pelin tilaa ylläpitävään olioon.
     */
    std::shared_ptr<GameState> gamestate_;

    /**
     * @brief Jotain hämärää liittyen johonkin.
     */
    double* view_scale_;

};
}

#endif // G_ACTOR_HH
