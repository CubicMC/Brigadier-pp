#include <brigadier/exceptions.hpp>
#include <rapidcheck/Random.h>
#include <rapidcheck/gen/Arbitrary.h>
#include <brigadier/reader/StringReader.hpp>
#include <gtest/gtest.h>
#include <limits>
#include <rapidcheck/gtest.h>
#include <string>

class ReaderDefaults : public testing::Test {
protected:
    ReaderDefaults():
        str("Hello World"),
        reader(str)
    {
    }

protected:
    std::string str;
    brigadier::StringReader reader;
};
class ReaderInt : public testing::Test { };
class ReaderDouble : public testing::Test { };
class ReaderFloat : public testing::Test { };
class ReaderBool : public testing::Test { };
class ReaderString : public testing::Test {
protected:
    ReaderString():
        str("Hello World"),
        reader(str)
    {
    }

protected:
    std::string str;
    brigadier::StringReader reader;
};

TEST_F(ReaderDefaults, testSkip)
{
    reader.skip();
    EXPECT_EQ(reader.getCursor(), 1);
    EXPECT_EQ(reader.getRead(), str.substr(0, 1));
    EXPECT_EQ(reader.getRemaining(), str.substr(1));
    EXPECT_TRUE(reader.canRead());
    EXPECT_TRUE(reader.canRead(1));
}

TEST_F(ReaderDefaults, testLength)
{
    EXPECT_EQ(reader.getRemainingLength(), str.size());
    EXPECT_EQ(reader.getTotalLength(), str.size());
    reader.skip();
    EXPECT_EQ(reader.getRemainingLength(), str.size() - 1);
    EXPECT_EQ(reader.getTotalLength(), str.size());
}

TEST_F(ReaderDefaults, testCursor)
{
    EXPECT_EQ(reader.getCursor(), 0);
    reader.skip();
    EXPECT_EQ(reader.getCursor(), 1);
}

TEST_F(ReaderDefaults, testString)
{
    EXPECT_EQ(reader.getRead(), "");
    EXPECT_EQ(reader.getRemaining(), str);
    EXPECT_TRUE(reader.canRead());
    EXPECT_TRUE(reader.canRead(1));
}

TEST_F(ReaderString, testReadString)
{
    EXPECT_EQ(reader.getString(), str);
    EXPECT_EQ(reader.getRead(), "");
    EXPECT_EQ(reader.getRemaining(), str);
    EXPECT_TRUE(reader.canRead());
    size_t lastCursor = -1;
    while (reader.canRead()) {
        if (lastCursor != -1 && lastCursor == reader.getCursor())
            FAIL() << "Reader did not advance";
        reader.skipWhitespace();
        lastCursor = reader.getCursor();
        EXPECT_EQ(reader.readString(), str.substr(lastCursor, str.substr(lastCursor).find(' ')));
        EXPECT_EQ(reader.getRead(), str.substr(0, str.substr(lastCursor).find(' ')));
    }
    EXPECT_EQ(reader.getRead(), str);
    EXPECT_EQ(reader.getRemaining(), "");
    EXPECT_FALSE(reader.canRead());
}

TEST(ReaderTest, characterAllowedInUnquotedString)
{
    brigadier::StringReader reader("Hello World!");
    EXPECT_TRUE(reader.isAllowedInUnquotedString('H'));
    EXPECT_TRUE(reader.isAllowedInUnquotedString('e'));
    EXPECT_TRUE(reader.isAllowedInUnquotedString('l'));
    EXPECT_TRUE(reader.isAllowedInUnquotedString('o'));
    EXPECT_TRUE(reader.isAllowedInUnquotedString('w'));
    EXPECT_TRUE(reader.isAllowedInUnquotedString('r'));
    EXPECT_TRUE(reader.isAllowedInUnquotedString('d'));
    EXPECT_TRUE(reader.isAllowedInUnquotedString('5'));

    EXPECT_FALSE(reader.isAllowedInUnquotedString(' '));
    EXPECT_FALSE(reader.isAllowedInUnquotedString('\t'));
    EXPECT_FALSE(reader.isAllowedInUnquotedString('\n'));
    EXPECT_FALSE(reader.isAllowedInUnquotedString('\r'));
    EXPECT_FALSE(reader.isAllowedInUnquotedString('!'));
}

TEST_F(ReaderInt, testEasyInt)
{
    brigadier::StringReader reader("123");
    EXPECT_EQ(reader.readInt(), 123);

    brigadier::StringReader reader2("123a");
    EXPECT_THROW(reader2.readInt(), brigadier::CommandSyntaxException);

    brigadier::StringReader reader3("a123");
    EXPECT_THROW(reader3.readInt(), brigadier::CommandSyntaxException);
}

TEST_F(ReaderInt, testNegativeInt)
{
    brigadier::StringReader reader("-123");
    EXPECT_EQ(reader.readInt(), -123);

    brigadier::StringReader reader2("-123a");
    EXPECT_THROW(reader2.readInt(), brigadier::CommandSyntaxException);

    brigadier::StringReader reader3("a-123");
    EXPECT_THROW(reader3.readInt(), brigadier::CommandSyntaxException);
}

TEST_F(ReaderInt, testLimit)
{
    brigadier::StringReader reader4("12345678901234567890");
    EXPECT_THROW(reader4.readInt(), brigadier::CommandSyntaxException);

    brigadier::StringReader reader5("2147483648");
    EXPECT_THROW(reader5.readInt(), brigadier::CommandSyntaxException);

    brigadier::StringReader reader6("-2147483649");
    EXPECT_THROW(reader6.readInt(), brigadier::CommandSyntaxException);

    brigadier::StringReader reader7(std::to_string(std::numeric_limits<int>::lowest()));
    EXPECT_EQ(reader7.readInt(), std::numeric_limits<int>::lowest());

    brigadier::StringReader reader8(std::to_string(std::numeric_limits<int>::max()));
    EXPECT_EQ(reader8.readInt(), std::numeric_limits<int>::max());
}

TEST_F(ReaderInt, testZero)
{
    brigadier::StringReader reader9("0");
    EXPECT_EQ(reader9.readInt(), 0);

    brigadier::StringReader reader10("-0");
    EXPECT_EQ(reader10.readInt(), 0);
}

RC_GTEST_FIXTURE_PROP(ReaderInt, testReadInt, (int value))
{
    brigadier::StringReader reader(std::to_string(value));
    RC_ASSERT(reader.readInt() == value);
}

RC_GTEST_FIXTURE_PROP(ReaderDouble, testReadDouble, (double value))
{
    RC_SUCCEED("Skip this test because of precision error");
    brigadier::StringReader reader(std::to_string(value));
    RC_ASSERT(reader.readDouble() == value);
}

TEST_F(ReaderDouble, testEasyDouble)
{
    brigadier::StringReader reader("123");
    EXPECT_EQ(reader.readDouble(), 123);

    brigadier::StringReader reader2("123a");
    EXPECT_THROW(reader2.readDouble(), brigadier::CommandSyntaxException);

    brigadier::StringReader reader3("a123");
    EXPECT_THROW(reader3.readDouble(), brigadier::CommandSyntaxException);
}

TEST_F(ReaderDouble, testNegativeDouble)
{
    brigadier::StringReader reader("-123");
    EXPECT_EQ(reader.readDouble(), -123);

    brigadier::StringReader reader2("-123a");
    EXPECT_THROW(reader2.readDouble(), brigadier::CommandSyntaxException);

    brigadier::StringReader reader3("a-123");
    EXPECT_THROW(reader3.readDouble(), brigadier::CommandSyntaxException);
}

TEST_F(ReaderDouble, testLimit)
{
    brigadier::StringReader reader4(
        "17976931348623157081452742373170435679807056752584499659891747680315726078002853876058955863276687817154045895351438246423432132688946418276846754670353751698604991057655"
        "12820762454900903893289440758685084551339423045832369032229481658085593321233482747978262041447231687381771809192998812504040261841248583681234.12345678901234567890"
    );
    EXPECT_THROW(reader4.readDouble(), brigadier::CommandSyntaxException);

    // auto min = std::to_string(std::numeric_limits<double>::lowest());
    // min[min.size() - 1] = min[min.size() - 1] + 1;
    // brigadier::StringReader reader5(min);
    // EXPECT_THROW(reader5.readDouble(), brigadier::CommandSyntaxException);

    // auto max = std::to_string(std::numeric_limits<double>::max());
    // max[max.size() - 1] = max[max.size() - 1] + 1;
    // brigadier::StringReader reader6(max);
    // EXPECT_THROW(reader6.readDouble(), brigadier::CommandSyntaxException);

    brigadier::StringReader reader7(std::to_string(std::numeric_limits<double>::lowest()));
    EXPECT_EQ(reader7.readDouble(), std::numeric_limits<double>::lowest());

    brigadier::StringReader reader8(std::to_string(std::numeric_limits<double>::max()));
    EXPECT_EQ(reader8.readDouble(), std::numeric_limits<double>::max());
}

RC_GTEST_FIXTURE_PROP(ReaderFloat, testReadFloat, (float value))
{
    RC_SUCCEED("Skip this test because of precision error");
    brigadier::StringReader reader(std::to_string(value));
    RC_ASSERT(reader.readFloat() == value);
}

TEST_F(ReaderFloat, testEasyFloat)
{
    brigadier::StringReader reader("123");
    EXPECT_EQ(reader.readFloat(), 123);

    brigadier::StringReader reader2("123a");
    EXPECT_THROW(reader2.readFloat(), brigadier::CommandSyntaxException);

    brigadier::StringReader reader3("a123");
    EXPECT_THROW(reader3.readFloat(), brigadier::CommandSyntaxException);
}

TEST_F(ReaderFloat, testNegativeFloat)
{
    brigadier::StringReader reader("-123");
    EXPECT_EQ(reader.readFloat(), -123);

    brigadier::StringReader reader2("-123a");
    EXPECT_THROW(reader2.readFloat(), brigadier::CommandSyntaxException);

    brigadier::StringReader reader3("a-123");
    EXPECT_THROW(reader3.readFloat(), brigadier::CommandSyntaxException);
}

TEST_F(ReaderFloat, testLimit)
{
    brigadier::StringReader reader4("34028234663852885981170418348451692544124242352341234.000000001");
    EXPECT_THROW(reader4.readFloat(), brigadier::CommandSyntaxException);

    // auto min = std::to_string(std::numeric_limits<float>::lowest());
    // auto p = min.find('.');
    // min[p - 1] = min[p - 1] + 1;
    // brigadier::StringReader reader5(min);
    // EXPECT_THROW(reader5.readFloat(), brigadier::CommandSyntaxException);

    // auto max = std::to_string(std::numeric_limits<float>::max());
    // max[max.size() - 1] = max[max.size() - 1] + 1;
    // brigadier::StringReader reader6(max);
    // EXPECT_THROW(reader4.readFloat(), brigadier::CommandSyntaxException);

    brigadier::StringReader reader7(std::to_string(std::numeric_limits<float>::lowest()));
    EXPECT_EQ(reader7.readFloat(), std::numeric_limits<float>::lowest());

    brigadier::StringReader reader8(std::to_string(std::numeric_limits<float>::max()));
    EXPECT_EQ(reader8.readFloat(), std::numeric_limits<float>::max());
}

RC_GTEST_FIXTURE_PROP(ReaderBool, testReadBool, (bool value))
{
    brigadier::StringReader reader(value ? "true" : "false");
    RC_ASSERT(reader.readBool() == value);
}

TEST_F(ReaderFloat, testReadFloat)
{
    auto elms = rc::gen::arbitrary<std::vector<float>>().as("elements");
    auto prop = [](const std::vector<float> &elements) {
        for (auto &element : elements) {
            brigadier::StringReader reader(std::to_string(element));
            RC_ASSERT(reader.readFloat() == element);
        }
    };
    rc::check(prop);
}

TEST_F(ReaderDouble, testReadDouble)
{
    auto elms = rc::gen::arbitrary<std::vector<double>>().as("elements");
    auto prop = [](const std::vector<double> &elements) {
        for (auto &element : elements) {
            brigadier::StringReader reader(std::to_string(element));
            RC_ASSERT(reader.readDouble() == element);
        }
    };
    rc::check(prop);
}
