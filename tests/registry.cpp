#include "brigadier/CommandNodeBuilder.hpp"
#include "brigadier/exceptions.hpp"
#include "brigadier/parser/Number.hpp"
#include <brigadier/Registry.hpp>
#include <brigadier/TypeHolder.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

class Context {
public:
    MOCK_METHOD(void, call, ());
};

TEST(registryParsing, simpleCommandParse)
{
    using brigadier::CommandNodeBuilder;
    using brigadier::Registry;
    using brigadier::StringReader;
    using brigadier::TypeHolder;

    Registry registry;

    Context obj;

    registry.add(CommandNodeBuilder("test", "A test command").execute([](TypeHolder &ctx) {
        ctx.getAs<Context>().call();
    }));

    EXPECT_CALL(obj, call()).Times(1);

    auto testCommand = StringReader("test");

    EXPECT_TRUE(registry.isValidInput(testCommand));
    EXPECT_NO_THROW(registry.parse(obj, testCommand));
}

TEST(registryParsing, invalidCommand)
{
    using brigadier::CommandNodeBuilder;
    using brigadier::Registry;
    using brigadier::StringReader;
    using brigadier::TypeHolder;

    Registry registry;

    Context obj;

    registry.add(CommandNodeBuilder("test", "A test command").execute([](TypeHolder &ctx) {
        ctx.getAs<Context>().call();
    }));

    EXPECT_CALL(obj, call()).Times(0);

    auto testCommand = StringReader("test2");

    EXPECT_FALSE(registry.isValidInput(testCommand));
    EXPECT_THROW(registry.parse(obj, testCommand), brigadier::CommandSyntaxException);
}

TEST(registryParsing, simpleCommandParseWithArgs)
{
    using brigadier::CommandNodeBuilder;
    using brigadier::NumberParser;
    using brigadier::Registry;
    using brigadier::StringReader;
    using brigadier::TypeHolder;

    Registry registry;

    Context obj;

    registry.add(CommandNodeBuilder("test", "A test command").expectArg<NumberParser<int>>("int", "An integer argument").execute([](TypeHolder &ctx, int arg) {
        ctx.getAs<Context>().call();
    }));

    EXPECT_CALL(obj, call()).Times(1);

    auto testCommand = StringReader("test 15");

    EXPECT_TRUE(registry.isValidInput(testCommand));
    EXPECT_NO_THROW(registry.parse(obj, testCommand));
}

TEST(registryParsing, invalidCommandWithArgs)
{
    using brigadier::CommandNodeBuilder;
    using brigadier::NumberParser;
    using brigadier::Registry;
    using brigadier::StringReader;
    using brigadier::TypeHolder;

    Registry registry;

    Context obj;

    registry.add(CommandNodeBuilder("test", "A test command").expectArg<NumberParser<int>>("int", "An integer argument").execute([](TypeHolder &ctx, int arg) {
        ctx.getAs<Context>().call();
    }));

    EXPECT_CALL(obj, call()).Times(0);

    auto testCommand = StringReader("test aze");

    EXPECT_FALSE(registry.isValidInput(testCommand));
    EXPECT_THROW(registry.parse(obj, testCommand), brigadier::ReaderException);
}

TEST(registryParsing, simpleCommandParseWithArgsAndSubcommand)
{
    using brigadier::CommandNodeBuilder;
    using brigadier::NumberParser;
    using brigadier::Registry;
    using brigadier::StringReader;
    using brigadier::TypeHolder;

    Registry registry;

    Context obj;

    // clang-format off
    registry.add(CommandNodeBuilder("test", "A test command")
        .expectArg<NumberParser<int>>("int", "An integer argument")
        .execute([](TypeHolder &ctx, int arg) {
            ctx.getAs<Context>().call();
        })
        .add(CommandNodeBuilder("subcommand", "A subcommand")
            .expectArg<NumberParser<int>>("int", "An integer argument")
            .execute([](TypeHolder &ctx, int arg) {
                ctx.getAs<Context>().call();
            }
        )
    ));
    // clang-format on

    EXPECT_CALL(obj, call()).Times(2);

    auto testCommand = StringReader("test     14");
    auto testSubCommand = StringReader("test  subcommand 14");

    EXPECT_TRUE(registry.isValidInput(testCommand));
    EXPECT_TRUE(registry.isValidInput(testSubCommand));
    EXPECT_NO_THROW(registry.parse(obj, testCommand));
    EXPECT_NO_THROW(registry.parse(obj, testSubCommand));
}
