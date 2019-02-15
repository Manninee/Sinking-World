#include "player.hh"

#include <QDebug>

namespace Student
{

Player::Player(player_info player):
    color_(player.color_str),
    player_id_(player.player_id),
    name_(player.name),
    actions_left_(AMOUNT_OF_ACTIONS),
    points_(0),
    pawn_vctr_({})
{}

int Player::getPlayerId() const
{
    return static_cast<int>(player_id_);
}

void Player::setActionsLeft(unsigned int actionsLeft)
{
    actions_left_ = actionsLeft;
}

unsigned int Player::getActionsLeft() const
{
    return actions_left_;
}

void Player::add_pawn(std::shared_ptr<Common::Pawn> pawn)
{

    std::vector<std::shared_ptr<Common::Pawn>>::iterator pawn_it =
            pawn_vctr_.begin();
    while(pawn_it != pawn_vctr_.end())
    {
        if(pawn_it->get()->getId() == pawn->getId())
        {
            pawn_vctr_.erase(pawn_it);
            break;
        }
        ++pawn_it;
    }
    pawn_vctr_.push_back(pawn);
}

std::shared_ptr<Common::Pawn> Player::get_pawn(int pawnId) const
{
    for(auto pawns: pawn_vctr_)
    {
        if(pawns->getId() == pawnId)
        {
            return pawns;
        }
    }

    return nullptr;
}

QString Player::get_color() const
{
    return color_;
}

std::vector<std::shared_ptr<Common::Pawn>>* Player::get_pawn_vector()
{
    return &pawn_vctr_;
}

QString Player::get_name()
{
    return name_;
}

void Player::remove_pawn(int pawn_id)
{
    std::vector<std::shared_ptr<Common::Pawn>>::iterator pawn_it = pawn_vctr_.begin();
    while(pawn_it != pawn_vctr_.end())
    {
        if(pawn_it->get()->getId() == pawn_id)
        {
            pawn_vctr_.erase(pawn_it);
            return;
        }
        ++pawn_it;
    }
}

void Player::add_point()
{
    ++points_;
}

int Player::get_points() const
{
    return points_;
}

}
