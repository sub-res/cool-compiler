/*
 * File:   main.cpp
 * Author: len
 *
 * Created on September 23, 2015, 6:17 PM
 */
#include <fstream>
#include <sstream>
#include <chrono>
#include <iostream>

#include "Analyser.h"
#include "Intermediater.h"
#include "Printer.h"
#include "CodeGenerator.h"
#include "Environment.h"
#include "driver.h"
#include "FileIO.h"
#include "basics.h"

using namespace Cool;

/*command arguments*/
#define ARG_DEBUG       "-d" //debug ON
#define ARG_NOGC        "-nogc" //don't use use assembly garbage collector 
#define ARG_OUTPUT      "-o" //use custom output file (next argument)
#define ARG_TPARS       "-p" //trace parser
#define ARG_TSCAN       "-s" //trace scanner
#define ARG_TESTGC      "-t" //test assembly garbage collector
#define ARG_STACKONLY   "-noreg" //use stack operations (no registers) only
#define ARG_LIB         "-lib" //library modus. Generate assembly for complete source code

using chrono = std::chrono::high_resolution_clock;
chrono::time_point startTime;

void build(Cool::Environment & env) {

    env.initInheritanceMap();

    if (!env.errors.hasCritical()) {

        bool debug = env.config.inDebug;
        if (debug) {
            Cool::Printer::print(env.getAllClasses());
        }

        Cool::Analyser analyser(env);
        analyser.start();

        if (!env.errors.hasCritical()) {

            bool hasError = false;
            auto ignores = Cool::IO::readAllLines(env.config.ignores, hasError);
            if (hasError) { //if can't open, read from resource
                const std::string s(ignConstrs);
                std::istringstream iss(s);
                std::string line;
                while (std::getline(iss, line)) {
                    ignores.push_back(line);
                }
            }

            Cool::Intermediater intermediater(env);
            intermediater.translate(env.getAllClasses(), ignores);

            if (debug) {
                std::cout << "\nINTERMEDIATE FORM:\n" << std::endl;
                Cool::Printer::print(env.getAllClasses());
            }
            Cool::CodeGenerator cg(env);
            cg.start();
        }
    }


    /*get duration in seconds since startTime*/
    chrono::time_point finish_time = chrono::now();
    std::chrono::duration<float> fsec = finish_time - startTime;
    const int seconds = fsec.count() / 1000; //seconds

    std::cout << "Total errors: " << env.errors.size() << "\n" << env.errors;
    std::cout << "BUILD " << (env.errors.hasCritical() ? "FAILED" : "SUCCESSFUL ") << " (total time: " << seconds << "s)" << std::endl;
    env.errors.clear();
    env.deleteUserClasses();
}

bool parseBasics(Cool::Environment & env, Cool::Driver & driver, const std::string & basicFile) {
    std::fstream infile(basicFile);
    if (infile.good()) {
        driver.parse_stream(infile, env, basicFile);
    } else { //if can't open, select default resource
        driver.parse_string(std::string(basics), env, basicFile);
    }
    if (env.errors.hasCritical()) {
        std::cerr << "Could not parse file: " << basicFile << std::endl;
        return false;
    }
    return true;
}

int main(int argc, char** argv) {

    startTime = std::chrono::high_resolution_clock::now();

    Cool::Driver driver; //linker for parser and scanner
    Cool::Environment env; //the environment 

    const std::string & basicFile = env.config.basicFile;
    if (!parseBasics(env, driver, basicFile)) {
        return 1;
    }

    bool readfile = false;

    for (int ai = 1; ai < argc; ++ai) {

        const std::string arg = argv[ai];
        if (arg == ARG_TPARS) {
            driver.trace_parsing = true;
        } else if (arg == ARG_DEBUG) {
            env.config.inDebug = true;
            std::cout << "debug mode ON" << std::endl;
        } else if (arg == ARG_TSCAN) {
            driver.trace_scanning = true;
        } else if (arg == ARG_OUTPUT) {
            if (ai + 1 >= argc) {
                std::cerr << "Error: missing output file name. Exiting." << std::endl;
                return 0;
            } else {
                env.config.outfile = argv[++ai];
            }
        } else if (arg == ARG_TESTGC) {
            env.config.gcTest = true;
        } else if (arg == ARG_NOGC) {
            env.config.gc = false;
        } else if (arg == ARG_STACKONLY) {
            env.config.useRegister = false;
        } else if (arg == ARG_LIB) {
            env.config.library = true;
        } else {
            if (arg == basicFile) { //prevent conflicts, basic file is compiled automatically
                std::cout << "Error: file \'" << basicFile << "\' can't be compiled this way." << std::endl;
                return 1;
            }
            std::fstream infile(arg);
            if (!infile.good()) {
                std::cerr << "Error: could not open file: " << arg << std::endl;
                return 2;
            }
            driver.parse_stream(infile, env, arg);
            readfile = true;
        }
    }

    if (readfile) {
        build(env);
    } else {
        env.config.inDebug = true;
        std::cout << "Reading Cool from stdin" << std::endl;
        std::string line;
        while (std::cout << "input: " && std::getline(std::cin, line) && !line.empty()) {
            startTime = chrono::now();
            driver.parse_string(line, env, "input");
            build(env);
            env.deleteBasicClasses();;
            parseBasics(env, driver, basicFile);
        }
    }

    return 0;
}

