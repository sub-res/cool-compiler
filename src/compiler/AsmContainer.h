/* 
 * File:   AsmContainer.h
 * Author: wessel
 *
 * Created on January 13, 2016, 12:46 PM
 */

#ifndef ASMCONTAINER_H
#define	ASMCONTAINER_H

#include <vector>
#include "Instruction.h"

namespace Cool {

    class AsmContainer {
    public:
        AsmContainer();

        void add(const std::string& ins, const std::string& com = "");
        void add(Assembly::Instruction&& ins);

        void print(const std::string& outfile, bool printComments = false) const;

        void setTextSegStart();
        std::vector<Assembly::Instruction> instructions;
        int textSegStart()const;
    private:
        unsigned text_start;
    };
}


#endif	/* ASMCONTAINER_H */

