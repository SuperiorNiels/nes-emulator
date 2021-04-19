#ifndef NES_TEST_H
#define NES_TEST_H

#include <iostream>
#include <vector>

class Test {
public:
    Test() : test(0), isTest(true) {};
    Test(const int test) : test(test), isTest(true) {};

    virtual void init() = 0;

    void print() const {
        std::cout << "This is a test: " << test << std::endl;
    }

    bool isTesting() const {
        return isTest;
    }

    virtual ~Test() {
        std::cout << "Test deconstruct" << std::endl;
    }

protected:
    int test;
    bool isTest;
};

class TestFactory {
public:
    virtual Test* createTest(const int test) const = 0; 
};

class ATest : public Test {
public:
    ATest() : Test() {};
    ATest(const int test) : Test(test) {}

    void init() override {
        std::cout << "init ATest" << std::endl;
        some_ptr = new std::vector<int>();
    }

    ~ATest() override {
        std::cout << "ATest deconstruct" << std::endl;
        delete some_ptr;
    }

private:
    std::vector<int>* some_ptr = nullptr;
};

class ATestFactory : public TestFactory {
public:
    ATest* createTest(const int test) const override {
        return new ATest(test);
    };
};

#endif //NES_TEST_H
