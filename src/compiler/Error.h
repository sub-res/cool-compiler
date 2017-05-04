/* 
 * File:   Error.h
 * Author: root
 *
 * Created on November 3, 2015, 4:53 PM
 */

#ifndef ERROR_H
#define ERROR_H


#include <string>
#include <memory>
#include <ostream>

namespace Cool {

    class location;
    enum class ErrorType;

    class Error {
        
        friend std::ostream& operator<<(std::ostream& out, const Error&);
        
    public:
        Error(const std::string& message);
        Error(const std::string& message, const ErrorType t);
        
        Error(const location & loc, const std::string& message);
        Error(const location & loc, const std::string& message, const ErrorType t);
        
        std::unique_ptr<location> loc;
        const std::string message;
        const ErrorType type;
    };
}
#endif /*ERROR_H*/ 

