/* 
 * File:   Visitor.cpp
 * Author: root
 * 
 * Created on October 15, 2015, 10:16 AM
 */

#include "Visitor.h"
#include "Nodes.h"

namespace Cool {

    void Visitor::visit(Node*n) {
    }

    void Visitor::visit(ClassNode*n) {visit(static_cast<Node*>(n));
    }

    void Visitor::visit(ClassBodyNode*n) {visit(static_cast<Node*>(n));
    }

    void Visitor::visit(ClassVariableNode*n) {visit(static_cast<Node*>(n));
    }

    void Visitor::visit(FunctionNode*n) {visit(static_cast<Node*>(n));
    }

    void Visitor::visit(BlockNode*n) {visit(static_cast<Node*>(n));
    }

    void Visitor::visit(TypeNode*n) {visit(static_cast<Node*>(n));
    }

    void Visitor::visit(IntNode*n) {visit(static_cast<Node*>(n));
    }

    void Visitor::visit(StringNode*n) {visit(static_cast<Node*>(n));
    }

    void Visitor::visit(ActualsNode*n) {visit(static_cast<Node*>(n));
    }

    void Visitor::visit(IdNode*n) {visit(static_cast<Node*>(n));
    }

    void Visitor::visit(ObjectNode*n) {visit(static_cast<Node*>(n));
    }

    void Visitor::visit(ExtendNode*n) {visit(static_cast<Node*>(n));
    }

    void Visitor::visit(BoolNode*n) {visit(static_cast<Node*>(n));
    }

    void Visitor::visit(AssignNode*n) {visit(static_cast<Node*>(n));
    }

    void Visitor::visit(NegateNode*n) {visit(static_cast<Node*>(n));
    }

    void Visitor::visit(FormalNode*n) {visit(static_cast<Node*>(n));
    }

    void Visitor::visit(FormalsNode*n) {visit(static_cast<Node*>(n));
    }

    void Visitor::visit(UnaryMinusNode*n) {visit(static_cast<Node*>(n));
    }

    void Visitor::visit(WhileNode*n) {visit(static_cast<Node*>(n));
    }

    void Visitor::visit(IfNode*n) {visit(static_cast<Node*>(n));
    }

    void Visitor::visit(FunctionCallNode*n) {visit(static_cast<Node*>(n));
    }

    void Visitor::visit(ObjectFunctionCallNode*n) {visit(static_cast<Node*>(n));
    }

    void Visitor::visit(NewNode*n) {visit(static_cast<Node*>(n));
    }

    void Visitor::visit(SuperNode*n) {visit(static_cast<Node*>(n));
    }

    void Visitor::visit(ParenthesesExpressionNode*n) {visit(static_cast<Node*>(n));
    }

    void Visitor::visit(UnitNode*n) {visit(static_cast<Node*>(n));
    }

    void Visitor::visit(NullNode*n) {visit(static_cast<Node*>(n));
    }

    void Visitor::visit(ThisNode*n) {visit(static_cast<Node*>(n));
    }

    void Visitor::visit(AddNode*n) {visit(static_cast<Node*>(n));
    }

    void Visitor::visit(SubtractNode*n) {visit(static_cast<Node*>(n));
    }

    void Visitor::visit(DivideNode*n) {visit(static_cast<Node*>(n));
    }

    void Visitor::visit(MultiplyNode*n) {visit(static_cast<Node*>(n));
    }

    void Visitor::visit(VariableDefinitionNode*n) {visit(static_cast<Node*>(n));
    }

    void Visitor::visit(NativeNode*n) {visit(static_cast<Node*>(n));
    }

    void Visitor::visit(CaseNode*n) {visit(static_cast<Node*>(n));
    }

    void Visitor::visit(MatchNode*n) {visit(static_cast<Node*>(n));
    }

    void Visitor::visit(LtEqNode*n) {visit(static_cast<Node*>(n));
    }

    void Visitor::visit(LtNode*n) {visit(static_cast<Node*>(n));
    }

    
    void Visitor::leave(Node*n) {
    }

    void Visitor::leave(ClassNode*n) {leave(static_cast<Node*>(n));
    }

    void Visitor::leave(ClassBodyNode*n) {leave(static_cast<Node*>(n));
    }

    void Visitor::leave(ClassVariableNode*n) {leave(static_cast<Node*>(n));
    }

    void Visitor::leave(FunctionNode*n) {leave(static_cast<Node*>(n));
    }

    void Visitor::leave(BlockNode*n) {leave(static_cast<Node*>(n));
    }

    void Visitor::leave(TypeNode*n) {leave(static_cast<Node*>(n));
    }

    void Visitor::leave(IntNode*n) {leave(static_cast<Node*>(n));
    }

    void Visitor::leave(StringNode*n) {leave(static_cast<Node*>(n));
    }

    void Visitor::leave(ObjectNode*n) {leave(static_cast<Node*>(n));
    }

    void Visitor::leave(ActualsNode*n) {leave(static_cast<Node*>(n));
    }

    void Visitor::leave(IdNode*n) {leave(static_cast<Node*>(n));
    }

    void Visitor::leave(ExtendNode*n) {leave(static_cast<Node*>(n));
    }

    void Visitor::leave(BoolNode*n) {leave(static_cast<Node*>(n));
    }

    void Visitor::leave(AssignNode*n) {leave(static_cast<Node*>(n));
    }

    void Visitor::leave(FormalsNode*n) {leave(static_cast<Node*>(n));

    }

    void Visitor::leave(FormalNode*n) {leave(static_cast<Node*>(n));
    }

    void Visitor::leave(NegateNode*n) {leave(static_cast<Node*>(n));
    }

    void Visitor::leave(UnaryMinusNode*n) {leave(static_cast<Node*>(n));
    }

    void Visitor::leave(WhileNode*n) {leave(static_cast<Node*>(n));
    }

    void Visitor::leave(IfNode*n) {leave(static_cast<Node*>(n));
    }

    void Visitor::leave(FunctionCallNode*n) {leave(static_cast<Node*>(n));
    }

    void Visitor::leave(ObjectFunctionCallNode*n) {leave(static_cast<Node*>(n));
    }

    void Visitor::leave(NewNode*n) {leave(static_cast<Node*>(n));
    }

    void Visitor::leave(SuperNode*n) {leave(static_cast<Node*>(n));
    }

    void Visitor::leave(ParenthesesExpressionNode*n) {leave(static_cast<Node*>(n));
    }

    void Visitor::leave(UnitNode*n) {leave(static_cast<Node*>(n));
    }

    void Visitor::leave(NullNode*n) {leave(static_cast<Node*>(n));
    }

    void Visitor::leave(ThisNode*n) {leave(static_cast<Node*>(n));
    }

    void Visitor::leave(AddNode*n) {leave(static_cast<Node*>(n));
    }

    void Visitor::leave(SubtractNode*n) {leave(static_cast<Node*>(n));
    }

    void Visitor::leave(DivideNode*n) {leave(static_cast<Node*>(n));
    }

    void Visitor::leave(MultiplyNode*n) {leave(static_cast<Node*>(n));
    }

    void Visitor::leave(VariableDefinitionNode*n) {leave(static_cast<Node*>(n));
    }

    void Visitor::leave(NativeNode*n) {leave(static_cast<Node*>(n));
    }

    void Visitor::leave(CaseNode*n) {leave(static_cast<Node*>(n));
    }

    void Visitor::leave(MatchNode*n) {leave(static_cast<Node*>(n));
    }

    void Visitor::leave(LtEqNode*n) {leave(static_cast<Node*>(n));
    }

    void Visitor::leave(LtNode*n) {leave(static_cast<Node*>(n));
    }
}