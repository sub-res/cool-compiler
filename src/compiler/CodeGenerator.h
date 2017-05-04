/* 
 * File:   CodeGenerator.h
 * Author: wessel
 *
 * Created on November 18, 2015, 12:27 PM
 */

#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H

#include "Visitor.h"
#include <string>
#include <map>
#include <memory>
#include <stack>
#include "Optimizer.h"
#include "MemManager.h"
#include "UsageChecker.h"
#include "SymbolTable.h"
namespace Cool {

    class Environment;
    class Config;

    class CodeGenerator {
    public:
        CodeGenerator(Environment & env);
        void start();
    private:
        Assembly::Optimizer optimizer;
        Environment & env;
        UsageChecker usageChecker;
        SymbolTable symTable;
        void createDispTab(const ClassNode*node);
        void writeProtoObj(const ClassNode* node);
        void buildSymbolTable();


        //AsmWriter visitor

        class AsmWriter : public Visitor {
        public:
            AsmWriter(const Environment & env, AsmContainer & asmcont);
            void start(const SymbolTable & sym, const UsageChecker * const usageChecker = nullptr);
            virtual ~AsmWriter();

            void visit(FunctionNode *);
            void visit(BlockNode *);

            void visit(IdNode*);

            void visit(BoolNode*);
            void visit(IntNode *);
            void visit(StringNode*);

            void visit(AssignNode*);
            void visit(VariableDefinitionNode*);

            void visit(NegateNode*);
            void visit(UnaryMinusNode*);

            void visit(WhileNode*);
            void visit(IfNode*);

            void visit(FunctionCallNode*);
            void visit(ObjectFunctionCallNode*);
            void visit(SuperNode*);

            void visit(NewNode*);

            void visit(ParenthesesExpressionNode*);

            void visit(CaseNode*);
            void visit(MatchNode*);

            void visit(LtEqNode*);
            void visit(LtNode*);

            void visit(MultiplyNode*);
            void visit(AddNode*);
            void visit(DivideNode*);
            void visit(SubtractNode*);

            void visit(ThisNode*);
            void visit(NullNode*);
            void visit(ObjectNode*);
        private:
            bool inAssign = false;
            const SymbolTable* symTable;
            std::string currclass;
            const Config * config;
            AsmContainer * asmc;
            MemManager mm;
            const UsageChecker * usageChecker;
            unsigned ifctr = 0, whilectr = 0, matchctr = 0;

            void ifIntWrite(const Node * node);
            void writeArithmetic(const BinNode*node, const int o);
            void writeReturn(std::string reg_dest);
        };
    };


}
#endif /* CODEGENERATOR_H */

