#include <brigadier/Dispatcher.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace test {

class MCommand : public brigadier::CommandNode {
public:
    MCommand(const std::string &name):
        CommandNode(name)
    {
    }
    MOCK_METHOD(void, execute, (), (override));
    MOCK_METHOD(void, help, (), (override));
    MOCK_METHOD(void, usage, (), (override));
};

class DispatcherTest : public testing::Test {
protected:
    void SetUp() override
    {
        dispatcher.registerCommand<MCommand>("test");
        // Called before first test
    }

    void TearDown() override
    {
        // Called after last test
    }

    brigadier::Dispatcher dispatcher;
    int _a = 42;
};

TEST_F(DispatcherTest, Parse)
{
    // auto results =  dispatcher.parse("test", _a);
    SUCCEED();
}

} // namespace test
