/* 
 * File:   UsageChecker.cpp
 * Author: len
 * 
 * Created on January 19, 2016, 1:19 PM
 */

#include "UsageChecker.h"
#include "Nodes.h"
#include "Environment.h"
#include <algorithm>
namespace Cool {

    UsageChecker::UsageChecker(const Environment & _env) : env(_env), visitor(_env) {
    }

    void UsageChecker::start() {
        const std::string & main = "Main";
        if (FunctionNode * f = env.getFunction(main, main, false)) {
            visitor.usedFuncs.clear();
            visitor.usedTypes.clear();
            f->accept(visitor, true);
        }
    }

    const std::vector<const FunctionNode*> & UsageChecker::getUsedFuncs() const {
        return visitor.usedFuncs;
    }

    const std::vector<const std::string*> & UsageChecker::getUsedTypes() const {
        return visitor.usedTypes;
    }

    bool UsageChecker::functionUsed(const FunctionNode & n) const {
        return std::find(visitor.usedFuncs.begin(), visitor.usedFuncs.end(), &n) != visitor.usedFuncs.end();
    }

    bool UsageChecker::typeUsed(const std::string & n) const {
        for (auto type : visitor.usedTypes) {
            if (*type == n) {
                return true;
            }
        }
        return false;
    }

    /*UsageChecker (Visitor)*/
    UsageChecker::UsageVisitor::UsageVisitor(const Environment & _env) : env(_env) {
    }

    void UsageChecker::UsageVisitor::visit(FunctionNode * node) {
        usedFuncs.push_back(node);
    }

    void UsageChecker::UsageVisitor::visit(ObjectFunctionCallNode * node) {
        visitFunction(env.getFunction(node->expr->getType(), node->getId()));
    }

    void UsageChecker::UsageVisitor::visit(FunctionCallNode * node) {

        visitFunction(env.getFunction(node->getClass(), node->getId()));
    }

    void UsageChecker::UsageVisitor::visit(NewNode * node) {

        visitFunction(env.getFunction(node->getType(), node->getType()));
    }

    void UsageChecker::UsageVisitor::visit(SuperNode * node) {

        visitFunction(env.getFunction(node->getClass(), node->getId()));
    }

    void UsageChecker::UsageVisitor::visit(TypeNode * type) {
        if (std::find(usedTypes.begin(), usedTypes.end(), &type->type_id) == usedTypes.end()) {
            usedTypes.push_back(&type->type_id);
        }
    }

    void UsageChecker::UsageVisitor::visitFunction(FunctionNode * func) {
        if (std::find(usedFuncs.begin(), usedFuncs.end(), func) == usedFuncs.end()) {
            func->accept(*this, true);
        }
    }
}
