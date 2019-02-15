#ifndef SPIN_ITEM_HH
#define SPIN_ITEM_HH

#include "constants.hh"
#include "igamerunner.hh"
#include "gamestate.hh"
#include "gameexception.hh"

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QObject>
#include <chrono>
#include <thread>
#include <string>
#include <QDebug>

/**
 * @file
 * @brief Tiedostossa määritellään wheel_item-luokka.
 */

namespace Student
{

/**
 * @brief Toteuttaa kiekon, jota napauttamalla saa "arvottua" juttuja.
 */
class wheel_item : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:

    /**
     * @brief Rakentajassa asetetaan kiekolle oikeanlainen kuva, riippuen
     * siitä, onko kyseessä oikea kiekko, vai pelkkä varjokiekko.
     * @param type: 0 -> oikea kiekko, 1 -> varjokiekko
     * @param runner: älykäs osoitin runneriin
     * @param gamestate: älykäs osoitin gamestate-olioon. tyypin 1 olio ei
     * tarvitse tätä parametriä.
     */
    wheel_item(int type,std::shared_ptr<Common::IGameRunner> runner,
               std::shared_ptr<Student::GameState> gamestate = nullptr);

    /**
     * @brief Annetaan oliolle osoitin view:iin, jossa se näkyy.
     * @param view
     */
    void set_view(QGraphicsView* view);

signals:

    /**
     * @brief Lähettää tiedon kiekon pyörityksen tuloksista.
     * @param pair, jossa std::Stringinä tieto tuloksista muodossa
     *          <actori- tai transporttityyppi, liikkeiden määrä>
     */
    void wheel_spinned(std::pair<std::string, std::string>);

    /**
     * @brief Kertoo pyörityksen alkeneen tai loppuneen
     * @param is_spinning
     */
    void spinning(bool is_spinning);

protected:

    /**
     * @brief Klikattaessa hiirellä tapahtuu mahdollinen pyöritys.
     * @param event
     */
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:

    /**
     * @brief Kääntää kiekkoa parametrin state määräämään asentoon
     * @param state: WHEEL_STATE-enumeraattori
     */
    void rotate(WHEEL_STATES state);

    /**
     * @brief type_:  0 -> oikea kiekko, 1 -> varjokiekko
     */
    int type_;

    /**
     * @brief picture_: Kuva, joka näytetään olion graafisena ilmentymänä.
     */
    QPixmap picture_;

    /**
     * @brief view_: osoitin view:iin, jossa olio näkyy
     */
    QGraphicsView* view_;

    /**
     * @brief runner_: Älykäs osoitin runneriin
     */
    std::shared_ptr<Common::IGameRunner> runner_;

    /**
     * @brief gamestate_: Älykäs osoitin gamestate-olioon.
     */
    std::shared_ptr<Student::GameState> gamestate_;

};
}
#endif // SPIN_ITEM_HH
