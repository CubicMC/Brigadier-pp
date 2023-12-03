#include "brigadier/TypeHolder.hpp"
#include "brigadier/parser/Bool.hpp"
#include "brigadier/parser/Number.hpp"
#include "brigadier/parser/String.hpp"
#include "brigadier/reader/StringReader.hpp"
#include <gtest/gtest.h>

using brigadier::StringReader;

//* int
TEST(parser, validInt)
{
    using parser = brigadier::NumberParser<int>;

    auto reader = StringReader("5");

    EXPECT_NO_THROW(parser::parse(reader));
    reader.setCursor(0);
    EXPECT_EQ(5, parser::parse(reader));
}

TEST(parser, invalidInt)
{
    using parser = brigadier::NumberParser<int>;

    auto reader = StringReader("5.5");

    EXPECT_THROW(parser::parse(reader), brigadier::CommandSyntaxException);
}

//* long
TEST(parser, validLong)
{
    using parser = brigadier::NumberParser<long>;

    auto reader = StringReader("5");

    EXPECT_NO_THROW(parser::parse(reader));
    reader.setCursor(0);
    EXPECT_EQ(5l, parser::parse(reader));
}

TEST(parser, invalidLong)
{
    using parser = brigadier::NumberParser<long>;

    auto reader = StringReader("5.5");

    EXPECT_THROW(parser::parse(reader), brigadier::CommandSyntaxException);
}

//* float
TEST(parser, validFloat)
{
    using parser = brigadier::NumberParser<float>;

    auto reader = StringReader("5.5");

    EXPECT_NO_THROW(parser::parse(reader));
    reader.setCursor(0);
    EXPECT_EQ(5.5f, parser::parse(reader));
}

TEST(parser, invalidFloat)
{
    using parser = brigadier::NumberParser<float>;

    auto reader = StringReader("5.5.5");

    EXPECT_THROW(parser::parse(reader), brigadier::CommandSyntaxException);
}

//* double
TEST(parser, validDouble)
{
    using parser = brigadier::NumberParser<double>;

    auto reader = StringReader("5.5");

    EXPECT_NO_THROW(parser::parse(reader));
    reader.setCursor(0);
    EXPECT_EQ(5.5, parser::parse(reader));
}

TEST(parser, invalidDouble)
{
    using parser = brigadier::NumberParser<double>;

    auto reader = StringReader("5.5.5");

    EXPECT_THROW(parser::parse(reader), brigadier::CommandSyntaxException);
}

//* bool
TEST(parser, validBoolParserWithStringInput)
{
    using parser = brigadier::BoolParser;

    auto reader = StringReader("true");
    auto reader2 = StringReader("false");

    EXPECT_NO_THROW(parser::parse(reader));
    EXPECT_NO_THROW(parser::parse(reader2));
    reader.setCursor(0);
    reader2.setCursor(0);
    EXPECT_EQ(true, parser::parse(reader));
    EXPECT_EQ(false, parser::parse(reader2));
}

TEST(parser, validBoolParserWithIntInput)
{
    using parser = brigadier::BoolParser;

    auto reader = StringReader("1");
    auto reader2 = StringReader("0");

    EXPECT_NO_THROW(parser::parse(reader));
    EXPECT_NO_THROW(parser::parse(reader2));
    reader.setCursor(0);
    reader2.setCursor(0);
    EXPECT_EQ(true, parser::parse(reader));
    EXPECT_EQ(false, parser::parse(reader2));
}

TEST(parser, invalidBool)
{
    using parser = brigadier::BoolParser;

    auto reader = StringReader("5.5");

    EXPECT_THROW(parser::parse(reader), brigadier::CommandSyntaxException);
}

//* string
TEST(parser, validUnquotedString)
{
    using parser = brigadier::StringParser;

    auto reader = StringReader("test mama");

    EXPECT_NO_THROW(parser::parse(reader));
    reader.setCursor(0);
    EXPECT_EQ("test", parser::parse(reader));
}

TEST(parser, validQuotedString)
{
    using parser = brigadier::StringParser;

    auto reader = StringReader("'test mama'");

    EXPECT_NO_THROW(parser::parse(reader));
    reader.setCursor(0);
    EXPECT_EQ("test mama", parser::parse(reader));
}

TEST(parser, emptyString)
{
    using parser = brigadier::StringParser;

    auto reader = StringReader("");

    EXPECT_THROW(parser::parse(reader), brigadier::CommandSyntaxException);
}

TEST(parser, invalidQuotedString)
{
    using parser = brigadier::StringParser;

    auto reader = StringReader("'test mama");

    EXPECT_THROW(parser::parse(reader), brigadier::CommandSyntaxException);
}

//* greedy string
TEST(parser, validUnquotedGreedyString)
{
    using parser = brigadier::GreedyStringParser;

    auto reader = StringReader("test mama");

    EXPECT_NO_THROW(parser::parse(reader));
    reader.setCursor(0);
    EXPECT_EQ("test mama", parser::parse(reader));
}

TEST(parser, validQuotedGreedyString)
{
    using parser = brigadier::GreedyStringParser;

    auto reader = StringReader("'test mama'");

    EXPECT_NO_THROW(parser::parse(reader));
    reader.setCursor(0);
    EXPECT_EQ("'test mama'", parser::parse(reader));
}

TEST(parser, emptyGreedyString)
{
    using parser = brigadier::GreedyStringParser;

    auto reader = StringReader("");

    EXPECT_THROW(parser::parse(reader), brigadier::CommandSyntaxException);
}
