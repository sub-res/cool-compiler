/* 
 * File:   Instruction.h
 * Author: len
 *
 * Created on January 13, 2016, 12:27 PM
 */

#ifndef INSTRUCTION_H
#define	INSTRUCTION_H
#include <string>

namespace Cool {
    namespace Assembly {

        class Instruction {
        public:
            Instruction(const std::string & line, const std::string & comment = "");

            std::string getInstruction() const;
            std::string getArg(unsigned i) const;
            
            bool isComment() const;
            bool isLabel() const;
            bool isInstruction() const;
            std::string get() const;
            std::string getCom() const;

            std::string line;
            std::string comment;
        private:
            std::string split(unsigned i) const;
        };
    }
}
#endif	/* INSTRUCTION_H */

