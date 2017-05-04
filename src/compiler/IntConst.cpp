/* 
 * File:   IntConst.cpp
 * Author: wessel
 * 
 * Created on November 20, 2015, 4:50 PM
 */

#include "IntConst.h"

namespace Cool {
    namespace Assembly {

        IntConst::IntConst(const int i, const std::string & l, const int v) : id(i), label(l), value(v) {
        }

        std::string IntConst::getAsm() const {
            std::string r = "\t.word\t-1\n" + 
                    label +
                    ":\n\t.word\t" + std::to_string(id) + //placeholder identifier
                    "\n\t.word\t" + std::to_string(4) +
                    "\n\t.word\tInt_dispTab" +
                    "\n\t.word\t" + std::to_string(value);
            return r;
        }
    }
}

