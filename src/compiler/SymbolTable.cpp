/* 
 * File:   SymbolTable.cpp
 * Author: len
 * 
 * Created on November 20, 2015, 6:04 PM
 */

#include <algorithm>
#include "SymbolTable.h"
#include "Nodes.h"
#include "Types.h"
#include "Config.h"
#include "UsageChecker.h"
#include "Environment.h"

namespace Cool {

    const std::string SymbolTable::boolLabel = "bool_const";
    const std::string SymbolTable::strLabel = "string_const";
    const std::string SymbolTable::intLabel = "int_const";

    SymbolTable::SymbolTable(const Environment & environment, const UsageChecker * const u) : env(environment), usageChecker(u) {

        const std::map<std::string, ClassNode*>& classes = environment.getUserClasses();
        for (std::map<std::string, ClassNode*>::const_iterator itt = classes.begin(); itt != classes.end(); ++itt) {
            //   if ((!u) || u->typeUsed(itt->first)) {
            classNameTab.push_back(itt->first);
            // }
        }
        const std::map<std::string, ClassNode*>& basicClasses = environment.getBasicClasses();
        for (std::map<std::string, ClassNode*>::const_iterator itt = basicClasses.begin(); itt != basicClasses.end(); ++itt) {
            //  if ((!u) || u->typeUsed(itt->first)) {
            classNameTab.push_back(itt->first);
            //}
        }

        boolConsts.insert({c, Assembly::BoolConst(getCNameIndex(Types::BOOL), "boolean_lit0", false)});
        defBool = c;
        ++c;
        boolConsts.insert({c, Assembly::BoolConst(getCNameIndex(Types::BOOL), "boolean_lit1", true)});
        ++c;
        
        defStr = createStrConst("");
        defInt = createIntConst(0);
    }

    int SymbolTable::getDefBoolConst() const {
        return defBool;
    }

    int SymbolTable::getDefIntConst() const {
        return defInt;
    }

    int SymbolTable::getDefStrConst() const {
        return defStr;
    }

    /*key = class symId, value= string const symId */
    const std::map<int, int>&SymbolTable::getCNameTabs() const {
        return nameLinks;
    }

    const std::map<std::string, DispTab>& SymbolTable::getDispTabs() const {
        return dispTabs;
    }

    const std::map<int, Assembly::StrConst>& SymbolTable::getStrConsts() const {
        return strConsts;
    }

    const std::map<int, Assembly::IntConst>& SymbolTable::getIntConsts() const {
        return intConsts;
    }

    const std::map<int, Assembly::BoolConst>& SymbolTable::getBoolConsts() const {
        return boolConsts;
    }
    
    void SymbolTable::build() {

        SymbolLinker linker(*this);
        linker.link(env.getUserClasses());
        linker.link(env.getBasicClasses());
    }

    int SymbolTable::insertCName(const std::string & cname) {
        int index = getCNameIndex(cname);
        nameLinks[index] = createStrConst(cname);
        return index;
    }

    void SymbolTable::addFileConst(const std::string & filename) {
        if (filenames.find(filename) == filenames.end()) {
            filenames[filename] = createStrConst(filename);
        }
    }

    const Assembly::StrConst& SymbolTable::getFileConst(const std::string & filename) const {
        return strConsts.at(filenames.at(filename));
    }

    int SymbolTable::getDispWordOffset(const std::string & cname, const std::string & fname) const {
        return dispTabs.at(cname).getIndex(fname);
    }

    int SymbolTable::getDispByteOffset(const std::string & cname, const std::string & fname) const {
        return getDispWordOffset(cname, fname)*4;
    }

    int SymbolTable::getConstIntId(const int value) const {
        for (const auto & i : intConsts) {
            if (i.second.value == value) {
                return i.first;
            }
        }
        return -1;
    }
    
    int SymbolTable::getConstBoolId(const bool value) const {
        for (const auto & b : boolConsts) {
            if (b.second.value == value) {
                return b.first;
            }
        }
        return -1;
    }

    int SymbolTable::getConstStrId(const std::string & value) const {
        for (const auto & s : strConsts) {
            if (s.second.value == value) {
                return s.first;
            }
        }
        return -1;
    }

    const Assembly::IntConst& SymbolTable::getIntConst(int i) const {
        const auto it = intConsts.find(i);
        if (it != intConsts.cend()) {
            return it->second;
        }
        throw std::runtime_error(__FUNCTION__ + std::string("No such  object"));
    }

    const Assembly::StrConst& SymbolTable::getStrConst(int i) const {
        const auto it = strConsts.find(i);
        if (it != strConsts.cend()) {
            return it->second;
        }
        throw std::runtime_error(__FUNCTION__ + std::string("No such  object"));
    }

    const Assembly::BoolConst& SymbolTable::getBoolConst(int i) const {
        const auto it = boolConsts.find(i);
        if (it != boolConsts.cend()) {
            return it->second;
        }
        throw std::runtime_error(__FUNCTION__ + std::string("No such  object"));
    }

    const DispTab& SymbolTable::getDispTab(const std::string & cname) const {
        return dispTabs.find(cname)->second;
    }

    int SymbolTable::getCNameIndex(const std::string & cname) const {
        for (unsigned int i = 0; i < classNameTab.size(); ++i) {
            if (classNameTab[i] == cname) {
                return i;
            }
        }
        return -1;
    }

    int SymbolTable::createBoolConst(bool v) {
        int id = getConstBoolId(v);
        if (id != -1) {
            return id;
        }
        boolConsts.insert({c, Assembly::BoolConst(getCNameIndex(Types::BOOL), boolLabel + std::to_string(boolC++), v)});

        return c++;
    }

    int SymbolTable::createIntConst(int v) {
        int id = getConstIntId(v);
        if (id != -1) {
            return id;
        }
        intConsts.insert({c, Assembly::IntConst(getCNameIndex(Types::INT), intLabel + std::to_string(intC++), v)});

        return c++;
    }

    int SymbolTable::createStrConst(std::string v) {
        int id = getConstStrId(v);
        if (id != -1) {
            return id;
        }
        strConsts.insert({c,Assembly::StrConst(getCNameIndex(Types::STRING), strLabel + std::to_string(strC++), v, intConsts.at(createIntConst(v.length())).label)});

        return c++;
    }

    void SymbolTable::createDispTab(const ClassNode*node) {
        std::vector<const ClassNode*>classes;
        while (true) {
            classes.insert(classes.begin(), node);
            if (node->extend->getType() == Types::NATIVE) {
                break;
            }
            node = env.getClass(node->extend->getType());
        }

        for (decltype(classes.size()) i = 0; i < classes.size(); ++i) {

            const ClassNode*node = classes[i];
            const std::string cName = node->getType();
            if (!dispTabExists(cName)) {

                std::vector<DispTab::Entry>cTable;

                if (i != 0) { //Any is 0
                    const ClassNode* parent = classes[i - 1];
                    const std::string pName = parent->getType();
                    const auto & pTable = getDispTab(pName).entries;
                    const auto oFuncs = env.getOFuncs(cName);
                    
                    bool overridden = false;
                    for (const auto & d : pTable) {
                        for (const auto & f : oFuncs) {
                            if (f->getId() == d.fName) {
                                cTable.push_back(DispTab::Entry(cName, f->getId()));
                                overridden = true;
                                break;
                            }
                        }
                        if (overridden) {
                            overridden = false;
                        } else {
                            cTable.push_back(DispTab::Entry(d));
                        }
                    }
                }
                //add own functions
                for (const auto f : env.getNOFuncs(cName)) {
                    cTable.push_back(DispTab::Entry(cName, f->getId()));
                }
                dispTabs.insert({cName, DispTab(getDispTabLabel(cName), cTable)});
            }
        }
    }

    int SymbolTable::getAttrWordOffset(const std::string & cname, const std::string & attr) const {
        const ClassNode* node = env.getClass(cname);
        std::vector<const ClassNode*> nodes;
        while (true) {
            nodes.insert(nodes.begin(), node); //create vector of classes in order Any, rest of classes

            if (node->extend->getType() == Types::NATIVE) {
                break;
            }
            node = env.getClass(node->extend->getType());
        }

        int attr_ctr = 0;
        for (unsigned int i = 0; i < nodes.size(); i++) {
            for (unsigned int j = 0; j < nodes[i]->body->feature_vars.size(); j++, attr_ctr++) {
                if (attr == nodes[i]->body->feature_vars[j]->getId()) {
                    return attr_ctr + 3;
                }
            }
        }
        return -1;
    }

    int SymbolTable::getAttrByteOffset(const std::string & cname, const std::string & attr) const {
        return getAttrWordOffset(cname, attr) * 4;
    }

    SymbolTable::SymbolLinker::SymbolLinker(SymbolTable & t) : symTable(t) {}
    
    void SymbolTable::SymbolLinker::link(const std::map<std::string, ClassNode*> & classes) {
        for (std::map<std::string, ClassNode*>::const_iterator itt = classes.begin(); itt != classes.end(); ++itt) {
            // if ((!symTable->usageChecker) || symTable->usageChecker->typeUsed(itt->first)) {
            itt->second->accept(*this, true);
            //}
        }
    }

    void SymbolTable::SymbolLinker::visit(ClassNode* node) {
        node->stId = symTable.insertCName(node->getType());
        symTable.createDispTab(node);
        symTable.addFileConst(*node->loc.begin.filename);
    }

    void SymbolTable::SymbolLinker::visit(BoolNode*node) {
        node->stId = symTable.createBoolConst(node->val);
    }

    void SymbolTable::SymbolLinker::visit(IntNode*node) {
        node->stId = symTable.createIntConst(node->val);
    }

    void SymbolTable::SymbolLinker::visit(StringNode* node) {
        node->stId = symTable.createStrConst(node->val);
    }

    bool SymbolTable::dispTabExists(const std::string& cname) const {
        return dispTabs.find(cname) != dispTabs.cend();
    }
}