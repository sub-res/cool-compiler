/* 
 * File:   Types.h
 * Author: root
 *
 * Created on November 3, 2015, 8:16 PM
 */

#ifndef TYPES_H
#define	TYPES_H
#include <string>
namespace Cool {
    namespace Types {
        const std::string INT = "Int";
        const std::string STRING = "String";
        const std::string BOOL = "Boolean";
        const std::string UNIT = "Unit";
        const std::string ANY = "Any";
        const std::string SYMBOL = "Symbol";
        const std::string ARRAYANY = "ArrayAny";
        const std::string IO = "IO";
       
        //special types
        const std::string NOTHING = "Nothing";
        const std::string _NULL = "Null";
        const std::string NATIVE = "native";
        
        //fictional types
        const std::string UNDEFINED = "undefined";
    }
    
    namespace Values{
        const std::string _NULL = "null";
    }
}
#endif	/* TYPES_H */

