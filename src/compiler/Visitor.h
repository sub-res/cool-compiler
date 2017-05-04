/* 
 * File:   Visitor.h
 * Author: root
 *
 * Created on October 15, 2015, 10:16 AM
 */

#ifndef VISITOR_H
#define	VISITOR_H

namespace Cool {
    class Node;
    class BinNode;
    class ClassNode;
    class ClassBodyNode;
    class ClassVariableNode;
    class FunctionNode;
    class BlockNode;
    class IntNode;
    class BoolNode;
    class StringNode;
    class ActualsNode;
    class IdNode;
    class ObjectNode;
    class TypeNode;
    class ExtendNode;
    class FormalsNode;
    class FormalNode;
    class AssignNode;
    class NegateNode;
    class UnaryMinusNode;
    class WhileNode;
    class IfNode;
    class SuperNode;
    class FunctionCallNode;
    class ObjectFunctionCallNode;
    class NewNode;
    class ParenthesesExpressionNode;
    class MatchNode;
    class CaseNode;
    class LtEqNode;
    class LtNode;
    class MultiplyNode;
    class DivideNode;
    class AddNode;
    class SubtractNode;
    class VariableDefinitionNode;
    class NativeNode;
    class NullNode;
    class ThisNode;
    class UnitNode;

    class Visitor {
    public:
        Visitor() = default;
        virtual ~Visitor() = default;

        virtual void visit(Node *);

        virtual void visit(ClassNode *);
        virtual void visit(ClassBodyNode *);

        virtual void visit(ClassVariableNode *);
        virtual void visit(FunctionNode *);

        virtual void visit(BlockNode *);
        virtual void visit(TypeNode *);
        virtual void visit(IntNode *);
        virtual void visit(ActualsNode*);
        virtual void visit(StringNode*);
        virtual void visit(IdNode*);
        virtual void visit(ObjectNode*);
        virtual void visit(ExtendNode*);
        virtual void visit(BoolNode*);
        virtual void visit(AssignNode*);
        virtual void visit(NegateNode*);
        virtual void visit(UnaryMinusNode*);

        virtual void visit(WhileNode*);
        virtual void visit(IfNode*);

        virtual void visit(FunctionCallNode*);
        virtual void visit(ObjectFunctionCallNode*);
        virtual void visit(NewNode*);
        virtual void visit(SuperNode*);
        virtual void visit(ParenthesesExpressionNode*);

        virtual void visit(CaseNode*);
        virtual void visit(MatchNode*);

        virtual void visit(LtEqNode*);
        virtual void visit(LtNode*);

        virtual void visit(MultiplyNode*);
        virtual void visit(AddNode*);
        virtual void visit(DivideNode*);
        virtual void visit(SubtractNode*);

        virtual void visit(FormalsNode*);
        virtual void visit(VariableDefinitionNode*);
        virtual void visit(NativeNode*);
        virtual void visit(ThisNode*);
        virtual void visit(NullNode*);
        virtual void visit(UnitNode*);
        virtual void visit(FormalNode*);


       
        virtual void leave(Node*);
        
        virtual void leave(ClassNode *);
        virtual void leave(ClassBodyNode *);
        virtual void leave(ClassVariableNode *);
        
        virtual void leave(FunctionNode *);
        virtual void leave(BlockNode *);
        virtual void leave(TypeNode *);
        virtual void leave(IntNode *);
        virtual void leave(ActualsNode*);
        virtual void leave(StringNode*);
        virtual void leave(IdNode*);
        virtual void leave(ExtendNode*);
        virtual void leave(BoolNode*);
        virtual void leave(AssignNode*);
        virtual void leave(NegateNode*);
        virtual void leave(UnaryMinusNode*);
        virtual void leave(FormalNode*);
        virtual void leave(FormalsNode*);
        virtual void leave(WhileNode*);
        virtual void leave(IfNode*);
        virtual void leave(ObjectNode*);
        virtual void leave(FunctionCallNode*);
        virtual void leave(ObjectFunctionCallNode*);
        virtual void leave(NewNode*);
        virtual void leave(SuperNode*);
        virtual void leave(ParenthesesExpressionNode*);
        virtual void leave(CaseNode*);
        virtual void leave(MatchNode*);
        virtual void leave(LtEqNode*);
        virtual void leave(LtNode*);
        
        virtual void leave(MultiplyNode*);
        virtual void leave(AddNode*);
        virtual void leave(DivideNode*);
        virtual void leave(SubtractNode*);
        
        virtual void leave(VariableDefinitionNode*);
        virtual void leave(NativeNode*);
        
        virtual void leave(ThisNode*);
        virtual void leave(NullNode*);
        virtual void leave(UnitNode*);

    };
}
#endif	/* VISITOR_H */

