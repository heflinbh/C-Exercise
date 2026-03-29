#ifndef C_EXERCISE_BENXMLOUTPUT_H
#define C_EXERCISE_BENXMLOUTPUT_H

#include <string>
#include <zlib.h>
#include "Serializer.h"

class BenXMLOutput {
public:
    BenXMLOutput(const char* pszXMLFilename);

    // Virtual is used for inheritance -> things inheriting BenXMLOutput will call this deconstructor when they are destroyed
    // Not necessary with this current implementation, but nice to have.
    virtual ~BenXMLOutput();

    // Prevent copying of this class
    // We don't want to accidentally copy the gzFileHandle and have multiple objects trying to write to the same file.
    BenXMLOutput(const BenXMLOutput&) = delete; // copy constructor -> causes a compile error
    BenXMLOutput& operator=(const BenXMLOutput&) = delete; // copy assignment operator -> causes a compile error

    // Inserts the Start Tag to the XML Buffer
    void StartTag(const char* pszTagName);

    // Inserts the End Tag to the XML Buffer
    void EndTag(const char* pszTagName);

    // This is where the data is put into an XML
    // It should be type agnostic -> let the Serializer worry about the data type.
    template <typename T>
    int SerializeData(const T& data) {
        xmlBuffer += Serializer::SerializeData(data);
        return 0;
    }

    // Write all our changes from the xmlBuffer to the gzFileHandle.
    void Flush();

    // Closes the gzFileHandle.
    void Close();

private:
    std::string filename;
    std::string xmlBuffer;
    gzFile gzFileHandle;
    bool isClosed;

    static std::string EnsureGzipExtension(const char* rawName);
};

#endif //C_EXERCISE_BENXMLOUTPUT_H
