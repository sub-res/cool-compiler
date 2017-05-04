/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FreeTreeVisitor.h
 * Author: ldejong
 *
 * Created on March 5, 2017, 1:50 PM
 */

#ifndef FREETREEVISITOR_H
#define FREETREEVISITOR_H

#include "Visitor.h"

namespace Cool {

    class Environment;

    class FreeTreeVisitor : public Visitor {
    public:
        FreeTreeVisitor(Environment& env);
        
        void start();

        void visit(Node *) override;

        void visit(ClassNode *) override;
        void visit(ClassBodyNode *) override;

        void visit(ClassVariableNode *) override;
        void visit(FunctionNode *) override;

        void visit(BlockNode *) override;
        void visit(TypeNode *) override;
        void visit(IntNode *) override;
        void visit(ActualsNode*) override;
        void visit(StringNode*) override;
        void visit(IdNode*) override;
        void visit(ObjectNode*) override;
        void visit(ExtendNode*) override;
        void visit(BoolNode*) override;
        void visit(AssignNode*) override;
        void visit(NegateNode*) override;
        void visit(UnaryMinusNode*) override;

        void visit(WhileNode*) override;
        void visit(IfNode*) override;

        void visit(FunctionCallNode*) override;
        void visit(ObjectFunctionCallNode*) override;
        void visit(NewNode*) override;
        void visit(SuperNode*) override;
        void visit(ParenthesesExpressionNode*) override;

        void visit(CaseNode*) override;
        void visit(MatchNode*) override;

        void visit(LtEqNode*) override;
        void visit(LtNode*) override;

        void visit(MultiplyNode*) override;
        void visit(AddNode*) override;
        void visit(DivideNode*) override;
        void visit(SubtractNode*) override;

        void visit(FormalsNode*) override;
        void visit(VariableDefinitionNode*) override;
        void visit(NativeNode*) override;
        void visit(ThisNode*) override;
        void visit(NullNode*) override;
        void visit(UnitNode*) override;
        void visit(FormalNode*) override;

    private:
        Environment& env;
    };

}
#endif /* FREETREEVISITOR_H */

