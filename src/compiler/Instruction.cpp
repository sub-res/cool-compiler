/* 
 * File:   Instruction.cpp
 * Author: len
 * 
 * Created on January 13, 2016, 12:27 PM
 */
#include <vector>
#include <iostream>
#include <sstream>
#include <exception>
#include <stdexcept>
#include "Instruction.h"
namespace Cool {
    namespace Assembly {

        Instruction::Instruction(const std::string& l, const std::string& c) : line(l), comment(c) {
        }
        
        std::string Instruction::getArg(unsigned i) const {
            return split(i);
        }

        std::string Instruction::getInstruction() const {
            return split(0);
        }

        std::string Instruction::get()const {
            return line;
        }

        std::string Instruction::getCom() const {
            return comment;
        }

        bool Instruction::isInstruction() const {
            return !(isComment() || isLabel());
        }

        std::string Instruction::split(unsigned i) const {
            std::vector<std::string> segs;
            std::string buf = "";
            for (const auto & c : line) {
                if (c == '\t' || c == ' ') {
                    if (!buf.empty()) {
                        segs.push_back(buf);
                        buf.clear();
                    }
                } else {
                    buf += c;
                }
            }
            if (!buf.empty()) {
                segs.push_back(buf);
            }

            if (i < segs.size()) {
                return segs[i];
            } else {
                throw std::logic_error("arg search out of range!");
            }
            return "";
        }

        bool Instruction::isComment() const {

            if (line.empty() || line.front() == '#') {
                return true;
            }
            
            for(const auto & c : line){
                if(!isspace(c)){
                    return false;
                }
            }
            return true;
        }

        bool Instruction::isLabel() const {
            for (auto i = line.cend() - 1; i != line.cbegin(); --i) {
                if (*i == ':') {
                    return true;
                }
            }
            return false;
        }
    }
}