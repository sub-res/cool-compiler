/* 
 * File:   RegManager.h
 * Author: wessel
 *
 * Created on January 11, 2016, 12:32 PM
 */

#ifndef REGMANAGER_H
#define	REGMANAGER_H

#include <string>
#include "Config.h"
#include "AsmContainer.h"

namespace Cool {

    class RegManager {
    public:
        RegManager(AsmContainer & asmc);
        
        void addVar(const std::string &src, const std::string& id, int idx, const std::string & com = "store in register");
        bool load(const std::string & dest, const std::string& id, const std::string & com = "load from register");
        
        bool storeInVar(const std::string & src, const std::string & id, const std::string & com = "store in register");
        
        void enterScope();
        void exitScope();
    private:
        int scopectr = 0;
        AsmContainer * asmc;
        
        struct reg_loc {
            std::string address;
            std::string value;
            int scope = 0;
        };
        struct reg_loc locs[Config::NR_REGS];
    };
}

#endif	/* REGMANAGER_H */

