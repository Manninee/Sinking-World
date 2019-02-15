#include "gtransport.hh"

namespace Student {

GTransport::GTransport(int transport_id, std::string type,
                       std::shared_ptr<Student::GameState> gamestate,
                       std::string* moving_actor, double* view_scale,
                       Common::CubeCoordinate coordinates,
                       QGraphicsItem *parent):
    QGraphicsPolygonItem(QPolygonF( SHAPES.at(type)), parent),
    coordinates_(coordinates),
    gamestate_(gamestate),
    transport_id_(transport_id),
    type_(type),
    moving_actor_(moving_actor),
    view_scale_(view_scale)
{
    // Vaihdetaan transportin väri sen tyypin perusteella
    setPen( QPen(Qt::black) );
    if(type == "boat")
    {
        setBrush( QColor("brown") );
    }
    else if(type == "dolphin")
    {
        setBrush( QColor("gray") );
    }

    setAcceptedMouseButtons(Qt::LeftButton);
    setAcceptDrops(true);
}

void GTransport::set_coordinate(Common::CubeCoordinate new_coord)
{
    coordinates_ = new_coord;
}

int GTransport::get_transport_id() const
{
    return transport_id_;
}

void GTransport::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Jos pelin vaihe on siirto tai pelin tilanne on pyöritys ja liikuteltava
    // olio on oikea hyväksytään painallus
    if(gamestate_->currentGamePhase() == Common::GamePhase::MOVEMENT || (
        gamestate_->currentGamePhase() == Common::GamePhase::SPINNING &&
        *moving_actor_ == type_))
    {
        setCursor(Qt::ClosedHandCursor);
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void GTransport::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
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
    if(type_ == "boat")
    {
        paint->translate(width / 2, 0);
    }
    else
    {
        paint->translate(width / 2, height / 2);
    }
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

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << static_cast<qint32>(transport_id_) <<
                  static_cast<qint32>(coordinates_.x) <<
                  static_cast<qint32>(coordinates_.y) <<
                  static_cast<qint32>(coordinates_.z);
    mime->setData("transport", itemData);
    drag->setMimeData(mime);

    drag->exec();

    // Vaihtaa transportin värin takaisin ja vaihtaa kursorin
    setBrush(original_color);
    setCursor(Qt::OpenHandCursor);
    delete drag;
}

void GTransport::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event)
    setCursor(Qt::OpenHandCursor);
}

void GTransport::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    event->accept();
}

void GTransport::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    // Jos tiputettava olio on pelaajan nappula
    if (event->mimeData()->hasFormat("pawn") &&
            gamestate_->currentGamePhase() != Common::GamePhase::SPINNING)
    {
        const QMimeData *mime = event->mimeData();
        QByteArray itemData = mime->data("pawn");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        // Otetaan talteen pelinappulan pelaajan id, pelinappulan id ja nappulan
        // koordinaatit
        qint32 player_id = 0, pawn_id = 0, x = 0, y = 0, z = 0;
        dataStream >> player_id >> pawn_id >> x >> y >> z;

        // Jos Pelin tilanne on liikuttelu ja oikean pelaaja nappulaa siirretään
        if(gamestate_->currentPlayer() == player_id)
        {
            // Lähetetään pelinappulan id, lähtökoordinaatit ja loppukoordinaatit
            emit pawn_dropped(pawn_id, transport_id_,
                              Common::CubeCoordinate(x, y, z), coordinates_);
            event->accept();
            return;
        }
    }
    event->ignore();
}

}
