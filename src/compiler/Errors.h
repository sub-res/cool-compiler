/* 
 * File:   Errors.h
 * Author: len
 *
 * Created on January 7, 2016, 11:56 PM
 */

#ifndef ERRORS_H
#define ERRORS_H

#include <vector>
#include <string>
#include <ostream>
#include "Error.h"

namespace Cool {
    class location;
    class Node;
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
    class BinNode;

    enum class ErrorType;

    class Errors {
        
        friend std::ostream& operator<<(std::ostream& out, const Errors&);

    public:

        bool hasCritical() const;
        bool hasWarning() const;
        unsigned size() const;
        void clear();

        void registerError(const std::string& message);
        void registerError(const std::string& message, const ErrorType t);
        void registerError(const Cool::location& loc, const std::string& message);
        void registerError(const Cool::location& loc, const std::string& message, const ErrorType t);

        void regFuncRedef(const FunctionNode&error, const FunctionNode&func);
        void regVarRedef(const location & loc, const std::string & id, const std::string&classname);

        void registerTypeUndefined(const TypeNode&type);
        void registerTypeUndefined(const NewNode&node);
        void registerTypeUndefined(const ExtendNode&node);

        void registerUndeclaredVar(const location & loc, const std::string & id);
        void registerUndeclaredFunction(const Node &node, const std::string & cname, const std::string & fname);
        void registerReservedClassName(const ClassNode&type);
        void registerIllegalKeyword(const NativeNode&node);
        void registerIllegalExtend(const ExtendNode&node);
        void registerUnintentionalOverride(const FunctionNode&node);

        void registerFormalTypeMismatch(const FunctionNode&node, const FunctionNode&parent);
        void registerFormalCountMismatch(const FunctionNode&node, const FunctionNode&parent);
        void registerActualCountMismatch(const Node&node, const std::string & fname, bool constructor, int actual, int expected);
        void registerEmptyOverride(const FunctionNode&node, std::string parent);
        void registerCaseRedefinition(const CaseNode&node);
        void registerCaseDerivesCase(const CaseNode&node, const CaseNode&parent);
        void registerCaseTypeMismatch(const CaseNode&node, const MatchNode&match);
        void registerNothingInDefinition(const VariableDefinitionNode&node);
        void registerTypeMismatch(const FunctionNode&node, const FunctionNode&parent);
        void registerTypeMismatch(const IfNode&node);
        void registerTypeMismatch(const WhileNode&node);
        void registerTypeMismatch(const AssignNode&node);
        void registerTypeMismatch(const VariableDefinitionNode&node);
        void registerTypeMismatch(const FunctionNode&node);
        void registerTypeMismatch(const ClassVariableNode&node);
        void registerTypeMisMatch(const BinNode&node, bool left, std::string op);
        void registerTypeMisMatch(const NegateNode&node);
        void registerActualTypeMismatch(const Node&node, const std::string & fname, bool constructor, std::string actual, std::string expected, int index);
        void registerCircularDependancy(const ClassNode&node, std::string at);
        void registerMainFormalsMismatch(const FormalsNode&node);
        void registerSyntaxError(const Cool::location & loc, const std::string message);

        //warnings
        void registerMissingMainClass(bool warning);
        void registerEqualsNull(const Node&node);

    private:

        template <typename... Params>
        void add(Params&&... params) {
            errors.push_back({std::forward<Params>(params)...});
        }

        std::vector<Error>errors;
    };
}
#endif /* ERRORS_H */

