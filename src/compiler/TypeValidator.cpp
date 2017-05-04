/* 
 * File:   TypeValidator.cpp
 * Author: len
 * 
 * Created on February 7, 2016, 12:05 AM
 */

#include "TypeValidator.h"
#include "Environment.h"
#include "Nodes.h"
#include "Errors.h"
#include "Types.h"

namespace Cool {

    TypeValidator::TypeValidator(const Environment & e) : env(e) {
    }

    bool TypeValidator::validate(const IfNode&node)const {
        if (!matchType(node.cond->getType(), Types::BOOL)) {
            env.errors.registerTypeMismatch(node);
            return false;
        }
        return true;
    }

    bool TypeValidator::validate(const WhileNode&node)const {

        if (!matchType(node.cond->getType(), Types::BOOL)) {
            env.errors.registerTypeMismatch(node);
            return false;
        }
       return true;
    }

    bool TypeValidator::validate(const AddNode&node)const {
        return matchBinOpIntExplicit(node, "+");
    }

    bool TypeValidator::validate(const SubtractNode&node)const {
        return matchBinOpIntExplicit(node, "-");
    }

    bool TypeValidator::validate(const MultiplyNode&node)const {
        return matchBinOpIntExplicit(node, "*");
    }

    bool TypeValidator::validate(const DivideNode&node)const {
        return matchBinOpIntExplicit(node, "/");
    }

    bool TypeValidator::validate(const LtEqNode & node)const {
        return matchBinOpIntExplicit(node, "<=");
    }

    bool TypeValidator::validate(const LtNode & node)const {
        return matchBinOpIntExplicit(node, "<");
    }

    bool TypeValidator::validate(const AssignNode&node)const {
        if (!matchType(node.expr->getType(), node.id->getType())) {
            env.errors.registerTypeMismatch(node);
            return false;
        }
       return true;
    }

    bool TypeValidator::validate(const VariableDefinitionNode&node)const {
        if (!matchType(node.expr->getType(), node.getType())) {
            env.errors.registerTypeMismatch(node);
            return false;
        }
       return true;
    }

    bool TypeValidator::validate(const FunctionNode & node)const {
        if (!matchType(node.expr->getType(), node.getType())) {
            env.errors.registerTypeMismatch(node);
            return false;
        }
      return true;
    }

    bool TypeValidator::validate(const ClassVariableNode & node)const {
        if (!matchType(node.expr->getType(), node.getType())) {
            env.errors.registerTypeMismatch(node);
            return false;
        }
       return true;
    }

    bool TypeValidator::validate(const NegateNode& node) const {
        if (node.expr->getType() != Types::BOOL) {
            env.errors.registerTypeMisMatch(node);
            return false;
        }
       return true;
    }

    bool TypeValidator::validate(const Node&node, const std::string& fname, bool constructor, const ActualsNode& ActualsNode, const FormalsNode& varFormals) const {
        const std::vector<FormalNode*> cons_arguments = varFormals.formals;
        const std::vector<Node*> actuals = ActualsNode.expressions;
        const auto actualCount = actuals.size();
        bool error = false;
        for (auto i = decltype(actualCount){0}; i < actualCount; i++) {
            if (!matchType(actuals[i]->getType(), cons_arguments[i]->getType())) {
                env.errors.registerActualTypeMismatch(node, fname, constructor, actuals[i]->getType(), cons_arguments[i]->getType(), i);
                error = true;
            }
        }
        return !error;
    }

    bool TypeValidator::matchFormals(const FormalsNode&n1, const FormalsNode&n2) const {
        std::vector<FormalNode*>formals1 = n1.formals;
        std::vector<FormalNode*>formals2 = n2.formals;
        for (decltype(formals1.size())i = 0; i < formals1.size(); i++) {
            if (formals1[i]->getType() != formals2[i]->getType()) {
                return false;
            }
        }
        return true;
    }

    bool TypeValidator::matchBinOpIntExplicit(const BinNode&node, std::string type_operator) const {
        bool error = false;
        if (node.left->getType() != Types::INT) {
            env.errors.registerTypeMisMatch(node, true, type_operator);
            error = true;
        }
        if (node.right->getType() != Types::INT) {
            env.errors.registerTypeMisMatch(node, false, type_operator);
            error = true;
        }
        return !error;
    }

    bool TypeValidator::matchBinOpInt(const BinNode&node, std::string type_operator)const {

        bool error = false;
        if (!matchType(node.left->getType(), Types::INT)) {
            env.errors.registerTypeMisMatch(node, true, type_operator);
            error = true;
        }
       
        if (!matchType(node.right->getType(), Types::INT)) {
            env.errors.registerTypeMisMatch(node, false, type_operator);
            error = true;
        }
        return !error;
    }

    bool TypeValidator::matchType(const std::string & actual_type, const std::string & expect_type)const {

        return ((actual_type == Types::NOTHING) || (actual_type == Types::NATIVE) || (expect_type == Types::NATIVE)) 
               || (actual_type == Types::NOTHING) || (expect_type == Types::ANY) 
               || ((actual_type == Types::_NULL) && !((expect_type == Types::BOOL) | (expect_type == Types::UNIT) | (expect_type == Types::INT))) 
               || ((expect_type != Types::UNDEFINED) && env.ltEqInherits(actual_type, expect_type));
    }

}
