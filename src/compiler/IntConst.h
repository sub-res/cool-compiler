/* 
 * File:   IntConst.h
 * Author: wessel
 *
 * Created on November 20, 2015, 4:50 PM
 */

#ifndef INTSYMBOL_H
#define INTSYMBOL_H

#include <string>
namespace Cool {

    namespace Assembly {

        class IntConst {
        public:
            IntConst(int id, const std::string & label, int v);
            std::string getAsm() const;
            const int id;
            const std::string label;
            const int value;
        };
    }
}
#endif /* INTSYMBOL_H */

