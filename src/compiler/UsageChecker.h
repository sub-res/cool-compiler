/* 
 * File:   UsageChecker.h
 * Author: len
 *
 * Created on January 19, 2016, 1:19 PM
 */

#ifndef USAGECHECKER_H
#define	USAGECHECKER_H

#include <vector>
#include <string>
#include "Visitor.h"

namespace Cool {

    class Environment;

    /*Collects used types/classes and used functions of those classes
     Usefull for code generation*/
    class UsageChecker {
    public:
        UsageChecker(const Environment &);
        void start();
        const std::vector<const FunctionNode *> & getUsedFuncs() const;
        const std::vector<const std::string*> & getUsedTypes() const;
        bool functionUsed(const FunctionNode & n) const;
        bool typeUsed(const std::string & n) const;
    private:
        const Environment & env;

        class UsageVisitor : public Visitor {
        public:

            UsageVisitor(const Environment &);
            void visit(FunctionNode*);
            void visit(ObjectFunctionCallNode*);
            void visit(FunctionCallNode*);
            void visit(SuperNode*);
            void visit(NewNode*);
            void visit(TypeNode*);

            std::vector<const FunctionNode*>    usedFuncs;
            std::vector<const std::string*>     usedTypes;
        private:
            void visitFunction(FunctionNode * node);
            const Environment & env;
        };
        UsageVisitor visitor;
    };
}

#endif	/* USAGECHECKER_H */

