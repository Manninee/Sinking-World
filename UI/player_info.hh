#ifndef PLAYER_INFO_HH
#define PLAYER_INFO_HH

#include <QLineEdit>
#include <QPushButton>
#include <QString>

/**
 * @file
 * @brief Määrittelee player_info-structin.
 */

namespace Student
{
/**
 * @brief Structi pelaajan nimikentän, värin ja muun tarpeellisen
 * tallentamiseen.
 */
struct player_info
{
    QLineEdit* name_text_field;
    QString name;
    QPushButton* delete_button;
    QString color_str;
    unsigned int player_id;
};
}

#endif // PLAYER_INFO_HH
