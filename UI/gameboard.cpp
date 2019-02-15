#include "gameboard.hh"

namespace Student
{

GameBoard::GameBoard():
    hex_map_({}),
    actor_map_({}),
    player_vector_({}),
    transport_map_({}),
    gamestate_(nullptr),
    pawn_map_({})
{

}

void GameBoard::set_player_pointer_vector(
                            std::vector<std::shared_ptr<Player>> players)
{
    player_vector_ = players;

    // Lisätään pelinappulat pelaajille
    int pawn_id = 0;
    for( std::shared_ptr<Student::Player> player: player_vector_)
    {
        for(unsigned int i = 0; i < AMOUNT_OF_PAWNS; i++)
        {
            addPawn(player->getPlayerId(), pawn_id);
            pawn_id++;
        }
    }
}

void GameBoard::set_gamestate_ptr(std::shared_ptr<GameState> gamestate)
{
    gamestate_ = gamestate;
}

void GameBoard::addHex(std::shared_ptr<Common::Hex> new_hex)
{
    if(hex_map_.count(new_hex->getCoordinates()) != 0)
    {
        hex_map_.erase(new_hex->getCoordinates());
    }

    hex_map_.insert({new_hex->getCoordinates(),new_hex});

}

std::shared_ptr<Common::Hex> GameBoard::getHex(
        Common::CubeCoordinate hex_coordinate) const
{
    // Tarkistaa että koordinaateilla löytyy ruutu
    if( hex_map_.count(hex_coordinate) != 0)
    {
        return hex_map_.at(hex_coordinate);
    }
    else
    {
        return nullptr;
    }
}

hexes_map_type* GameBoard::get_map()
{
    return &hex_map_;
}

std::map<int, std::shared_ptr<Common::Transport>>*
                                        GameBoard::get_transport_map()
{
    return &transport_map_;
}

std::map<int, std::shared_ptr<Common::Actor> > *GameBoard::get_actor_map()
{
    return &actor_map_;
}

std::shared_ptr<Common::Transport> GameBoard::get_transport(int transport_id) const
{
    if(transport_map_.count(transport_id) != 0)
    {
        return transport_map_.at(transport_id);
    }
    else
    {
        return nullptr;
    }

}

std::shared_ptr<Common::Pawn> GameBoard::get_pawn(int pawn_id) const
{
    if(pawn_map_.count(pawn_id) != 0)
    {
        return pawn_map_.at(pawn_id);
    }
    else
    {
       return nullptr;
    }
}

int GameBoard::checkTileOccupation(Common::CubeCoordinate hex_coordinate) const
{
    if( hex_map_.count(hex_coordinate) != 0)
    {
        return hex_map_.at(hex_coordinate)->getPawnAmount();
    }
    else
    {
        return -1;
    }
}

bool GameBoard::isWaterTile(Common::CubeCoordinate hex_coordinate) const
{
    if( hex_map_.count(hex_coordinate) != 0)
    {
        if(hex_map_.at(hex_coordinate)->getPieceType() == "Water")
        {
            return true;
        }
    }
    return false;
}

void GameBoard::addPawn(int playerId, int pawnId, Common::CubeCoordinate coord)
{
    addPawn(playerId, pawnId);

    std::shared_ptr<Common::Pawn> pawn = get_pawn(pawnId);
    if(pawn != nullptr)
    {
        pawn->setCoordinates(coord);

        if(hex_map_.count(coord) != 0)
        {
            hex_map_.at(coord)->addPawn(pawn);
        }
    }
}

void GameBoard::addPawn(int playerId, int pawnId)
{
    std::shared_ptr<Common::Pawn> pawn = std::shared_ptr<Common::Pawn>
                                                (new Common::Pawn());
    pawn->setId(pawnId, playerId);
    pawn_map_.insert({pawnId, pawn});

    // Lisätään nappula pelaajalle
    for(std::shared_ptr<Student::Player> player: player_vector_)
    {
        if(player->getPlayerId() == playerId)
        {
            player->add_pawn(pawn);
            break;
        }
    }
}

void GameBoard::movePawn(int pawnId, Common::CubeCoordinate pawnCoord)
{
    // Jos ruutua annetuilla koordinaateilla ei ole niin lopetetaan
    if(hex_map_.count(pawnCoord) == 0)
    {
        return;
    }

    std::shared_ptr<Common::Pawn> pawn = get_pawn(pawnId);
    if(pawn != nullptr)
    {

        // Poistetaan nappula lähtöruudusta
        hex_map_.at(pawn->getCoordinates())->removePawn(pawn);

        // Lisätään päämääräruutun nappula
        hex_map_.at(pawnCoord)->addPawn(pawn);

        // Asetetaan nappulalle uudeti koordinaatit
        pawn->setCoordinates(pawnCoord);
    }
}

void GameBoard::removePawn(int pawnId)
{
    if(pawn_map_.count(pawnId) != 0)
    {
        std::shared_ptr<Common::Pawn> pawn = pawn_map_.at(pawnId);
        std::shared_ptr<Common::Hex> hex = hex_map_.at(pawn->getCoordinates());

        // Poistetaan nappula ruudulta
        hex->removePawn(pawn);

        // Poistetaan nappula transporteista
        for(auto transport: hex->getTransports())
        {
            transport->removePawn(pawn);
        }

        // Poistetaan nappulla gameboardilta
        if(pawn_map_.count(pawnId) != 0)
        {
            pawn_map_.erase(pawnId);
        }

        // Poistetaan graaffinen nappula
        emit remove_pawn(pawnId);
    }

    // Poistetaan nappula pelaajalta
    for(std::shared_ptr<Student::Player> player: player_vector_)
    {
        std::shared_ptr<Common::Pawn> pawn = player->get_pawn(pawnId);
        player->remove_pawn(pawnId);
    }
}

void GameBoard::moveActor(int actorId, Common::CubeCoordinate actorCoord)
{
    // Jos ruutua annetuilla koordinaateilla ei ole niin lopetetaan
    if(hex_map_.count(actorCoord) == 0)
    {
        return;
    }

    std::shared_ptr<Common::Hex> old_hex = actor_map_.at(actorId)->getHex();

    // Poistetaan actori vanhalta ruudulta
    old_hex->removeActor(actor_map_.at(actorId));

    // Lisätään actori uudelle ruudulle
    hex_map_.at(actorCoord)->addActor(actor_map_.at(actorId));
    //actor_map_.at(actorId)->doAction(); <- Tämä ei mahdollistanut poistoa

    // Lisätään  ruutu actorille
    actor_map_.at(actorId)->addHex(hex_map_.at(actorCoord));
    this->use_actor(actorId);
}

void GameBoard::removeActor(int actorId)
{
    std::shared_ptr<Common::Hex> hex = actor_map_.at(actorId)->getHex();
    hex->removeActor(actor_map_.at(actorId));
    emit remove_g_actor(actorId);
    if(actor_map_.count(actorId) > 0 )
    {
        actor_map_.erase(actorId);
    }
}

void GameBoard::addActor(std::shared_ptr<Common::Actor> actor,
                         Common::CubeCoordinate actorCoord)
{
    // Selvitetään mitä tyyppiä luotava olio on.
    std::string actor_type_string = actor->getActorType();

    int actor_type = -1;
    try
    {
        actor_type = ACTOR_TYPE_MAP.at(actor_type_string);
    }
    catch (std::out_of_range &e)
    {
        (void)e;
        //Taas varmaan kirjoitusvirhe kurssin puolen koodissa
        return;
    }

    // Selvitetään uuden actorin id
    int id = actor->getId();

    // Lisätään Common::Actor mappiin
    actor_map_.insert({id,actor});
    emit new_actor(actor_type, id, actorCoord);

    // Lisätään Common::Hex actorille
    actor->addHex(hex_map_.at(actorCoord));

    // rock n' roll
    if(actor_type == VORTEX)
    {
        emit spin_vortex(id);
        this->use_actor(id);
        removeActor(id);
    }
    else
    {
        this->use_actor(id);
    }
}

void GameBoard::addTransport(std::shared_ptr<Common::Transport> transport,
                             Common::CubeCoordinate coord)
{
    // Lisätään uusi transport mappiin
    transport_map_.insert({transport->getId(), transport});

    // Asetetaan transporille ruutu
    transport->addHex(hex_map_.at(coord));

    // Lisätään ruutuun transport
    //hex_map_.at(coord)->addTransport(transport);

    // Lähetetään tieto transportin luonnista
    emit transport_created(transport, coord);
}

void GameBoard::moveTransport(int id, Common::CubeCoordinate coord)
{
    // Jos ruutua annetuilla koordinaateilla ei ole niin lopetetaan
    if(hex_map_.count(coord) == 0)
    {
        return;
    }

    // Haetaan transportin osoitin
    std::shared_ptr<Common::Transport> transport = transport_map_.at(id);

    // Poistetaan transport vanhalta ruudulta
    transport->getHex()->removeTransport(transport);

    // Asetettaan transport uuteen ruutuun ja annetaan sille uusi ruutu
    transport->addHex(hex_map_.at(coord));
    //hex_map_.at(coord)->addTransport(transport);

    // Siirretään transportissa olleet nappulat uuteen ruutuun
    for( auto pawn: transport->getPawnsInTransport())
    {
        movePawn(pawn->getId(), coord);
    }
}

void GameBoard::removeTransport(int id)
{
    std::shared_ptr<Common::Hex> hex = transport_map_.at(id)->getHex();
    hex->removeTransport(transport_map_.at(id));

    // Tarkistetaan että transport on ja poistetaan se
    if(transport_map_.count(id) != 0)
    {
        transport_map_.erase(id);
    }

    emit remove_transport(id, hex->getCoordinates());
}

void GameBoard::use_actor(int actor_id)
{
    std::shared_ptr<Common::Actor> actor = actor_map_.at(actor_id);
    std::shared_ptr<Common::Hex> hex = actor->getHex();
    std::string type_string = actor->getActorType();

    // Muutetaan tieto olion tyypistä enumin mukaiseksi kokonaisluvuksi
    int type = -1;
    try
    {
        type = ACTOR_TYPE_MAP.at(type_string);
    }
    catch (std::out_of_range &e)
    {
        (void)e;
        throw Common::GameException("Taas varmaan kirjoitusvirhe");
    }

    // Luodaan muuttujia olioiden poistoa varten
    std::vector<std::shared_ptr<Common::Transport>> transport_vector
            = hex->getTransports();
    std::vector<std::shared_ptr<Common::Pawn>> pawn_vector
            = hex->getPawns();
    std::vector<std::shared_ptr<Common::Pawn>> pawns_in_safe_vector = {};
    std::vector<Common::CubeCoordinate> hex_coords = {};
    std::vector<std::shared_ptr<Common::Hex>> hex_vector = {hex};
    std::vector<std::shared_ptr<Common::Actor>> actor_vector = {};


    // Valitaan oikeanlaiset olioidenpoistometodit
    switch(type)
    {
    case KRAKEN:
        // Poistetaan transportteja
        for (std::shared_ptr<Common::Transport> transport :transport_vector)
        {
           this->removeTransport(transport->getId());
        }
        break;

    case SEAMUNSTER:
        // Poistetaan hexin veneet
        for(auto transport : transport_vector)
        {
            int id = transport->getId();
            this->removeTransport(id);
        }
        // poistetaan hexin uimarit
        for (auto pawn : pawn_vector)
        {
            int id = pawn->getId();
            this->GameBoard::removePawn(id);
        }
        break;

    case SHARK:
        // poistetaan hexin uimarit
        for (std::shared_ptr<Common::Transport> transport :transport_vector)
        {
            if (transport->getTransportType() != "boat")
            {
                continue;
            }
            // Tässä vectorissa olevat nappulat ovat turvassa hailta
            pawns_in_safe_vector = transport->getPawnsInTransport();
        }

        for (std::shared_ptr<Common::Pawn> pawn : pawn_vector)
        {
            int id = pawn->getId();
            if ( count(pawns_in_safe_vector.begin(),
                       pawns_in_safe_vector.end(), pawn) > 0)
            {
                continue;
            }
            this->GameBoard::removePawn(id);
        }
        break;

    case VORTEX:
        // Poistetaan kaikki
        hex_coords = hex->getNeighbourVector();
        for (Common::CubeCoordinate coord : hex_coords)
        {
            hex_vector.push_back(this->getHex(coord));
        }

        for (std::shared_ptr<Common::Hex> temp_hex : hex_vector)
        {
            // Poistetaan hexin veneet
            transport_vector = temp_hex->getTransports();
            for(auto transport : transport_vector)
            {
                int id = transport->getId();
                this->removeTransport(id);
            }
            // Poistetaan hexin toimijat
            actor_vector = temp_hex->getActors();
            for (std::shared_ptr<Common::Actor> actor : actor_vector)
            {
                // Ei poisteta vortexia vielä
                if (hex == temp_hex)
                {
                    continue;
                }
                int id = actor->getId();
                this->removeActor(id);
            }
            // Poistetaan uimarit
            pawn_vector = temp_hex->getPawns();
            for (std::shared_ptr<Common::Pawn> pawn : pawn_vector)
            {
                int id = pawn->getId();
                this->GameBoard::removePawn(id);
            }
        }
        break;
    default:
        return;
    }

    // tehään juttuja
    actor->doAction();
}

}
