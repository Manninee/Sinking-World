#ifndef MAIN_WINDOW_HH
#define MAIN_WINDOW_HH

#include "hexagon.hh"
#include "gameboard.hh"
#include "cubecoordinate.hh"
#include "constants.hh"
#include "gpawn.hh"
#include "g_actor.hh"
#include "gamestate.hh"
#include "player.hh"
#include "igamerunner.hh"
#include "illegalmoveexception.hh"
#include "wheel_item.hh"
#include "gtransport.hh"
#include "actor.hh"
#include "kraken.hh"
#include "seamunster.hh"
#include "shark.hh"
#include "vortex.hh"

#include <QMainWindow>
#include <QGridLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <QApplication>
#include <QDesktopWidget>
#include <QStyle>
#include <QLabel>
#include <QMessageBox>
#include <QSpacerItem>
#include <QScreen>
#include <unordered_map>
#include <map>
#include <string>
#include <vector>
#include <random>

#include <chrono>
#include <thread>

namespace Student
{

/**
 * @brief Toteuttaa pelille pääikkunan ja graaffiset elementit
 */
class Main_window : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Luo ja asettelee pääikkunan, luo ja asettelee pelilaudan,
     *        asettelee pelaajien nappulat ja alkuveneet
     * @param application: Osoitin applikaatioon
     * @param gameboard: Osoitin pelilautaan
     * @param gamestate: Osoitin pelin tilannetta seuraavaan olioon
     * @param players: Pelaajien osoittimia sisältävä vektori
     * @param runner: Osoitin pelimoottoriin
     * @param parent: Osoitin vanhempaan
     * @post Poikkeustakuu: Perus
     */
    explicit Main_window(QApplication* application,
                         std::shared_ptr<Student::GameBoard> gameboard,
                         std::shared_ptr<Student::GameState> gamestate,
                         std::vector<std::shared_ptr<Student::Player>> players,
                         std::shared_ptr<Common::IGameRunner> runner,
                         QWidget *parent = nullptr);
    ~Main_window();

public slots:
    /**
     * @brief Lähentää pelilautaa
     * @post Poikkeustakuu: Nothrow
     */
    void zoom_board();

    /**
     * @brief Kauentaa pelilautaa
     * @post Poikkeustakuu: Nothrow
     */
    void unzoom_board();

    /**
     * @brief Siirtää graaffisen pelinappulan jos sallittu
     * @param pawn_id: Siirrettävä nappulan id
     * @param origin: Lähtökoordinaatit
     * @param target: Loppukoordinaatit
     * @post Poikkeustakuu: Peus
     */
    void move_pawn(int pawn_id, Common::CubeCoordinate origin,
                      Common::CubeCoordinate target);

    /**
     * @brief Siirtää meren toimijoita
     * @param actor_id: Toimijan yksilöivä tunnus
     * @param origin: Lähtöruutu
     * @param target: Kohderuutu
     * @param moves: Liikkeiden lukumäärä
     * @post Poikkeustakuu: Perus
     */
    void move_actor(int actor_id, Common::CubeCoordinate origin,
                    Common::CubeCoordinate target);
    /**
     * @brief Kääntää ruudun vedeksi jos sallittu
     * @param hexagon_coord: Ruudun koordinaatit
     * @post Poikkeustakuu: Nothrow
     */
    void flip_tile(Common::CubeCoordinate hexagon_coord);

    /**
     * @brief Poistaa ruudun käytöstä hyrrän pyörimisen ajaksi
     * @param state: Hyrrän pyrimisen tilanne
     * @post Poikkeustakuu: Nothrow
     */
    void wheel_state(bool state);

    /**
     * @brief Luo uuden graaffisen actorin
     * @param type: Uuden actorin tyyppi
     * @param id: uuden actorin id
     * @param coordinates: Uuden actorin koordinaatit
     * @post Poikkeustakuu: Perus
     */
    void new_g_actor(int type, int id, Common::CubeCoordinate coordinates);

    /**
     * @brief Poistaa graaffisen actorin jos id:llä löytyy jos ei löydy ei tee
     *        mitään
     * @param id: Poistettavan actorin id
     * @post Poikkeustakuu: Perus
     */
    void remove_g_actor(int id);

    /**
     * @brief Pyörittää graaffista pyörrettä
     * @param id: Pyöritettävän vortexin id
     * @post Poikkeustakuu: minimi
     */
    void spin_vortex(int id);

    /**
     * @brief Asettaa talteen hyrrältä tulevat tiedot liikuvasta actorista ja
     *        jos actoria ei ole laudalla jatkaa automaattisesti seuraavaan
     *        vaiheeseen
     * @param actor_move_info: Hyrrältä tulevat tiedot kuka liikkuu ja paljonko
     * @post Poikkeustakuu: Perus
     */
    void set_actor_move_info( std::pair<std::string,
                              std::string> actor_move_info);

    /**
     * @brief Luo uuden graaffisen transportin
     * @param transport: Uuden transportin id
     * @param coord: Koordinaatit johon uusi transport laitetaan
     * @post Poikkeustakuu: Perus
     */
    void create_transport(std::shared_ptr<Common::Transport> transport,
                          Common::CubeCoordinate coord);
    /**
     * @brief Vaihtaa pelaajatietokentän tekstin ja näyttää ponnahdusikkunan
     *        ilmoituksen vuoron vaihtumisesta
     * @post Poikkeustakuu: Nothrow
     */
    void player_changed();

    /**
     * @brief Vaihtaa vaihetieto- ja infokentän teksti ilmoittmaan pelin
     *        tietoja. Vaihtaa graaffisten olioiden kursorit ilmoittamaan
     *        voiko niillä tehdä mitään
     * @post Poikkeustakuu: Perus
     */
    void phase_changed();

    /**
     * @brief Liikuttaa transportin ja sen kyydissä olevat nappulat jos siirto
     *        on sallittu
     * @param transport_id: Liikutettavan transportin id
     * @param origin: Lähtökoordinaatit
     * @param target: Loppukoordinaatit
     * @post Poikkeustakuu: Perus
     */
    void move_transport(int transport_id, Common::CubeCoordinate origin,
                        Common::CubeCoordinate target);

    /**
     * @brief Liikuttaa nappulan transporttiin jos liike on sallittu
     * @param pawn_id: Lastattavan nappulan id
     * @param transport_id: Transportin id
     * @param origin: Nappulan lähtökoordinaatit
     * @param target: Nappulan tulo- ja transportin koordinaatit
     * @post Poikkeustakuu: Perus
     */
    void pawn_into_transport(int pawn_id, int transport_id,
                                Common::CubeCoordinate origin,
                                Common::CubeCoordinate target);

    /**
     * @brief Vaihtaa pelin vaiheen seuraavaan jos vaihe ei ole
     *        Common::GamePhase::SINKING. Vuoronlopetus painiketta varten
     * @post Poikkeustakuu: Nothrow
     */
    void end_turn();

    /**
     * @brief Poistaa graaffisen nappulan jos se löytyy ja jos ei löydy ei tee
     *        mitään
     * @param pawn_id: Poistettavan nappulan id
     * @post Poikkeustakuu: Perus
     */
    void remove_gpawn(int pawn_id);

    /**
     * @brief Asettaa poistettavass transportissa olevat nappulat veteen ja
     *        poistaa transportin jos se löytyy
     * @param transport_id: Poistettavan transportin id
     * @param coord: Poistettavan transportin koordinaatit
     * @post Poikkeustakuu: Perus
     */
    void remove_gtransport(int transport_id, Common::CubeCoordinate coord);

private:

    /**
     * @brief Luo ja asettelee graaffisen käyttöliittymän elementit
     * @post Poikkeustakuu: Perus
     */
    void setup_layout();

    /**
     * @brief Yhdistää signaalit ja slotit
     * @post Poikkeustakuu: Perus
     */
    void setup_signals();

    /**
     * @brief Luo ja asettelee graafisen pelilaudan ruudut
     * @post Poikkeustakuu: Perus
     */
    void setup_board();

    /**
     * @brief Luo graaffiset pelinappulat ja asettaa ne laudalle
     * @post Poikkeustakuu: Perus
     */
    void setup_pieces();

    /**
     * @brief Vaihtaa seuraavan pelaajan vuoroon ja tarkistaa onko pelaajilla
     *        nappuloita jäljellä ja lopettaa pelin
     * @post Poikkeustakuu: Perus
     */
    void next_player_turn();

    /**
     * @brief Järjestää ruudun ja transporttien pelinappulat nätisti vierekkäin
     * @param hexagon: Ruudun koordinaatit
     * @post Poikkeustakuu: Perus
     */
    void sort_gpawns(Common::CubeCoordinate coord);

    /**
     * @brief Vaihtaa pelin vaihetta ja automaattisesti hyppää upotusvaiheen
     *        yli jos saaripaloja ei enään ole
     * @post Poikkeustakuu: Perus
     */
    void next_phase();

    /**
      * @brief Asettaa graaaffisten oloiden kursorit ilmoittamaan voiko niitä
      *        painaa
      * @param type: Graaffisen olion tyyppi jonka kursoria halutaan muutta
      * @param enable: Lippu osoittamaan halutaanko laitta estokursori vai
      *                sallimiskursori
      * @param subtypes: Vektori actoreiden tai transporttien tyypeistä jotka
      *                  halutaan sallia
      * @post Poikkeustakuu: Perus
      */
    void set_item_cursors(PIECE_TYPES type, bool enable,
                          std::vector<std::string> subtypes = {});

    /**
     * @brief Antaa viimeiselle pelaajalle pisteet lopuista nappuloista ja
     *        näyttää ponnahdusikkunan jossa on tiedot voittajista
     * @param player_id: Viimeisen pelaajan id
     */
    void end(int player_id = -1);

    /**
     * @brief base_widget_: Pohjawidgetti
     */
    QWidget* base_widget_;

    /**
     * @brief board_: Osoitin graaffiseen pelilautaan
     */
    QGraphicsScene* board_;

    /**
     * @brief view_: Osoitin pelilaudan graaffiseen näkymään
     */
    QGraphicsView* view_;

    /**
     * @brief spin_scene_: Osoitin kiekon graafiseen toteutukseen
     */
    QGraphicsScene* spin_scene_;

    /**
     * @brief spin_view_: Osoitin hyrrän graaffiseen näkymään
     */
    QGraphicsView* spin_view_;

    /**
     * @brief button_map_: Graaffisen käyttöliittymän painikkeet
     */
    std::unordered_map<std::string, QPushButton*> button_map_;

    /**
     * @brief label_map_: Graaffisen käyttöliitymän tekstikentät
     */
    std::unordered_map<std::string, QLabel*> label_map_;

    /**
     * @brief hexagon_map_: Sisältää graaffiset pelilaudan ruudut avaimena
     *                      ruudun koordinaatit
     */
    std::map<Common::CubeCoordinate, Student::Hexagon*> hexagon_map_;

    /**
     * @brief gpawn_map_: Sisältää graffiset pelinappulat avaimena nappulan id
     */
    std::map<int, Student::GPawn*> gpawn_map_;

    /**
     * @brief gtransport_map_: Sisältää graaffiset transportit avaimena
     *                         transportin id
     */
    std::map<int, Student::GTransport*> gtransport_map_;

    /**
     * @brief actor_map: Sisältää toimijat avaimena toimijatunniste
     */
    std::map<int, g_actor*> g_actor_map_;

    /**
     * @brief gameboard_: Osoitin pelilautaan
     */
    std::shared_ptr<Student::GameBoard> gameboard_;

    /**
     * @brief gamestate_: Osoitin pelin tilannetta seuraavaan olioon
     */
    std::shared_ptr<Student::GameState> gamestate_;

    /**
     * @brief player_vector_: Sisältää pelaajien osoittimet
     */
    std::vector<std::shared_ptr<Student::Player>> player_vector_;

    /**
     * @brief runner_: Osoitin pelimoottoriin
     */
    std::shared_ptr<Common::IGameRunner> runner_;

    /**
     * @brief wheel_: Osoitin hyrrän graaffiseen toteutukseen
     */
    wheel_item* wheel_;

    /**
     * @brief shadow_: Osoitin hyrrän päällä olevan varjon toteutukseen
     */
    wheel_item* shadow_;

    /**
     * @brief actor_move_info_: Pari jossa ensimmäinen on liikutettava actor ja
     *                          toisena liikkeen muoto
     */
    std::pair<std::string, std::string> actor_move_info_;

    /**
     * @brief view_scale_: Pelilautanälkymän skaalausta seuraava muuttuja
     */
    double view_scale_;
};

}
#endif // MAIN_WINDOW_HH
