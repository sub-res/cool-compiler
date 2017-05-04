/* 
 * File:   BoolConst.cpp
 * Author: wessel
 * 
 * Created on November 20, 2015, 4:51 PM
 */

#include "BoolConst.h"
namespace Cool {
    namespace Assembly {

        BoolConst::BoolConst(const int i, const std::string & l, const bool v) : id(i), value(v), label(l) {
        }

        std::string BoolConst::getAsm() const{
            std::string r = "\t.word\t-1\n" +  
                    label +
                    ":\n\t.word\t" + std::to_string(id) + //placeholder identifier
                    "\n\t.word\t" + std::to_string(4) +
                    "\n\t.word\tBoolean_dispTab" +
                    "\n\t.word\t" + std::to_string(value ? 1 : 0);

            return r;
        }
    }
}