/* 
 * File:   Environment.h
 * Author: len
 *
 * Environment contains the uClasses for compilation
 * After parsing this class is created as a singleton and passed to
 * the analyser, intermediate former and code generator
 * Created on November 21, 2015, 5:19 PM
 */

#ifndef ENVIRONMENT_H
#define	ENVIRONMENT_H
#include <map>
#include <string>
#include <vector>
#include <memory>

#include "AsmContainer.h"
#include "Config.h"
#include "Errors.h"
#include "location.hh"
#include "Nodes.h"

namespace Cool {
   
    class Config;
    class Errors;

    class Environment {
        friend class Parser;
    public:
        using Classes = std::map<std::string, ClassNode*>;
        using Functions = std::vector<FunctionNode*>;
        using Formals = std::vector<FormalNode*>;
        using Blocks = std::vector<BlockNode*>;
        using ClassVariables = std::vector<ClassVariableNode*>;

        Config config;
        mutable Errors errors;
        AsmContainer asmc;

        const Classes & getUserClasses() const;
        const Classes & getBasicClasses() const;
        const std::map<std::string, std::string> & getInheritanceMap() const;
        const Classes getAllClasses()const;

        ClassNode* getUserClass(const std::string& name)const;
        ClassNode* getBasicClass(const std::string& name)const;
        ClassNode* getClass(const std::string & name) const;
        FunctionNode* getFunction(std::string cName, const std::string & fName, bool inheritance = true) const;
        

        const Functions &getClassFunctions(const std::string & cName) const;
        const ClassVariables &getClassVariables(const std::string & cName) const;
        const Blocks &getClassBlocks(const std::string & cName) const;
        const Formals &getClassFormals(const std::string & cName) const;
        Functions getOFuncs(const std::string & cName)const;
        Functions getNOFuncs(const std::string & cName)const;
        
        std::string leastUpperBound(const std::string & type1, const std::string & type2) const;

        //Returns true if 'name' is Null, Nothing or a basic class
        bool isBasicType(const std::string & name) const;
        
        //Returns true if type
        bool typeExists(const std::string & type) const;
        
        bool ltEqInherits(std::string type1, const std::string & type2) const;

        void initInheritanceMap();

        void deleteUserClasses();
        void deleteBasicClasses();
        
        void free(const Node& node);
        
       template<class T>
        T* mk(){
            return static_cast<T*>(allNodes.insert(allNodes.end(), std::make_unique<T>())->get());
        }
           
        template<class T, typename ...Params>
        T* mk(Params&&... params){
            return static_cast<T*>(allNodes.insert(allNodes.end(), std::make_unique<T>(std::forward<Params>(params)...))->get());
        }
        
    private:
        Classes uClasses;
        Classes bClasses;
        std::map<std::string, std::string> inheritanceMap;
        bool addClass(ClassNode&);
        std::vector<std::unique_ptr<Node>> allNodes;
    };
}
#endif	/* ENVIRONMENT_H */

