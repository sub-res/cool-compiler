#include <string>
#include <iostream>
#include <fstream>
#include "FileIO.h"
#include "Error.h"
namespace Cool {

    bool IO::clearFile(const std::string & outputLoc) {
        std::ofstream file;
        file.open(outputLoc);
        if (file.is_open()) {
            file.clear();
            file.close();
            return true;
        } else {
            std::cout << "Couldn't open file " + outputLoc << std::endl;
            return false;
        }
    }

    bool IO::write(const std::string & data, const std::string & outputLoc) {
        std::ofstream file;
        file.open(outputLoc, std::ios::app);
        if (file.is_open()) {
            file << data;
            file.close();
            return true;
        } else {
            std::cout << "Couldn't open file " + outputLoc << std::endl;
            return false;
        }
    }

    bool IO::writeLine(const std::string & data, const std::string & outputLoc) {
        return write(data + "\n", outputLoc);
    }

    std::vector<std::string> IO::readAllLines(const std::string & infile, bool & hasError) {
        std::ifstream ifs(infile);
        std::vector<std::string> res;

        if (ifs.is_open()) {
            hasError = false;
            std::string line;
            while (std::getline(ifs, line)) {
                res.push_back(line);
            }
        } else {
            hasError = true;
        }
        return res;
    }
}