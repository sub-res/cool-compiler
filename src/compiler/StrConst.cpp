/* 
 * File:   StrConst.cpp
 * Author: wessel
 * 
 * Created on November 20, 2015, 4:51 PM
 */

#include "StrConst.h"
namespace Cool {
    namespace Assembly {

        StrConst::StrConst(const int i, const std::string & lab, const std::string & v, const std::string & l) : id(i), label(lab), value(v), lengthLabel(l) {
        }

        std::string StrConst::getAsm() const {

            std::string r = "\t.word\t-1\n" + 
                    label +
                    ":\n\t.word\t" + std::to_string(id) +
                    "\n\t.word\t" + std::to_string(4 + (((value.length() + 1) / 4) + ((value.length() + 1) % 4 != 0 ? 1 : 0))) +
                    "\n\t.word\tString_dispTab\n\t.word\t" + lengthLabel;
            if (!value.empty()) {
                r += "\n\t.ascii\t\"" + value + "\"";
            }
            r +=
                    "\n\t.byte\t" + std::to_string(0) +
                    "\n\t.align\t" + std::to_string(2);
            return r;
        }
    }
}