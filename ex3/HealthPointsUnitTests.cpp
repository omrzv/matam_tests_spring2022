

#include <string>
#include <iostream>
#include "catch.hpp"
#include "relativeIncludes.h"


enum BOOL_OPERATORS {
    EQUAL,
    NOT_EQUAL,
    LESS_THAN,
    GREATER_THAN,
    LESS_THAN_OR_EQUAL,
    GREATER_THAN_OR_EQUAL,
    NUM_OF_OPERATORS
};

template<typename T>
bool make_comp(int op, T a, T b) {
    switch (op) {
        case EQUAL:
            return a == b;
        case NOT_EQUAL:
            return a != b;
        case LESS_THAN:
            return a < b;
        case GREATER_THAN:
            return a > b;
        case LESS_THAN_OR_EQUAL:
            return a <= b;
        case GREATER_THAN_OR_EQUAL:
            return a >= b;
        default:
            REQUIRE(false);// operator not implemented
            return false;
    }
}


void helperReadHP(const HealthPoints &hp, std::string &result)
{
    std::stringstream ss;
    ss << hp;
    ss >> result;
}

TEST_CASE("HealthBasics")
{

    SECTION("C'tor")
    {
        HealthPoints hp1 = HealthPoints(150);
        REQUIRE(hp1 == 150);

        HealthPoints hp2 = 1984; // Literally 1984  
        REQUIRE(hp2 == 1984);

        HealthPoints hp3;
        REQUIRE(hp3 == 100);


        REQUIRE_THROWS_AS(HealthPoints(0), HealthPoints::InvalidArgument);
        REQUIRE_THROWS_AS(HealthPoints(-2), HealthPoints::InvalidArgument);
    }

    SECTION("Addition And Subtruction")
    {
        HealthPoints healthPoints1; /* has 100 points out of 100 */
        HealthPoints healthPoints2(150); /* has 150 points out of 150 */
        
        healthPoints1 -= 20; /* now has 80 points out of 100 */
        REQUIRE(healthPoints1 == 80);

        healthPoints1 +=100; /* now has 100 points out of 100 */
        REQUIRE(healthPoints1 == 100);
        
        healthPoints1 -= 150; /* now has 0 points out of 100 */
        REQUIRE(healthPoints1 < 1);

        healthPoints2 = healthPoints2 - 160; /* now has 0 points out of 150 */
        REQUIRE(healthPoints2 < 1);
        
        healthPoints2 = 160 + healthPoints1; /* now has 100 out of 100 */
        REQUIRE(healthPoints2 == 100);
    }

    SECTION("Comparison")
    {
        bool comparisonResult;
        HealthPoints healthPoints1(100);
        HealthPoints healthPoints2 = 100; /* 100 points out of 100 */
        
        comparisonResult = (healthPoints1 == healthPoints2); /* returns true */
        REQUIRE(comparisonResult == true);

        healthPoints2 = HealthPoints(150); /* has 150 points out of 150 */
        comparisonResult = (healthPoints1 == healthPoints2); /* returns false */
        REQUIRE(comparisonResult == false);

        healthPoints2 -= 50; /* now has 100 points out of 150 */
        comparisonResult = (100 == healthPoints2); /* returns true */
        REQUIRE(comparisonResult == true);
        
        comparisonResult = (healthPoints1 < healthPoints2); /* returns false */
        REQUIRE(comparisonResult == false);

        healthPoints1 -= 50; /* now has 50 points out of 100 */
        comparisonResult = (healthPoints1 < healthPoints2); /* returns true */
        REQUIRE(comparisonResult == true);
    }

    SECTION("Printing Operator (<<)")
    {
        std::string result;

        HealthPoints healthPoints1; /* 100 points out of 100 */
        HealthPoints healthPoints2 = 42069; /* 42069 points out of 42069 */
        
        helperReadHP(healthPoints1, result);
        REQUIRE(result == "100(100)");

        helperReadHP(healthPoints2, result);
        REQUIRE(result == "42069(42069)");

        HealthPoints healthPoints3 = healthPoints2 - 42000; /* 69 points out of 42069 */
        helperReadHP(healthPoints3, result);
        REQUIRE(result == "69(42069)");
    }
}

TEST_CASE("Health Advanced C'tor")
{
    std::string result;
        
    SECTION("Full C'tors")
    {
        const HealthPoints hp1 = 2;
        helperReadHP(hp1, result);
        REQUIRE(result == "2(2)");

        const HealthPoints hp2 = hp1;
        helperReadHP(hp2, result);
        REQUIRE(result == "2(2)");

        const HealthPoints hp3 = HealthPoints(3);
        helperReadHP(hp3, result);
        REQUIRE(result == "3(3)");

        const HealthPoints hp4 = HealthPoints(hp3);
        helperReadHP(hp4, result);
        REQUIRE(result == "3(3)");

        HealthPoints hp5 = 4;
        helperReadHP(hp5, result);
        REQUIRE(result == "4(4)");

        HealthPoints hp6 = hp5;
        helperReadHP(hp6, result);
        REQUIRE(result == "4(4)");

        HealthPoints hp7 = HealthPoints(5);
        helperReadHP(hp7, result);
        REQUIRE(result == "5(5)");

        HealthPoints hp8 = HealthPoints(hp7);
        helperReadHP(hp8, result);
        REQUIRE(result == "5(5)");
        
        hp8 = 6;
        helperReadHP(hp8, result);
        REQUIRE(result == "6(6)");
    }

    SECTION("Dinamic copies")
    {   
        HealthPoints hp1 = 6;
        hp1 -= 3;
        HealthPoints hp2;

        hp2 = HealthPoints(hp1);
        helperReadHP(hp2, result);
        REQUIRE(result == "3(6)");
        
        HealthPoints hp3 = hp1;
        helperReadHP(hp3, result);
        REQUIRE(result == "3(6)");
        
        hp2 = hp1;
        helperReadHP(hp2, result);
        REQUIRE(result == "3(6)");
        
        HealthPoints hp4(hp1);
        helperReadHP(hp4, result);
        REQUIRE(result == "3(6)");

        hp1 -= 3;
        hp2 = HealthPoints(hp1);
        helperReadHP(hp2, result);
        REQUIRE(result == "0(6)");

        HealthPoints hp5 = HealthPoints(hp1);
        helperReadHP(hp5, result);
        REQUIRE(result == "0(6)");

        hp2 = hp1;
        helperReadHP(hp2, result);
        REQUIRE(result == "0(6)");

        HealthPoints hp6(hp1);
        helperReadHP(hp6, result);
        REQUIRE(result == "0(6)");

        hp1 += 6;
        hp2 = HealthPoints(hp1);
        helperReadHP(hp2, result);
        REQUIRE(result == "6(6)");

        HealthPoints hp7 = hp1;
        helperReadHP(hp7, result);
        REQUIRE(result == "6(6)");

        HealthPoints hp8(hp1);
        helperReadHP(hp8, result);
        REQUIRE(result == "6(6)");

        hp2 = hp1;
        helperReadHP(hp2, result);
        REQUIRE(result == "6(6)");

        hp1 += 6;
        hp2 = HealthPoints(hp1);
        helperReadHP(hp2, result);
        REQUIRE(result == "6(6)");

        hp2 = hp1;
        helperReadHP(hp2, result);
        REQUIRE(result == "6(6)");

        HealthPoints hp9 = HealthPoints(hp1);
        helperReadHP(hp9, result);
        REQUIRE(result == "6(6)");

        HealthPoints hp10(hp1);
        helperReadHP(hp10, result);
        REQUIRE(result == "6(6)");
    }

    SECTION("InvlaidArguments")
    {
        for (int i = 0; i > -10000000; i -= i * i + 1)
        {
            REQUIRE_THROWS_AS(HealthPoints(i), HealthPoints::InvalidArgument);
        }

        HealthPoints hp;
        REQUIRE_THROWS_AS(hp = -1, HealthPoints::InvalidArgument);
        REQUIRE_THROWS_AS(hp = HealthPoints(0), HealthPoints::InvalidArgument);
    }
}

TEST_CASE("Health + and - Operations")
{
    SECTION("All Operators")
    {
        std::string result;
        
        HealthPoints hp1 = 1;
        helperReadHP(hp1, result);
        REQUIRE(result == "1(1)");

        hp1 += 1;
        helperReadHP(hp1, result);
        REQUIRE(result == "1(1)");
        
        helperReadHP(hp1 + 1, result);
        REQUIRE(result == "1(1)");

        helperReadHP(1 + hp1, result);
        REQUIRE(result == "1(1)");

        HealthPoints hp2 = 80085;
        helperReadHP(hp2, result);
        REQUIRE(result == "80085(80085)");

        hp2 -= 80000;
        helperReadHP(hp2, result);
        REQUIRE(result == "85(80085)");
        
        hp2 -= 80;
        helperReadHP(hp2, result);
        REQUIRE(result == "5(80085)");
        
        hp2 -= 80;
        helperReadHP(hp2, result);
        REQUIRE(result == "0(80085)");
        
        hp2 -= 1;
        helperReadHP(hp2, result);
        REQUIRE(result == "0(80085)");

        hp2 += 1;
        helperReadHP(hp2, result);
        REQUIRE(result == "1(80085)");
        
        hp2 += 80;
        helperReadHP(hp2, result);
        REQUIRE(result == "81(80085)");
        
        hp2 += 8000;
        helperReadHP(hp2, result);
        REQUIRE(result == "8081(80085)");

        hp2 += 80000;
        helperReadHP(hp2, result);
        REQUIRE(result == "80085(80085)");
        
        hp2 -= 3;
        helperReadHP(hp2, result);
        REQUIRE(result == "80082(80085)");

        hp2 += 3;
        helperReadHP(hp2, result);
        REQUIRE(result == "80085(80085)");

        hp2 -= 80085;
        helperReadHP(hp2, result);
        REQUIRE(result == "0(80085)");
        
        hp1 = hp2 + 100;
        helperReadHP(hp1, result);
        REQUIRE(result == "100(80085)");
        
        hp1 = hp1 - 1;
        helperReadHP(hp1, result);
        REQUIRE(result == "99(80085)");
        
        hp1 = hp1 - 1;
        helperReadHP(hp1, result);
        REQUIRE(result == "98(80085)");
        
        hp1 = hp1 + 1;
        helperReadHP(hp1, result);
        REQUIRE(result == "99(80085)");
        
        hp1 = hp1 + 1;
        helperReadHP(hp1, result);
        REQUIRE(result == "100(80085)");

        hp1 = 1 + hp1;
        helperReadHP(hp1, result);
        REQUIRE(result == "101(80085)");
        
        hp1 = 1 + hp1;
        helperReadHP(hp1, result);
        REQUIRE(result == "102(80085)");
        
        hp1 = hp2 + 90000;
        helperReadHP(hp1, result);
        REQUIRE(result == "80085(80085)");
        
        hp1 = 90000 + hp2;
        helperReadHP(hp1, result);
        REQUIRE(result == "80085(80085)");
        
        hp2 = hp1 - 80000;
        helperReadHP(hp2, result);
        REQUIRE(result == "85(80085)");
        
        hp1 = hp2 - 90000;
        helperReadHP(hp1, result);
        REQUIRE(result == "0(80085)");

        hp1 = hp2 + (-2);
        helperReadHP(hp1, result);
        REQUIRE(result == "83(80085)");
        
        hp1 = (-84) + hp2;
        helperReadHP(hp1, result);
        REQUIRE(result == "1(80085)");
        
        hp1 -= (-1023);
        helperReadHP(hp1, result);
        REQUIRE(result == "1024(80085)");

        hp1 += (-512);
        helperReadHP(hp1, result);
        REQUIRE(result == "512(80085)");

        hp1 = hp1 - (-512);
        helperReadHP(hp1, result);
        REQUIRE(result == "1024(80085)");        


        hp1 = hp2 + (-1025);
        helperReadHP(hp1, result);
        REQUIRE(result == "0(80085)");
        
        hp1 = (-84) + hp1;
        helperReadHP(hp1, result);
        REQUIRE(result == "0(80085)");
        
        hp1 -= (-80086);
        helperReadHP(hp1, result);
        REQUIRE(result == "80085(80085)");

        hp1 += (-100000);
        helperReadHP(hp1, result);
        REQUIRE(result == "0(80085)");

        hp1 = hp1 - (-100000);
        helperReadHP(hp1, result);
        REQUIRE(result == "80085(80085)");
        
        hp1 -= 80045;

        hp1 += 0;
        helperReadHP(hp1, result);
        REQUIRE(result == "40(80085)");

        hp1 -= 0;
        helperReadHP(hp1, result);
        REQUIRE(result == "40(80085)");

        hp1 = hp1 + 0;
        helperReadHP(hp1, result);
        REQUIRE(result == "40(80085)");

        hp1 = 0 + hp1;
        helperReadHP(hp1, result);
        REQUIRE(result == "40(80085)");

        hp1 = hp1 - 0;
        helperReadHP(hp1, result);
        REQUIRE(result == "40(80085)");
    }
}

TEST_CASE("Compering Operations")
{
    bool result;
    HealthPoints hp0outof1 = 1;
    hp0outof1 -= 1;
    HealthPoints hp2outof2 = 2;
    HealthPoints hp2outof3 = 3;
    hp2outof3 -= 1;
    HealthPoints hp3outof3 = 3;
    SECTION("'<' operator")
    {
        result = hp0outof1 < hp0outof1;
        REQUIRE(result == false);
        
        result = hp0outof1 < hp2outof2;
        REQUIRE(result == true);

        result = hp0outof1 < hp2outof3;
        REQUIRE(result == true);
        
        result = hp0outof1 < hp3outof3;
        REQUIRE(result == true);

        result = hp2outof2 < hp0outof1;
        REQUIRE(result == false);

        result = hp2outof2 < hp2outof2;
        REQUIRE(result == false);

        result = hp2outof2 < hp2outof3;
        REQUIRE(result == false);
        
        result = hp2outof2 < hp3outof3;
        REQUIRE(result == true);

        result = hp2outof3 < hp0outof1;
        REQUIRE(result == false);

        result = hp2outof3 < hp2outof2;
        REQUIRE(result == false);

        result = hp2outof3 < hp2outof3;
        REQUIRE(result == false);
        
        result = hp2outof3 < hp3outof3;
        REQUIRE(result == true);
        
        result = hp3outof3 < hp0outof1;
        REQUIRE(result == false);

        result = hp3outof3 < hp2outof2;
        REQUIRE(result == false);

        result = hp3outof3 < hp2outof3;
        REQUIRE(result == false);
        
        result = hp3outof3 < hp3outof3;
        REQUIRE(result == false);
    }
    
    SECTION("'<=' operator")
    {
        result = hp0outof1 <= hp0outof1;
        REQUIRE(result == true);
        
        result = hp0outof1 <= hp2outof2;
        REQUIRE(result == true);

        result = hp0outof1 <= hp2outof3;
        REQUIRE(result == true);
        
        result = hp0outof1 <= hp3outof3;
        REQUIRE(result == true);

        result = hp2outof2 <= hp0outof1;
        REQUIRE(result == false);

        result = hp2outof2 <= hp2outof2;
        REQUIRE(result == true);

        result = hp2outof2 <= hp2outof3;
        REQUIRE(result == true);
        
        result = hp2outof2 <= hp3outof3;
        REQUIRE(result == true);

        result = hp2outof3 <= hp0outof1;
        REQUIRE(result == false);

        result = hp2outof3 <= hp2outof2;
        REQUIRE(result == true);

        result = hp2outof3 <= hp2outof3;
        REQUIRE(result == true);
        
        result = hp2outof3 <= hp3outof3;
        REQUIRE(result == true);
        
        result = hp3outof3 <= hp0outof1;
        REQUIRE(result == false);

        result = hp3outof3 <= hp2outof2;
        REQUIRE(result == false);

        result = hp3outof3 <= hp2outof3;
        REQUIRE(result == false);
        
        result = hp3outof3 <= hp3outof3;
        REQUIRE(result == true);
    }
    
    SECTION("'==' operator")
    {
        result = hp0outof1 == hp0outof1;
        REQUIRE(result == true);
        
        result = hp0outof1 == hp2outof2;
        REQUIRE(result == false);

        result = hp0outof1 == hp2outof3;
        REQUIRE(result == false);
        
        result = hp0outof1 == hp3outof3;
        REQUIRE(result == false);

        result = hp2outof2 == hp0outof1;
        REQUIRE(result == false);

        result = hp2outof2 == hp2outof2;
        REQUIRE(result == true);

        result = hp2outof2 == hp2outof3;
        REQUIRE(result == true);
        
        result = hp2outof2 == hp3outof3;
        REQUIRE(result == false);

        result = hp2outof3 == hp0outof1;
        REQUIRE(result == false);

        result = hp2outof3 == hp2outof2;
        REQUIRE(result == true);

        result = hp2outof3 == hp2outof3;
        REQUIRE(result == true);
        
        result = hp2outof3 == hp3outof3;
        REQUIRE(result == false);
        
        result = hp3outof3 == hp0outof1;
        REQUIRE(result == false);

        result = hp3outof3 == hp2outof2;
        REQUIRE(result == false);

        result = hp3outof3 == hp2outof3;
        REQUIRE(result == false);
        
        result = hp3outof3 == hp3outof3;
        REQUIRE(result == true);
    }
    
    SECTION("'!=' operator")
    {
        result = hp0outof1 != hp0outof1;
        REQUIRE(result == false);
        
        result = hp0outof1 != hp2outof2;
        REQUIRE(result == true);

        result = hp0outof1 != hp2outof3;
        REQUIRE(result == true);
        
        result = hp0outof1 != hp3outof3;
        REQUIRE(result == true);

        result = hp2outof2 != hp0outof1;
        REQUIRE(result == true);

        result = hp2outof2 != hp2outof2;
        REQUIRE(result == false);

        result = hp2outof2 != hp2outof3;
        REQUIRE(result == false);
        
        result = hp2outof2 != hp3outof3;
        REQUIRE(result == true);

        result = hp2outof3 != hp0outof1;
        REQUIRE(result == true);

        result = hp2outof3 != hp2outof2;
        REQUIRE(result == false);

        result = hp2outof3 != hp2outof3;
        REQUIRE(result == false);
        
        result = hp2outof3 != hp3outof3;
        REQUIRE(result == true);
        
        result = hp3outof3 != hp0outof1;
        REQUIRE(result == true);

        result = hp3outof3 != hp2outof2;
        REQUIRE(result == true);

        result = hp3outof3 != hp2outof3;
        REQUIRE(result == true);
        
        result = hp3outof3 != hp3outof3;
        REQUIRE(result == false);
    }
    
    SECTION("'>' operator")
    {
        result = hp0outof1 > hp0outof1;
        REQUIRE(result == false);
        
        result = hp0outof1 > hp2outof2;
        REQUIRE(result == false);

        result = hp0outof1 > hp2outof3;
        REQUIRE(result == false);
        
        result = hp0outof1 > hp3outof3;
        REQUIRE(result == false);

        result = hp2outof2 > hp0outof1;
        REQUIRE(result == true);

        result = hp2outof2 > hp2outof2;
        REQUIRE(result == false);

        result = hp2outof2 > hp2outof3;
        REQUIRE(result == false);
        
        result = hp2outof2 > hp3outof3;
        REQUIRE(result == false);

        result = hp2outof3 > hp0outof1;
        REQUIRE(result == true);

        result = hp2outof3 > hp2outof2;
        REQUIRE(result == false);

        result = hp2outof3 > hp2outof3;
        REQUIRE(result == false);
        
        result = hp2outof3 > hp3outof3;
        REQUIRE(result == false);
        
        result = hp3outof3 > hp0outof1;
        REQUIRE(result == true);

        result = hp3outof3 > hp2outof2;
        REQUIRE(result == true);

        result = hp3outof3 > hp2outof3;
        REQUIRE(result == true);
        
        result = hp3outof3 > hp3outof3;
        REQUIRE(result == false);
        
    }
    
    SECTION("'>=' operator")
    {
        result = hp0outof1 >= hp0outof1;
        REQUIRE(result == true);
        
        result = hp0outof1 >= hp2outof2;
        REQUIRE(result == false);

        result = hp0outof1 >= hp2outof3;
        REQUIRE(result == false);
        
        result = hp0outof1 >= hp3outof3;
        REQUIRE(result == false);

        result = hp2outof2 >= hp0outof1;
        REQUIRE(result == true);

        result = hp2outof2 >= hp2outof2;
        REQUIRE(result == true);

        result = hp2outof2 >= hp2outof3;
        REQUIRE(result == true);
        
        result = hp2outof2 >= hp3outof3;
        REQUIRE(result == false);

        result = hp2outof3 >= hp0outof1;
        REQUIRE(result == true);

        result = hp2outof3 >= hp2outof2;
        REQUIRE(result == true);

        result = hp2outof3 >= hp2outof3;
        REQUIRE(result == true);
        
        result = hp2outof3 >= hp3outof3;
        REQUIRE(result == false);
        
        result = hp3outof3 >= hp0outof1;
        REQUIRE(result == true);

        result = hp3outof3 >= hp2outof2;
        REQUIRE(result == true);

        result = hp3outof3 >= hp2outof3;
        REQUIRE(result == true);
        
        result = hp3outof3 >= hp3outof3;
        REQUIRE(result == true);
    }
}

TEST_CASE("HPBooleanOperators") {

    for (int operator_number = 0; operator_number < NUM_OF_OPERATORS; ++operator_number) {
        for (int i = 1; i < 3; i++) {
            for (int j = 1; j < 3; j++) {
                int a = i;
                int b = j;
                HealthPoints hp_a(a);
                HealthPoints hp_b(b);
                REQUIRE(make_comp(operator_number, hp_a, hp_b) == make_comp(operator_number, a, b));
            }
        }
    }
}


TEST_CASE("HPArithmeticOperatorEquals") {
    SECTION("OperatorEqualsWithInt") {
        // Can't access HPs private members, so we'll check them by comparing them to another HP object
        SECTION("PlusEqualsWithInt") {
            int a_max_hp = 555;
            int a_current_hp = 555;
            int b_max_hp = 555;
            int b_current_hp = 555;
            HealthPoints a(a_max_hp);
            HealthPoints b(b_max_hp);
            SECTION("IncreaseOverMaxHP") {
                a += a_current_hp;
                REQUIRE(a == b);
            }
            a -= a_current_hp;
            a_current_hp = 0;
            b -= b_current_hp;
            b_current_hp = 0;
            SECTION("IncreaseWhenZero") {
                a += 1;
                a_current_hp = 1;
                REQUIRE(a > b);
            }
            a -= a_current_hp;
            a_current_hp = 0;
            SECTION("IncreaseNegativeValue") {
                a += -100;
                a_current_hp = 0; // since can't be negative
                REQUIRE(a == b);
            }
        }SECTION("MinusEqualsWithInt") {
            int a_max_hp = 555;
            int a_current_hp = 555;
            int b_max_hp = 555;
            int b_current_hp = 555;
            HealthPoints a(a_max_hp);
            HealthPoints b(b_max_hp);
            REQUIRE(a == b);
            b -= 1;
            b_current_hp -= 1;
            REQUIRE(a > b); // test decrease while not under 0
            b -= b_current_hp;
            b_current_hp = 0;

            REQUIRE(a > b); // test decrease under 0
            b -= 10;
            b_current_hp = 0;
            REQUIRE(a > b);
            a -= a_current_hp;
            a_current_hp = 0;
            REQUIRE(a == b);
            a -= 333;
            a_current_hp = 0;
            REQUIRE(a == b);
        }
    }

}


TEST_CASE("HPArithmeticOperators") {
    SECTION("OperatorPlusWithInt") {
        int a_max_hp = 555;
        int a_current_hp = a_max_hp;
        HealthPoints a(a_max_hp);
        HealthPoints b(a);
        REQUIRE(a == b);
        b = a + 1;
        REQUIRE(a == b);// can't increase over max hp
        a = a - a_current_hp;
        a_current_hp = 0;
        while (a_current_hp < a_max_hp) {
            a = a + 1;
            a_current_hp += 1;
            b = HealthPoints(a_current_hp);
            REQUIRE(a == b);
        }
        a = a - 10;
        a_current_hp -= 10;
        b = HealthPoints(a_max_hp);
        int b_max_hp = a_max_hp;
        while (a_current_hp + 1 < b_max_hp) {
            a = a + 1;
            a_current_hp += 1;
            b = HealthPoints(a_max_hp);
            REQUIRE(a < b);
        }
        a = a + 1;
        a_current_hp += 1;
        REQUIRE(a == b);
        SECTION("IncreaseNegativeValue") {
            a = HealthPoints(a_max_hp);
            a_current_hp = a_max_hp;
            a = a + -100;
            a_current_hp -= 100;
            b = HealthPoints(a_current_hp);
            REQUIRE(a == b);
            a = a - 2 * a_max_hp;
            b = b - b_max_hp;
            REQUIRE(a == b);
        }
    }
}

TEST_CASE("IntHPArithmeticOperation") {
    HealthPoints a = HealthPoints(100);
    HealthPoints b;
    b = a + 100;
    b = a - 100;
    b = 100 + a;
    b = -100 + a;
    REQUIRE(a > b);
    a -= 100;
    REQUIRE(a == b);
}

TEST_CASE("HPCreation") {
    HealthPoints a = HealthPoints(); // default constructor, DEFAULT_MAX_HP
    HealthPoints b = HealthPoints(100);
    REQUIRE(a == b);
    b = HealthPoints(100); 
    REQUIRE(a == b);
    for (int i = 1; i <= 100; i++) {// assignment operator
        a = HealthPoints(i);
        b = a;
        REQUIRE(a == b);
    }
    for (int i = 1; i <= 100; i++) {// copy constructor
        a = HealthPoints(i);
        b = HealthPoints(a);
        REQUIRE(a == b);
    }
    for (int i = 1; i <= 100; i++) {// creation with int
        a = HealthPoints(i);
        b = HealthPoints(i);
        REQUIRE(a == b);
    }
}