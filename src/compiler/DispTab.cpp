/* 
 * File:   DispTab.cpp
 * Author: len
 * 
 * Created on November 25, 2015, 6:13 PM
 */

#include "DispTab.h"

namespace Cool {

    DispTab::DispTab(const std::string & l, const std::vector<Entry> & e) : entries(e), label(l){
    }

    std::string DispTab::getAsm() const {
        std::string r = label + ":";
        for (decltype(entries.size())i = 0; i < entries.size(); ++i) {
            r += "\n\t.word\t" + entries[i].cName + "." + entries[i].fName;
        }
        return r;
    }

    int DispTab::getIndex(const std::string & fname) const {
        for (decltype(entries.size()) i = 0; i < entries.size(); ++i) {
            if (entries[i].fName == fname) {
                return i;
            }
        }
        return -1;
    }

    DispTab::Entry::Entry(const std::string & c, const std::string & f) : cName(c), fName(f) {
    }

    DispTab::Entry::Entry(const Entry & orig) : cName(orig.cName), fName(orig.fName) {
    }
}

