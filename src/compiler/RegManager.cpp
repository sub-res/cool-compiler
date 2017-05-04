/* 
 * File:   RegManager.cpp
 * Author: wessel
 * 
 * Created on January 11, 2016, 12:32 PM
 */

#include "RegManager.h"
#include "FileIO.h"

namespace Cool {
    
    //available registers: $s1-$s6, $t5-$t9

    RegManager::RegManager(AsmContainer & _asmc): asmc(&_asmc){
        for (unsigned i = 0; i < Config::NR_REGS; ++i) {
            if (i < 6) {
                locs[i].address = "$s" + std::to_string(i + 1);
            } else {
                locs[i].address = "$t" + std::to_string(i - 1);
            }
        }
    }
    
    bool RegManager::load(const std::string& dest, const std::string& id, const std::string& com){
        for (unsigned i = 0; i < Config::NR_REGS; ++i) {
            if (locs[i].value == id && scopectr >= locs[i].scope) {
                asmc->add("\tmove\t" + dest + "\t" + locs[i].address, com);
                return true;
            }
        }
        return false; //not found in regs, switch to stack
    }
    
    
    bool RegManager::storeInVar(const std::string& src, const std::string& id, const std::string& com) {
        for (unsigned i = 0; i < Config::NR_REGS; ++i) {
            if (locs[i].scope <= scopectr && locs[i].value == id) {
                asmc->add("\tmove\t" + locs[i].address + "\t" + src, com);
                return true;
            }
        }
        return false;
    }
    
    void RegManager::addVar(const std::string& src, const std::string& id, int idx, const std::string& com){        
        locs[idx].value = id;
        locs[idx].scope = scopectr;
        asmc->add("\tmove\t" + locs[idx].address + "\t" + src, com);
    }
    
    void RegManager::enterScope() {
        ++scopectr;
    }
    
    void RegManager::exitScope() {
        for (unsigned i = 0; i < Config::NR_REGS; ++i) {
            if (locs[i].scope >= scopectr) {
                locs[i].value = "";
            }
        }
        --scopectr;
    }
    
}