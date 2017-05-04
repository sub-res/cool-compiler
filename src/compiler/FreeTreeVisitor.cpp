/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FreeTreeVisitor.cpp
 * Author: ldejong
 * 
 * Created on March 5, 2017, 1:50 PM
 */

#include "FreeTreeVisitor.h"
#include "Environment.h"
namespace Cool {

    FreeTreeVisitor::FreeTreeVisitor(Environment& env) : env(env) {
    }

    void start() {

    }

    void FreeTreeVisitor::visit(Node*n) {
        env.free(*n);
    }

    void FreeTreeVisitor::visit(ClassNode*n) {
        env.free(*n);
    }

    void FreeTreeVisitor::visit(ClassBodyNode*n) {
        env.free(*n);
    }

    void FreeTreeVisitor::visit(ClassVariableNode*n) {
        env.free(*n);
    }

    void FreeTreeVisitor::visit(FunctionNode*n) {
        env.free(*n);
    }

    void FreeTreeVisitor::visit(BlockNode*n) {
        env.free(*n);
    }

    void FreeTreeVisitor::visit(TypeNode*n) {
        env.free(*n);
    }

    void FreeTreeVisitor::visit(IntNode*n) {
        env.free(*n);
    }

    void FreeTreeVisitor::visit(StringNode*n) {
        env.free(*n);
    }

    void FreeTreeVisitor::visit(ActualsNode*n) {
        env.free(*n);
    }

    void FreeTreeVisitor::visit(IdNode*n) {
        env.free(*n);
    }

    void FreeTreeVisitor::visit(ObjectNode*n) {
        env.free(*n);
    }

    void FreeTreeVisitor::visit(ExtendNode*n) {
        env.free(*n);
    }

    void FreeTreeVisitor::visit(BoolNode*n) {
        env.free(*n);
    }

    void FreeTreeVisitor::visit(AssignNode*n) {
        env.free(*n);
    }

    void FreeTreeVisitor::visit(NegateNode*n) {
        env.free(*n);
    }

    void FreeTreeVisitor::visit(FormalNode*n) {
        env.free(*n);
    }

    void FreeTreeVisitor::visit(FormalsNode*n) {
        env.free(*n);
    }

    void FreeTreeVisitor::visit(UnaryMinusNode*n) {
        env.free(*n);
    }

    void FreeTreeVisitor::visit(WhileNode*n) {
        env.free(*n);
    }

    void FreeTreeVisitor::visit(IfNode*n) {
        env.free(*n);
    }

    void FreeTreeVisitor::visit(FunctionCallNode*n) {
        env.free(*n);
    }

    void FreeTreeVisitor::visit(ObjectFunctionCallNode*n) {
        env.free(*n);
    }

    void FreeTreeVisitor::visit(NewNode*n) {
        env.free(*n);
    }

    void FreeTreeVisitor::visit(SuperNode*n) {
        env.free(*n);
    }

    void FreeTreeVisitor::visit(ParenthesesExpressionNode*n) {
        env.free(*n);
    }

    void FreeTreeVisitor::visit(UnitNode*n) {
        env.free(*n);
    }

    void FreeTreeVisitor::visit(NullNode*n) {
        env.free(*n);
    }

    void FreeTreeVisitor::visit(ThisNode*n) {
        env.free(*n);
    }

    void FreeTreeVisitor::visit(AddNode*n) {
        env.free(*n);
    }

    void FreeTreeVisitor::visit(SubtractNode*n) {
        env.free(*n);
    }

    void FreeTreeVisitor::visit(DivideNode*n) {
        env.free(*n);
    }

    void FreeTreeVisitor::visit(MultiplyNode*n) {
        env.free(*n);
    }

    void FreeTreeVisitor::visit(VariableDefinitionNode*n) {
        env.free(*n);
    }

    void FreeTreeVisitor::visit(NativeNode*n) {
        env.free(*n);
    }

    void FreeTreeVisitor::visit(CaseNode*n) {
        env.free(*n);
    }

    void FreeTreeVisitor::visit(MatchNode*n) {
        env.free(*n);
    }

    void FreeTreeVisitor::visit(LtEqNode*n) {
        env.free(*n);
    }

    void FreeTreeVisitor::visit(LtNode*n) {
        env.free(*n);
    }
}
