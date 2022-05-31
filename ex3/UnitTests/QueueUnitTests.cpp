

#include <string>
#include <iostream>
#include <vector>
#include "catch.hpp"
#include "relativeIncludes.h"


template <class T>
void readQueue(std::string& string, Queue<T> &q)
{
    std::stringstream ss;

    bool first = true;
    ss << "{" ;
    for (T& data : q)
    {
        ss << (first ? "" : ", ") << data;
        first = false;
    }
    ss << "}";
    string = ss.str();
}

static bool isPrime(const int n)
{
    if (n < 2 || (n % 2 == 0 && n != 2))
    {
        return false;
    }
    for (int i = 3; i * i <= n; i += 2)
    {
        if (n % i == 0)
        {
            return false;
        }
    }
    return true;
}

static void setSixtyNine(int& n)
{
    n = 69;
}


class ControlledAllocer{
public:
    static int allowedAllocs;
    int someInteger;

    ControlledAllocer()
    {
        if (!allowedAllocs){
            throw std::bad_alloc();
        }
        --allowedAllocs;
    }
    
    ControlledAllocer(const ControlledAllocer &other)
    {
        if (!allowedAllocs){
            throw std::bad_alloc();
        }
        --allowedAllocs;
    }

    ControlledAllocer &operator=(const ControlledAllocer &other)
    {
        if (!allowedAllocs){
            throw std::bad_alloc();
        }
        --allowedAllocs;
        return *this;
    }
};
int ControlledAllocer::allowedAllocs;

TEST_CASE("Queue Basics")
{
    SECTION("int Queue"){
        Queue<int> q;

        REQUIRE(q.size() == 0);
        REQUIRE_THROWS_AS(q.front(), Queue<int>::EmptyQueue);
        REQUIRE_THROWS_AS(q.popFront(), Queue<int>::EmptyQueue);

        q.pushBack(0);
        REQUIRE(q.size() == 1);
        REQUIRE(q.front() == 0);
        q.front() = 1;
        REQUIRE(q.front() == 1);

        q.popFront();
        REQUIRE(q.size() == 0);
        REQUIRE_THROWS_AS(q.front(), Queue<int>::EmptyQueue);
        REQUIRE_THROWS_AS(q.popFront(), Queue<int>::EmptyQueue);
        
        // Fill The Queue
        for (int i = 0; i < 1984; i++){
            q.pushBack(i);
            REQUIRE(q.size() == i + 1);
        }

        Queue<int> primesQ = filter(q, isPrime);
        REQUIRE(primesQ.size() == 299); // I counted, trust me.
        for (int prime: primesQ)
        {
            REQUIRE(isPrime(prime));
        }

        REQUIRE(primesQ.front() == 2);

        Queue<int> sixtyNineQ = primesQ;
        transform(sixtyNineQ, setSixtyNine);
        REQUIRE(sixtyNineQ.size() == 299); // I counted, trust me.
        for (int sixtyNine: sixtyNineQ)
        {
            REQUIRE(sixtyNine == 69);
        }
        
        const Queue<int> constPrimesQ = filter(q, isPrime);
        REQUIRE(constPrimesQ.size() == 299); // I counted, trust me.
        for (const int prime: constPrimesQ)
        {
            REQUIRE(isPrime(prime));
        }
        REQUIRE(constPrimesQ.front() == 2); // Because this is the first prime; declare: const T& Q<t>::front() const;

        // Empty the Queue
        for (int i = 0; i < 1984; i++){
            REQUIRE(q.front() == i);
            q.popFront();
            REQUIRE(q.size() == 1983 - i);
        }

        Queue<int>::Iterator endIterator = q.end();
        REQUIRE_THROWS_AS(++endIterator, Queue<int>::Iterator::InvalidOperation);

        Queue<int>::ConstIterator constEndIterator = constPrimesQ.end();
        REQUIRE_THROWS_AS(++constEndIterator, Queue<int>::ConstIterator::InvalidOperation);

        REQUIRE_THROWS_AS(q.front(), Queue<int>::EmptyQueue);
        REQUIRE_THROWS_AS(q.popFront(), Queue<int>::EmptyQueue);
    }

    SECTION("vector Queue"){
        Queue<std::vector<char>> q;
        std::vector<char> v1 = {'O', 'A'};
        q.pushBack(v1);
        REQUIRE(q.size() == 1);
        REQUIRE(q.front()[0] == 'O');
        REQUIRE(q.front()[1] == 'A');
        v1[0] = 'A';

        REQUIRE(q.front()[0] == 'O');
        REQUIRE(q.front()[1] == 'A');

        q.front()[1] = 'O';
        
        REQUIRE(v1[1] == 'A');

        std::vector<char> v2 = {'O', 'A', 'O'};
        q.pushBack(v2);
        REQUIRE(q.size() == 2);
        
        q.popFront();
        REQUIRE(q.size() == 1);
        REQUIRE(q.front()[0] == 'O');
        REQUIRE(q.front()[1] == 'A');
        REQUIRE(q.front()[2] == 'O');
        REQUIRE(q.front().size() == 3);
        
        q.front()[0] = 'A';
        v2[2] = 'A';
        REQUIRE(q.front()[2] == 'O');
        REQUIRE(v2[0] == 'O');

        q.front().push_back('Z');

        REQUIRE(q.front()[0] == 'A');
        REQUIRE(q.front()[1] == 'A');
        REQUIRE(q.front()[2] == 'O');
        REQUIRE(q.front()[3] == 'Z');
        REQUIRE(q.front().size() == 4);

        q.front()[0] = ' ';
        q.front()[1] = ' ';
        q.front()[2] = ' ';
        
        Queue<std::vector<char>> q1(q), q2;
        q2 = q;
        const Queue<std::vector<char>> q3 = q;

        q.front()[0] = '0';
        q1.front()[1] = '1';
        q2.front()[2] = '2';
        
        REQUIRE(q.front()[0] == '0');
        REQUIRE(q.front()[1] == ' ');
        REQUIRE(q.front()[2] == ' ');

        REQUIRE(q1.front()[0] == ' ');
        REQUIRE(q1.front()[1] == '1');
        REQUIRE(q1.front()[2] == ' ');

        REQUIRE(q2.front()[0] == ' ');
        REQUIRE(q2.front()[1] == ' ');
        REQUIRE(q2.front()[2] == '2');

        REQUIRE(q3.front()[0] == ' ');
        REQUIRE(q3.front()[1] == ' ');
        REQUIRE(q3.front()[2] == ' ');
    }
}

TEST_CASE("Queue Advanced")
{
    SECTION("Proper Coping")
    {
        Queue<double> q1, q2;
        for (double i = 0.0; i < 5; i += 0.5)
        {
            q1.pushBack(i);
        }
        q2 = q1;
        Queue<double> q3(q1);

        REQUIRE(q1.front() == 0.0);
        REQUIRE(q2.front() == 0.0);
        REQUIRE(q3.front() == 0.0);

        q1.front() = 10.0;
        REQUIRE(q1.front() == 10.0);
        REQUIRE(q2.front() == 0.0);
        REQUIRE(q3.front() == 0.0);
        
        q2.front() = 11.0;
        REQUIRE(q1.front() == 10.0);
        REQUIRE(q2.front() == 11.0);
        REQUIRE(q3.front() == 0.0);
        
        q2.popFront();

        REQUIRE(q1.size() == 10);
        REQUIRE(q2.size() == 9);
        REQUIRE(q3.size() == 10);

        q3.front() = q2.front();
        REQUIRE(q1.front() == 10.0);
        REQUIRE(q2.front() == 0.5);
        REQUIRE(q3.front() == 0.5);
        
        double &q3_front = q3.front();
        q3_front += 3.0;

        REQUIRE(q1.front() == 10.0);
        REQUIRE(q2.front() == 0.5);
        REQUIRE(q3.front() == 3.5);

        REQUIRE(q1.size() == 10);
        REQUIRE(q2.size() == 9);
        REQUIRE(q3.size() == 10);
        
        q2.popFront();
        q1.popFront();
        
        REQUIRE(q1.size() == 9);
        REQUIRE(q2.size() == 8);
        REQUIRE(q3.size() == 10);

        const Queue<double> q4 = q2;
        
        q2.front() = 15.0;

        REQUIRE(q4.front() == 1.0);
    }

    SECTION("Bad Allocs: weird section :3")
    {
        ControlledAllocer::allowedAllocs = 1000;
        
        Queue<ControlledAllocer> q1, q2;
        ControlledAllocer c;
        for (int i = 0; i < 10; i++)
        {
            q1.pushBack(c);
        }

        ControlledAllocer::allowedAllocs = 0;

        REQUIRE_THROWS_AS(q1.pushBack(c), std::bad_alloc);

        REQUIRE_THROWS_AS(q2.pushBack(c), std::bad_alloc);

        ControlledAllocer::allowedAllocs = 5;

        REQUIRE_THROWS_AS(q2 = q1, std::bad_alloc);

        auto newCopyBadAllocerQ = [](Queue<ControlledAllocer> q)
        {
            Queue<ControlledAllocer> newQ(q);
        };
        ControlledAllocer::allowedAllocs = 5;
        REQUIRE_THROWS_AS(newCopyBadAllocerQ(q1), std::bad_alloc);
        
        auto newConstCopyBadAllocerQ = [](Queue<ControlledAllocer> q)
        {
            const Queue<ControlledAllocer> newQ(q);
        };
        ControlledAllocer::allowedAllocs = 5;
        REQUIRE_THROWS_AS(newConstCopyBadAllocerQ(q1), std::bad_alloc);

        q1.front().someInteger = 666;
        ControlledAllocer::allowedAllocs = 1000;
        Queue<ControlledAllocer> shortQ;
        for (int i = 0; i < 5; i++)
        {
            shortQ.pushBack(c);
        }
        
        shortQ.front().someInteger = -1;

        ControlledAllocer::allowedAllocs = 2;
        REQUIRE_THROWS_AS(q1 = shortQ, std::bad_alloc); // Use failing assainment operator
        REQUIRE(q1.size() == 10); // Check that q1 wasn't changes
        
        REQUIRE(q1.front().someInteger == 666); // Check that q1 wasn't changes

        int counter = 0;
        for (ControlledAllocer& data: q1){
            data.someInteger = 1;
            ++counter;
        }
        REQUIRE(counter == 10); // Check that q1 wasn't changes
    }
    SECTION("Mixed Operations (with HealthPoints")
    {
        Queue<HealthPoints> healthyQ;
        for (int i = 1; i < 5; ++i)
        {
            healthyQ.pushBack(i);
        }

        int i = 1;
        for (HealthPoints &hp : healthyQ)
        {
            hp -= (i++) % 3;
        }

        std::string result;
        readQueue(result, healthyQ);
        std::string expected = "{0(1), 0(2), 3(3), 3(4)}";
        REQUIRE(result == expected);

        healthyQ.popFront();
        healthyQ.pushBack(healthyQ.front());
        healthyQ.popFront();
        readQueue(result, healthyQ);
        expected = "{3(3), 3(4), 0(2)}";
        REQUIRE(result == expected);
        
        healthyQ.popFront();
        healthyQ.pushBack(healthyQ.front());
        healthyQ.popFront();
        readQueue(result, healthyQ);
        expected = "{0(2), 3(4)}";
        REQUIRE(result == expected);

        healthyQ.popFront();
        healthyQ.pushBack(healthyQ.front());
        healthyQ.popFront();
        readQueue(result, healthyQ);
        expected = "{3(4)}";
        REQUIRE(result == expected);

        for (int i = 5; i < 10; ++i)
        {
            healthyQ.pushBack(i);
        }

        auto isGreaterThen7 = [](const HealthPoints &hp)
        {
            return hp > 7;
        };
        Queue<HealthPoints> filterHealthQ = filter(healthyQ, isGreaterThen7);
        readQueue(result, filterHealthQ);
        expected = "{8(8), 9(9)}";
        REQUIRE(result == expected);
        
        filterHealthQ.pushBack(11);
        healthyQ.pushBack(12);
        filterHealthQ.popFront();

        readQueue(result, filterHealthQ);
        expected = "{9(9), 11(11)}";
        REQUIRE(result == expected);
        
        readQueue(result, healthyQ);
        expected = "{3(4), 5(5), 6(6), 7(7), 8(8), 9(9), 12(12)}";
        REQUIRE(result == expected);

        
        auto remove2HP = [](HealthPoints &hp)
        {
            hp = (-1) + hp;
            hp = hp + (-1);
        };
        transform(healthyQ, remove2HP);
        readQueue(result, healthyQ);
        expected = "{1(4), 3(5), 4(6), 5(7), 6(8), 7(9), 10(12)}";
        REQUIRE(result == expected);
        
        healthyQ.popFront();
        readQueue(result, healthyQ);
        expected = "{3(5), 4(6), 5(7), 6(8), 7(9), 10(12)}";
        REQUIRE(result == expected);
        
        healthyQ.pushBack(filterHealthQ.front());
        readQueue(result, healthyQ);
        expected = "{3(5), 4(6), 5(7), 6(8), 7(9), 10(12), 9(9)}";
        REQUIRE(result == expected);
        
        transform(healthyQ, remove2HP);
        readQueue(result, healthyQ);
        expected = "{1(5), 2(6), 3(7), 4(8), 5(9), 8(12), 7(9)}";
        REQUIRE(result == expected);

        readQueue(result, filterHealthQ);
        expected = "{9(9), 11(11)}";
        REQUIRE(result == expected);

        filterHealthQ = filter(healthyQ, isGreaterThen7);

        readQueue(result, filterHealthQ);
        expected = "{8(12)}";
        REQUIRE(result == expected);

        readQueue(result, healthyQ);
        expected = "{1(5), 2(6), 3(7), 4(8), 5(9), 8(12), 7(9)}";
        REQUIRE(result == expected);
    }
}