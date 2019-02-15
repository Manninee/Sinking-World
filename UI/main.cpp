/*
Saaripeli
Tekijät:
Eetu Manninen 266049 eetu.manninen@student.tut.fi
Ilmari Marttila 265040 ilmari.marttila@student.tut.fi
*/

#include "gameboard.hh"
#include "gamestate.hh"
#include "iplayer.hh"
#include "player.hh"
#include "start_dialog.hh"
#include "initialize.hh"
#include "main_window.hh"
#include "igamerunner.hh"
#include "gamestate.hh"
#include "ioexception.hh"
#include "formatexception.hh"

#include <memory>
#include <QApplication>
#include <QMessageBox>
#include <vector>

#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Luodaan vektori, johon tallennetaan osoittimet pelaajiin
    std::vector<std::shared_ptr<Student::Player>> player_pointer_vector = {};

    Student::Dialog dialog(&player_pointer_vector);
    bool flag = dialog.Student::Dialog::exec();
    if (!flag)
    {
        // Mikäli aloitusdialogissa painetaan raksia, keskeytetään suoritus
        return 0;
    }

    std::shared_ptr<Student::GameState> state =
            std::shared_ptr<Student::GameState>(new Student::GameState);

    std::vector<std::shared_ptr<Common::IPlayer>>
            player_pointer_vector_runner = {};
    for(auto player: player_pointer_vector)
    {
        player_pointer_vector_runner.push_back(player);
    }

    std::shared_ptr<Student::GameBoard> board =
            std::shared_ptr<Student::GameBoard>(
                new Student::GameBoard());
    board->Student::GameBoard::set_player_pointer_vector(player_pointer_vector);
    board->Student::GameBoard::set_gamestate_ptr(state);

    std::shared_ptr<Common::IGameRunner> runner = nullptr;
    std::string error_msg = "";
    try
    {
        runner = Common::Initialization::getGameRunner(board, state,
                                                player_pointer_vector_runner);
    }
    // Tiedoston lukeminen ei onnistunut
    catch (Common::IoException &msg)
    {
        error_msg = msg.msg();
    }
    // Tiedosto tiedot on väärin aseteltu
    catch(Common::FormatException &msg)
    {
        error_msg = msg.msg();
    }

    // Jos virhetekstiin tulee muutos lopetetaan peli
    if(error_msg != "")
    {
        QMessageBox msg_box;
        msg_box.setText("Tiedosto lukemisessa virhe.\n'" +
                        QString::fromStdString(error_msg) +
                        "'\nLopetetaan ohjelma!");
        msg_box.setWindowFlag(Qt::Sheet);
        msg_box.setWindowTitle("Virhe!");
        msg_box.QMessageBox::exec();
        return 1;
    }

    Student::Main_window window(&a, board, state, player_pointer_vector,
                                runner);
    window.Student::Main_window::show();

    return a.QApplication::exec();
}
