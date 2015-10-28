
#include "ConfigReader.hpp"

ConfigReader::ConfigReader(const string& fileName) {

    // Read the file. If there is an error, report it and exit.
    try
    {
        cfg.readFile(fileName.c_str());
    }
    catch(const FileIOException &fioex)
    {
        std::cerr << "I/O error while reading file " << fileName << std::endl;
        throw;
    }
    catch(const ParseException &pex)
    {
        std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
                << " - " << pex.getError() << std::endl;
        throw;
    }
}

ConfigReader::~ConfigReader() { }