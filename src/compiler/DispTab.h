/* 
 * File:   DispTab.h
 * Author: len
 *
 * Created on November 25, 2015, 6:13 PM
 */

#ifndef DispTab_H
#define	DispTab_H

#include <vector>
#include <string>

namespace Cool {

    /*DispatchTable contains all functions (in the form of entries) of a class,
     including inherited functions*/
    class DispTab {
    public:

        /*A single entry for the DispTab
         ex. for: Any.toString
         cName = Any
         fName = toString*/
        class Entry {
        public:
            Entry(const std::string & c, const std::string & f);
            Entry(const Entry&orig);
            //name of the class
            const std::string cName;
            //name of the function
            const std::string fName;
        };

        //entries (functions) in table
        const std::vector<Entry>entries;

        //name for the symbol in assembly
        const std::string label;

        
        DispTab(const std::string & label, const std::vector<Entry> & e);

        //returns the assembly code
        std::string getAsm() const;

        //returns the index of the given function in the table, from 0 to length-1
        int getIndex(const std::string & fname)const;

    };
}

#endif	/* DispTab_H */

