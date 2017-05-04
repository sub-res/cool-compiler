/* 
 * File:   StringContainer.h
 * Author: len
 *
 * Created on December 25, 2015, 9:11 PM
 */

#ifndef STRINGCONTAINER_H
#define	STRINGCONTAINER_H
#include <string>
#include <unordered_set>

namespace Cool {

    /*Central storage class for unique strings that have been scanned and parsed
     Includes strings from StringNodes, IdNodes, TypeNodes. This is for optimizing memory usage during compiletime.
     Because source code tends to have many duplicate  string literals, this class is used to only store unique ones and return
     a reference to them.
     Each time the scanner or parser must create a new string during runtime, it calls the get method.
     This method saves the new string if it was not already in strings and returns a reference to it.
     */
    class StringContainer {
    public:
        static const std::string& get(std::string && forString);
    private:
        static std::unordered_set<std::string> strings;
    };
}
#endif	/* STRINGCONTAINER_H */

