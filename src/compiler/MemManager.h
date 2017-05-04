/* 
 * File:   MemManager.h
 * Author: wessel
 *
 * Created on January 11, 2016, 12:32 PM
 */

#ifndef MEMMANAGER_H
#define	MEMMANAGER_H

#include <string>
#include "RegManager.h"
#include "StackManager.h"
#include "GraphBuilder.h"

namespace Cool {
    
    class VariableDefinitionNode;
    class Environment;
    class AsmContainer;
    
    class MemManager {
    public:
        MemManager(const Environment & env, AsmContainer & asmc);
        virtual ~MemManager();

        void store(const std::string& src, const std::string& id, const std::string& com);
        void store(const std::string& src, const std::string& id);
        void load(const std::string& dest, const std::string& id, const std::string& com);
        void load(const std::string& dest, const std::string& id);

        void pushVar(const std::string& src, VariableDefinitionNode & node, const std::string& com);
        void pushVar(const std::string& src, VariableDefinitionNode & node);

        void enterScope();
        void exitScope();

        StackManager sm;
        RegManager rm;
        std::map<const VariableDefinitionNode*, RIG::NodeInfo> graph;
    };
}

#endif	/* MEMMANAGER_H */

