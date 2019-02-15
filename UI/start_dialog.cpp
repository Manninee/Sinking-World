#include "start_dialog.hh"
#include <QDebug>

namespace Student
{
using player_vect = std::vector<std::shared_ptr<Student::Player>>*;

Dialog::Dialog(player_vect player_pointer_vector):
    player_pointer_vector_(player_pointer_vector),
    player_count_(0),
    middle_layout_(new QGridLayout),
    player_layout_ptr_(new QGridLayout),
    deletebutton_layout_ptr_(new QGridLayout),
    playerinfo_vector_({}),
    add_player_button_ptr_(nullptr),
    ok_button_ptr_(nullptr)
{
    // Laitetaan ikkunan ulkoasu kuntoon
    this->setWindowTitle("Aloitusdialogi");
    this->setFixedHeight(DIALOG_HEIGHT);
    this->setFixedWidth(DIALOG_WIDTH);
    this->setWindowFlag(Qt::WindowContextHelpButtonHint, false);

    setup_layout();

    connect(ok_button_ptr_, &QPushButton::clicked, this, &Dialog::accept);
    connect(add_player_button_ptr_, &QPushButton::clicked,
                    this, &Dialog::add_player);

}

void Dialog::accept()
{
    // Peliä ei voi aloittaa ilman pelaajia.
    if ( player_count_ < 2 )
    {
        QMessageBox  msg_box(this);
        msg_box.setText("Game without players isn't funny.");
        msg_box.setWindowFlag(Qt::Sheet);
        msg_box.setWindowTitle(" ");  // Tähän ei haluta otsikkoa
        msg_box.QMessageBox::exec();
        return;
    }

    // Toteutetaan dialogin purkua täällä
    using shrd_player = std::shared_ptr<Student::Player>;

    for ( unsigned int i = 0 ; i < player_count_; ++i)
    {
        // Siirretään käyttäjän valitsemat nimitiedot structin oikeille kentille
        playerinfo_vector_.at(i).name =
                playerinfo_vector_.at(i).name_text_field->text();
        playerinfo_vector_.at(i).player_id = i;

        delete playerinfo_vector_.at(i).delete_button;
        delete playerinfo_vector_.at(i).name_text_field;

        // Muodostetaan pelaajaolio, ja tallennetaan se
        player_pointer_vector_->
                push_back(shrd_player(new Player(playerinfo_vector_.at(i))));
    }

    delete player_layout_ptr_;
    delete deletebutton_layout_ptr_;

    QDialog::done(Accepted);
}

void Dialog::add_player()
{
     // Pelaajanimikenttiä ei saa luoda enempää kuin mahdollisia värejä on
    if ( player_count_ == PLAYER_COLOURS.size() )
    {
        return;
    }

    // valitaan pelaajalla varaamaton väri
    QString color = get_color();

    // Luodaan pelaajatietostructi, joissa pelaajan nimi ja väri määritellään.
    player_info player;
    player.name_text_field = new QLineEdit;
    player.name_text_field->setStyleSheet(QString("color: black;")
                               + QString("background-color: ")
                               +color);
    player.color_str = color;
    player.delete_button = new QPushButton;
    player.delete_button->setText("X");
    // Mahdollistetaan pelaajan poistaminen
    connect(player.delete_button, &QPushButton::clicked,this,
            [this, player](){Dialog::remove_player(player.color_str);});

    // Lisätään tiedot niille tarkoitettuun vektoriin
    playerinfo_vector_.push_back(player);

    // Naulataan widgetit seinälle
    player_layout_ptr_->addWidget(playerinfo_vector_
                                  .at(player_count_).name_text_field);
    deletebutton_layout_ptr_->addWidget(playerinfo_vector_
                                  .at(player_count_).delete_button);

    // Asetetaan uusi pelaajanimikenttä aktiiviseksi
    playerinfo_vector_.at(player_count_).name_text_field->setFocus();

    // Kasvatetaan pelaajamäärälaskuria yhdellä
    ++player_count_;


}

void Dialog::remove_player(QString color)
{
    // Käydään pelaajia läpi indeksin i kanssa
    unsigned int i = 0;
    for (;;)
    {
        // Oikean pelaajan tunnistamisessa käytetään pelaajan väriä
        if (color == playerinfo_vector_.at(i).color_str)
        {
            // Poistetaan pelaajaan liittyvät tiedot ja napit
            delete playerinfo_vector_.at(i).delete_button;
            delete playerinfo_vector_.at(i).name_text_field;
            playerinfo_vector_.erase(playerinfo_vector_.begin()
                                     + static_cast<int>(i));

            // Vähennetään pelaajalaskuria yhdellä
            --player_count_;
            return;
        }
        ++i;
    }
}

void Dialog::setup_layout()
{
    // Luodaan isoin layout
    QGridLayout* big_layout = new QGridLayout(this);

    QLabel* info_label = new QLabel(this);
    info_label->setText("Add players and name them");

    // Luodaan OK-nappula
    ok_button_ptr_ = new QPushButton(this);
    ok_button_ptr_->setText("OK");

    // Luodaan lisää pelaaja -nappula
    add_player_button_ptr_ = new QPushButton(this);
    add_player_button_ptr_->setText("ADD PLAYER");

    big_layout->addWidget(info_label);

    big_layout->addLayout(middle_layout_, 1,0);

    // Lisätään pelaajatietolayout keski-layouttiin
    middle_layout_->addLayout(player_layout_ptr_, 0, 0);

    // Lisätään pelaajatiedonpoistopainikelayout keski-layouttiin
    middle_layout_->addLayout(deletebutton_layout_ptr_, 0, 1);

    big_layout->addWidget(add_player_button_ptr_);
    big_layout->addWidget(ok_button_ptr_);

    // Asetetaan isoin layout dialogi-ikkunaan
    setLayout(big_layout);
}

QString Dialog::get_color()
{
    // jos yhtään pelaajaa ei vielä ole, voidaan ottaa listan ensimmäinen väri
    if ( playerinfo_vector_.empty())
    {
        return PLAYER_COLOURS.at(0);
    }

    bool unused_color = false;
    for (unsigned int i = 0 ; i < PLAYER_COLOURS.size() ; ++i)
    {
        unused_color = true;
        for (unsigned int i2 = 0 ; i2 < playerinfo_vector_.size() ; ++i2)
        {
            if (playerinfo_vector_.at(i2).color_str.
                    compare(PLAYER_COLOURS.at(i)) == 0)
            {
                unused_color = false;
            }
        }
        if ( unused_color )
        {
            return PLAYER_COLOURS.at(i);
        }
    }
    // Palautetaan varmuuden vuoksi tyhjä QString, joka ei kaada ohjelmaa
    return QString();
}
}
