#include "g_actor.hh"

namespace Student
{

g_actor::g_actor(QObject* parent, QGraphicsItem *parent_item,
                          double* view_scale,
                          ACTOR_TYPES type, int id, std::string* moving_actor,
                          Common::CubeCoordinate coordinates,
                          std::shared_ptr<GameState> gamestate) :
    QObject(parent),
    parent_(parent_item),
    picture_(QPixmap()),
    type_(type),
    id_(id),
    moving_actor_(moving_actor),
    coordinates_(coordinates),
    gamestate_(gamestate),
    view_scale_(view_scale)
{
    // Asetetaan toimijalle oikea kuva
    picture_ = QPixmap(ACTOR_PIC_URLS.at(type));
    if (picture_.QPixmap::operator!())
    {
        //Missing actor pictures
        return;
    }

    this->setPixmap(picture_);
    // Asetetaan kuva kesekelle sen vanhempaa eli hexagonia
    // Kuvat ovat vakiokokoisia
    if(type == VORTEX)
    {
        // Pyörteen täytyy täyttää seitsämän ruutua
        this->setX(-200);
        this->setY(-200);
        // Pyörrettä pitää voidaa pyörittää keskeltä, melkein
        this->setTransformOriginPoint(190, 190);
    }
    else
    {
        this->setX(-100);
        this->setY(-100);
    }

    // Asetetaan alla oleva ruutu eteen, pitää muistaa muuttaa takaisin
    parent_->setZValue(1);
    setAcceptedMouseButtons(Qt::LeftButton);
}

void g_actor::set_parent_item(QGraphicsItem *parent)
{
    parent_= parent;
}

void g_actor::set_coordinates(Common::CubeCoordinate coordinates)
{
    coordinates_ = coordinates;
}

int g_actor::get_actor_type()
{
    return type_;
}

void g_actor::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if( gamestate_->currentGamePhase() == Common::GamePhase::SPINNING)
    {
        if(ACTOR_TYPE_MAP.count(*moving_actor_) != 0 &&
                ACTOR_TYPE_MAP.at(*moving_actor_) == type_)
        {
            setCursor(Qt::ClosedHandCursor);
            event->accept();
            return;
        }
    }
    event->ignore();
}

void g_actor::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
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

    // Asetetaan raahaukseen kuva hiiren alle
    drag->setPixmap(picture_.scaled(width, height));
    drag->setHotSpot(QPoint(width / 2, height / 2));

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << static_cast<qint32>(id_) <<
                  static_cast<qint32>(coordinates_.x) <<
                  static_cast<qint32>(coordinates_.y) <<
                  static_cast<qint32>(coordinates_.z);
    mime->setData("actor", itemData);
    drag->setMimeData(mime);

    drag->exec();
    setCursor(Qt::OpenHandCursor);
    delete drag;
}

void g_actor::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    (void)event;
    setCursor(Qt::OpenHandCursor);
}

}
