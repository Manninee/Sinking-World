#include "main_window.hh"

namespace Student
{

Main_window::Main_window(QApplication* application,
                         std::shared_ptr<Student::GameBoard> gameboard,
                         std::shared_ptr<Student::GameState> gamestate,
                         std::vector<std::shared_ptr<Student::Player>> players,
                         std::shared_ptr<Common::IGameRunner> runner,
                         QWidget *parent):
    QMainWindow(parent),
    base_widget_(new QWidget),
    board_(new QGraphicsScene),
    view_(new QGraphicsView),
    spin_scene_(new QGraphicsScene),
    spin_view_(new QGraphicsView),
    button_map_({}),
    label_map_({}),
    hexagon_map_({}),
    gpawn_map_({}),
    gtransport_map_({}),
    g_actor_map_({}),
    gameboard_(gameboard),
    gamestate_(gamestate),
    player_vector_(players),
    runner_(runner),
    wheel_(new wheel_item(0, runner_, gamestate_)),
    shadow_(new wheel_item(1, runner_)),
    actor_move_info_("",""),
    view_scale_(0.8)
{
    setWindowTitle("Saaripeli");
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);

    // Haetaan päänäyton koko
    QSize max_size = application->primaryScreen()->availableSize();
    int width = max_size.width();
    int height = max_size.height();

    // Lasketaan uusi koko ikkunalle
    width *= MAIN_WINDOW_SCALE;
    height *= MAIN_WINDOW_SCALE;
    QSize newSize( width, height );

    // Asetetaan ikkunalle uusi koko keskitettynä
    setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            newSize,
            application->primaryScreen()->availableGeometry()
        )
    );

    setup_layout();
    setup_board();
    setup_signals();
    setup_pieces();

    for(auto boat: *gameboard_->get_transport_map())
    {
        create_transport(boat.second, boat.second->getHex()->getCoordinates());
    }

    // Asetetaan ruutujen kursori ilmoittamaan että niitä ei voi kääntää
    set_item_cursors(HEX, false);

    // Aseteetaan pelaajatietokentän teksti ensimmäisen kerran
    for( auto player: player_vector_)
    {
        if(player.get()->getPlayerId() == gamestate_->currentPlayer())
        {
            Student::Player* current_player = player.get();

            // Haetaan pelaajan väri
            QString color = current_player->get_color();

            // Asetetaan pelaajatietokentän pohjaväriksi pelaajan väri
            QLabel* label = label_map_.at("player");
            label->setStyleSheet("color:black; font:13pt; background-color:" +
                                 color);

            // Muodostetaan pelaajatietokentän teksi ja asetetaan se kenttään
            QString text = "Pelaaja: #" +
                    QString::number(current_player->getPlayerId()) + " " +
                    current_player->get_name();
            label->setText(text);
            break;
        }
    }

    // Asetetaan vaihetietokentän teksti
    phase_changed();
}

Main_window::~Main_window()
{
    // Dynaamisesti varatun muistin vapautukset kommentoitava pois, jotta ohjelma ei kaatuisi linuxilla.
    // Poistetaan käyttöliittymän painikkeet
    for(auto button: button_map_)
    {
        delete button.second;
        button_map_.erase(button.first);
    }

    // Poistetaan käyttöliittymän tekstikentät
    for(auto label: label_map_)
    {
        delete label.second;
        button_map_.erase(label.first);
    }

    // Poistetaan graaffiset nappulat
    for(auto gpawn: gpawn_map_)
    {
        delete gpawn.second;
        gpawn_map_.erase(gpawn.first);
    }

    // Poistetaan graaffiset actorit
    for(auto g_actor: g_actor_map_)
    {
        delete g_actor.second;
        g_actor_map_.erase(g_actor.first);
    }

    // Poistetaan graafiset transportit
    for(auto gtransport: gtransport_map_)
    {
        delete gtransport.second;
        gtransport_map_.erase(gtransport.first);
    }

    // Poistetaan graafiset pelilaudan ruudut
    for(auto hexagon: hexagon_map_)
    {
        delete hexagon.second;
        hexagon_map_.erase(hexagon.first);
    }

    delete view_;
    delete board_;
    delete wheel_;
    delete shadow_;
    delete spin_view_;
    delete spin_scene_;
    delete base_widget_;
}

void Main_window::zoom_board()
{
    view_->scale(1.3,1.3);
    view_scale_ *= 1.3;
}

void Main_window::unzoom_board()
{
    view_->scale(1/1.3,1/1.3);
    view_scale_ *= (1/1.3);
}

void Main_window::move_pawn(int pawn_id, Common::CubeCoordinate origin,
                               Common::CubeCoordinate target)
{
    std::shared_ptr<Common::Pawn> pawn = gameboard_->get_pawn(pawn_id);
    Student::GPawn* gpawn = gpawn_map_.at(pawn_id);

    // Jos nappulaa siirretään ruudun sisällä
    if(origin.x == target.x && origin.y == target.y && origin.z == target.z)
    {
        for(auto transport: gameboard_->getHex(target)->getTransports())
        {
            // Siirretään nappula transportista ruutuun
            if(transport->isPawnInTransport(pawn))
            {
                transport->removePawn(pawn);
                gpawn->setPos(0,0);
                gpawn->setRotation(0);
                gpawn->setParentItem(hexagon_map_.at(target));
                sort_gpawns(target);
                return;
            }
        }
    }
    try
    {
        // Liikutetaan nappulaa
        int moves = runner_->movePawn(origin, target, pawn_id);

        // Liikutetaan graaffista pelinappulaa
        gpawn->setParentItem(hexagon_map_.at(target));
        gpawn->setPos(0,0);
        gpawn->setRotation(0);
        gpawn->set_coordinate(target);

        // Poistetaan nappula transportin kyydistä
        for(auto transport: gameboard_->getHex(origin)->getTransports())
        {
            transport->removePawn(pawn);
        }

        // Nappula tiputetaan koraaliin
        if(gameboard_->getHex(target)->getPieceType() == "Coral")
        {
            // Lisätään pelaajalle piste
            int player_id = gameboard_->get_pawn(pawn_id)->getPlayerId();
            for(std::shared_ptr<Student::Player> player: player_vector_)
            {
                if(player->getPlayerId() == player_id)
                {
                    player->add_point();
                    break;
                }
            }
            // Poistetaan pelaajan nappula
            gameboard_->removePawn(pawn_id);
        }

        // Actor tekee hommansa myös nappulan siirron jälkeen
        for(auto actor: gameboard_->getHex(target)->getActors())
        {
            gameboard_->use_actor(actor->getId());
        }

        // Järjestetään lähtö- ja tuloruudun nappulat
        sort_gpawns(target);
        sort_gpawns(origin);

        // Jos liikkeitä ei ole lopetetaan vaihe
        if( moves == 0)
        {
            next_phase();
        }
        else
        {
            // Asetetaan infotietokenttä ilmoittamaan jäljellä olevat liikkeet
            label_map_.at("info")->setText("Liikkeitä: " +
                                           QString::number(moves));
        }
    }
    catch (Common::IllegalMoveException &e)
    {
        (void)e;
        // Liike ei ollut sallittu
    }
}

void Main_window::move_actor(int actor_id, Common::CubeCoordinate origin,
                             Common::CubeCoordinate target)
{
    try
    {
        // Kiekkoa pitää pyörittää
        if(actor_move_info_.first == "" || actor_move_info_.second == "")
        {
            throw Common::IllegalMoveException("Ei viela pyoritety");
        }

        Student::g_actor* actor = g_actor_map_.at(actor_id);

        // Muutetaan tieto olion tyypistä enumin mukaiseksi kokonaisluvuks
        int actor_type = -1;
        try
        {
            actor_type = ACTOR_TYPE_MAP.at(actor_move_info_.first);
        }
        catch (std::out_of_range &e)
        {
            (void)e;
            // Taas varmaan kirjoitusvirhe kurssin puolen koodissa
        }

        if (actor->get_actor_type() != actor_type)
        {
            throw Common::IllegalMoveException("vaaran tyyppinen actori");
        }
        runner_->moveActor(origin, target, actor_id, actor_move_info_.second);

        actor->set_parent_item(hexagon_map_.at(target));
        actor->setParentItem(hexagon_map_.at(target));
        actor->set_coordinates(target);
        next_phase();

        // Säädetään hexgonien z-suunnat
        hexagon_map_.at(origin)->setZValue(0);
        hexagon_map_.at(target)->setZValue(1);
    }
    catch (Common::IllegalMoveException &e)
    {
        (void)e;
        // Ei liikkeita toimijalla
    }
}


void Main_window::flip_tile(Common::CubeCoordinate hexagon_coord)
{
    try
    {
        // Käännetään ruutu ja otetaan talteen uusi olio
        std::string new_actor_string = runner_->flipTile(hexagon_coord);
        hexagon_map_.at(hexagon_coord)->
                setBrush(Student::HEX_COLORS.at("Water"));
        next_phase();
    }
    catch (Common::IllegalMoveException &e)
    {
        (void)e;
        // Ei sallittu
    }
}

void Main_window::wheel_state(bool state)
{
    setEnabled(!state);
    view_->setEnabled(!state);
}

void Main_window::new_g_actor(int type, int id,
                            Common::CubeCoordinate coordinates)
{
    // Osoitin uuteen actoriin
    g_actor* new_g_actor = new g_actor(this,
                                       hexagon_map_.at(coordinates),
                                       &view_scale_,
                                       static_cast<ACTOR_TYPES>(type),
                                       id,
                                       &actor_move_info_.first, coordinates,
                                       gamestate_);
    new_g_actor->setParentItem(hexagon_map_.at(coordinates));
    g_actor_map_.insert({id, new_g_actor});
}

void Main_window::remove_g_actor(int id)
{
    if(g_actor_map_.count(id) != 0)
    {
        // Kun actor poistetaan sen alapuolisen ruudun z arvo oltava 0
        g_actor_map_.at(id)->parentItem()->setZValue(0);
        delete g_actor_map_.at(id);
        g_actor_map_.erase(id);
    }
}

void Main_window::spin_vortex(int id)
{
    this->wheel_state(true);
    for(int d = 500; d > 0;d--)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        g_actor_map_.at(id)->setRotation(d);
        view_->viewport()->repaint();
    }
    this->wheel_state(false);
}

void Main_window::set_actor_move_info(std::pair<std::string,
                                      std::string> actor_move_info)
{
    // Otetaan hyrrän tulos talteen
    actor_move_info_ = actor_move_info;

    if(actor_move_info_.first != "")
    {
        bool found = false;
        if(actor_move_info_.first == "dolphin")
        {
            // Etsitään onko liikuteltavia delffiinejä pelilaudalla
            for(auto transport: *gameboard_->get_transport_map())
            {
                if(transport.second->getTransportType() == "dolphin")
                {
                    found = true;
                    // Asetetaan kursori näyttämään että delffiinejä voi siirtää
                    set_item_cursors(TRANSPORT, true, {"dolphin"});
                    break;
                }
            }
        }
        else
        {
            // Etsitään onko liikuteltavia actoreita pelilaudalla
            for(auto actor: *gameboard_->get_actor_map())
            {
                if(actor.second->getActorType() == actor_move_info_.first)
                {
                    found = true;
                    // Asetetaan kursori ilmoittamaan mitä actoreita voi siirtää
                    set_item_cursors(ACTOR, true, {actor_move_info.first});
                    break;
                }
            }
        }

        // Jos liikuteltavia actoreita tai transportteja ei ole jatketaan
        // seuraavaan vaiheeseen automaattisesti
        if(!found)
        {
            next_phase();
            return;
        }

        // Vaihdetaan actortietokenttä ilmoittamaan hyrrän tulokset
        QString actor_text = "Toimija: " +
                QString::fromStdString(actor_move_info.first) + "\nLiikkeitä: "+
                QString::fromStdString(actor_move_info.second);
        label_map_.at("info")->setText(actor_text);
    }
}

void Main_window::create_transport(std::shared_ptr<Common::Transport> transport,
                                   Common::CubeCoordinate coord)
{
    int id = transport->getId();
    Student::GTransport* gtransport = new Student::GTransport(id,
                                    transport->getTransportType(), gamestate_,
                                    &actor_move_info_.first, &view_scale_,
                                    coord, hexagon_map_.at(coord));
    gtransport_map_.insert({id, gtransport});
    gtransport->setPos(0, 20);
    connect(gtransport, &GTransport::pawn_dropped, this,
            &Main_window::pawn_into_transport);
}

void Main_window::player_changed()
{
    // Etsitään vurossa oleva pelaaja
    Student::Player* current_player = nullptr;
    for( auto player: player_vector_)
    {
        if(player.get()->getPlayerId() == gamestate_->currentPlayer())
        {
            current_player = player.get();
            break;
        }
    }

    // Jos pelaajaa ei löydy
    if(current_player == nullptr)
    {
        return;
    }

    // Haetaan pelaajan väri
    QString color = current_player->get_color();

    // Haetaan pelaajatietokenttä ja asetetaan sille pohjaväriksi pelaajan väri
    QLabel* label = label_map_.at("player");
    label->setStyleSheet("color:black; font:13pt; background-color:" + color);

    // Muodostetaan pelaajatietokentän teksi ja asetetaan se kenttään
    QString text = "Pelaaja: #" + QString::number(current_player->getPlayerId() + 1)
            + " " + current_player->get_name();
    label->setText(text);

    // Luodaan ponnahdusikkuna ilmoittamaan vuoron vaiihtumisesta
    QMessageBox  msg_box(this);

    // Muodostetaan teksi ikkunaan ja asetetaan se
    QString box_text = "Pelaajan numero: " +
            QString::number(gamestate_->currentPlayer()+ 1) +
            "\nPelaajan nimi: " + current_player->get_name();
    msg_box.setText(box_text);
    msg_box.setWindowFlag(Qt::Sheet);
    msg_box.setWindowTitle("Seuraava pelaaja");

    // Asetetaan ikkunalle pohjaväriksi pelaajan väri
    msg_box.setStyleSheet("color:black; font:15pt; background-color:" + color);
    msg_box.QMessageBox::exec();
}

void Main_window::phase_changed()
{
    // Muokataan vaihetietokentän teksti vaiheen mukaan
    QString text = "";
    switch (gamestate_->currentGamePhase())
    {
    case Common::GamePhase::MOVEMENT:
        text += "Liikkumis";

        // Asetetaan kursorit ilmoittamaan mitä oliota voi siirtää
        set_item_cursors(ACTOR, false);
        set_item_cursors(PAWN, true);
        set_item_cursors(TRANSPORT, true, {"boat", "dolphin"});

        // Asetetaan infotietokenttä ilmoittamaan maksimi liikkeet
        label_map_.at("info")->setText("Liikkeitä: " +
            QString::number(runner_->getCurrentPlayer()->getActionsLeft()));

        // Nollataan actorien liikkeet
        set_actor_move_info({"",""});
        break;

    case Common::GamePhase::SINKING:
        text += "Upotus";

        label_map_.at("info")->setText("");

        // Asetetaan kursorit ilmoittamaan mitä voi käätää
        set_item_cursors(PAWN, false);
        set_item_cursors(TRANSPORT, false);
        set_item_cursors(HEX, true);
        break;

    case Common::GamePhase::SPINNING:
        set_item_cursors(HEX, false);

        // Asetetaan tinfotietokenttä "nollaan"
        label_map_.at("info")->setText("Toimija:\nLiikkeitä:");
        text += "Pyöritys";
        break;
    }
    text += "vaihe";
    label_map_.at("phase")->setText(text);
}

void Main_window::move_transport(int transport_id,
                                 Common::CubeCoordinate origin,
                                 Common::CubeCoordinate target)
{
    // Estetään monen transportin oleminen samassa ruudussa
    if(gameboard_->getHex(target)->getTransports().size() != 0)
    {
        return;
    }

    // Estetään siirto jos siirron jälkeen ruudussa
    // olisi enemmän kuin 3 nappulaa
    if(gameboard_->get_transport(transport_id)->getPawnsInTransport().size() >
    static_cast<unsigned int>(3 - gameboard_->getHex(target)->getPawnAmount()))

    {
        return;
    }

    try
    {
        // Valitaan siirretäänkö liikkeillä vai hyrrällä
        int moves = 0;
        (void)moves; //hiljennetään varoitukset
        switch (gamestate_->currentGamePhase())
        {
        case Common::GamePhase::MOVEMENT:
            moves = runner_->moveTransport(origin, target, transport_id);
            break;
        case Common::GamePhase::SPINNING:

            // Kiekkoa pitää pyörittää ensiksi
            if(actor_move_info_.first == "" || actor_move_info_.second == "")
            {
                throw Common::IllegalMoveException("Ei viela pyoritety");
            }

            // Vain deffiiniä saa liikuttaa pyörittämisvaiheessa
            if(actor_move_info_.first == "dolphin")
            {
                moves = runner_->moveTransportWithSpinner(origin, target,
                                        transport_id, actor_move_info_.second);
                // Asetetaan liikkumistietokentät "nollaan"
                set_actor_move_info({"",""});
            }
            else
            {
                throw Common::IllegalMoveException("Venettä ei saa liikuttaa");
            }

            break;
        case Common::GamePhase::SINKING:
            return;
        }

        // Siirretään transport
        Student::GTransport* gtransport = gtransport_map_.at(transport_id);
        gtransport->setParentItem(hexagon_map_.at(target));
        gtransport->set_coordinate(target);

        // Lisätään graaffisille nappuloille uusi ruutu
        for(auto pawn: gameboard_->get_transport(transport_id)->getPawnsInTransport())
        {
            gpawn_map_.at(pawn->getId())->set_coordinate(target);
        }

        // Actor tekee hommansa myös trasportin siirron jälkeen
        for(auto actor: gameboard_->getHex(target)->getActors())
        {
            gameboard_->use_actor(actor->getId());
        }

        // Jos ei ole liikkeitä tai vaihe on pyöritys lopetetaan vuoro
        if(gamestate_->currentGamePhase() == Common::GamePhase::SPINNING ||
                moves == 0)
        {
            next_phase();
        }
        else
        {
            // Asetetaan infotietokenttä ilmoittamaan jäljellä olevat liikkeet
            label_map_.at("info")->setText("Liikkeitä: " +
                                           QString::number(moves));
        }
    }
    catch (Common::IllegalMoveException &e)
    {
        (void)e;
        // Ei sallittu liikkumista
    }
}

void Main_window::pawn_into_transport(int pawn_id, int transport_id,
                                      Common::CubeCoordinate origin,
                                      Common::CubeCoordinate target)
{
    // Tarkistetaan pitääkö nappula siirtää ennen veneeseen laittamista
    if(origin.x != target.x || origin.y != target.y || origin.z != target.z)
    {
        // Tarkistetaan onnistuuko siirto ja jos onnistuu niin siirretään
        int moves = runner_->checkPawnMovement(origin, target, pawn_id);
        if( moves != -1)
        {
            move_pawn(pawn_id, origin, target);
        }
        else
        {
            return;
        }
    }

    // Haetaan osoittimet nappulalle ja transportille
    std::shared_ptr<Common::Pawn> pawn = gameboard_->get_pawn(pawn_id);
    std::shared_ptr<Common::Transport> transport =
            gameboard_->get_transport(transport_id);

    // Tarkistetaan että on tilaa ja nappula ei ole jo transportissa
    if(transport->getCapacity() != 0 && !transport->isPawnInTransport(pawn))
    {
        //Lisätään nappula veneeseen
        transport->addPawn(pawn);
        Student::GPawn* gpawn = gpawn_map_.at(pawn_id);
        gpawn->setParentItem(gtransport_map_.at(transport_id));
        gpawn->setRotation(10);

        // Järjestetään nappulat
        sort_gpawns(target);
        sort_gpawns(origin);
    }
}

void Main_window::end_turn()
{
    if(gamestate_->currentGamePhase() != Common::GamePhase::SINKING)
    {
        next_phase();
    }
}

void Main_window::remove_gpawn(int pawn_id)
{
    // Tarkistetaan että nappula löytyy ja poistetaan se
    if(gpawn_map_.count(pawn_id) != 0)
    {
        // Seuraava rivi kommentoitava pois, jotta pelinappuloiden poisto toimisi linuxilla.
        delete gpawn_map_.at(pawn_id);

        // Jotta pelinappulan poisto toimisi linuxilla, on seuraava rivi otettava käyttöön.
        // gpawn_map_.at(pawn_id)->hide();
        gpawn_map_.erase(pawn_id);
    }
}

void Main_window::remove_gtransport(int transport_id,
                                    Common::CubeCoordinate coord)
{
    // Siirretään transportissa olleet nappulat veteen
    for(auto pawn: gameboard_->getHex(coord)->getPawns())
    {
        gpawn_map_.at(pawn->getId())->setParentItem(hexagon_map_.at(coord));
    }

    // Tarkistetaan että transport löytyy ja poistetaan se.
    if(gtransport_map_.count(transport_id) != 0)
    {
        delete gtransport_map_.at(transport_id);
        gtransport_map_.erase(transport_id);
    }

    sort_gpawns(coord);
}

void Main_window::setup_layout()
{
    QGridLayout* layout = new QGridLayout;
    QGridLayout* button_layout = new QGridLayout;

    // Luodaan zoomausnapit
    button_map_.insert(std::pair<std::string,QPushButton*>
                       ("zoom-", new QPushButton("Zoom-")));
    button_map_.insert(std::pair<std::string,QPushButton*>
                       ("zoom+", new QPushButton("Zoom+")));

    // Luodaan vuoronlopetusnappula
    button_map_.insert(std::pair<std::string,QPushButton*>
                       ("turn", new QPushButton("Lopeta vuoro")));

    // Luodaan tietokentät pelaajan tiedoille ja vaiheen tiedoile
    label_map_.insert(std::pair<std::string, QLabel*> ("player", new QLabel()));
    label_map_.insert(std::pair<std::string, QLabel*> ("phase", new QLabel()));
    label_map_.insert(std::pair<std::string, QLabel*> ("info", new QLabel()));

    // Lisätään kiekko käyttöliittymään 
    wheel_->set_view(spin_view_);
    spin_scene_->addItem(wheel_);
    spin_scene_->addItem(shadow_);
    spin_view_->setScene(spin_scene_);
    spin_view_->setFixedWidth(WHEEL_PIC_HEIGHT);
    spin_view_->setFixedHeight(WHEEL_PIC_HEIGHT);
    spin_view_->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    spin_view_->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    button_layout->addWidget(spin_view_, 0, 0, 1, 2);


    // Lisätään tietokentät oikeaan reunaan
    button_layout->addWidget(label_map_.at("player"), 1, 0, 1, 2);
    button_layout->addWidget(label_map_.at("phase"), 2, 0, 1, 2);
    button_layout->addWidget(label_map_.at("info"), 3, 0, 1, 2);

    // Asetetaan tietokenttien korkeudet vakioiksi
    label_map_.at("player")->setFixedHeight(LABEL_HEIGHT);
    label_map_.at("phase")->setFixedHeight(LABEL_HEIGHT);
    label_map_.at("info")->setFixedHeight(LABEL_HEIGHT * 2);
    label_map_.at("player")->setStyleSheet("color: black; font: 12pt;");
    label_map_.at("phase")->setStyleSheet("color: black; font: 12pt;");
    label_map_.at("info")->setStyleSheet("color: black; font: 12pt;");

    // Lisätään vaihee lopettamisnappula käyttöliittymään
    button_layout->addWidget(button_map_.at("turn"), 4, 0, 1, 2);

    // Lisätään zoomausnapit käyttöliittymään
    button_layout->addWidget(button_map_.at("zoom-"), 5, 1);
    button_layout->addWidget(button_map_.at("zoom+"), 5, 0);

    // Luodaan välikappale jolla siirretään muut elementit yläreunaan
    QSpacerItem* spacer = new QSpacerItem(2, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);
    button_layout->addItem(spacer, 7, 0);

    view_->setScene(board_);
    layout->addWidget(view_, 0, 0);
    layout->addLayout(button_layout, 0, 1);

    base_widget_->setLayout(layout);
    this->setCentralWidget(base_widget_);
}

void Main_window::setup_signals()
{
    // Yhdistetään signaalit ruudun zoomausta varten
    connect(button_map_.at("zoom+"), &QPushButton::clicked, this,
            &Main_window::zoom_board);
    connect(button_map_.at("zoom-"), &QPushButton::clicked, this,
            &Main_window::unzoom_board);

    // Yhdisteään signaali vaiheen vaihtamista varten
    connect(button_map_.at("turn"), &QPushButton::clicked, this,
            &Main_window::end_turn);

    // Yhdistetään signaalit hyrrän toimintaa varten
    connect(wheel_, &wheel_item::spinning, this, &Main_window::wheel_state);

    // Yhdistettään gameboardind signaalit
    connect(gameboard_.get(), &GameBoard::transport_created,
            this, &Main_window::create_transport);
    connect(gameboard_.get(), &GameBoard::remove_transport,
            this, &Main_window::remove_gtransport);
    connect(gameboard_.get(), &GameBoard::remove_pawn,
            this, &Main_window::remove_gpawn);
    connect(gameboard_.get(), &GameBoard::new_actor,
            this, &Main_window::new_g_actor);
    connect(gameboard_.get(), &GameBoard::remove_g_actor,
            this, &Main_window::remove_g_actor);
    connect(gameboard_.get(), &GameBoard::spin_vortex,
            this, &Main_window::spin_vortex);
    connect(wheel_, &wheel_item::wheel_spinned,
            this, &Main_window::set_actor_move_info);

    // Yhdistetään gamestaten signaalit
    connect(gamestate_.get(), &GameState::turn_changed,
            this, &Main_window::player_changed);
    connect(gamestate_.get(), &GameState::phase_changed,
            this, &Main_window::phase_changed);

    // Yhdistetään graaffisten ruutujen signaalit
    for( auto &pair: hexagon_map_)
    {
        connect(pair.second, &Hexagon::pawn_dropped,
                this, &Main_window::move_pawn);
        connect(pair.second, &Hexagon::actor_dropped,
                this, &Main_window::move_actor);
        connect(pair.second, &Hexagon::clicked,
                this, &Main_window::flip_tile);
        connect(pair.second, &Hexagon::transport_dropped, this,
                &Main_window::move_transport);
    }
}

void Main_window::setup_board()
{
    hexes_map_type* hexes_map = gameboard_->get_map();
    for(const auto &pair: *hexes_map)
    {
        // Luodaan uusi graaffinen ruutu ja tallennetaan se
        Student::Hexagon* hexagon = new Student::Hexagon(pair.first,
                                                         gamestate_);
        hexagon_map_.insert({pair.first, hexagon});

        // Lasketaan ruutujen x,y -koordinaatit
        int x_coordinate = pair.first.x;
        int z_coordinate = pair.first.z;
        int x_move = x_coordinate * HEXAGON_X * 2 + z_coordinate * HEXAGON_X;
        int y_move = z_coordinate * HEXAGON_Y * 3/2;

        // Lisätään ruutu laudalle ja liikutetaan se paikalleen
        board_->addItem(hexagon);
        hexagon->moveBy(x_move, y_move);

        // Värjätään ruudut niiden tyypin mukaan
        std::shared_ptr<Common::Hex> hex = pair.second;
        std::string type = hex->getPieceType();
        if(Student::HEX_COLORS.count(type) != 0)
        {
            hexagon->setBrush(Student::HEX_COLORS.at(type));
        }
    }

    view_->scale(MAIN_WINDOW_SCALE / 2, MAIN_WINDOW_SCALE / 2);
    view_scale_ /= 2;
}

void Main_window::setup_pieces()
{
    // Käydään kaikki pelaajat läpi
    std::map<Common::CubeCoordinate, Student::Hexagon*>::iterator hexagon_it;
    for( std::shared_ptr<Student::Player> player: player_vector_)
    {
        // Otetaan pelaajan  väri, id ja nappulat talteen
        QString color = player->get_color();
        int player_id = player->getPlayerId();
        std::vector<std::shared_ptr<Common::Pawn>>* pawns =
                player->get_pawn_vector(); 

        // Käöydään pelaajan nappulat läpi
        for( std::shared_ptr<Common::Pawn> pawn: *pawns )
        {
            Common::CubeCoordinate coordinates = Common::CubeCoordinate(0,0,0);

            while(true)
            {
                // Arvotaan ruutu
                hexagon_it = hexagon_map_.begin();

                std::advance(hexagon_it,
                    static_cast<unsigned int>(rand()) % hexagon_map_.size());

                // Haetaan ruudun koordinaatit
                coordinates = hexagon_it->first;

                // Haetaan ruudun tiedot
                std::shared_ptr<Common::Hex> hex = gameboard_->getHex(coordinates);
                std::string hex_type = hex->getPieceType();
                int pawns = hex->getPawnAmount();

                // Jos ruutu ei ole vettä tai koraalia ja ruudussa ei ole yli
                // kolmea pelaajaa -> hyväksytään ruutu
                if(hex_type != "Water" && hex_type != "Coral" && pawns < 3)
                {
                    break;
                }
            }

            // Haetaan graaffinen ruutu
            Student::Hexagon* hexagon = hexagon_map_.at(coordinates);

            // Luodaan uusi graaffinen pelinappula
            int pawn_id = pawn->getId();
            Student::GPawn* gpawn = new Student::GPawn(player_id, pawn_id,
                                            gamestate_, &view_scale_,
                                            coordinates, hexagon);
            gpawn->setBrush(QColor(color));
            gpawn_map_.insert({pawn_id, gpawn});

            // Lisätään koordinaatit pelinappulalle ja ruudulle jossa nappula on
            gameboard_->getHex(coordinates)->addPawn(pawn);
            pawn->setCoordinates(coordinates);
        }
    }

    // Järjestetään koko laudan nappulat järjestykseen
    for( auto pair: hexagon_map_)
    {
        sort_gpawns(pair.first);
    }
}

void Main_window::next_player_turn()
{
    // Haetaan nykyinen pelaaja
    int current_player = gamestate_->currentPlayer();

    // Luodaan liput osoittamaan seuraavaa pelaajaa ja uutta läpikäyntikierrosta
    bool next_player = false, new_loop = false;

    // Käydään läpi kaikki pelaajat
    std::vector<std::shared_ptr<Student::Player>>::iterator player_it =
            player_vector_.begin();
    while (player_it != player_vector_.end())
    {
        // Jos seuraava pelaaja lippu on asetettu ja pelaajalla on nappuloita
        if(next_player && player_it->get()->get_pawn_vector()->size() != 0)
        {
            for(auto player: player_vector_)
            {
                // Jos muilla pelaajilla on nappuloita ei lopeteta
                if(player->get_pawn_vector()->size() != 0 &&
                        player->getPlayerId() != player_it->get()
                                                ->Player::getPlayerId())
                {
                    // Asetetaan pelaajalle uudet liikkeet
                    player_it->get()->setActionsLeft(3);
                    gamestate_->changePlayerTurn(player_it->get()
                                                 ->Player::getPlayerId());
                    return;
                }
            }


            // Viimeinen pelaaja jäljellä joten lopetetaan peli
            end(player_it->get()->getPlayerId());
            break;
        }

        // Asetetaan nykyisen pelaajan seuraavan pelaaja seurvaksi pelaajaksi
        else if( player_it->get()->getPlayerId() == current_player)
        {
            next_player = true;
        }

        player_it++;
        // Jos ollaan vektorin lopussa
        if( player_it == player_vector_.end())
        {
            // Hypätään vektorin alkuun
            player_it = player_vector_.begin();

            // Jos käydään vektoria toista kertaa niin lopetetaan
            if( new_loop )
            {
                break;
            }
            // Muuten aloitetaan toinen läpikäyntikerta
            else
            {
                new_loop = true;
            }
        }
    }
}

void Main_window::sort_gpawns(Common::CubeCoordinate coord)
{
    // Haetaan haluttu ruutu
    std::shared_ptr<Common::Hex> hex = gameboard_->getHex(coord);

    // Haetaan ruudussa olevat nappulat
    std::vector<std::shared_ptr<Common::Pawn>> pawns = hex->getPawns();

    // Asetellaan nappulat vierekkän ruutuun
    switch (hex->getPawnAmount())
    {
    case 1 :
        gpawn_map_.at(pawns.at(0)->getId())->setPos(0, 0);
        break;
    case 2 :
        gpawn_map_.at(pawns.at(0)->getId())->setPos(-20, 0);
        gpawn_map_.at(pawns.at(1)->getId())->setPos(20, 0);
        break;
    case 3 :
        gpawn_map_.at(pawns.at(0)->getId())->setPos(-40, 0);
        gpawn_map_.at(pawns.at(1)->getId())->setPos(0, 0);
        gpawn_map_.at(pawns.at(2)->getId())->setPos(40, 0);
        break;
    default : break;
    }

    // Käydään läpi ruudun kaikki mahdolliset veneet tai delffiinit
    for(std::shared_ptr<Common::Transport> transport: hex->getTransports())
    {
        // Lasketaan montako nappulaa transportissa on
        int pawn_count = transport->getMaxCapacity() - transport->getCapacity();

        // Haetaan transportissa olavat nappulat
        std::vector<std::shared_ptr<Common::Pawn>> pawns =
                transport->getPawnsInTransport();

        // Asetellaan nappulat vierekkän transporttiin
        switch (pawn_count)
        {
        case 1 :
            gpawn_map_.at(pawns.at(0)->getId())->setPos(0, 0);
            break;
        case 2 :
            gpawn_map_.at(pawns.at(0)->getId())->setPos(-20, 0);
            gpawn_map_.at(pawns.at(1)->getId())->setPos(20, 0);
            break;
        case 3 :
            gpawn_map_.at(pawns.at(0)->getId())->setPos(-40, 0);
            gpawn_map_.at(pawns.at(1)->getId())->setPos(0, 0);
            gpawn_map_.at(pawns.at(2)->getId())->setPos(40, 0);
            break;
        default : break;
        }

    }
}

void Main_window::next_phase()
{
    // Haetaan nykyinen vaihe
    Common::GamePhase phase = gamestate_->currentGamePhase();

    // Siirrytään seuraavaan vaiheeseen
    switch (phase)
    {
    case Common::GamePhase::MOVEMENT:
        // Tarkistetaan että upotettavia ruutuja on
        for(auto hex: *gameboard_->get_map())
        {
            std::string piece_type = hex.second->getPieceType();
            if(piece_type != "Coral" && piece_type != "Water")
            {
                gamestate_->changeGamePhase(Common::GamePhase::SINKING);
                return;
            }
        }
        // Jos saariruutuja ei ole hypätään suoraan pyöritysvaiheeseen
        gamestate_->changeGamePhase(Common::GamePhase::SPINNING);
        break;

    case Common::GamePhase::SINKING:
        gamestate_->changeGamePhase(Common::GamePhase::SPINNING);
        wheel_->setEnabled(true);
        break;

    case Common::GamePhase::SPINNING:
        next_player_turn();
        gamestate_->changeGamePhase(Common::GamePhase::MOVEMENT);
        break;
    }
}

void Main_window::set_item_cursors(PIECE_TYPES type, bool enable,
                                   std::vector<std::string> subtypes)
{
    switch (type)
    {
    case HEX:
        for(auto hex: *gameboard_->get_map())
        {
            Student::Hexagon* hexagon = hexagon_map_.at(hex.first);
            std::string hex_type = hex.second->getPieceType();

            // Jos ruutu ei ole vettä tai koraalia ja se enabloidaan
            if(enable && hex_type != "Water" && hex_type != "Coral")
            {
                hexagon->setCursor(Qt::ArrowCursor);
            }
            else
            {
                hexagon->setCursor(Qt::ForbiddenCursor);
            }
        }
        break;

    case PAWN:
        for(auto player: player_vector_)
        {
            for(auto pawn: *player->get_pawn_vector())
            {
                Student::GPawn* gpawn = gpawn_map_.at(pawn->getId());

                // Jos nappulat enabloidaan ja nappulat on vuorossa olevan pelaajan
                if(player->getPlayerId() == gamestate_->currentPlayer() && enable)
                {
                    gpawn->setCursor(Qt::OpenHandCursor);
                    gpawn->setZValue(2);
                }
                else
                {
                    gpawn->setCursor(Qt::ForbiddenCursor);
                    gpawn->setZValue(0);
                }
            }
        }
        break;

    case TRANSPORT:
        for(auto transport: *gameboard_->get_transport_map())
        {
            Student::GTransport* gtransport =
                    gtransport_map_.at(transport.first);

            // Jos transportit enabloidaan ja ne ovat oikeaa tyyppiä
            if(enable && std::count(subtypes.begin(), subtypes.end(),
                                    transport.second->getTransportType()) > 0)
            {
               gtransport->setCursor(Qt::OpenHandCursor);
               gtransport->setZValue(2);
            }
            else
            {
                gtransport->setCursor(Qt::ForbiddenCursor);
                gtransport->setZValue(0);
            }
        }
        break;

    case ACTOR:
        for(auto actor: *gameboard_->get_actor_map())
        {
            Student::g_actor* g_actor = g_actor_map_.at(actor.first);

            // Jos actori enabloidaan ja on oikeaa tyyppiä
            if(enable && std::count(subtypes.begin(), subtypes.end(),
                                    actor.second->getActorType()) > 0)
            {
                g_actor->setCursor(Qt::OpenHandCursor);
                g_actor->setZValue(2);
            }
            else
            {
                g_actor->setCursor(Qt::ForbiddenCursor);
                g_actor->setZValue(0);
            }
        }
        break;
    }
}

void Main_window::end(int player_id)
{
    if(player_id != -1)
    {
        // Lasketaan viimeiselle pelaajalle selvinneiden nappuloiden pisteet
        for(std::shared_ptr<Student::Player> player: player_vector_)
        {
            if(player->getPlayerId() == player_id)
            {
                for(std::shared_ptr<Common::Pawn> pawn: *player->get_pawn_vector())
                {
                    player->add_point();
                }
                break;
            }
        }
    }

    // Lasketaan keillä pelaajilla on suurimmat pisteet
    int max_points = 0;
    std::unordered_map<std::shared_ptr<Student::Player>, int> player_points = {};
    for(std::shared_ptr<Student::Player> player: player_vector_)
    {
        int points  = player->get_points();
        if(points >= max_points)
        {
            if (points > max_points)
            {
                player_points.clear();
            }
            player_points.insert({player, points});
            max_points = points;
        }
    }

    // Piilotetaan pääruutu
    QMessageBox  msg_box(this);

    // Muodostetaan teksi ikkunaan ja asetetaan se
    QString box_text = "Voittajat:";

    for(std::pair<std::shared_ptr<Student::Player>, int> points: player_points)
    {
        box_text += " \n\nPelaajan numero: " +
                QString::number(points.first->getPlayerId() + 1) + " \n" +
                "Pelaajan nimi: " + points.first->get_name();
    }

    msg_box.setText(box_text);
    msg_box.setWindowFlag(Qt::Popup);
    msg_box.setWindowTitle("Voittajat");

    // Asetetaan ikkunalle tekstin väri ja fontin koko
    msg_box.setStyleSheet("color:black; font:15pt;");
    msg_box.QMessageBox::exec();

    close();
}

}
