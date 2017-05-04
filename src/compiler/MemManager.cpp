/* 
 * File:   MemManager.cpp
 * Author: wessel
 * 
 * Created on January 11, 2016, 12:32 PM
 */

#include "MemManager.h"
#include "Environment.h"
#include "Nodes.h"
#include "AsmContainer.h"

namespace Cool {

    MemManager::MemManager(const Environment & env, AsmContainer & asmc) : sm(asmc), rm(asmc) {
        RIG::GraphBuilder rigBuilder(env);
        graph = rigBuilder.build();

        if (env.config.inDebug) {
            rigBuilder.print();
        }
    }

    MemManager::~MemManager() {
    }

    void MemManager::store(const std::string& src, const std::string& id, const std::string& com) {
        if (!rm.storeInVar(src, id, com)) {
            sm.storeInVar(src, id, com);
        }
    }

    void MemManager::store(const std::string& src, const std::string& id) {
        if (!rm.storeInVar(src, id)) {
            sm.storeInVar(src, id);
        }
    }

    void MemManager::pushVar(const std::string& src, VariableDefinitionNode & node, const std::string& com) {
        if (graph.find(&node) != graph.end()) {
            int idx = graph.at(&node).reg_idx;
            if (idx < 0) {
                sm.pushVar(src, node.id->getId(), com);
            } else {
                rm.addVar(src, node.id->getId(), idx, com);
            }
        }
    }

    void MemManager::pushVar(const std::string& src, VariableDefinitionNode & node) {
        if (graph.find(&node) != graph.end()) {
            int idx = graph.at(&node).reg_idx;
            if (idx < 0) {
                sm.pushVar(src, node.id->getId());
            } else {
                rm.addVar(src, node.id->getId(), idx);
            }
        }
    }

    void MemManager::load(const std::string& dest, const std::string& id, const std::string& com) {
        if (!rm.load(dest, id, com)) {
            sm.load(dest, id, com);
        }
    }

    void MemManager::load(const std::string& dest, const std::string& id) {
        if (!rm.load(dest, id)) {
            sm.load(dest, id);
        }
    }

    void MemManager::enterScope() {
        sm.enterScope();
        rm.enterScope();

    }

    void MemManager::exitScope() {
        sm.exitScope();
        rm.exitScope();
    }

}