/* 
 * File:   GraphBuilder.cpp
 * Author: len
 * 
 * Created on January 16, 2016, 2:57 PM
 */

#include "GraphBuilder.h"
#include "Nodes.h"
#include "Environment.h"
#include "Config.h"
#include <algorithm>

namespace Cool {

    // Returns true is position p1 is greater than p2
    static bool gt(const position &p1, const position & p2) {
            return p1.line > p2.line || (p1.line == p2.line && p1.column > p2.column);
    }
    
    namespace RIG {

        GraphBuilder::GraphBuilder(const Environment & _env) : env(_env) {
        }

        std::map<const VariableDefinitionNode*, NodeInfo> GraphBuilder::build() {
            reset();
            DispatchInterferenceFinder finder(*this);

            const auto & classes = env.getAllClasses();
            for (const auto & cEntry : classes) {
                const auto & functions = cEntry.second->body->feature_funcs;
                for (const auto function : functions) {
                    curFunc = function;
                    curFunc->expr->accept(*this, true);
                }
            }

            //iterate over every dispatch in live range of var 
            for (auto & interference : interferingFuncs) {
                for (auto function : interference.second) {
                    finder.linkIfAny(*interference.first, get(*interference.first), *function);
                }
            }
            colorGraph();
            return graph;
        }

        void GraphBuilder::colorGraph() {
            std::vector<NodeInfo*> ni;

            for (auto & entry : graph) {
                ni.push_back(&entry.second);
            }

            //sort using lambda 
            std::sort(ni.begin(), ni.end(), [](const NodeInfo*a, const NodeInfo * b)->bool {
                return a->interferences.size() > b->interferences.size();
            });

            for (auto nInfo : ni) {
                if (nInfo->interferesSelf) {
                    continue;
                }

                std::vector<int> used_regs;
                for (auto interference : nInfo->interferences) {
                    if (graph.at(interference).reg_idx != -1) {
                        used_regs.push_back(graph.at(interference).reg_idx);
                    }
                }

                for (int i = 0; i < Config::NR_REGS; ++i) {
                    if (std::find(used_regs.begin(), used_regs.end(), i) == used_regs.end()) {
                        nInfo->reg_idx = i;
                        break;
                    }
                }
            }
        }

        void GraphBuilder::reset() {
            graph.clear();
            funcVars.clear();
            interferingFuncs.clear();
            scopeBlock = nullptr;
        }

        void GraphBuilder::visit(VariableDefinitionNode*node) {
            addNode(*node);
        }

        void GraphBuilder::visit(FunctionCallNode*node) {
            addDispatch(node->loc.begin, *env.getFunction(node->getClass(), node->getId()));
        }

        void GraphBuilder::visit(SuperNode*node) {
            addDispatch(node->loc.begin, *env.getFunction(node->getClass(), node->getId()));
        }

        void GraphBuilder::visit(ObjectFunctionCallNode*node) {
            addDispatch(node->loc.begin, *env.getFunction(node->expr->getType(), node->getId()));
        }

        void GraphBuilder::visit(BlockNode*node) {
            curBlock = node;
            enterScope(*node);
        }

        void GraphBuilder::leave(BlockNode*) {
            exitScope();
        }

        void GraphBuilder::visit(NewNode*node) {
            addDispatch(node->loc.begin, *env.getFunction(node->getType(), node->getType()));
        }

        void GraphBuilder::enterScope(BlockNode& BlockNode) {
            scopeBlock = std::make_unique<ScopeBlock>(std::move(scopeBlock), &BlockNode);
        }

        void GraphBuilder::exitScope() {
            scopeBlock = std::move(scopeBlock->prev);
        }

        void GraphBuilder::addNode(VariableDefinitionNode& node) {
            const position * pos = lastUsageFinder.find(node, *scopeBlock->blNode);
            if (*pos == node.id->loc.begin) {
                return; //unused var, ignore
            }

            graph[&node] = NodeInfo(*pos, curFunc);

            { //create connections with earlier var definitions
                //  NodeInfo & info = get(node);

                for (auto s = scopeBlock.get(); s; s = s->prev.get()) {
                    for (std::vector<const VariableDefinitionNode*>::iterator itt = s->nodes.begin(); itt != s->nodes.end();) {
                        if (gt((*get(**itt).lastUsedPos),node.loc.begin)) {
                            connectNodes(node, **itt);
                            //info.interferences.push_back(*itt);
                        } else {
                            // itt = s->nodes.erase(itt);
                        }
                        ++itt;
                    }
                }
            }

            scopeBlock->push(node);
            addToFunction(node, *curFunc);
        }

        void GraphBuilder::addToFunction(const VariableDefinitionNode& node, FunctionNode& func) {
            if (funcVars.find(&func) == funcVars.end()) {
                funcVars[&func] = std::vector<const VariableDefinitionNode*>();
            }
            funcVars[&func].push_back(&node);
        }

        void GraphBuilder::addDispatch(const position & dispatchPos, FunctionNode&func) {
            for (auto s = scopeBlock.get(); s; s = s->prev.get()) {
                for (std::vector<const VariableDefinitionNode*>::iterator itt = s->nodes.begin(); itt != s->nodes.end(); ++itt) {
                    if (gt((*get(**itt).lastUsedPos), dispatchPos)) {
                        addDispatch(func, **itt);
                    } else {
                        // itt = s->nodes.erase(itt);
                    }
                }
            }
        }

        void GraphBuilder::addDispatch(FunctionNode& func, const VariableDefinitionNode& node) {
            if (interferingFuncs.find(&node) == interferingFuncs.end()) {
                interferingFuncs[&node] = std::vector<FunctionNode*>();
            }
            interferingFuncs[&node].push_back(&func);
        }

        bool GraphBuilder::inGraph(const VariableDefinitionNode& node)const {
            for (const auto & entry : graph) {
                if (entry.first == &node) {
                    return true;
                }
            }
            return false;
        }

        void GraphBuilder::connectNodes(const VariableDefinitionNode& n1, const VariableDefinitionNode& n2) {
            NodeInfo & info1 = get(n1);
            NodeInfo & info2 = get(n2);
            if (!info1.contains(n2)) {
                info1.interferences.push_back(&n2);
            }
            if (!info2.contains(n1)) {
                info2.interferences.push_back(&n1);
            }
        }

        NodeInfo & GraphBuilder::get(const VariableDefinitionNode&node) {
            for (auto & entry : graph) {
                if (entry.first == &node) {
                    return entry.second;
                }
            }
            throw std::runtime_error(__PRETTY_FUNCTION__ + std::string("No such entry in graph"));
        }

        void GraphBuilder::print() {
            for (const auto & entry : graph) {
                std::cout << entry.first->getId() << ": dies(";
                const position * const pos = entry.second.lastUsedPos;
                std::cout << std::to_string(pos->line) << ":" << std::to_string(pos->column) << ") conflicts(";
                for (const auto varDef : entry.second.interferences) {
                    std::cout << varDef->getId() << " ";
                }
                std::cout << "). reg: " << entry.second.reg_idx << std::endl;
            }
        }

        /*SCOPEBLOCK*/
        GraphBuilder::ScopeBlock::ScopeBlock(std::unique_ptr<ScopeBlock>&& env, BlockNode * node) : blNode(node) {
            prev = std::move(env);
        }

        void GraphBuilder::ScopeBlock::push(const VariableDefinitionNode & node) {
            nodes.push_back(&node);
        }

        /*NODEINFO*/
        NodeInfo::NodeInfo(const position & lastPos, const FunctionNode*f) : lastUsedPos(&lastPos), func(f) {
        }

        bool NodeInfo::contains(const VariableDefinitionNode& node) {
            for (const auto varDef : interferences) {
                if (varDef == &node) {
                    return true;
                }
            }
            return false;
        }

        /*LASTUSAGEFINDER*/
        const position * GraphBuilder::LastUsageFinder::find(VariableDefinitionNode& forDef, BlockNode & block) {
            lastPos = &forDef.id->loc.begin;
            def = &forDef;
            whileDepth = 0;
            whileDepthOfUse = 0;
            defInWhile = false;
            block.accept(*this, true);
            return lastPos;
        }

        void GraphBuilder::LastUsageFinder::visit(WhileNode*node) {
            ++whileDepth;
        }

        void GraphBuilder::LastUsageFinder::leave(WhileNode*node) {
            if (!defInWhile) {
                if (whileDepthOfUse) {
                    lastPos = &node->end;
                }
            }
            --whileDepth;
        }

        void GraphBuilder::LastUsageFinder::visit(VariableDefinitionNode*node) {
            if (node == def) {
                if (whileDepth) {
                    defInWhile = true;
                }
            }
        }

        void GraphBuilder::LastUsageFinder::visit(ObjectNode*node) {
            if (node->getId() == def->getId()) {
                lastPos = &node->loc.begin;
                if (whileDepth) {
                    whileDepthOfUse = whileDepth;
                }
            }
        }

        GraphBuilder::DispatchInterferenceFinder::DispatchInterferenceFinder(GraphBuilder & b) : builder(b) {
        }

        void GraphBuilder::DispatchInterferenceFinder::linkIfAny(const VariableDefinitionNode & _forDef, NodeInfo & info, FunctionNode& func) {
            callStack.clear();
            forDef = &_forDef;
            nodeInfo = &info;
            callStack.push_back(info.func);
            func.accept(*this, true);
        }

        void GraphBuilder::DispatchInterferenceFinder::visit(FunctionNode*node) {
            callStack.push_back(node);
            if (builder.funcVars.find(node) != builder.funcVars.end()) {
                for (auto varDef : builder.funcVars[node]) {
                    builder.connectNodes(*forDef, *varDef);
                }
            }

        }

        void GraphBuilder::DispatchInterferenceFinder::visit(ObjectFunctionCallNode*node) {
            visitFunction(builder.env.getFunction(node->expr->getType(), node->getId()));
        }

        void GraphBuilder::DispatchInterferenceFinder::visit(FunctionCallNode*node) {
            visitFunction(builder.env.getFunction(node->getClass(), node->getId()));

        }

        void GraphBuilder::DispatchInterferenceFinder::visit(SuperNode*node) {
            visitFunction(builder.env.getFunction(node->getClass(), node->getId()));
        }

        void GraphBuilder::DispatchInterferenceFinder::visit(NewNode*node) {
            visitFunction(builder.env.getFunction(node->getType(), node->getType()));
        }

        void GraphBuilder::DispatchInterferenceFinder::visitFunction(FunctionNode* func) {
            if (std::find(callStack.begin(), callStack.end(), func) == callStack.end()) {
                func->accept(*this, true);
            } else if (func == nodeInfo->func) {
                nodeInfo->interferesSelf = true;
            }
        }
    }
}