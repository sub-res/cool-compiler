/* 
 * File:   StrConst.h
 * Author: wessel
 *
 * Created on November 20, 2015, 4:51 PM
 */

#ifndef STRINGSYMBOL_H
#define STRINGSYMBOL_H

#include <string>
namespace Cool {
    namespace Assembly {

        class StrConst {
        public:
            StrConst(int id, const std::string & label, const std::string & v, const std::string & lengthLabel);
            std::string getAsm() const;
            const int id;
            const std::string label;
            const std::string value;
            const std::string lengthLabel;
        private:

        };
    }
}

#endif /* STRINGSYMBOL_H */

