/* 
 * File:   Errors.cpp
 * Author: len
 * 
 * Created on January 7, 2016, 11:56 PM
 */

#include "Errors.h"
#include "Nodes.h"
#include "Error.h"
#include "Config.h"
#include "ErrorType.h"

namespace Cool {

    bool Errors::hasCritical() const {
        for (const auto& error : errors) {
            if (error.type == ErrorType::CRITICAL) {
                return true;
            }
        }
        return false;
    }

    bool Errors::hasWarning() const {
        for (const auto& error : errors) {
            if (error.type == ErrorType::WARNING) {
                return true;
            }
        }
        return false;
    }

    void Errors::clear() {
        errors.clear();
    }
    
    unsigned Errors::size() const{
        return errors.size();
    }

    void Errors::registerError(const std::string& message) {
        add(message, ErrorType::CRITICAL);
    }

    void Errors::registerError(const std::string& message, const ErrorType type) {
        add(message, type);
    }

    void Errors::registerError(const Cool::location& location, const std::string& message) {
        add(location, message, ErrorType::CRITICAL);
    }

    void Errors::registerError(const Cool::location& location, const std::string& message, const ErrorType t) {
        add(location, message, t);
    }

    void Errors::regFuncRedef(const FunctionNode& at, const FunctionNode& func) {
        const Cool::location& loc = at.loc;
        add(at.loc, "Function with name \'" + at.getId() + "\' is already defined in the same scope. Here: " + std::to_string(loc.begin.line) + ":" + std::to_string(loc.begin.column) + ".");
    }

    void Errors::regVarRedef(const location & loc, const std::string & id, const std::string&classname) {
        add(loc, "Variable already declared in the scope: \'" + id + "\' (" + classname + ")");
    }

    void Errors::registerTypeUndefined(const TypeNode& node) {
        add(node.loc, "Type \'" + node.getType() + "\' is used but never declared.");
    }

    void Errors::registerUndeclaredVar(const location & loc, const std::string & id) {
        add(loc, "Variable not declared in the scope: \'" + id + "\'");
    }

    void Errors::registerReservedClassName(const ClassNode& node) {
        add(node.loc, "Type \'" + node.getType() + "\' is reserved and can not be redefined.");
    }

    void Errors::registerIllegalKeyword(const NativeNode& node) {
        add(node.loc, "Illegal use of keyword \'native\', which is only allowed in file \'" + Config::basicFile + "\'.");
    }

    void Errors::registerUnintentionalOverride(const FunctionNode& node) {
        add(node.loc, "Unintentional override. Expected \'override\'-keyword for function \'" + node.getId() + "\'.");
    }

    void Errors::registerFormalCountMismatch(const FunctionNode& node, const FunctionNode& parent) {
        add(node.loc, "Function \'" + node.getId() + "\' overrides but number of arguments do not match. Expected \'" + std::to_string(parent.formals->getCount()) + "\', got \'" + std::to_string(node.formals->getCount()) + "\'.");
    }

    void Errors::registerFormalTypeMismatch(const FunctionNode& node, const FunctionNode& parent) {
        add(node.loc, "Function \'" + node.getId() + "\' overrides but types of arguments do not match.");
    }

    void Errors::registerTypeMisMatch(const NegateNode&node) {
        add(node.loc, "Wrong type for negation operator. Expected \'Boolean\', got \' " + node.expr->getType() + "\'.");
    }

    void Errors::registerTypeMismatch(const FunctionNode& node, const FunctionNode& parent) {
        add(node.loc, "Function \'" + node.getId() + "\' overrides but does not match return type of overridden function.");
    }

    void Errors::registerEmptyOverride(const FunctionNode& node, std::string parent) {
        add(node.loc, "Trying to override function \'" + node.getId() + "\', but is not found in base class \'" + parent + "\'.");
    }

    void Errors::registerCaseRedefinition(const CaseNode& node) {
        add(node.loc, "Case for type \'" + node.type->getType() + "\' is already defined in this pattern matching block.");
    }

    void Errors::registerCaseDerivesCase(const CaseNode& node, const CaseNode& parent) {
        add(node.loc, "Case for type \'" + node.type->getType() + "\' is already defined in this pattern matching block for base class \'" + parent.type->getType() + "\'.");
    }

    void Errors::registerCaseTypeMismatch(const CaseNode& node, const MatchNode& match) {
        add(node.loc, "Case type \'" + node.type->getType() + "\' is not a derived class of type \'" + match.expr->getType() + "\'");
    }

    void Errors::registerNothingInDefinition(const VariableDefinitionNode& node) {
        add(node.loc, "Type \'Nothing\' is not allowed in a value declaration.");
    }

    void Errors::registerTypeMismatch(const IfNode& node) {
        add(node.cond->loc, "Wrong type for condition of if-statement. Expected \'Boolean\', got \'" + node.cond->getType() + "\'.");
    }

    void Errors::registerTypeMismatch(const WhileNode& node) {
        add(node.cond->loc, "Wrong type for condition of while-statement. Expected \'Boolean\', got \'" + node.cond->getType() + "\'.");
    }

    void Errors::registerTypeMismatch(const AssignNode& node) {
        add(node.loc, "Wrong type of value being assigned to \'" + node.getId() + "\'. Expected \'" + node.id->getType() + "\', got \'" + node.expr->getType() + "\'.");
    }

    void Errors::registerTypeMismatch(const VariableDefinitionNode& node) {
        add(node.loc, "Wrong type of value being assigned to \'" + node.getId() + "\'. Expected \'" + node.getType() + "\', got \'" + node.expr->getType() + "\'");
    }

    void Errors::registerTypeMismatch(const FunctionNode& node) {
        add(node.loc, "Wrong return type for function \'" + node.getId() + "\'. Expected \'" + node.getType() + "\', got \'" + node.expr->getType() + "\'.");
    }

    void Errors::registerTypeMismatch(const ClassVariableNode& node) {
        add(node.loc, "Wrong type of value being assigned to \'" + node.getId() + "\'. Expected \'" + node.getType() + "\', got \'" + node.expr->getType() + "\'.");
    }

    void Errors::registerActualTypeMismatch(const Node& node, const std::string&fname, bool constructor, std::string actual, std::string expected, int index) {

        if (constructor) {
            add(node.loc, "Type of argument " + std::to_string(index + 1) + " for constructor of class \'" + fname + "\' is incorrect. Expected \'" + expected + "\', got \'" + actual + "\'.");
        }
        add(node.loc, "Wrong type for argument " + std::to_string(index + 1) + " in function call \'" + fname + "\'. Expected \'" + expected + "\', got \'" + actual + "\'.");
    }

    void Errors::registerTypeMisMatch(const BinNode& node, bool left, std::string op) {
        if (left) {
            add(node.left->loc, "Wrong type for left operand of binary operator \'" + op + "\'. Expected \'Int\', got \'" + node.left->getType() + "\'.");
        }
        add(node.right->loc, "Wrong type for right operand of binary operator \'" + op + "\'. Expected \'Int\', got \'" + node.right->getType() + "\'.");
    }

    void Errors::registerCircularDependancy(const ClassNode& node, std::string at) {
        add(node.loc, "Circular Dependency. Class \'" + at + "\' extends itself (in)directly.");
    }

    void Errors::registerUndeclaredFunction(const Node& node, const std::string & cname, const std::string & fname) {
        add(node.loc, "Function \'" + fname + "\' not found in class \'" + cname + "\'.");
    }

    void Errors::registerTypeUndefined(const NewNode& node) {
        add(node.loc, "Unable to instantiate new object of class \'" + node.getType() + "\', class not declared.");
    }

    void Errors::registerTypeUndefined(const ExtendNode& node) {
        add(node.loc, "Extending non-existent type \'" + node.getType() + "\'.");
    }

    void Errors::registerIllegalExtend(const ExtendNode& node) {
        add(node.loc, "Extending type \'" + node.getType() + "\' is not allowed.");
    }

    void Errors::registerActualCountMismatch(const Node& node, const std::string & fname, bool constructor, int actual, int expected) {
        if (constructor) {
            add(node.loc, "Expected " + std::to_string(expected) + " argument(s) for constructor of class \'" + fname + "\', got " + std::to_string(actual) + ".");
        }
        add(node.loc, "Expected " + std::to_string(expected) + " argument(s) for function call \'" + fname + "\', got " + std::to_string(actual) + ".");
    }

    void Errors::registerMainFormalsMismatch(const FormalsNode& node) {
        add(node.loc, "Class \'Main\' must have 0 parameters, got " + std::to_string(node.getCount()) + ".");
    }

    //warnings

    void Errors::registerMissingMainClass(bool warning) {
        add("No class 'Main' found in input files.", warning ? ErrorType::WARNING : ErrorType::CRITICAL);
    }

    void Errors::registerEqualsNull(const Node&node) {
        add(node.loc, "Unsafe Null check: this causes a possible runtime-exception. Checking for Null should done using a match-statement. ", ErrorType::WARNING);
    }

    void Errors::registerSyntaxError(const Cool::location & loc, const std::string message) {
        registerError(loc, "syntax error, " + message);
    }

    std::ostream& operator<<(std::ostream& out, const Errors& errors) {
        for (const auto& error : errors.errors) {
            out << error << "\n";
        }
        return out;
    }
}