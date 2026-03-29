#ifndef C_EXERCISE_SERIALIZER_H
#define C_EXERCISE_SERIALIZER_H

#include <string>

class Serializer {
public:
    Serializer() = delete; // This is a static utility class, no instances needed

    template <typename T>
    static std::string SerializeData(const T&) = delete; // unsupported types will result in compile errors.

    // This is where the data is put into an XML
    static std::string SerializeData(const char* data);  // simplified version
    static std::string SerializeData(const int& data);
    static std::string SerializeData(const double& data);
    static std::string SerializeData(const float& data);
    static std::string SerializeData(const long& data);
    static std::string SerializeData(const bool& data);

private:
    static std::string IncludeEscapeCharacters(const std::string& input);
};

#endif //C_EXERCISE_SERIALIZER_H
