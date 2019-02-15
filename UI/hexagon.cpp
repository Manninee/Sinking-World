#include "hexagon.hh"

#include <QDebug>
namespace Student
{
Hexagon::Hexagon(Common::CubeCoordinate coordinates,
                 std::shared_ptr<Student::GameState> gamestate,
                 QGraphicsItem *parent):
    QGraphicsPolygonItem(QPolygonF(SHAPES.at("hexagon")), parent),
    coordinates_(coordinates),
    gamestate_(gamestate)
{
    setPen( QPen(Qt::black) );
    setAcceptDrops(true);
}

void Hexagon::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Jos vaihe on upotus hyväksytään klikkaus
    if(gamestate_->currentGamePhase() == Common::GamePhase::SINKING)
    {
        event->accept();
        emit clicked(coordinates_);
    }
    else
    {
        event->ignore();
    }

}

void Hexagon::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    event->accept();
    update();
}

void Hexagon::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    event->setDropAction(Qt::MoveAction);

    // Jos tiputettava olio on pelaajan nappula
    if (event->mimeData()->hasFormat("pawn") &&
            gamestate_->currentGamePhase() == Common::GamePhase::MOVEMENT)
    {
        const QMimeData *mime = event->mimeData();
        QByteArray itemData = mime->data("pawn");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        // Otetaan talteen pelinappulan pelaajan id, pelinappulan id ja nappulan
        // koordinaatit
        int player_id = 0, pawn_id = 0, x = 0, y = 0, z = 0;
        dataStream >> player_id >> pawn_id >> x >> y >> z;

        // Jos Pelin tilanne on liikuttelu ja oikean pelaaja nappulaa siirretään
        if(gamestate_->currentPlayer() == player_id)
        {
            // Lähetetään pelinappulan id, lähtökoordinaatit ja loppukoordinaatit
            emit pawn_dropped(pawn_id, Common::CubeCoordinate(x, y, z),
                              coordinates_);
            event->accept();
            return;
        }
    }
    // Jos tiputettava olio on transport
    else if (event->mimeData()->hasFormat("transport") &&
             gamestate_->currentGamePhase() != Common::GamePhase::SINKING)
    {
        const QMimeData *mime = event->mimeData();
        QByteArray itemData = mime->data("transport");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        // Otetaan talteen transportin id ja sen koordinaatit
        qint32 transport_id = 0, x = 0, y = 0, z = 0;
        dataStream >> transport_id >> x >> y >> z;

        // Lähetetään transportin id, lähtökoordinaatit ja loppukoordinaatit
        emit transport_dropped(transport_id, Common::CubeCoordinate(x, y, z),
                                coordinates_);
        event->accept();
        return;
    }
    // Jos tiputettava olio on actor
    else if(event->mimeData()->hasFormat("actor"))
    {
        const QMimeData *mime = event->mimeData();
        QByteArray itemData = mime->data("actor");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        int actor_id = 0, x = 0, y = 0, z = 0;
        dataStream >> actor_id >> x >> y >> z;

        if(gamestate_->currentGamePhase() == Common::GamePhase::SPINNING)
        {
            emit actor_dropped(actor_id, Common::CubeCoordinate(x, y, z),
                              coordinates_);

            event->setDropAction(Qt::MoveAction);
            event->accept();
            update();
            return;
        }
    }
    event->ignore();
}
}
