/* 
 * File:   Analyser.h
 * Author: root
 *
 * Created on October 15, 2015, 1:20 PM
 */

#ifndef ANALYSER_H
#define	ANALYSER_H

#include <map>
#include <memory>
#include <stack>
#include "Types.h"
#include "Visitor.h"
#include "Nodes.h"
#include "TypeValidator.h"
namespace Cool {
    class Environment;
    class Error;

    class Analyser {
    public:
        Analyser(const Environment & environment);
        void start();

    private:
        const Environment & environment; //the class environment
        const TypeValidator typeValidator; //the type validator

        bool validateFunctionCall(const std::string & cname, const Node & expr, const std::string & fname, const ActualsNode & actuals) const;
        bool validateAc(const Node&node, const std::string & fname, bool constructor, const ActualsNode&, const FormalsNode&)const;
        bool validateNew(const NewNode&) const;
        bool validateExtend(const ExtendNode&) const;

        bool validateInheritanceTree()const;

        bool validExtend(const std::string & cname) const;
        bool illegalExtend(const std::string & cname)const;

        bool validUserClassName(const std::string& name) const;

        class TreeAnalyser : public Visitor {
        public:
            TreeAnalyser(const Analyser& analyser);

            void start();

            //check if valid objects            
            void visit(ObjectNode*);
            //enter new scope
            void visit(BlockNode*);
            void visit(ClassNode*);
            void visit(FunctionNode*);
            //check if Type exists
            void visit(TypeNode*);
            void visit(ClassBodyNode*);
            //set type of node to curClass
            void visit(SuperNode*);

            void visit(ThisNode*);
            void visit(NativeNode*);

            //declarations (add new var)
            void visit(MatchNode*);
            void visit(CaseNode*);
            void visit(FormalNode*);
            void visit(ExtendNode*);

            void visit(FunctionCallNode*);

            void visit(ObjectFunctionCallNode*);
            void visit(NewNode*);

            //type match 
            void visit(LtEqNode*);
            void visit(LtNode*);
            void visit(AddNode*);
            void visit(SubtractNode*);
            void visit(MultiplyNode*);
            void visit(DivideNode*);
            void visit(AssignNode*);
            void visit(ClassVariableNode*);
            void visit(VariableDefinitionNode*n);
            void visit(IfNode*n);
            void visit(WhileNode*n);
            void visit(NegateNode*);
            void visit(FormalsNode*);
            void visit(ActualsNode*);
            void visit(ParenthesesExpressionNode*);

        private:
            const Analyser & analyser;
            std::string curClass;
            bool inBody = false;
            std::map<std::string, FunctionNode*> pastFuncs;

            void addScopeVar(const std::string & id, const std::string & type, const location & loc);
       
            class Scope {
            public:
                Scope();
                ~Scope();   
                Scope(const Scope&) = delete;
                static std::string* get(const std::string & id);
                static void set(const std::string & id, const std::string & type);
                static void reset();
            private:
                static std::vector<Scope*>scopes;
                std::map<std::string, std::string>vars;
            };
        };
       
    };
}
#endif	/* ANALYSER_H */

