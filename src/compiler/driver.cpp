// $Id: driver.cc 39 2008-08-03 10:07:15Z tb $
/** \file driver.cc Implementation of the Cool::Driver class. */

#include <fstream>
#include <sstream>

#include "driver.h"
#include "Parser.h"
#include "Scanner.h"
#include "Errors.h"
#include "StringContainer.h"
#include "Environment.h"

namespace Cool {

    bool Driver::parse_stream(std::istream& in, Environment & env, const std::string & sname) {

        this->env = &env;
        
        streamname = filenames.insert(filenames.cend(),std::make_unique<std::string>(sname))->get();
        
        Scanner scanner(env.errors, &in);
        scanner.set_debug(trace_scanning);
        this->lexer = &scanner;

        Parser parser(*this, env);
        parser.set_debug_level(trace_parsing);
        return (parser.parse() == 0);
    }

    bool Driver::parse_file(const std::string &filename, Environment & env) {
        std::ifstream in(filename.c_str());
        if (!in.good()) return false;
        return parse_stream(in, env, filename);
    }

    bool Driver::parse_string(const std::string &input, Environment & env, const std::string& sname) {
        std::istringstream iss(input);
        return parse_stream(iss, env, sname);
    }
}
