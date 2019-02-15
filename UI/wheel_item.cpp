#include "wheel_item.hh"

namespace Student
{

wheel_item::wheel_item(int type, std::shared_ptr<Common::IGameRunner> runner,
                       std::shared_ptr<Student::GameState> gamestate):
    type_(type),
    picture_(QPixmap()),
    view_(nullptr),
    runner_(runner),
    gamestate_(gamestate)
{
    picture_ = QPixmap(WHEEL_PIC_URLS.at(type));
    // Tarkastetaan kuvien oikeellisuus
    if (picture_.QPixmap::operator!())
    {
        //Missing picture files
        return;
    }

    this->setPixmap(picture_);
    this->setTransformOriginPoint(WHEEL_PIC_HEIGHT/2,WHEEL_PIC_HEIGHT/2);
}

void wheel_item::set_view(QGraphicsView* view)
{
    view_ = view;
}


void wheel_item::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // kiekon varjoa ei pyöritetä
    if (type_ == 1)
    {
        event->ignore();
        return;
    }

    // Pyöritetään vain, jos gamephase on oikeassa asennosssa
    if (gamestate_->currentGamePhase() != Common::GamePhase::SPINNING
            || gamestate_ == nullptr)
    {
        event->ignore();
        return;
    }

    event->accept();
    std::pair<std::string,std::string> results = runner_->spinWheel();

    if (results.second == "1")
    {
        this->rotate(ONE);
    }
    else if (results.second == "2")
    {
        this->rotate(TWO);
    }
    else if (results.second == "3")
    {
        this->rotate(THREE);
    }
    else if (results.second == "D")
    {
        this->rotate(D);
    }
    emit wheel_spinned(results);
}

void wheel_item::rotate(WHEEL_STATES state)
{
    this->setEnabled(false);
    emit spinning(true);
    this->setRotation(this->rotation() - 2 * 360);
    while(state != static_cast<int>(this->rotation()))
    {
        // Odotetaan yksi millisekuntti
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        // Käännetään kiekkoa yksi aste
        int current_rotation = static_cast<int>(this->rotation());

        current_rotation++;
        this->setRotation(current_rotation);
        view_->viewport()->repaint();
    }
    emit spinning(false);
}
}
