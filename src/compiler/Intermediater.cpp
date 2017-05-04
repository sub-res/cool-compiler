/* 
 * File:   IntermediaterFormer.cpp
 * Author: root
 * 
 * Created on November 6, 2015, 3:22 PM
 */

#include "Intermediater.h"
#include <vector>
#include "Nodes.h"
#include "FileIO.h"
#include "Visitor.h"
#include "Types.h"
#include "StringContainer.h"
#include "Environment.h"

namespace Cool {

    Intermediater::Intermediater(Environment& env) : env(env){}
    
    const std::string Intermediater::varFPrefix = "A_";

    void Intermediater::ExtendFormer::visit(ObjectNode* node) {
            const std::string& id = *node->id;
            for (decltype(formalIds.size())i = 0; i < formalIds.size(); ++i) {
                if (formalIds[i] == id) {
                    node->id = &StringContainer::get(Intermediater::varFPrefix + id);
                }
            }
    }

    void Intermediater::ExtendFormer::start(ClassNode&c) {
        ExtendNode*ext = c.extend;
        if (!ext->native && ext->actuals->getCount() > 0) {
            std::vector<FormalNode*> formals = c.varFormals->formals;
            for (std::vector<FormalNode*>::const_iterator it = formals.begin(); it != formals.end(); ++it) {
                formalIds.push_back((*it)->getId());
            }
            c.extend->accept(*this,true);
            formalIds.clear();
        }
    }

    void Intermediater::addDummyConstructor(ClassNode& c) {
        std::vector<FormalNode*> formals = c.varFormals->formals;

        FunctionNode* constructor = env.mk<FunctionNode>(false, c.getType(), c.varFormals, env.mk<TypeNode>(c.getType()), env.mk<NativeNode>());
        c.body->feature_funcs.insert(c.body->feature_funcs.begin(), constructor);
        c.body->features.push_back(constructor);

        for (std::vector<FormalNode*>::const_iterator it = formals.begin(); it != formals.end(); ++it) {
            FormalNode * fN = *it;
            auto cvNode = env.mk<ClassVariableNode>(env.mk<IdNode>(fN->getId()), env.mk<TypeNode>(fN->getType()), nullptr);
            c.body->feature_vars.insert(c.body->feature_vars.begin(), cvNode);
            c.body->features.push_back(cvNode);
        }


    }

    void Intermediater::translate(const std::map<std::string, ClassNode*> & classes, std::vector<std::string> ignores) {
        ExtendFormer extFormer;
        for (std::map<std::string, ClassNode*>::const_iterator it = classes.begin(); it != classes.end(); it++) {

            ClassNode * c = it->second;

            if (std::find(ignores.begin(), ignores.end(), c->getType()) != ignores.end() || c->extend->getType() == Types::NATIVE) {
                addDummyConstructor(*c);
                extFormer.start(*c);
                continue;
            }
            extFormer.start(*c);

            //add 'this' in block to end of classbody
            ThisNode * thisNode = env.mk<ThisNode>(c->loc);
            thisNode->type = c->getType();
            std::vector<Node*>expressions;
            expressions.push_back(thisNode);
            c->body->feature_blocks.push_back(env.mk<BlockNode>(expressions));

            //copy all blocks to constructor
            const auto size = c->body->feature_blocks.size();
            std::vector<Node*>constructorExprs;
            for (auto i = decltype(size){0}; i < size; ++i) {
                constructorExprs.push_back(c->body->feature_blocks.at(i));
            }

            std::vector<ClassVariableNode*>class_vars = c->body->feature_vars;
            std::vector<FormalNode*>classFormals = c->varFormals->formals;


            //create AssignNodes for the constructor for every existing featurevar
            for (std::vector<ClassVariableNode*>::iterator it = class_vars.begin(); it != class_vars.end(); ++it) {
                AssignNode*aN = env.mk<AssignNode>(env.mk<ObjectNode>((*it)->id->id), (*it)->expr);
                constructorExprs.insert(constructorExprs.begin(), aN);
            }


            //storage for constructor formals
            std::vector<FormalNode*>constructorFormals; //constructorFormal


            for (std::vector<FormalNode*>::iterator it = classFormals.begin(); it != classFormals.end(); it++) {

                FormalNode*n = *it;
                
                const std::string& renamedFormalId = StringContainer::get(varFPrefix + n->getId()); //rename constructor argument
                constructorFormals.push_back(env.mk<FormalNode>(env.mk<ObjectNode>(renamedFormalId), env.mk<TypeNode>(n->getType())));

                //create AssignNodes for the constructor for every class formal
                AssignNode*aN = env.mk<AssignNode>(env.mk<ObjectNode>(n->getId()), env.mk<ObjectNode>(renamedFormalId));
                constructorExprs.insert(constructorExprs.begin(), aN);
            }

            //create supercall to parents constructor (this.(Parent constructor)
            ThisNode * t = env.mk<ThisNode>(c->loc);
            t->type = c->getType();


            

            ObjectFunctionCallNode*baseCall = env.mk<ObjectFunctionCallNode>(t, env.mk<IdNode>(c->extend->getType()), c->extend->actuals,c->loc);
            constructorExprs.insert(constructorExprs.begin(), baseCall);


            FunctionNode*constructor = env.mk<FunctionNode>(false, c->getType(), env.mk<FormalsNode>(location(),constructorFormals), env.mk<TypeNode>(c->getType()), env.mk<BlockNode>(constructorExprs));

            //add constructor to funcs
            c->body->feature_funcs.insert(c->body->feature_funcs.begin(), constructor);
            //c->body->features.push_back(constructor);


            //add all construct arguments->featurevars
            for (decltype(classFormals.size())i = 0; i < classFormals.size(); ++i) {
                FormalNode * fN = classFormals.at(i);
                ClassVariableNode* newVar = env.mk<ClassVariableNode>(env.mk<IdNode>(fN->getId()), env.mk<TypeNode>(fN->getType()), nullptr);
                c->body->features.push_back(newVar);
                c->body->feature_vars.push_back(newVar);
            }

            c->varFormals->formals.clear(); //not needed anymore
            c->body->feature_blocks.clear(); 

            c->body->features.clear();
            for (decltype(c->body->feature_vars.size()) i = 0; i < c->body->feature_vars.size(); ++i) {
                c->body->features.push_back(c->body->feature_vars.at(i));
            }
            for (decltype(c->body->feature_funcs.size()) i = 0; i < c->body->feature_funcs.size(); ++i) {
                c->body->features.push_back(c->body->feature_funcs.at(i));
            }
        }
    }
}