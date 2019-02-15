#include "gamestate.hh"
#include "igamestate.hh"

#include <QString>
#include <QtTest>


Q_DECLARE_METATYPE(Common::GamePhase)

class Game_stateTest : public QObject
{
    Q_OBJECT

public:
    Game_stateTest() = default;

private Q_SLOTS:
    void test_game_phase();
    void test_game_phase_data();

    void test_turn();
    void test_turn_data();


};


void Game_stateTest::test_game_phase()
{
    QFETCH(Common::GamePhase, phase);

    Student::GameState* object = new Student::GameState;
    QSignalSpy spy(object, &Student::GameState::phase_changed);

    object->Student::GameState::changeGamePhase(phase);
    Common::GamePhase returned_phase
            = object->Student::GameState::currentGamePhase();
    delete object;
    QCOMPARE(phase, returned_phase);
    QVERIFY2(spy.QSignalSpy::isValid(),"signal was not emitted");

}

void Game_stateTest::test_game_phase_data()
{
    // Määritellään testidata pelifaasien vaihdon testaamiseen.
    QTest::addColumn<Common::GamePhase>("phase");

    QTest::newRow("movement") << Common::MOVEMENT;
    QTest::newRow("sinking") << Common::SINKING;
    QTest::newRow("spinning") << Common::SPINNING;
}

void Game_stateTest::test_turn()
{
    QFETCH(int, id);

    Student::GameState* object = new Student::GameState;
    QSignalSpy spy(object, &Student::GameState::turn_changed);

    object->changePlayerTurn(id);
    int changed_id = object->currentPlayer();
    delete object;
    QVERIFY2(spy.QSignalSpy::isValid(), "signal was not emitted");
    QCOMPARE(id, changed_id);


}

void Game_stateTest::test_turn_data()
{
    // Määritellään testidata pelaajan vaihdon testaamiseen.
    // id ei voi olla negatiivinen mutta se on määritelty intiksi
    QTest::addColumn<int>("id");

    QTest::newRow("0") << 0;
    QTest::newRow("1") << 1;
    QTest::newRow("2") << 2;
    QTest::newRow("3") << 3;
    QTest::newRow("7") << 7; // Suurin mahdollinen pelaaja-id
}

QTEST_MAIN(Game_stateTest)

#include "game_stateTest.moc"
