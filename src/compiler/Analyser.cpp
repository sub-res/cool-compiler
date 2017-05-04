/* 
 * File:   Analyser.cpp
 * Author: root
 * 
 * Created on October 15, 2015, 1:20 PM
 */

#include "Analyser.h"
#include <algorithm>
#include "Error.h"
#include "Printer.h"
#include "Environment.h"
#include "Nodes.h"
#include "Config.h"
#include <assert.h>

namespace Cool {

    Analyser::Analyser(const Environment& env) : environment(env), typeValidator(env) {
    }

    void Analyser::start() {
        if (!validateInheritanceTree()) {
            return;
        }

        TreeAnalyser sc(*this); //abstract syntax tree visitor
        sc.start(); //start semantic analysis

        if (const ClassNode * maincl = environment.getUserClass("Main")) {
            if (maincl->varFormals->getCount()) { //main should have 0 formals
                environment.errors.registerMainFormalsMismatch(*maincl->varFormals);
            }
        } else {
            environment.errors.registerMissingMainClass(environment.config.library); //give warning
        }
    }

    /*Validate actuals for Constructor*/
    bool Analyser::validateAc(const Node& node, const std::string & fname, bool constructor, const ActualsNode&actuals, const FormalsNode&formals) const {

        if (actuals.getCount() != formals.getCount()) {
            environment.errors.registerActualCountMismatch(node, fname, constructor, actuals.getCount(), formals.getCount());
            return false;
        }
        return typeValidator.validate(node, fname, constructor, actuals, formals);
    }

    bool Analyser::validateExtend(const ExtendNode&node) const {

        const std::string & type = node.getType();
        if (type == Types::ANY || type == Types::IO) {
            if(node.actuals->getCount() == 0){
                return true;
            }else{
             environment.errors.registerActualCountMismatch(*node.type, type, true, node.actuals->getCount(), 0);
             return false;
            }
        }
        if (type == Types::NATIVE) {
            return true;
        }
        if (illegalExtend(type)) {
            environment.errors.registerIllegalExtend(node);
            return false;
        }
        if (!validExtend(type)) {
            environment.errors.registerTypeUndefined(node);
            return false;
        }
        return validateAc(*node.type, type, true, *node.actuals, *environment.getClass(type)->varFormals);
    }

    bool Analyser::validateNew(const NewNode&node)const {
        //new Any causes crash
        const std::string & type = node.getType();
        if (!environment.typeExists(type)) {
            environment.errors.registerTypeUndefined(node);
            return false;
        }
        return validateAc(*node.type, type, true, *node.actuals, *environment.getClass(type)->varFormals);
    }

    /*private function, checks if function exists and arguments are correct*/
    bool Analyser::validateFunctionCall(const std::string & cname, const Node& node, const std::string & fname, const ActualsNode&actuals) const {

        if (cname != Types::NOTHING) { //special type, doesn't have any functions
            std::string type = cname;

            const auto & inheritanceMap = environment.getInheritanceMap();
            while (inheritanceMap.find(type) != inheritanceMap.end()) {

                if (FunctionNode * ffn = environment.getFunction(type, fname)) {
                    //CHECK POSSIBLE RUNTIME EXCEPTION ERROR
                    if (fname == "equals" && actuals.getCount() && actuals.expressions[0]->getType() == Types::_NULL) {
                        environment.errors.registerEqualsNull(node); //warning
                    }
                    return validateAc(node, fname, false, actuals, *ffn->formals);
                }

                type = inheritanceMap.at(type);
            }
        }
        environment.errors.registerUndeclaredFunction(node, cname, fname);
        return false;
    }

    bool Analyser::validExtend(const std::string & cname) const {
        return (cname == Types::ANY || cname == Types::IO || environment.getUserClass(cname));
    }

    bool Analyser::illegalExtend(const std::string & cname) const {
        return cname != Types::IO && cname != Types::ANY && environment.isBasicType(cname);
    }

    bool Analyser::validUserClassName(const std::string& name) const {
        return !environment.isBasicType(name);
    }

    bool Analyser::validateInheritanceTree()const {

        const auto & inheritanceMap = environment.getInheritanceMap();
        const auto & classes = environment.getUserClasses();

        for (const auto & c : classes) {
            std::string ename = inheritanceMap.at(c.first);
            std::vector<std::string>past{c.first};

            while (ename != Types::ANY) {

                if (std::find(past.begin(), past.end(), ename) != past.end()) {
                    environment.errors.registerCircularDependancy(*c.second, ename);
                    return false;
                }

                if (inheritanceMap.find(ename) == inheritanceMap.end()) {
                    break;
                }
                past.push_back(ename);
                ename = inheritanceMap.at(ename);
            }
        }
        return true;
    }

    /*------------------------------TREE ANALYSER---------------------------*/
    Analyser::TreeAnalyser::TreeAnalyser(const Analyser & ana) : analyser(ana) {
    }

    void Analyser::TreeAnalyser::start() {
        Scope::reset();
        const auto & bClasses = analyser.environment.getBasicClasses();
        for (const auto & entry : bClasses) {
            entry.second->accept(*this);
        }
        const auto & uClasses = analyser.environment.getUserClasses();
        for (const auto & entry : uClasses) {
            if (analyser.validUserClassName(entry.first)) {
                entry.second->accept(*this);
            } else {
                analyser.environment.errors.registerReservedClassName(*entry.second);
            }
        }
    }

    void Analyser::TreeAnalyser::addScopeVar(const std::string & id, const std::string & type, const location & loc) {
        if (Scope::get(id)) {
            analyser.environment.errors.regVarRedef(loc, id, curClass);
        } else {
            Scope::set(id, type);
        }
    }

//    void Analyser::TreeAnalyser::enterScope() {
//        auto v = std::make_unique<Scope>(std::move(vars_env));
//        vars_env = std::move(v);
//    }
//
//    void Analyser::TreeAnalyser::exitScope() {
//        vars_env = std::move(vars_env->prev);
//    }

    void Analyser::TreeAnalyser::visit(ObjectNode*node) {
        const std::string & id = node->getId();
        if (const std::string * type = Scope::get(id)) {
            node->type = *type;
        } else {
            analyser.environment.errors.registerUndeclaredVar(node->loc, id);
        }
    }

    void Analyser::TreeAnalyser::visit(TypeNode*node) {
        if (!analyser.environment.typeExists(node->getType())) {
            analyser.environment.errors.registerTypeUndefined(*node);
        }
    }

    void Analyser::TreeAnalyser::visit(SuperNode*node) {
        node->actuals->accept(*this);

        const std::string & type = analyser.environment.getInheritanceMap().at(curClass);
        if (type != Types::UNDEFINED) {
            node->supClass = analyser.environment.getClass(type);
            if (analyser.validateFunctionCall(type, *node, node->getId(), *node->actuals)) {
                node->type = analyser.environment.getFunction(type, node->getId())->getType();
            }
        }
    }

    void Analyser::TreeAnalyser::visit(ExtendNode*n) {
        if (n->native) {
            n->native->accept(*this);
        } else {
            n->type->accept(*this);
            n->actuals->accept(*this);
        }
        analyser.validateExtend(*n);
    }

    void Analyser::TreeAnalyser::visit(ObjectFunctionCallNode*node) {
        node->expr->accept(*this);
        node->actuals->accept(*this);
        const std::string type = node->expr->getType();
        if (type != Types::UNDEFINED) {
            if (analyser.validateFunctionCall(type, *node, node->getId(), *node->actuals)) {
                node->type = analyser.environment.getFunction(type, node->getId())->getType();
            }
        }
    }

    void Analyser::TreeAnalyser::visit(FunctionCallNode*node) {
        node->actuals->accept(*this);
        node->pClass = analyser.environment.getClass(curClass);
        if (analyser.validateFunctionCall(curClass, *node, node->getId(), *node->actuals)) {
            node->type = analyser.environment.getFunction(curClass, node->getId())->getType();
        }
    }

    void Analyser::TreeAnalyser::visit(NewNode*node) {
        node->type->accept(*this);
        node->actuals->accept(*this);
        analyser.validateNew(*node);
    }

    void Analyser::TreeAnalyser::visit(NativeNode*node) {
        if (!analyser.environment.getBasicClass(curClass)) {
            analyser.environment.errors.registerIllegalKeyword(*node);
        }
    }

    void Analyser::TreeAnalyser::visit(ThisNode*node) {
        node->type = curClass;
    }

    void Analyser::TreeAnalyser::visit(BlockNode*node) {
        Scope scope;
        for (auto expr : node->nodes) {
            expr->accept(*this);
        }
    }

    void Analyser::TreeAnalyser::visit(FormalNode*node) {
        if (inBody) {
            addScopeVar(node->getId(), node->getType(), node->id->loc);
            node->id->accept(*this);
            node->type->accept(*this);
        }
    }

    void Analyser::TreeAnalyser::visit(ClassNode*node) {
        Scope scope; // Enter new scope. Scope is removed by default on destruction
        inBody = false;
        curClass = node->getType();

        //add inherited vars if inheritance
        const auto & inheritanceMap = analyser.environment.getInheritanceMap();

        std::string parenttype = node->extend->getType();
        while (analyser.validExtend(parenttype)) {

            for (const auto formal : analyser.environment.getClassFormals(parenttype)) {
                addScopeVar(formal->getId(), formal->getType(), formal->id->loc);
            }

            for (const auto var : analyser.environment.getClassVariables(parenttype)) {
                addScopeVar(var->getId(), var->getType(), var->id->loc);
            }

            if (inheritanceMap.find(parenttype) == inheritanceMap.end()) {
                break;
            }
            parenttype = inheritanceMap.at(parenttype);
        }

        for (const auto formal : analyser.environment.getClassFormals(curClass)) {
            addScopeVar(formal->getId(), formal->getType(), formal->id->loc);
        }
        for (const auto var : analyser.environment.getClassVariables(curClass)) {
            addScopeVar(var->getId(), var->getType(), var->id->loc);
        }

        node->type->accept(*this);
        node->varFormals->accept(*this);
        node->extend->accept(*this);
        node->body->accept(*this);

        pastFuncs.clear();
    }

    void Analyser::TreeAnalyser::visit(FunctionNode*node) {
        Scope scope;
        const std::string & func_name = node->getId();

        if (pastFuncs.find(func_name) != pastFuncs.end()) {
            analyser.environment.errors.regFuncRedef(*node, *pastFuncs.at(func_name));
            return;
        }

        const std::string & parent = analyser.environment.getInheritanceMap().at(curClass);
        if (parent == Types::NATIVE) {
            pastFuncs[func_name] = node;
            return;
        }

        FunctionNode* baseFunc = nullptr;
        if (analyser.validExtend(parent)) {
            baseFunc = analyser.environment.getFunction(parent, func_name);
        }
        const bool overrides = node->isOverride;
        if (baseFunc) {
            if (!overrides) {
                analyser.environment.errors.registerUnintentionalOverride(*node);
                return;
            }
            if (node->formals->getCount() != baseFunc->formals->getCount()) {
                analyser.environment.errors.registerFormalCountMismatch(*node, *baseFunc);
                return;
            }
            if (!analyser.typeValidator.matchFormals(*node->formals, *baseFunc->formals)) {
                analyser.environment.errors.registerFormalTypeMismatch(*node, *baseFunc);
                return;
            } //Type equals or inherits
            if (!analyser.environment.ltEqInherits(node->getType(), baseFunc->getType())) {
                analyser.environment.errors.registerTypeMismatch(*node, *baseFunc);
                return;
            }

        } else {
            if (overrides) {
                analyser.environment.errors.registerEmptyOverride(*node, parent);
                return;
            }
        }

        pastFuncs[func_name] = node;

        node->formals->accept(*this);
        node->type->accept(*this);
        node->expr->accept(*this);

        analyser.typeValidator.validate(*node);
    }

    void Analyser::TreeAnalyser::visit(ActualsNode*node) {
        for (auto expr : node->expressions) {
            expr->accept(*this);
        }
    }

    void Analyser::TreeAnalyser::visit(FormalsNode*node) {
        for (auto formal : node->formals) {
            formal->accept(*this);
        }
    }

    void Analyser::TreeAnalyser::visit(CaseNode*node) {
        Scope scope;
        if (node->getId() != "null") {
            addScopeVar(node->getId(), node->type->getType(), node->id->loc);
        }
        node->type->accept(*this);
        node->expr->accept(*this);
    }

    void Analyser::TreeAnalyser::visit(MatchNode* node) {
        std::map<std::string, CaseNode*> foundTypes;
        for (CaseNode * cNode : node->cases) {
            const std::string casetype = cNode->type->getType();

            for (const auto & prevCase : foundTypes) {

                const std::string & prevType = prevCase.first;
                if (casetype == prevType) {
                    analyser.environment.errors.registerCaseRedefinition(*cNode);
                    break;
                } else if (casetype != Types::_NULL && analyser.environment.ltEqInherits(casetype, prevType)) {
                    analyser.environment.errors.registerCaseDerivesCase(*cNode, *prevCase.second);
                }
            }
            foundTypes[casetype] = cNode;
        }

        node->expr->accept(*this);
        for (auto caseNode : node->cases) {
            caseNode->accept(*this);
        }

        const std::string exprtype = node->expr->getType();
        for (const auto caseNode : node->cases) {
            const std::string casetype = caseNode->type->getType();
            if (casetype != Types::_NULL && !analyser.environment.ltEqInherits(casetype, exprtype)) {
                analyser.environment.errors.registerCaseTypeMismatch(*caseNode, *node);
            }
        }
        std::string currLUB = node->cases.front()->getType();
        for (unsigned ctr = 1; (currLUB != Types::ANY) && (ctr < node->cases.size()); ++ctr) {
            currLUB = analyser.environment.leastUpperBound(currLUB, node->cases.at(ctr)->getType());
        }
        node->type = currLUB;
    }

    void Analyser::TreeAnalyser::visit(VariableDefinitionNode*node) {
        node->type->accept(*this);
        node->expr->accept(*this);

        if (node->getType() == Types::NOTHING) {
            analyser.environment.errors.registerNothingInDefinition(*node);
        }

        addScopeVar(node->getId(), node->getType(), node->id->loc);
        analyser.typeValidator.validate(*node);
    }

    void Analyser::TreeAnalyser::visit(ClassBodyNode*node) {
        inBody = true;
        for (auto feature : node->features) {
            feature->accept(*this);
        }
    }

    void Analyser::TreeAnalyser::visit(ParenthesesExpressionNode*node) {
        node->expr->accept(*this);
    }

    void Analyser::TreeAnalyser::visit(NegateNode*node) {
        node->expr->accept(*this);
        analyser.typeValidator.validate(*node);
    }

    void Analyser::TreeAnalyser::visit(DivideNode*node) {
        node->left->accept(*this);
        node->right->accept(*this);
        analyser.typeValidator.validate(*node);
    }

    void Analyser::TreeAnalyser::visit(AssignNode*node) {
        node->id->accept(*this);
        node->expr->accept(*this);
        analyser.typeValidator.validate(*node);
    }

    void Analyser::TreeAnalyser::visit(ClassVariableNode*node) {
        if (node->type) {
            node->type->accept(*this);
        }
        if (node->expr) {
            node->expr->accept(*this);
        }
        analyser.typeValidator.validate(*node);
    }

    void Analyser::TreeAnalyser::visit(AddNode*node) {
        node->left->accept(*this);
        node->right->accept(*this);
        analyser.typeValidator.validate(*node);
    }

    void Analyser::TreeAnalyser::visit(SubtractNode*node) {
        node->left->accept(*this);
        node->right->accept(*this);
        analyser.typeValidator.validate(*node);
    }

    void Analyser::TreeAnalyser::visit(MultiplyNode*node) {
        node->left->accept(*this);
        node->right->accept(*this);
        analyser.typeValidator.validate(*node);
    }

    void Analyser::TreeAnalyser::visit(IfNode*node) {
        node->cond->accept(*this);
        node->tBranch->accept(*this);
        node->fBranch->accept(*this);
        analyser.typeValidator.validate(*node);
        node->return_type = analyser.environment.leastUpperBound(node->tBranch->getType(), node->fBranch->getType());
    }

    void Analyser::TreeAnalyser::visit(WhileNode*node) {
        node->cond->accept(*this);
        node->expr->accept(*this);
        analyser.typeValidator.validate(*node);
    }

    void Analyser::TreeAnalyser::visit(LtEqNode*node) {
        node->left->accept(*this);
        node->right->accept(*this);
        analyser.typeValidator.validate(*node);
    }

    void Analyser::TreeAnalyser::visit(LtNode*node) {
        node->left->accept(*this);
        node->right->accept(*this);
        analyser.typeValidator.validate(*node);
    }
    
    std::vector<Analyser::TreeAnalyser::Scope*> Analyser::TreeAnalyser::Scope::scopes;
       
    Analyser::TreeAnalyser::Scope::Scope(){
        scopes.push_back(this);
    }
    
    Analyser::TreeAnalyser::Scope::~Scope(){
        scopes.erase(scopes.end()-1);
    }
    
    void Analyser::TreeAnalyser::Scope::set(const std::string & id, const std::string & type) {
        scopes.back()->vars[id] = type;
    }
    
    void Analyser::TreeAnalyser::Scope::reset() {
        scopes.clear();
    }

    std::string * Analyser::TreeAnalyser::Scope::get(const std::string & id) {
        assert(!scopes.empty());
        
        for (size_t i = scopes.size(); i > 0; --i) {
            const auto scope = scopes[i-1];
            
            const auto& v = scope->vars.find(id);
            if (v != scope->vars.end()) {
                return &v->second;
            }
        }
        return nullptr;
    }
}