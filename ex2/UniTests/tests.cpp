#define CATCH_CONFIG_MAIN

#include "Card.h"
#include "Player.h"
#include "Mtmchkin.h"
#include "utilities.h"
#include "catch.hpp"

// using Catch::Matchers::ContainsSubstring;
// using Catch::Matchers::EndsWith;

TEST_CASE("PlayerCtor")
{

    SECTION("Onlyname")
    {
        Player player = Player("Omer");
        REQUIRE(player.getLevel() == 1);
        REQUIRE(player.getAttackStrength() == 6);
    }

    SECTION("NameHP")
    {
        Player player = Player("Omer", 2);
        REQUIRE(player.getLevel() == 1);
        REQUIRE(player.getAttackStrength() == 6);
    }

    SECTION("All")
    {
        Player player = Player("\n\n", 2, 14);
        REQUIRE(player.getLevel() == 1);
        REQUIRE(player.getAttackStrength() == 15);
    }

    SECTION("NegativeHP")
    {
        Player player = Player("", -2);
        REQUIRE(player.getLevel() == 1);
        REQUIRE(player.getAttackStrength() == 6);
    }

    SECTION("NegativeBoth")
    {
        Player player = Player("Omer", -10, -10);
        REQUIRE(player.getLevel() == 1);
        REQUIRE(player.getAttackStrength() == 6);
    }
}

TEST_CASE("PlayerLevelTests")
{
    Player player = Player("Omer");

    int i;
    for (i = 1; i < 10; i++)
    {
        REQUIRE(player.getLevel() == i);
        player.levelUp();
    }
    for (int j = 1; j < 10; j++)
    {
        REQUIRE(player.getLevel() == i);
        player.levelUp();
    }
}

TEST_CASE("PlayerForceTests")
{
    Player player = Player("Omer");
    player.buff(5);
    REQUIRE(player.getAttackStrength() - player.getLevel() == 10);
    player.buff(5);
    REQUIRE(player.getAttackStrength() - player.getLevel() == 15);
    player.buff(5);
    REQUIRE(player.getAttackStrength() - player.getLevel() == 20);
    player.buff(100);
    REQUIRE(player.getAttackStrength() - player.getLevel() == 120);
    player.buff(-5);
    REQUIRE(player.getAttackStrength() - player.getLevel() == 120);
    player.buff(0);
    REQUIRE(player.getAttackStrength() - player.getLevel() == 120);
    player.buff(100);
    REQUIRE(player.getAttackStrength() - player.getLevel() == 220);
    player.buff(-5);
    REQUIRE(player.getAttackStrength() - player.getLevel() == 220);
    player.buff(0);
    REQUIRE(player.getAttackStrength() - player.getLevel() == 220);
}

TEST_CASE("PlayerHPTests")
{
    // start hp == 105
    Player player = Player("Omer", 105, 5);
    player.heal(5);
    player.damage(100);
    REQUIRE(player.isKnockedOut() == false);
    player.damage(5);
    REQUIRE(player.isKnockedOut() == true);
    player.heal(10);
    REQUIRE(player.isKnockedOut() == false);
    player.heal(-5);
    REQUIRE(player.isKnockedOut() == false);
    player.damage(15);
    REQUIRE(player.isKnockedOut() == true);
    player.heal(3);
    REQUIRE(player.isKnockedOut() == false);
    player.damage(3);
    REQUIRE(player.isKnockedOut() == true);
    player.heal(300);
    REQUIRE(player.isKnockedOut() == false);
    player.damage(105);
    REQUIRE(player.isKnockedOut() == true);
}

TEST_CASE("PlayerCoinsTests")
{
    Player player = Player("Omer", 100, 5);
    REQUIRE(player.pay(10) == false);
    player.addCoins(10);
    REQUIRE(player.pay(3) == true);
    REQUIRE(player.pay(5) == true);
    REQUIRE(player.pay(10) == false);
    REQUIRE(player.pay(2) == true);
    player.addCoins(20);
    player.addCoins(-5);
    REQUIRE(player.pay(20) == true);
    REQUIRE(player.pay(-5) == true);
    REQUIRE(player.pay(0) == true);
    player.addCoins(0);
    player.addCoins(5);
    REQUIRE(player.pay(5) == true);
}
