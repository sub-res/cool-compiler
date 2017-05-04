/* 
 * File:   Config.h
 * Author: wessel
 *
 * Created on November 18, 2015, 2:57 PM
 */

#ifndef CONFIG_H
#define	CONFIG_H
#include <string>
namespace Cool {
    class Config{
    public:
        Config(){}
        //for runtime garbage collector
        bool inDebug = false;
        bool gcTest = false;
        bool gc = true;
        bool useRegister = true;
        bool library = false; //if true generates assembly of complete source code, only for used code otherwise.
        
        static const int NR_REGS = 11;
        
        std::string outfile = "file1.s"; //output file
        
        static const std::string basicFile;
        static const std::string ignores;
    };
}


#endif	/* CONFIG_H */

