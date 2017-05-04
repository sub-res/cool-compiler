/* 
 * File:   Printer.h
 * Author: root
 *
 * Created on October 17, 2015, 6:27 PM
 */

#ifndef PRINTER_H
#define	PRINTER_H

#include <string>
#include <map>
#include "Visitor.h"

namespace Cool {

    class Printer {
    public:
        static void print(const std::map<std::string, ClassNode*>&);
    private:

        class TreePrinter : public Visitor {
        public:
            TreePrinter();

            void print(const std::map<std::string, ClassNode*>&);
      
            void visit(ClassNode*node);
            void visit(ClassBodyNode*node);
            void visit(ActualsNode*node);
            void visit(ExtendNode*node);

            void visit(BlockNode*node);

            void visit(ClassVariableNode*node);
            void visit(FunctionNode*node);

            void visit(NegateNode*node);
            void visit(AssignNode*node);


            void visit(TypeNode*node);
            void visit(IdNode*node);
            void visit(ObjectNode*node);
            void visit(IntNode*node);
            void visit(StringNode*node);
            void visit(BoolNode*node);

            void visit(FunctionCallNode*node);
            void visit(ObjectFunctionCallNode*node);

            void visit(ParenthesesExpressionNode*node);
            void visit(UnitNode*node);
            void visit(AddNode*node);
            void visit(MultiplyNode*node);
            void visit(DivideNode*node);
            void visit(SubtractNode*node);
            void visit(UnaryMinusNode*node);

            void visit(LtEqNode*node);
            void visit(LtNode*node);

            void visit(FormalsNode*node);
            void visit(VariableDefinitionNode*node);
            void visit(NewNode*node);

            void visit(WhileNode*node);
            void visit(IfNode*node);

            void visit(SuperNode*node);
            void visit(NativeNode*node);
            void visit(ThisNode*node);
            void visit(NullNode*node);
            void visit(MatchNode*node);
            void visit(CaseNode*node);

            void visit(FormalNode*);

            
            
            void leave(ClassNode *);
            void leave(ClassBodyNode *);
            void leave(ClassVariableNode *);
            
            void leave(FunctionNode *);
            void leave(BlockNode *);
            void leave(TypeNode *);
            void leave(IntNode *);
            void leave(ActualsNode*);
            void leave(StringNode*);
            void leave(IdNode*);
            void leave(ExtendNode*);
            void leave(BoolNode*);
            void leave(AssignNode*);
            void leave(NegateNode*);
            void leave(UnaryMinusNode*);
            void leave(ObjectNode*);
            void leave(WhileNode*);
            void leave(IfNode*);
            void leave(FunctionCallNode*);
            void leave(ObjectFunctionCallNode*);
            void leave(NewNode*);
            void leave(SuperNode*);
            void leave(ParenthesesExpressionNode*);
            void leave(CaseNode*);
            void leave(MatchNode*);
            
            void leave(LtEqNode*);
            void leave(LtNode*);
            
            void leave(AddNode*);   
            void leave(DivideNode*);
            void leave(SubtractNode*);
            void leave(MultiplyNode*);
            
            void leave(FormalNode*);
            void leave(FormalsNode*);
            void leave(VariableDefinitionNode*);
            void leave(NativeNode*);
            
            void leave(ThisNode*);
            void leave(NullNode*);
            void leave(UnitNode*);

        private:
            unsigned int depth;
            void print(std::string s);

            static inline std::string indent(unsigned int in) {
                return std::string(in >= 0 ? in : 0, ' ');
            }
        };
    };
}
#endif	/* PRINTER_H */

