/* 
 * File:   TreePrinter.cpp
 * Author: root
 * 
 * Created on October 17, 2015, 6:27 PM
 */

#include <iostream>
#include <string>

#include "Printer.h"
#include "Analyser.h"
#include "Nodes.h"

namespace Cool {

    void Printer::print(const std::map<std::string, ClassNode*>&classes) {
        TreePrinter printer;
        printer.print(classes);
    }

    Printer::TreePrinter::TreePrinter() {
        depth = 0;
    }

    void Printer::TreePrinter::leave(ClassNode*) {
        --depth;
    }

    void Printer::TreePrinter::leave(ClassBodyNode*) {
        --depth;
    }

    void Printer::TreePrinter::leave(ClassVariableNode*) {
        --depth;
    }

    void Printer::TreePrinter::leave(FunctionNode*) {
        --depth;
    }

    void Printer::TreePrinter::leave(BlockNode*) {
        --depth;
    }

    void Printer::TreePrinter::leave(TypeNode*) {
        --depth;
    }

    void Printer::TreePrinter::leave(IntNode*) {
        --depth;
    }

    void Printer::TreePrinter::leave(StringNode*) {
        --depth;
    }

    void Printer::TreePrinter::leave(ActualsNode*) {
        --depth;
    }

    void Printer::TreePrinter::leave(IdNode*) {
        --depth;
    }

    void Printer::TreePrinter::leave(ObjectNode*) {
        --depth;
    }

    void Printer::TreePrinter::leave(ExtendNode*) {
        --depth;
    }

    void Printer::TreePrinter::leave(BoolNode*) {
        --depth;
    }

    void Printer::TreePrinter::leave(AssignNode*) {
        --depth;
    }

    void Printer::TreePrinter::leave(NegateNode*) {
        --depth;
    }

    void Printer::TreePrinter::leave(UnaryMinusNode*) {
        --depth;
    }

    void Printer::TreePrinter::leave(WhileNode*) {
        --depth;
    }

    void Printer::TreePrinter::leave(IfNode*) {
        --depth;
    }

    void Printer::TreePrinter::leave(FunctionCallNode*) {
        --depth;
    }

    void Printer::TreePrinter::leave(ObjectFunctionCallNode*) {
        --depth;
    }

    void Printer::TreePrinter::leave(NewNode*) {
        --depth;
    }

    void Printer::TreePrinter::leave(SuperNode*) {
        --depth;
    }

    void Printer::TreePrinter::leave(ParenthesesExpressionNode*) {
        --depth;
    }

    void Printer::TreePrinter::leave(UnitNode*) {
        --depth;
    }

    void Printer::TreePrinter::leave(NullNode*) {
        --depth;
    }

    void Printer::TreePrinter::leave(ThisNode*) {
        --depth;
    }

    void Printer::TreePrinter::leave(AddNode*) {
        --depth;
    }

    void Printer::TreePrinter::leave(SubtractNode*) {
        --depth;
    }

    void Printer::TreePrinter::leave(FormalsNode*) {
        --depth;
    }

    void Printer::TreePrinter::leave(DivideNode*) {
        --depth;
    }

    void Printer::TreePrinter::leave(MultiplyNode*) {
        --depth;
    }

    void Printer::TreePrinter::leave(VariableDefinitionNode*) {
        --depth;
    }

    void Printer::TreePrinter::leave(NativeNode*) {
        --depth;
    }

    void Printer::TreePrinter::leave(CaseNode*) {
        --depth;
    }

    void Printer::TreePrinter::leave(MatchNode*) {
        --depth;
    }

    void Printer::TreePrinter::leave(LtEqNode*) {
        --depth;
    }

    void Printer::TreePrinter::leave(LtNode*) {
        --depth;
    }

    void Printer::TreePrinter::leave(FormalNode*){
    --depth;
    }
    
    void Printer::TreePrinter::visit(UnaryMinusNode* node) {
        print("Unary minus:");
    }


    void Printer::TreePrinter::visit(ClassNode* node) {
        print("Class:");
    }

    void Printer::TreePrinter::visit(ClassBodyNode*node) {
        print("Body:");
    }

    void Printer::TreePrinter::visit(TypeNode*node) {
        print("Type: " + node->getType());
    }

    void Printer::TreePrinter::visit(IdNode*node) {
        print("Id: " + node->getId());
    }

    void Printer::TreePrinter::visit(ObjectNode*node) {
        print("Object Id: " + node->getId());
    }

    void Printer::TreePrinter::visit(IntNode* node) {
        std::cout << indent(depth) << "Int: " << node->val << std::endl;
        depth++;
    }

    void Printer::TreePrinter::visit(StringNode* node) {
        print("String: " + (node->val));
    }

    void Printer::TreePrinter::visit(BlockNode*) {
        print("Block:");
    }

    void Printer::TreePrinter::visit(ClassVariableNode*) {
        print("Feature Var:");
    }

    void Printer::TreePrinter::visit(FunctionNode*node) {
        std::string s = ((node->isOverride) ? " override" : " ");
        print("Function:" + s);
    }

    void Printer::TreePrinter::visit(ActualsNode*node) {
        print("Actuals:");
    }

    void Printer::TreePrinter::visit(ExtendNode*node) {
        print("Extends:");
    }

    void Printer::TreePrinter::visit(BoolNode*node) {
        std::cout << indent(depth) << "Boolean: " << node->val << std::endl;
        depth++;
    }

    void Printer::TreePrinter::visit(AssignNode*node) {
        print("Assignment:");
    }

    void Printer::TreePrinter::visit(NegateNode*node) {
        print("!");
    }

    void Printer::TreePrinter::print(const std::map<std::string, ClassNode*> & classes) {

        for (std::map<std::string, ClassNode*>::const_iterator itt = classes.begin(); itt != classes.end(); ++itt) {
            itt->second->accept(*this, true);
        }
    }

    void Printer::TreePrinter::visit(SuperNode*node) {
        print("super:");
    }

    void Printer::TreePrinter::visit(ParenthesesExpressionNode*node) {
        print("(expr):");
    }

    void Printer::TreePrinter::visit(FunctionCallNode*node) {
        print("Function Call");
    }

    void Printer::TreePrinter::visit(ObjectFunctionCallNode*node) {
        print("OFunction Call");
    }

    void Printer::TreePrinter::visit(AddNode* node) {
        print("+");
    }

    void Printer::TreePrinter::visit(MultiplyNode* node) {
        print("*");
    }

    void Printer::TreePrinter::visit(DivideNode*node) {
        print("/");
    }

    void Printer::TreePrinter::visit(LtEqNode* node) {
        print("<=");
    }

    void Printer::TreePrinter::visit(ThisNode* node) {
        print("this");
    }

    void Printer::TreePrinter::visit(SubtractNode* node) {
        print("-");
    }

    void Printer::TreePrinter::visit(NativeNode* node) {
        print("native");
    }

    void Printer::TreePrinter::visit(NullNode*) {
        print("null");
    }

    void Printer::TreePrinter::visit(WhileNode* node) {
        print("While");
    }

    void Printer::TreePrinter::visit(IfNode*node) {
        print("If");
    }

    void Printer::TreePrinter::visit(VariableDefinitionNode*node) {
        print("Decl. and assign:");
    }

    void Printer::TreePrinter::visit(LtNode*node) {
        print("<");
    }

    void Printer::TreePrinter::visit(FormalsNode * node) {
        print("Formals:");
    }
    
    void Printer::TreePrinter::visit(FormalNode*node){
        print("Formal:");
    }

    void Printer::TreePrinter::visit(NewNode*node) {
        print("new");
    }

    void Printer::TreePrinter::visit(UnitNode*) {
        print("Unit");
    }

    void Printer::TreePrinter::visit(MatchNode*node) {
        print("match:");
    }

    void Printer::TreePrinter::visit(CaseNode*node) {
        print("case");
    }

    void Printer::TreePrinter::print(std::string s) {
        std::string a = indent(depth);
        std::cout << a << s << std::endl;
        depth++;
    }
}