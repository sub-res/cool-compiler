/* 
 * File:   Optimizer.cpp
 * Author: len
 * 
 * Created on January 13, 2016, 3:15 PM
 */

#include "Optimizer.h"
#include "Instruction.h"
#include "AsmContainer.h"
#include <iostream>

namespace Cool {
    namespace Assembly {

        void Optimizer::optimize(AsmContainer& container) {
            auto & instructions = container.instructions;

            for (auto itt = instructions.begin() + container.textSegStart(); itt != instructions.end() - 1;) {

                Instruction & instr = *itt;

                //optimize instructions only ;)
                if (instr.isInstruction()) {
                    const std::string command = instr.getInstruction();

                    if (command == "addiu") { 
                        /*optimize on stack instructions
                         * Example: merging a stack addiu instruction with another if it is succeeding.  )
                         */
                        Instruction & next = *(itt + 1);
                        if (next.isInstruction() && next.getInstruction() == command) {
                            if (next.getArg(1) == instr.getArg(1) && next.getArg(2) == instr.getArg(2)) {
                                const int bytes = std::stoi(instr.getArg(3)) + std::stoi(next.getArg(3));
                                if (bytes == 0) { //increase/decrease stack with 0 bytes is useless, so remove it
                                    itt = instructions.erase(itt);
                                } else { //next instruction becomes the merged one
                                    next.line = "\t" + command + "\t" + instr.getArg(1) + "\t" + instr.getArg(2) + "\t" + std::to_string(bytes);
                                    next.comment = "";
                                }
                                itt = instructions.erase(itt); //remove old instruction
                                continue;
                            }
                        }
                    }
                }
                ++itt;
            }

        }
    }
}