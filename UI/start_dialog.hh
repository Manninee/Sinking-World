#ifndef START_DIALOG_HH
#define START_DIALOG_HH

#include "constants.hh"
#include "player_info.hh"
#include "player.hh"

#include <QFrame>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QString>
#include <QLabel>
#include <QMessageBox>
#include <vector>

/**
 * @file
 * @brief Määrittelee Dialog-luokan.
 */

namespace Student
{

/**
 * @brief Toteuttaa aloitusdialogi-ikkunan.
 */
class Dialog : public QDialog
{
    Q_OBJECT
    using player_vector = std::vector<std::shared_ptr<Student::Player>>*;
public:
    /**
     * @brief Rakentaja.
     * @param player_pointer_vectoriin tallennetaan lopuksi uudet pelaajat.
     */
    explicit Dialog(player_vector player_pointer_vector);

    /**
     * @brief Tyhjä purkaja.
     */
    ~Dialog() = default;

public slots:

    /**
     * @brief Käytännössä aloitusdialogi-ikkuna puretaan täällä. Slottia
     * kutsutaan kun OK-painiketta tai enter-näppäintä on painettu.
     * Dynaamisesti varattu muisti vapautetaan.
     * Tarkistaa, että pelaajia on ainakin kaksi.
     */
    void accept() override;

    /**
     * @brief Lisää uuden pelaajanimikentän aloitusdialogiin.
     */
    void add_player();

    /**
     * @brief Poistaa valitun pelaajanimikentän aloitusdialogista
     * @param color: poistettavan pelaajan väri, jolla pelaaja tunnistetaan.
     */
    void remove_player(QString color);

private:

    /**
     * @brief Tekee aloitusdialogin graafisen alustuksen.
     */
    void setup_layout();

    /**
     * @brief Palauttaa varaamattoman värin pelaajan käytettäväksi.
     * @return QT:n ymmärtämä värin nimi
     */
    QString get_color();

    /**
     * @brief Osoitin pelaajaolioiden tallentamiseen aloitusdialogin
     * sulkeutuessa.
     */
    std::vector<std::shared_ptr<Student::Player>>* player_pointer_vector_;

    /**
     * @brief Aloitusdialogissa näkyvien pelaajanimikenttien määrä.
     */
    unsigned int player_count_;

    /**
     * @brief On tallennettu atribuutiksi, jotta voidaan hävittää hallitusti.
     */
    QGridLayout* middle_layout_;

    /**
     * @brief Määrittelee pelaajanimikenttien sijainnit.
     */
    QGridLayout* player_layout_ptr_;

    /**
     * @brief Määrittelee pelaajanimikenttien viereen sijoitettujen
     * pelaajanimikenttien poistonappuloiden sijainnit.
     */
    QGridLayout* deletebutton_layout_ptr_;

    /**
     * @brief Sisältää tiedot pelaajista.
     */
    std::vector<player_info> playerinfo_vector_;

    /**
     * @brief Painike pelaajan lisäämiseen.
     */
    QPushButton* add_player_button_ptr_;

    /**
     * @brief OK-painike
     */
    QPushButton* ok_button_ptr_;

};
}

#endif // START_DIALOG_HH
