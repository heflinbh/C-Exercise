#include "BenXMLOutput.h"

#include <iostream>
#include <zlib.h> // this library holds our gzip stuff

/**
 * The constructor
 * Sets initial values for filename, gzFileHandle, and the boolean isClosed.
 * For the filename, EnsureGzipExtension is called to make ensure a .gz extension
 */
BenXMLOutput::BenXMLOutput(const char* pszXMLFilename)
    : filename(EnsureGzipExtension(pszXMLFilename)), gzFileHandle(nullptr), isClosed(false) {
    gzFileHandle = gzopen(filename.c_str(), "wb");
    if (!gzFileHandle) {
        std::cerr << "Error opening file: " << pszXMLFilename << std::endl;
    }
    std::cout << "Creating XML output object for: " << pszXMLFilename << std::endl;
}

/**
 * The deconstructor
 */
BenXMLOutput::~BenXMLOutput() {
    std::cout << "Destroying XML output object\n";
    if (!isClosed) {
        Close(); // Close on destroy, just in case.
    }
}

/**
 * Ensures that the provided filename has a .gz extension.
 * If the input filename is null, it defaults to "default_output.xml.gz".
 * If the filename already ends with ".gz", it returns the name as-is.
 * Otherwise, it appends ".gz" to the name.
 */
std::string BenXMLOutput::EnsureGzipExtension(const char* rawName) {
    std::string name = (rawName != nullptr) ? rawName : "default_output.xml.gz";
    if (name.size() >= 3 && name.substr(name.size() - 3) == ".gz") {
        return name;
    }
    return name + ".gz";
}

/**
 * Produces the start of an XML tag
 */
void BenXMLOutput::StartTag(const char* pszTagName) {
    xmlBuffer += "<";
    xmlBuffer += pszTagName;
    xmlBuffer += ">";
}

/**
 * Produces the end of an XML tag
 */
void BenXMLOutput::EndTag(const char* pszTagName) {
    xmlBuffer += "</";
    xmlBuffer += pszTagName;
    xmlBuffer += ">";
}

/**
 * Puts whatever is in the xmlBuffer into the gzFileHandle
 */
void BenXMLOutput::Flush() {
    std::cout << "[Flush Called]" << std::endl;
    if (!gzFileHandle) {
        std::cerr << "[No file to write to]" << std::endl;
        return;
    }

    if (xmlBuffer.empty()) {
        std::cout << "[Flush called - No data to write]" << std::endl;
        return;
    }

    int bytesWritten = gzwrite(gzFileHandle, xmlBuffer.c_str(), xmlBuffer.length());
    if (bytesWritten == 0 ) {
        std::cerr << "Error: Failed to write data to " << gzFileHandle << std::endl;
        return;
    }

    // Now that everything has been written into the gzFileHandle, we can erase from the buffer and continue
    xmlBuffer.clear();
    std::cout << "[Buffer Flushed Successfully]" << std::endl;
}

/**
 * Flushes and closes gzFileHandle, then sets it to null.
 * If the file is already closed, returns without doing anything.
 */
void BenXMLOutput::Close() {
    if (isClosed) {
        std::cout << "[File already closed]" << std::endl;
        return;
    }

    Flush(); // Ensure all leftover data is written before closing

    if (gzFileHandle) gzclose(gzFileHandle); // Close only if we initialize gzFileHandler -> otherwise it could be null
    gzFileHandle = nullptr; // Once the handle is closed, prevent anything else from trying to use it.

    isClosed = true;
    std::cout << "[File closed]" << std::endl;
}
