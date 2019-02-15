#ifndef CONSTANTS_HH
#define CONSTANTS_HH

#include <vector>
#include <QString>
#include <unordered_map>
#include <QColor>
#include <QVector>
#include <QPointF>
#include <map>

/**
 *@file
 *@brief Tiedostossa määritellään globaaleja vakioita.
 */

/**
 * @brief Meidän oma nimiavaruus
 */
namespace Student
{

/**
 * @brief Määrittelee listan, josta pelaajien värit valitaan.
 * Listan pituus määrää pelaajien maksimimäärän.
 */
const std::vector<QString> PLAYER_COLOURS = {
    "cyan",
    "magenta",
    "green",
    "yellow",
    "blue",
    "red",
    "darkRed",
    "darkCyan"
};

/**
 * @brief Määrittelee aloitusdialogi-ikkunan leveyden
 */
const int DIALOG_WIDTH = 600;

/**
 * @brief Määrittelee  aloitusdialogi-ikkunan korkeuden.
 */
const int DIALOG_HEIGHT = 400;

/**
 * @brief Määrittelee pelaajan käytössä olevien siirtojen
 *  määrän vuoronsa alussa. Vaatimuksien mukaan 3.
 */
const unsigned int AMOUNT_OF_ACTIONS = 3;

/**
 * @brief Määrittelee pelaajalla olevien nappuloiden määrän
 *  pelin alussa.
 */
const unsigned int AMOUNT_OF_PAWNS = 5;


/**
 * @brief Määrittelee peliruutujen värit peliruututyypin mukaan.
 */
const std::unordered_map<std::string,QColor> HEX_COLORS = {
    {"Peak","darkGray"},
    {"Mountain","gray"},
    {"Forest","green"},
    {"Beach","yellow"},
    {"Water","blue"},
    {"Coral","cyan"}};

/**
 * @brief Asettaa pääikkunan koon 4/5:n näytön
 * koosta pääikkunan käynnistyessä
 */
const double MAIN_WINDOW_SCALE = 0.8;

/**
 * @brief Määritellään graafisen peliruudun kokoa
 */
const int HEXAGON_Y = 100;

/**
 * @brief Määritellään graafisen peliruudun kokoa
 */
const int HEXAGON_X = 86 * HEXAGON_Y /100;

/**
 * @brief Määrittelee graafisia muotoja, joita käytetään pelilaudan ja
 * ja muiden olioiden graafisiin toteutuksiin
 */
const std::unordered_map<std::string, QVector<QPointF>> SHAPES =
{
    {"hexagon", QVector<QPointF>({ QPointF( HEXAGON_X, HEXAGON_Y / 2 ),
                                   QPointF( HEXAGON_X * 0, HEXAGON_Y ),
                                   QPointF( -HEXAGON_X, HEXAGON_Y / 2 ),
                                   QPointF( -HEXAGON_X, -HEXAGON_Y / 2 ),
                                   QPointF( HEXAGON_X * 0, -HEXAGON_Y ),
                                   QPointF( HEXAGON_X, -HEXAGON_Y / 2 )})
    },
    {"gpawn", QVector<QPointF>({QPointF( 20, 40 ), QPointF( 10, 0 ),
                                QPointF( 20, -20 ), QPointF( 10, -40 ),
                                QPointF( -10, -40 ), QPointF( -20, -20),
                                QPointF( -10, 0), QPointF( -20, 40)})
    },
    {"boat", QVector<QPointF>({ QPointF( -HEXAGON_X, 0), QPointF( HEXAGON_X, 0),
                                QPointF( HEXAGON_X / 2, HEXAGON_Y / 2 ),
                                QPointF( -HEXAGON_X / 2, HEXAGON_Y / 2 )})
    },
    {"dolphin", QVector<QPointF>({QPointF( 60, -30 ), QPointF( 30, -37 ),
                                  QPointF( -15, -52 ), QPointF( -3, -35 ),
                                  QPointF( -30, -25 ), QPointF( -54, 8 ),
                                  QPointF( -85, 16 ), QPointF( -58, 18 ),
                                  QPointF( -50, 38 ), QPointF( -45, 16 ),
                                  QPointF( -30, 0 ), QPointF( 3, -12 ),
                                  QPointF( 30, -6 ), QPointF( 16, 8 ),
                                  QPointF( 54, 6 ), QPointF( 70, 24 ),
                                  QPointF( 80, 27 ), QPointF( 80, 5 )})
    }
};

/**
 * @brief Määrittelee kiekon kuvatiedoston koon pikseleinä
 * Kuva on oltava neliön muotoinen.
 */
const unsigned int WHEEL_PIC_HEIGHT = 262; // AND WIDTH

/**
 * @brief url:it Kuviin, jotka asetetaan kiekkoihin
 */
const std::map<int, QString> WHEEL_PIC_URLS = {{0,QString(":/wheel.png")},
                                               {1, QString(":/shadow.png")}};

/**
 * @brief Infotekstikentän korkeus.
 */
const unsigned int LABEL_HEIGHT = 50;

/**
 * @brief Enumeraatio määrittelee kulmat,
 * johon kiekko asetetaan kussakin tilanteessa.
 */
enum WHEEL_STATES{ D = 45 , ONE = 315, TWO = 225, THREE = 135};

/**
 * @brief Enumeraatio määrittelee eri toimijatyypit.
 */
enum ACTOR_TYPES{KRAKEN, SEAMUNSTER, SHARK, VORTEX};

/**
 * @brief Määrittelee url:t kuviin, joita käytetään toimijoissa.
 */
const std::map<ACTOR_TYPES, QString> ACTOR_PIC_URLS =
                                    {{KRAKEN, QString(":/kraken.png")},
                                     {SEAMUNSTER, QString(":/munster.png")},
                                     {SHARK, QString(":/shark.png")},
                                     {VORTEX, QString(":/vortex.png")}};

/**
 * @brief Määrittelee mahdolliset muutokset kurssin puolen
 * koodin käyttämistä std::String-tyyppisestä toimijatyyppitiedosta
 * järkevämmäksi ACTOR_TYPE-enumeraatioksi.
 */
const std::map<std::string, int> ACTOR_TYPE_MAP = {{"kraken", KRAKEN},
                                                   {"seamunster",SEAMUNSTER},
                                                   {"sea munster", SEAMUNSTER},
                                                   {"shark", SHARK},
                                                   {"vortex", VORTEX}};

/**
 * @brief Enumeraatio määrittelee graafisten olioiden päätyypit.
 */
enum PIECE_TYPES{HEX, PAWN, TRANSPORT, ACTOR};
}

#endif // CONSTANTS_HH
