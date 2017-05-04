/* 
 * File:   BoolConst.h
 * Author: wessel
 *
 * Created on November 20, 2015, 4:51 PM
 */

#ifndef BOOLSYMBOL_H
#define BOOLSYMBOL_H

#include <string>
namespace Cool {
    namespace Assembly {

        class BoolConst {
        public:
            BoolConst(int id, const std::string & label, bool v);
            
            std::string getAsm() const;
            
            const int id;
            const bool value;
            const std::string label;
        };
    }
}
#endif /* BOOLSYMBOL_H */

