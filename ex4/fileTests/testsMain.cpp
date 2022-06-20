#include "../Mtmchkin.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>


static const int MAX_GAME_ROUNDS = 100;

void GeneralGameSimulationTest(std::string input_path, std::string deck_path, std::string res_path)
{
    //   init cin from file
    std::ifstream inputFile(input_path);
    if (!inputFile){
        std::cerr << "cant open " << input_path << std::endl;
    }
    std::streambuf *cinbuf = std::cin.rdbuf(); //save old buf
    std::cin.rdbuf(inputFile.rdbuf());

    std::ofstream outFile(res_path);
    std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
    std::cout.rdbuf(outFile.rdbuf());
    try
    {
        Mtmchkin game(deck_path);
        while(!game.isGameOver() && game.getNumberOfRounds() < MAX_GAME_ROUNDS){
            game.playRound();
            game.printLeaderBoard();
        }
    }
    catch (std::exception &e)
    {
        std::cout << e.what();
    }

    outFile.close();
    std::cin.rdbuf(cinbuf);
    std::cout.rdbuf(coutbuf);
}


int main(int argc, char** argv)
{
    if (argc != 4)
    {
        std::cerr << "Wrong amount of argumants for program, expected 4, got " << argc << std::endl;
        return 0;
    }
    else
    {
        GeneralGameSimulationTest(argv[1], argv[2], argv[3]);
        return 0;
    }
}
