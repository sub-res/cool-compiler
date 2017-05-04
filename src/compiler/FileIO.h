/* 
 * File:   FileIO.h
 * Author: wessel
 *
 * Created on November 18, 2015, 2:09 PM
 */

#ifndef FILEIO_H
#define	FILEIO_H

#include <string>
#include <vector>
namespace Cool {
    
    namespace IO {
        bool clearFile(const std::string & outputloc);
        bool write(const std::string & data, const std::string & outputloc);
        bool writeLine(const std::string & data, const std::string & outputloc);
        
        std::vector<std::string> readAllLines(const std::string & infile,bool & hasError);
    }
}
#endif	/* FILEIO_H */

