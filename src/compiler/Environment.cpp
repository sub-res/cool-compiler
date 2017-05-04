/* 
 * File:   Environment.cpp
 * Author: len
 * 
 * Created on November 21, 2015, 5:19 PM
 */

#include "Environment.h"
#include "Types.h"
#include "Nodes.h"
#include "Config.h"
#include "Errors.h"
#include "FreeTreeVisitor.h"

namespace Cool {

    ClassNode* Environment::getClass(const std::string & cname) const {
        if (ClassNode * c = getBasicClass(cname)) {
            return c;
        }
        return getUserClass(cname);
    }

    ClassNode* Environment::getUserClass(const std::string& name) const {
        const auto & itt = uClasses.find(name);
        if (itt != uClasses.end()) {
            return itt->second;
        }
        return nullptr;
    }

    ClassNode* Environment::getBasicClass(const std::string & name) const {
        const auto & itt = bClasses.find(name);
        if (itt != bClasses.end()) {
            return itt->second;
        }
        return nullptr;
    }

    const std::map<std::string, ClassNode*> & Environment::getBasicClasses() const {
        return bClasses;
    }

    const std::map<std::string, ClassNode*> & Environment::getUserClasses()const {
        return uClasses;
    }

    const std::map<std::string, ClassNode*> Environment::getAllClasses() const {
        std::map<std::string, ClassNode*>allNodes(uClasses);
        allNodes.insert(bClasses.begin(), bClasses.end());
        return allNodes;
    }

    const std::map<std::string, std::string> & Environment::getInheritanceMap()const {
        return inheritanceMap;
    }

    void Environment::initInheritanceMap() {
        inheritanceMap.clear();
        for (auto entry : getAllClasses()) {
            inheritanceMap[entry.first] = entry.second->extend->getType();
        }
    }

    std::vector<FunctionNode*> Environment::getOFuncs(const std::string & cname) const {
        std::vector<FunctionNode*>funcs = getClassFunctions(cname);
        for (auto itt = funcs.begin(); itt != funcs.end();) {
            if (!(*itt)->isOverride) {
                itt = funcs.erase(itt);
            } else {
                ++itt;
            }
        }
        return funcs;
    }

    std::vector<FunctionNode*> Environment::getNOFuncs(const std::string & cname) const {
        std::vector<FunctionNode*>funcs = getClassFunctions(cname);
        for (auto itt = funcs.begin(); itt != funcs.end();) {
            if ((*itt)->isOverride) {
                itt = funcs.erase(itt);
            } else {
                ++itt;
            }
        }
        return funcs;
    }

    const std::vector<BlockNode*>& Environment::getClassBlocks(const std::string & cname) const {
        return getClass(cname)->body->feature_blocks;
    }

    const std::vector<FunctionNode*>& Environment::getClassFunctions(const std::string & cname) const {
        return getClass(cname)->body->feature_funcs;
    }

    const std::vector<ClassVariableNode*>& Environment::getClassVariables(const std::string & cname) const {
        return getClass(cname)->body->feature_vars;
    }

    const std::vector<FormalNode*>& Environment::getClassFormals(const std::string & cname) const {
        return getClass(cname)->varFormals->formals;
    }

    bool Environment::isBasicType(const std::string & name) const {
        return name == Types::NOTHING || name == Types::_NULL || getBasicClass(name);
    }

    bool Environment::typeExists(const std::string & type) const {
        return isBasicType(type) || getUserClass(type);
    }

    FunctionNode* Environment::getFunction(std::string cname, const std::string & fname, bool inherited) const {
        while (typeExists(cname) && cname != Types::NOTHING) {
            for (auto func : getClassFunctions(cname)) {
                if (func->getId() == fname) {
                    return func;
                }
            }

            if (!inherited || inheritanceMap.find(cname) == inheritanceMap.end()) {
                return nullptr;
            }
            cname = inheritanceMap.at(cname);
        }
        return nullptr;
    }

    /*returns true if type1 (is equal to/inherits from) type2*/
    bool Environment::ltEqInherits(std::string type1, const std::string & type2) const {
        do {
            if (type1 == type2) {
                return true;
            }
            if (inheritanceMap.find(type1) == inheritanceMap.end()) {
                return false;
            }
            type1 = inheritanceMap.at(type1);
        } while (true);
    }

    /*returns least upper bound of two types. Returns type 'Any' if none is found*/
    std::string Environment::leastUpperBound(const std::string & type1, const std::string & type2) const {
        if (type1 == type2) {
            return type1;
        }

        //NOTHING conforms to all
        if (type1 == Types::NOTHING) {
            return type2;
        }

        if (type2 == Types::NOTHING) {
            return type1;
        }

        //not sure if this is correct regarding nullptr
        if (type1 == Types::_NULL && !(type2 == Types::INT || type2 == Types::BOOL || type2 == Types::UNIT)) {
            return type2;
        }
        if (type2 == Types::_NULL && !(type1 == Types::INT || type1 == Types::BOOL || type1 == Types::UNIT)) {
            return type1;
        }

        std::vector<std::string> vec1{type1};
        std::vector<std::string> vec2{type2};

        std::string stemp = type1;

        while (stemp != Types::ANY && inheritanceMap.find(stemp) != inheritanceMap.end()) {
            stemp = inheritanceMap.at(stemp);
            vec1.push_back(stemp);
        }

        stemp = type2;
        while (stemp != Types::ANY && inheritanceMap.find(stemp) != inheritanceMap.end()) {
            stemp = inheritanceMap.at(stemp);
            vec2.push_back(stemp);
        }

        for (auto i : vec1) {
            for (auto j : vec2) {
                if (i == j) {
                    return i;
                }
            }
        }
        return Types::ANY;
    }

    void Environment::deleteBasicClasses() {
        FreeTreeVisitor freeer(*this);
        for (auto entry : bClasses) {
            entry.second->accept(freeer);
        }
        bClasses.clear();
    }

    void Environment::deleteUserClasses() {
        for (auto entry : uClasses) {
           // delete entry.second;
        }
        uClasses.clear();
    }
    
    void Environment::free(const Node& node){
        allNodes.erase(std::remove_if(allNodes.begin(), allNodes.end(), [&](const std::unique_ptr<Node>& ptr){
            return (ptr.get() == &node);}), allNodes.end());
    }

    bool Environment::addClass(ClassNode & cl) {
        const std::string & cname = cl.getType();
        if (*cl.loc.begin.filename == config.basicFile) {
            if (bClasses.find(cname) == bClasses.end()) {
                bClasses[cname] = &cl;
                return true;
            }
        } else if (uClasses.find(cname) == uClasses.end()) {
            uClasses[cname] = &cl;
            return true;
        }
        return false;
    }
    
    


}