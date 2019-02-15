#include "gpawn.hh"

namespace Student
{

GPawn::GPawn(int playerId, int pawnId,
             std::shared_ptr<Student::GameState> gamestate, double* view_scale,
             Common::CubeCoordinate coordinates,QGraphicsItem *parent):
    QGraphicsPolygonItem(QPolygonF( SHAPES.at("gpawn")), parent),
    coordinates_(coordinates),
    gamestate_(gamestate),
    view_scale_(view_scale),
    playerid_(playerId),
    pawnid_(pawnId)
{
    this->setPen( QPen(Qt::black) );
    setAcceptedMouseButtons(Qt::LeftButton);
}

void GPawn::set_coordinate(Common::CubeCoordinate new_coord)
{
    coordinates_ = new_coord;
}

int GPawn::get_pawn_id() const
{
    return pawnid_;
}

void GPawn::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Jos pelin tilanne on siirto ja oikea pelaaja siirtää nappulaa
    if( gamestate_->currentGamePhase() == Common::GamePhase::MOVEMENT &&
            gamestate_->currentPlayer() == playerid_)
    {
        setCursor(Qt::ClosedHandCursor);
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void GPawn::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // Tarkistetaan että nappulaa on siirretty tarpeeksi jotta voidaan aloittaa
    // raahaus
    if (QLineF(event->screenPos(), event->buttonDownScreenPos(Qt::LeftButton))
            .length() < QApplication::startDragDistance())
    {
        return;
    }

    // Luodaan raahaus ja data sille
    QDrag *drag = new QDrag(event->widget());
    QMimeData *mime = new QMimeData;

    // Lasketaan uudet koot skaalauksen perusteella
    double scale = *view_scale_;
    int width = static_cast<int>(boundingRect().width() * scale);
    int height = static_cast<int>(boundingRect().height() * scale);

    // Luodaan raahauksessa hiiren alla oleva kuva
    QPixmap pix(width, height);
    pix.fill(Qt::transparent);
    QPainter *paint = new QPainter(&pix);

    // Skaalataan nappulan piirros oikean kokoiseksi
    paint->translate(width / 2, height / 2);
    paint->scale(scale, scale);

    paint->setRenderHint(QPainter::Antialiasing);
    QStyleOptionGraphicsItem opt;
    this->paint(paint, &opt);
    delete paint;

    // Tallennetaan alkuperäinen väri
    QBrush original_color = brush();

    // Asetetaan nappulalle uusi väri ilmoittamaan mitä ollaan siirtämässä
    setBrush(QColor("orange"));

    // Asetetaan raahaukseen kuva hiiren alle
    drag->setPixmap(pix);
    drag->setHotSpot(QPoint(width / 2, height / 2));

    // Tallennetaan pelaaja id, pelinappulan id ja koordinaatit raahaukselle
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << static_cast<qint32>(playerid_) <<
                  static_cast<qint32>(pawnid_) <<
                  static_cast<qint32>(coordinates_.x) <<
                  static_cast<qint32>(coordinates_.y) <<
                  static_cast<qint32>(coordinates_.z);
    mime->setData("pawn", itemData);
    drag->setMimeData(mime);

    // Suoritetaan raahaus
    drag->exec();

    setBrush(original_color);
    setCursor(Qt::OpenHandCursor);
    delete drag;
}

void GPawn::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event)
    setCursor(Qt::OpenHandCursor);
}

}
