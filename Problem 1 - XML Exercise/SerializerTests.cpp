#include <gtest/gtest.h>
#include "Serializer.h"
#include <string>

TEST(SerializerTests, PlainStringUnchanged) {
    EXPECT_EQ(Serializer::SerializeData("Hi guys!"), "Hi guys!");
}

TEST(SerializerTests, EmptyString) {
    EXPECT_EQ(Serializer::SerializeData(""), "");
}

TEST(SerializerTests, EscapesAmpersand) {
    EXPECT_EQ(Serializer::SerializeData("&"), "&amp;");
}

TEST(SerializerTests, EscapesLessThan) {
    EXPECT_EQ(Serializer::SerializeData("<"), "&lt;");
}

TEST(SerializerTests, EscapesGreaterThan) {
    EXPECT_EQ(Serializer::SerializeData(">"), "&gt;");
}

TEST(SerializerTests, EscapesDoubleQuote) {
    EXPECT_EQ(Serializer::SerializeData("\""), "&quot;");
}

TEST(SerializerTests, EscapesSingleQuote) {
    EXPECT_EQ(Serializer::SerializeData("'"), "&apos;");
}

TEST(SerializerTests, StringEscaping) {
    const char* input = "Ben&Jerry's <ice-cream> \"grade\" 'A'";
    const std::string out = Serializer::SerializeData(input);
    EXPECT_EQ(out, "Ben&amp;Jerry&apos;s &lt;ice-cream&gt; &quot;grade&quot; &apos;A&apos;");
}

TEST(SerializerTests, NullCString) {
    const char* input = nullptr;
    EXPECT_EQ(Serializer::SerializeData(input), "");
}

TEST(SerializerTests, IntValue) {
    EXPECT_EQ(Serializer::SerializeData(42), "42");
}

TEST(SerializerTests, IntZero) {
    EXPECT_EQ(Serializer::SerializeData(0), "0");
}

TEST(SerializerTests, NegativeInt) {
    EXPECT_EQ(Serializer::SerializeData(-42), "-42");
}

TEST(SerializerTests, DoubleValue) {
    EXPECT_EQ(Serializer::SerializeData(3.5), "3.5");
}

TEST(SerializerTests, DoubleNoTrailingZeros) {
    EXPECT_EQ(Serializer::SerializeData(3.0), "3");
}

TEST(SerializerTests, DoubleZeroNoTrailingZeros) {
    EXPECT_EQ(Serializer::SerializeData(0.0), "0");
}

TEST(SerializerTests, NegativeDouble) {
    EXPECT_EQ(Serializer::SerializeData(-3.5), "-3.5");
}

TEST(SerializerTests, NegativeDoubleNoTrailingZeros) {
    EXPECT_EQ(Serializer::SerializeData(-3.0), "-3");
}

TEST(SerializerTests, LongValue) {
    EXPECT_EQ(Serializer::SerializeData(42L), "42");
}

TEST(SerializerTests, LongZero) {
    EXPECT_EQ(Serializer::SerializeData(0L), "0");
}

TEST(SerializerTests, LongNegative) {
    EXPECT_EQ(Serializer::SerializeData(-42L), "-42");
}

TEST(SerializerTests, FloatValue) {
    EXPECT_EQ(Serializer::SerializeData(2.5f), "2.5");
}

TEST(SerializerTests, FloatValueNoTrailingZeros) {
    EXPECT_EQ(Serializer::SerializeData(2.0f), "2");
}

TEST(SerializerTests, FloatZeroNoTrailingZeros) {
    EXPECT_EQ(Serializer::SerializeData(0.0f), "0");
}

TEST(SerializerTests, FloatNegative) {
    EXPECT_EQ(Serializer::SerializeData(-2.5f), "-2.5");
}

TEST(SerializerTests, FloatNegativeNoTrailingZeros) {
    EXPECT_EQ(Serializer::SerializeData(-2.0f), "-2");
}

TEST(SerializerTests, BoolValue) {
    EXPECT_EQ(Serializer::SerializeData(true), "true");
    EXPECT_EQ(Serializer::SerializeData(false), "false");
}
