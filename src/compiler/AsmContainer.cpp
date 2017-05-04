/* 
 * File:   AsmContainer.cpp
 * Author: wessel
 * 
 * Created on January 13, 2016, 12:46 PM
 */

#include <iostream>
#include <fstream>

#include "AsmContainer.h"

namespace Cool {

    AsmContainer::AsmContainer() {
        text_start = 0;
    }

    int AsmContainer::textSegStart() const{
        return text_start;
    }
    
    void AsmContainer::add(const std::string& ins, const std::string& com) {
        add(Assembly::Instruction(ins, com));
    }
    
    void AsmContainer::add(Assembly::Instruction&& ins) {
        instructions.push_back(std::move(ins));
    }
    
    void AsmContainer::print(const std::string& outfile, bool printComments) const{
        std::ofstream file;
        file.open(outfile);
        if (file.is_open()) {
            file.clear(); //clear contents
            for (auto it = instructions.cbegin(); it != instructions.cend(); ++it) {
                file << it->get()<< (it->getCom().length() > 0 && printComments ? ("\t#" + it->getCom()) : "") << "\n";
            }
            file.close();
        } else {
            std::cout << "Couldn't open file " + outfile << std::endl;
        }
    }
    
    void AsmContainer::setTextSegStart() {
        text_start = instructions.size();
    }
}

