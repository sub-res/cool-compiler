/* 
 * File:   GraphBuilder.h
 * Author: len
 *
 * Created on January 16, 2016, 2:57 PM
 */

#ifndef GRAPHBUILDER_H
#define GRAPHBUILDER_H
#include <map>
#include <vector>
#include <stack>
#include <memory>
#include "Visitor.h"
#include "position.hh"

namespace Cool {
    class Environment;

    namespace RIG {

        class NodeInfo {
        public:
            NodeInfo() = default;
            NodeInfo(const position & lastUsedPos, const FunctionNode* func = nullptr);
            std::vector<const VariableDefinitionNode*>interferences;
            bool interferesSelf = false;
            int reg_idx = -1;
            const position * lastUsedPos = nullptr; //last usage position
            const FunctionNode * func;
            bool contains(const VariableDefinitionNode& node);
        };

        class GraphBuilder : public Visitor {
        public:
            GraphBuilder(const Environment & env);
            std::map<const VariableDefinitionNode*, NodeInfo> build();
            void reset();

            void visit(VariableDefinitionNode*);
            void visit(ObjectFunctionCallNode*);
            void visit(FunctionCallNode*);
            void visit(SuperNode*);
            void visit(NewNode*);

            void visit(BlockNode*);
            void leave(BlockNode*);

            void print();
            void colorGraph();
        private:
            const Environment & env;

            BlockNode*curBlock; //current visiting block
            FunctionNode*curFunc; //current visiting function

            std::map<const VariableDefinitionNode*, NodeInfo> graph; //the interference graph
            std::map<FunctionNode*, std::vector<const VariableDefinitionNode*> > funcVars; //functions linked to their local vars
            std::map<const VariableDefinitionNode*, std::vector<FunctionNode*> > interferingFuncs; //functions called during life of var

            /*class for determining the life (duration range) of a local defined variable*/
            class LastUsageFinder : public Visitor {
            public:
                const position * find(VariableDefinitionNode & forDef, BlockNode & inBlock);
                void visit(ObjectNode*);
                void visit(VariableDefinitionNode*);
                void visit(WhileNode*);
                void leave(WhileNode*);
            private:
                bool defInWhile;
                int whileDepthOfUse;
                int whileDepth;
                int scopeOfDef;
                int scope;
                const position * lastPos;
                const VariableDefinitionNode * def;
            } lastUsageFinder;

            class DispatchInterferenceFinder : public Visitor {
            public:
                DispatchInterferenceFinder(GraphBuilder &);

                void linkIfAny(const VariableDefinitionNode & forDef, NodeInfo & info, FunctionNode & dispFunc);
                void visit(FunctionNode*);
                void visit(ObjectFunctionCallNode*);
                void visit(FunctionCallNode*);
                void visit(SuperNode*);
                void visit(NewNode*);

            private:
                void visitFunction(FunctionNode * node);
                NodeInfo * nodeInfo;
                std::vector<const FunctionNode*> callStack;
                GraphBuilder & builder;
                const VariableDefinitionNode * forDef;
                std::string currFunc;
            };

            /*class representing a scope, used to iterate over the scoped var definitions*/
            class ScopeBlock {
            public:
                ScopeBlock(std::unique_ptr<ScopeBlock>&&env, BlockNode*block);
                void push(const VariableDefinitionNode & node);
                std::unique_ptr<ScopeBlock> prev;
                std::vector<const VariableDefinitionNode*>nodes;
                BlockNode*blNode;
            };
            std::unique_ptr<ScopeBlock> scopeBlock;

            void enterScope(BlockNode & BlockNode);
            void exitScope();

            void connectNodes(const VariableDefinitionNode & n1, const VariableDefinitionNode & n2);

            bool inGraph(const VariableDefinitionNode & node)const;
            NodeInfo & get(const VariableDefinitionNode & node);
            void addNode(VariableDefinitionNode & node);
            void addToFunction(const VariableDefinitionNode & node, FunctionNode & func);

            void addDispatch(FunctionNode & func, const VariableDefinitionNode & node);
            void addDispatch(const position & dispatchPos, FunctionNode&func);
        };
    }
}
#endif /* GRAPHBUILDER_H */

