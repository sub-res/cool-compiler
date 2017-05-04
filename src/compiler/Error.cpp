/* 
 * File:   Error.cpp
 * Author: root
 * 
 * Created on November 3, 2015, 4:53 PM
 */
#include "Error.h"
#include "location.hh"
#include "ErrorType.h"

namespace Cool {

    Error::Error(const std::string& m) : Error(m, ErrorType::CRITICAL) {
    }

    Error::Error(const std::string& m, const ErrorType t) : loc(nullptr), message(m), type(t) {
    }

    Error::Error(const location & l, const std::string& m) : Error(l, m, ErrorType::CRITICAL) {
    }

    Error::Error(const location & l, const std::string& m, const ErrorType t) :
    loc(std::make_unique<location>(l)), message(m), type(t) {
    }

    std::ostream& operator<<(std::ostream& out, const Error& err) {
        if (err.loc) {
            const auto & pos = err.loc->begin;
            out << *pos.filename << ":" << pos.line << ":" << pos.column << ":";
        }
        return out << (err.type == ErrorType::CRITICAL ? "Error: " : "Warning: ") << err.message;
    }
}
