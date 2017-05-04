// $Id: driver.h 17 2007-08-19 18:51:39Z tb $ 	
/** \file driver.h Declaration of the Cool::Driver class. */

#ifndef Cool_DRIVER_H
#define Cool_DRIVER_H

#include <string>
#include <map>
#include <vector>
#include <memory>

/** The Cool namespace is used to encapsulate the three parser classes
 * Cool::Parser, Cool::Scanner and Cool::Driver */

namespace Cool {
    class Environment;
    class Scanner;

    /** The Driver class brings together all components. It creates an instance of
     * the Parser and Scanner classes and connects them. Then the input stream is
     * fed into the scanner object and the parser gets it's token
     * sequence. Furthermore the driver object is available in the grammar rules as
     * a parameter. Therefore the driver class contains a reference to the
     * structure into which the parsed data is saved. */

    class Driver {
    public:
        /// enable debug output in the flex scanner
        bool trace_scanning = false;

        /// enable debug output in the bison parser
        bool trace_parsing = false;

        /// stream name (file or input stream) used for error messages.
        std::string * streamname;

        /** Invoke the scanner and parser for a stream.
         * @param in	input stream
         * @param sname	stream name for error messages
         * @return		true if successfully parsed
         */
        bool parse_stream(std::istream& in, Environment&,
                const std::string & sname = "stream input");

        /** Invoke the scanner and parser on an input string.
         * @param input	input string
         * @param sname	stream name for error messages
         * @return		true if successfully parsed
         */
        bool parse_string(const std::string& input, Environment&,
                const std::string& sname = "string stream");

        /** Invoke the scanner and parser on a file. Use parse_stream with a
         * std::ifstream if detection of file reading errors is required.
         * @param filename	input file name
         * @return		true if successfully parsed
         */
        bool parse_file(const std::string& filename, Environment&);

        // To demonstrate pure handling of parse errors, instead of
        // simply dumping them on the standard error output, we will pass
        // them to the driver using the following two member functions.

        /** Pointer to the current lexer instance, this is used to connect the
         * parser to the scanner. It is used in the yylex macro. */
        Scanner* lexer;

    private:
        std::vector<std::unique_ptr<std::string>> filenames;
        const Environment * env;
    };

} // namespace Cool

#endif // Cool_DRIVER_H
