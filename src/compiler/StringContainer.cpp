/* 
 * File:   StringContainer.cpp
 * Author: len
 * 
 * Created on December 25, 2015, 9:11 PM
 */

#include "StringContainer.h"

namespace Cool {
    std::unordered_set<std::string> StringContainer::strings;
    
    const std::string& StringContainer::get(std::string && forText) {
        return *strings.insert(std::move(forText)).first;
    }
}