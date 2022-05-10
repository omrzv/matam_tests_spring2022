#include <functional>
#include <string>
#include <iostream>
#include <fstream>
#include "../Player.h"
#include "../Card.h"
#include "../Mtmchkin.h"


const int EXPECTED_NUM_OF_ARG = 3;
const int PATH_IN_FILE_POSITION = 1;
const int PATH_OUT_FILE_POSITION = 2;
const int MAX_TURNS = 1111;

const std::string BATTLE = "Battle";
const std::string BUFF = "Buff";
const std::string HEAL = "Heal";
const std::string TREASURE = "Treasure";

//*****************************************

int main(int argc, char **argv);


bool testMtmchkinFromFileToFile(std::ifstream& sourceFile, std::ofstream& destFile){    
    // Redirect cout
    std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
    std::cout.rdbuf(destFile.rdbuf()); //redirect std::cout to destFile
    
    // Read file info
    int cardsCount;
    if (!(sourceFile >> cardsCount))
    {
        std::cerr << "Invalid source file. Failed to read cards count." << std::endl;
        return false;
    }
    std::string playerName;
    if (!(sourceFile >> playerName))
    {
        std::cerr << "Invalid source file. Failed to read player name" << std::endl;
        return false;
    }

    // Read cards
    Card *cards = new Card[cardsCount];
    int cardIndex = 0; 
    int force, hpLoss, cost, heal, buff, loot;
    std::string type;
    CardType actuallType;
    while (cardIndex < cardsCount){
        if (!(sourceFile >> type)){
            std::cerr << "Invalid source file. Failed to read type. on card: " << cardIndex << std::endl;
            return false;
        }
        actuallType = type == BATTLE? CardType::Battle : (type == BUFF? CardType::Buff : (type == HEAL? CardType::Heal : CardType::Treasure));
        if (type != TREASURE && actuallType == CardType::Treasure){
            std::cerr << "Invalid source file. Recived invalid type. on card: " << cardIndex << std::endl;
            return false;
        }
        if (!(sourceFile >> force)){
            std::cerr << "Invalid source file. Failed to read force. on card: " << cardIndex << std::endl;
            return false;
        }
        if (!(sourceFile >> hpLoss)){
            std::cerr << "Invalid source file. Failed to read hpLoss. on card: " << cardIndex << std::endl;
            return false;
        }
        if (!(sourceFile >> cost)){
            std::cerr << "Invalid source file. Failed to read cost. on card: " << cardIndex << std::endl;
            return false;
        }
        if (!(sourceFile >> heal)){
            std::cerr << "Invalid source file. Failed to read heal. on card: " << cardIndex << std::endl;
            return false;
        }
        if (!(sourceFile >> buff)){
            std::cerr << "Invalid source file. Failed to read buff. on card: " << cardIndex << std::endl;
            return false;
        }
        if (!(sourceFile >> loot)){
            std::cerr << "Invalid source file. Failed to read loot. on card: " << cardIndex << std::endl;
            return false;
        }
        
        CardStats stats(force, hpLoss, cost, heal, buff, loot);
        cards[cardIndex] = Card(actuallType, stats);
        ++cardIndex;
    }

    // Run Game:
    Mtmchkin game(playerName.c_str(), cards, cardsCount);
    delete[] cards;
    int turns = 0;
    while(!game.isOver() && turns < MAX_TURNS){
        game.playNextCard();
        turns++;
    }
    if (turns == MAX_TURNS){
        std::cout << "The game was automaticly ended do to exceeding max turns (" << MAX_TURNS << ")." << std::endl;
    }
    switch (game.getGameStatus()){
        case GameStatus::Loss:
            std::cout << "The player was defeated... :(" << std::endl;
            break;
        case GameStatus::Win:
            std::cout << "The player has won! :D" << std::endl;
            break;
        case GameStatus::MidGame:
            std::cout << "The game is still running *~*" << std::endl;
            break;
    }

    std::cout.rdbuf(coutbuf); // Reset to cout to be the standard output buf again
    
    return true;
}


int main(int argc, char **argv)
{
    if (argc != EXPECTED_NUM_OF_ARG)
    {
        std::cout << "Usage: program <source file> <destination file>" << std::endl;
        return 0;
    }

    std::ifstream sourceFile(argv[PATH_IN_FILE_POSITION]);
    if (!sourceFile.is_open())
    {
        std::cerr << "Error: was unable to open source file at path:" << std::endl << argv[PATH_IN_FILE_POSITION] << std::endl;
        return 0;
    }

    std::ofstream destFile(argv[PATH_OUT_FILE_POSITION]);
    if (!destFile.is_open())
    {
        std::cerr << "Error: was unable to open dest file at path:" << std::endl << argv[PATH_OUT_FILE_POSITION] << std::endl;
        return 0;
    }

    testMtmchkinFromFileToFile(sourceFile, destFile);

    return 0;
}
