/* 
 * File:   TypeValidator.h
 * Author: len
 *
 * Created on February 7, 2016, 12:05 AM
 */

#ifndef TYPEVALIDATOR_H
#define	TYPEVALIDATOR_H

#include <string>

namespace Cool {

    class NegateNode;
    class LtEqNode;
    class LtNode;
    class AddNode;
    class SubtractNode;
    class BinNode;
    class MultiplyNode;
    class DivideNode;
    class AssignNode;
    class ClassVariableNode;
    class FunctionNode;
    class VariableDefinitionNode;
    class IfNode;
    class WhileNode;
    class Node;
    class FormalsNode;
    class ActualsNode;

    class Environment;

    class TypeValidator {
    public:
        const Environment & env;
        TypeValidator(const Environment & env);

        bool validate(const NegateNode&)const;
        bool validate(const LtEqNode&) const;
        bool validate(const LtNode&) const;
        bool validate(const AddNode&)const;
        bool validate(const SubtractNode&)const;
        bool validate(const MultiplyNode&)const;
        bool validate(const DivideNode&)const;
        bool validate(const AssignNode&)const;
        bool validate(const ClassVariableNode&)const;
        bool validate(const FunctionNode&)const;
        bool validate(const VariableDefinitionNode&n)const;
        bool validate(const IfNode&n)const;
        bool validate(const WhileNode&n)const;

        bool validate(const Node&, const std::string & fname, bool constructor, const ActualsNode&actuals, const FormalsNode&varFormals) const;
        bool matchFormals(const FormalsNode&n1, const FormalsNode&n2)const;
    private:
        bool matchBinOpInt(const BinNode&, std::string type_operator) const;
        bool matchBinOpIntExplicit(const BinNode&, std::string type_operator)const;
        bool matchType(const std::string & actual_type, const std::string & expect_type) const;
    };
}
#endif	/* TYPEVALIDATOR_H */

