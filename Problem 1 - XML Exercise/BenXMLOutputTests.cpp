#include <gtest/gtest.h>
#include "BenXMLOutput.h"

#include <string>
#include <cstdio> // included for std::remove() portability, because the internet told me to.
#include <zlib.h>
#include <filesystem>

// Things inside namespaces are visible only to this class.
namespace fs = std::filesystem;
namespace {
    constexpr const char* tagNameA = "SomeTagA";
    constexpr const char* tagNameB = "SomeTagB";
    constexpr const char* someSimpleCharData = "SomeCharData";
    constexpr const char* someComplicatedCharData = "Ben&Jerry's <ice-cream> \"grade\" 'A'";
    constexpr int someIntData = 1;
    constexpr bool someBool = true;
}

static std::string ReadGzipFile(const char* path) {
    gzFile file = gzopen(path, "rb");
    if (!file) {
        return "";
    }

    std::string output;
    char buffer[256];
    int bytesRead = 0;

    while ((bytesRead = gzread(file, buffer, sizeof(buffer))) > 0) {
        output.append(buffer, bytesRead);
    }

    gzclose(file);
    return output;
}

TEST(BenXMLOutputTests, WritesXml) {
    const char* path = "writes_xml.xml.gz";

    {   // These brackets create "nested scopes".
        // Any objects created inside the scope will be destroyed once the scope ends. Nifty!
        BenXMLOutput output(path);
        output.StartTag(tagNameA);
        output.SerializeData(someSimpleCharData);
        output.EndTag(tagNameA);
        output.Close();
    }

    const std::string actual = ReadGzipFile(path);
    const std::string expected = std::string("<") + tagNameA + ">" + someSimpleCharData + "</" + tagNameA + ">";
    EXPECT_EQ(actual, expected);
    std::remove(path); // Deletes whatever file was created during the test. Nice and clean.
}

TEST(BenXMLOutputTests, MultipleFlushesAreSafe) {
    const char* path = "multiple_flushes_are_safe.xml.gz";

    {
        BenXMLOutput output(path);

        output.StartTag(tagNameA);
        output.SerializeData(someIntData);
        output.EndTag(tagNameA);
        output.Flush(); // first flush

        output.StartTag(tagNameB);
        output.SerializeData(someBool);
        output.EndTag(tagNameB);
        output.Close(); // second flush included in Close()
    }

    const std::string actual = ReadGzipFile(path);

    // Note that someBool is set to true in the namespace, so "false" will never be reached
    // A more robust implementation would be some sort of parameterized test which takes both values.
    // But the existing works for the purposes of this exercise.
    const std::string expected = std::string("<") + tagNameA + ">" + std::to_string(someIntData) + "</" + tagNameA +">"
    + "<" + tagNameB + ">" + (someBool ? "true" : "false") + "</" + tagNameB + ">";

    EXPECT_EQ(actual, expected);
    std::remove(path);
}

TEST(BenXMLOutputTests, CloseFlushesRemainingData) {
    const char* path = "close_flushes_remaining_data.xml.gz";

    {
        BenXMLOutput output(path);
        output.StartTag(tagNameA);
        output.SerializeData(someIntData);
        output.EndTag(tagNameA);
        output.Close();
    }

    const std::string actual = ReadGzipFile(path);
    const std::string expected = std::string("<") + tagNameA + ">" + std::to_string(someIntData) + "</" + tagNameA + ">";

    EXPECT_EQ(actual, expected);
    std::remove(path);
}

TEST(BenXMLOutputTests, DestructorClosesFileIfNeeded) {
    const char* path = "destructor_closes_file_if_needed.xml.gz";

    {
        BenXMLOutput output(path);
        output.StartTag(tagNameA);
        output.SerializeData(someSimpleCharData);
        output.EndTag(tagNameA);
        // no explicit Close()
    }

    const std::string actual = ReadGzipFile(path);
    const std::string expected = std::string("<") + tagNameA + ">" + someSimpleCharData + "</" + tagNameA + ">";

    EXPECT_EQ(actual, expected);
    std::remove(path);
}

TEST(BenXMLOutputTests, WritesEscapeStringData) {
    const char* path = "writes_escape_string_data.xml.gz";
    const char* expectedData = "Ben&amp;Jerry&apos;s &lt;ice-cream&gt; &quot;grade&quot; &apos;A&apos;";

    {
        BenXMLOutput output(path);
        output.StartTag(tagNameA);
        output.SerializeData(someComplicatedCharData);
        output.EndTag(tagNameA);
        output.Close();
    }

    const std::string actual = ReadGzipFile(path);
    const std::string expected = std::string("<") + tagNameA + ">" + expectedData + "</" + tagNameA + ">";

    ASSERT_FALSE(actual.empty());
    EXPECT_EQ(actual, expected);
    std::remove(path);
}

TEST(BenXMLOutputTests, MultipleCloseCallsAreSafe) {
    const char* path = "multiple_close_calls_are_safe.xml.gz";

    {
        BenXMLOutput output(path);
        output.StartTag(tagNameA);
        output.SerializeData(someSimpleCharData);
        output.EndTag(tagNameA);

        output.Close();
        output.Close();  // second call should be safe
    }

    const std::string actual = ReadGzipFile(path);
    const std::string expected = std::string("<") + tagNameA + ">" + someSimpleCharData + "</" + tagNameA + ">";

    ASSERT_FALSE(actual.empty());
    EXPECT_EQ(actual, expected);
    std::remove(path);
}

TEST(BenXMLOutputTests, FlushWithEmptyBufferDoesNotCorruptOutput) {
    const char* path = "flush_with_empty_buffer_does_not_corrupt_output.xml.gz";

    {
        BenXMLOutput output(path);

        output.Flush();  // No data yet; should safely do nothing

        output.StartTag(tagNameA);
        output.SerializeData(someIntData);
        output.EndTag(tagNameA);
        output.Close(); // second flush included in Close()
    }

    const std::string actual = ReadGzipFile(path);
    const std::string expected = std::string("<") + tagNameA + ">" + std::to_string(someIntData) + "</" + tagNameA + ">";

    ASSERT_FALSE(actual.empty());
    EXPECT_EQ(actual, expected);
    std::remove(path);
}

TEST(BenXMLOutputTests, AppendsGzExtensionWhenMissing) {
    const char* inputPath = "extension.xml";
    const char* expectedOutputPath = "extension.xml.gz";

    {
        BenXMLOutput output(inputPath);
        output.StartTag(tagNameA);
        output.SerializeData(someSimpleCharData);
        output.EndTag(tagNameA);
        output.Close();
    }

    EXPECT_TRUE(fs::exists(expectedOutputPath));

    const std::string actual = ReadGzipFile(expectedOutputPath);
    const std::string expected =
        std::string("<") + tagNameA + ">" + someSimpleCharData + "</" + tagNameA + ">";
    EXPECT_EQ(actual, expected);

    // Cleanup both names in case implementation wrote either one
    std::remove(inputPath);
    std::remove(expectedOutputPath);
}

TEST(BenXMLOutputTests, KeepsGzExtensionWhenAlreadyProvided) {
    const char* path = "already_gz.xml.gz";

    {
        BenXMLOutput output(path);
        output.StartTag(tagNameA);
        output.SerializeData(someSimpleCharData);
        output.EndTag(tagNameA);
        output.Close();
    }

    EXPECT_TRUE(fs::exists(path));

    const std::string actual = ReadGzipFile(path);
    const std::string expected =
        std::string("<") + tagNameA + ">" + someSimpleCharData + "</" + tagNameA + ">";
    EXPECT_EQ(actual, expected);

    std::remove(path);
}

