/* 
 * File:   IntermediateFormer.h
 * Author: root
 *
 * Created on November 6, 2015, 3:22 PM
 */

#ifndef INTERMEDIATEFORMER_H
#define	INTERMEDIATEFORMER_H
#include <map>
#include <vector>
#include <string>
#include "Visitor.h"

namespace Cool {

    class Environment;

    class Intermediater {
    public:
        Intermediater(Environment &env);
        void translate(const std::map<std::string, ClassNode*> & classes, std::vector<std::string> ignores);
    private:
        //prefix added to var formal ids and actual ids in extend (aliases)
        static const std::string varFPrefix;

        void addDummyConstructor(ClassNode & c);

        Environment&env;
        
        class ExtendFormer : public Visitor { //visitor for replacing id's referring to var-formals
        public:
            void visit(ObjectNode*node);
            void start(ClassNode&node);
        private:
            std::vector<std::string>formalIds;
        };
    };
}
#endif	/* INTERMEDIATEFORMER_H */

