#include "Serializer.h"
#include <sstream>

std::string Serializer::SerializeData(const char* data) {
    //Because data parameter is a pointer here, we could have null values.
    if (data == nullptr) {
        return {}; // Brackets are equivalent to std::string(). Yay learning!
    }
    return IncludeEscapeCharacters(data);
}

std::string Serializer::SerializeData(const int& data) {
    return std::to_string(data);
}

std::string Serializer::SerializeData(const double& data) {
    // Streams will remove any unnecessary trailing zeros
    std::ostringstream stream;
    stream << data;
    return stream.str();
}

std::string Serializer::SerializeData(const float& data) {
    // Streams will remove any unnecessary trailing zeros
    std::ostringstream stream;
    stream << data;
    return stream.str();
}

std::string Serializer::SerializeData(const long& data) {
    return std::to_string(data);
}

std::string Serializer::SerializeData(const bool& data) {
    // Mapping a boolean directly to XML produces 1 for true and 0 for false.
    // Instead, we map to the English directly.
    // We assume here that any recipient of the data can translate/deserialize correctly based on the English values
    return data ? "true" : "false";
}

/**
 * Takes in a string and replaces any characters that have special meaning in XML with their corresponding escape values.
 */
std::string Serializer::IncludeEscapeCharacters(const std::string& input) {
    std::string output;
    output.reserve(input.size());

    for (const char c : input) {
        switch (c) {
            case '&': output += "&amp;";  break;
            case '<': output += "&lt;";   break;
            case '>': output += "&gt;";   break;
            case '"': output += "&quot;"; break;
            case '\'': output += "&apos;"; break;
            default: output += c; break;
        }
    }
    return output;
}
