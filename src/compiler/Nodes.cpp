/* 
 * File:   Nodes.cpp
 * Author: wessel
 * 
 * Created on October 7, 2015, 12:57 PM
 */

#include "Nodes.h"
#include "Types.h"
#include "Visitor.h"

namespace Cool {
    
    Node::Node(const location & _loc) : loc(_loc) {
    }

    void Node::accept(Visitor& visitor, bool forward) {
        visitor.visit(this);
        if (forward) {
            visitor.leave(this);
        }
    }

    IntNode::IntNode(const location & loc, int v) : Node(loc), val(v) {
    }

    void IntNode::accept(Visitor & visitor, bool forward) {
        visitor.visit(this);
        if (forward) {
            visitor.leave(this);
        }
    }

    IdNode::IdNode(const std::string& i, const location loc) :  Node(loc), id(i) {
    }

    void IdNode::accept(Visitor& visitor, bool forward) {
        visitor.visit(this);
        if (forward) {
            visitor.leave(this);
        }
    }

    ObjectNode::ObjectNode(const std::string& _id, const location loc) : Node(loc), id(&_id), type(Types::UNDEFINED){
    }
    
    void ObjectNode::accept(Visitor& visitor, bool forward) {
        visitor.visit(this);
        if (forward) {
            visitor.leave(this);
        }
    }

    TypeNode::TypeNode(const std::string& id, const location loc) :  Node(loc), type_id(id) {
    }

    void TypeNode::accept(Visitor& visitor, bool forward) {
        visitor.visit(this);
        if (forward) {
            visitor.leave(this);
        }
    }

    StringNode::StringNode(const location & loc, const std::string& s) : Node(loc), val(s) {
    }

    void StringNode::accept(Visitor& visitor, bool forward) {
        visitor.visit(this);
        if (forward) {
            visitor.leave(this);
        }
    }

    BoolNode::BoolNode(const location & loc, bool b) : Node(loc), val(b) {
    }

    void BoolNode::accept(Visitor& visitor, bool forward) {
        visitor.visit(this);
        if (forward) {
            visitor.leave(this);
        }
    }

    AssignNode::AssignNode(ObjectNode* i, Node* _expr, const location loc) : Node(loc), id(i), expr(_expr) {
    }

    void AssignNode::accept(Visitor& visitor, bool forward) {

        visitor.visit(this);
        if (forward) {
            id->accept(visitor, true);
            expr->accept(visitor, true);
            visitor.leave(this);
        }
    }

    NegateNode::NegateNode(const location & loc, Node* e) : Node(loc), expr(e) {
    }

    void NegateNode::accept(Visitor& visitor, bool forward) {
        visitor.visit(this);
        if (forward) {
            expr->accept(visitor, true);
            visitor.leave(this);
        }
    }

    UnaryMinusNode::UnaryMinusNode(const location & loc, Node* e) : Node(loc) , expr(e){
    }

    void UnaryMinusNode::accept(Visitor& visitor, bool forward) {
        visitor.visit(this);
        if (forward) {
            expr->accept(visitor, true);
            visitor.leave(this);
        }
    }

    IfNode::IfNode(const location & loc, Node* _cond, Node* tB, Node* fB) :  Node(loc), cond(_cond), tBranch(tB), fBranch(fB) {
    }

    void IfNode::accept(Visitor& visitor, bool forward) {
        visitor.visit(this);
        if (forward) {
            cond->accept(visitor, true);
            tBranch->accept(visitor, true);
            fBranch->accept(visitor, true);
            visitor.leave(this);
        }
    }

    WhileNode::WhileNode(const location & loc, position _end, Node* _cond, Node* _expr) : Node(loc), cond(_cond), expr(_expr), end(_end) {
    }

    void WhileNode::accept(Visitor& visitor, bool forward) {
        visitor.visit(this);
        if (forward) {
            cond->accept(visitor, true);
            expr->accept(visitor, true);
            visitor.leave(this);
        }
    }

    SuperNode::SuperNode(const location & loc, IdNode* oid, ActualsNode* ac) : Node(loc) , id(oid), actuals(ac){
    }

    void SuperNode::accept(Visitor& visitor, bool forward) {
        visitor.visit(this);
        if (forward) {
            id->accept(visitor, true);
            actuals->accept(visitor, true);
            visitor.leave(this);
        }
    }
    
    FunctionCallNode::FunctionCallNode(const location & loc, IdNode* _id, ActualsNode* ac) :  Node(loc), id(_id), actuals(ac) {
    }

    void FunctionCallNode::accept(Visitor& visitor, bool forward) {
        visitor.visit(this);
        if (forward) {
            id->accept(visitor, true);
            actuals->accept(visitor, true);
            visitor.leave(this);
        }
    }

    NewNode::NewNode(const location & loc, TypeNode* _type, ActualsNode* ac) : Node(loc), type(_type), actuals(ac) {
    }

    void NewNode::accept(Visitor& visitor, bool forward) {
        visitor.visit(this);
        if (forward) {
            type->accept(visitor, true);
            actuals->accept(visitor, true);
            visitor.leave(this);
        }
    }

    BlockNode::BlockNode(const std::vector<Node*> &_nodes, const location loc) : Node(loc), nodes(_nodes){
    }

    void BlockNode::accept(Visitor& visitor, bool forward) {
        visitor.visit(this);
        if (forward) {
            for (auto node : nodes) {
                node->accept(visitor, true);
            }
            visitor.leave(this);
        }
    }

    ParenthesesExpressionNode::ParenthesesExpressionNode(const location & loc, Node* ex) : Node(loc), expr(ex) {
    }

    void ParenthesesExpressionNode::accept(Visitor& visitor, bool forward) {
        visitor.visit(this);
        if (forward) {
            expr->accept(visitor, true);
            visitor.leave(this);
        }
    }

    ObjectFunctionCallNode::ObjectFunctionCallNode(Node* ex, IdNode* oid, ActualsNode* ac, const location loc) :  Node(loc), expr(ex), id(oid), actuals(ac) {
    }

    void ObjectFunctionCallNode::accept(Visitor& visitor, bool forward) {
        visitor.visit(this);
        if (forward) {
            expr->accept(visitor, true);
            id->accept(visitor, true);
            actuals->accept(visitor, true);
            visitor.leave(this);
        }
    }

    MatchNode::MatchNode(const location & loc, Node* ex, const std::vector<CaseNode*> &_cases) : Node(loc), expr(ex), cases(_cases) {
    }

    void MatchNode::accept(Visitor& visitor, bool forward) {
        visitor.visit(this);
        if (forward) {
            expr->accept(visitor, true);
            for (auto c : cases){
                c->accept(visitor, true);
            }
            visitor.leave(this);
        }
    }


    BinNode::BinNode(const location & loc, Node* l, Node* r) : Node(loc), left(l), right(r) {
    }

    LtEqNode::LtEqNode(const location & loc, Node* l, Node* r) : BinNode(loc, l, r) {
    }

    void LtEqNode::accept(Visitor& visitor, bool forward) {
        visitor.visit(this);
        if (forward) {
            left->accept(visitor, true);
            right->accept(visitor, true);
            visitor.leave(this);
        }
    }


    LtNode::LtNode(const location & loc, Node* l, Node* r) : BinNode(loc, l, r) {
    }

    void LtNode::accept(Visitor& visitor, bool forward) {
        visitor.visit(this);
        if (forward) {
            left->accept(visitor, true);
            right->accept(visitor, true);
            visitor.leave(this);
        }
    }

    MultiplyNode::MultiplyNode(const location & loc, Node* l, Node* r) : BinNode(loc, l, r) {
    }

    void MultiplyNode::accept(Visitor& visitor, bool forward) {
        visitor.visit(this);
        if (forward) {
            left->accept(visitor, true);
            right->accept(visitor, true);
            visitor.leave(this);
        }
    }

    DivideNode::DivideNode(const location & loc, Node* l, Node* r) : BinNode(loc, l, r) {
    }

    void DivideNode::accept(Visitor& visitor, bool forward) {
        visitor.visit(this);
        if (forward) {
            left->accept(visitor, true);
            right->accept(visitor, true);
            visitor.leave(this);
        }
    }

    void AddNode::accept(Visitor& visitor, bool forward) {
        visitor.visit(this);
        if (forward) {
            left->accept(visitor, true);
            right->accept(visitor, true);
            visitor.leave(this);
        }
    }

    AddNode::AddNode(const location & loc, Node* l, Node* r) : BinNode(loc, l, r) {
    }

    SubtractNode::SubtractNode(const location & loc, Node* l, Node* r) : BinNode(loc, l, r) {
    }

    void SubtractNode::accept(Visitor& visitor, bool forward) {
        visitor.visit(this);
        if (forward) {
            left->accept(visitor, true);
            right->accept(visitor, true);
            visitor.leave(this);
        }
    }

    CaseNode::CaseNode(const location & loc, IdNode*i, TypeNode*_type, BlockNode*_expr) : Node(loc), id(i), type(_type), expr(_expr) {
    }
    
    void CaseNode::accept(Visitor& visitor, bool forward) {
        visitor.visit(this);
        if (forward) {
            id->accept(visitor, true);
            type->accept(visitor, true);
            expr->accept(visitor, true);
            visitor.leave(this);
        }
    }

    ActualsNode::ActualsNode(const location loc, const std::vector<Node*> & exprs) : Node(loc), expressions(exprs){
    }

    void ActualsNode::accept(Visitor& visitor, bool forward) {
        visitor.visit(this);
        if (forward) {
            for (auto e : expressions) {
                e->accept(visitor, true);
            }
            visitor.leave(this);
        }
    }

    FormalsNode::FormalsNode(const location & loc, const std::vector<FormalNode*> & form) : Node(loc), formals(form) {
    }

    void FormalsNode::accept(Visitor& visitor, bool forward) {
        visitor.visit(this);
        if (forward) {
            for (auto f : formals) {
                f->accept(visitor, true);
            }
            visitor.leave(this);
        }
    }

    FormalNode::FormalNode(ObjectNode* i, TypeNode* _type, const location loc) : Node(loc), id(i), type(_type) {
    }

    void FormalNode::accept(Visitor& visitor, bool forward) {
        visitor.visit(this);
        if (forward) {
            id->accept(visitor, true);
            type->accept(visitor, true);
            visitor.leave(this);
        }
    }

    VariableDefinitionNode::VariableDefinitionNode(const location & loc, IdNode* i, TypeNode* _type, Node* e) : Node(loc), id(i), type(_type), expr(e) {
    }

    void VariableDefinitionNode::accept(Visitor& visitor, bool forward) {
        visitor.visit(this);
        if (forward) {
            id->accept(visitor, true);
            type->accept(visitor, true);
            expr->accept(visitor, true);
            visitor.leave(this);
        }
    }

    ClassBodyNode::ClassBodyNode(const std::vector<Node*> & _features, const std::vector<ClassVariableNode*> & vars, const std::vector<FunctionNode*> & funcs, const std::vector<BlockNode*> & blocks, const location loc) :
    Node(loc), features(_features), feature_vars(vars),  feature_funcs(funcs), feature_blocks(blocks) {
    }

    void ClassBodyNode::accept(Visitor& visitor, bool forward) {
        visitor.visit(this);
        if (forward) {
            for (auto f : features) {
                f->accept(visitor, true);
            }
            visitor.leave(this);
        }
    }

    FunctionNode::FunctionNode(bool _override, const std::string& _id, FormalsNode* form, TypeNode* _type, Node* _expr, const location loc) :
     Node(loc), isOverride(_override), id(_id), formals(form), type(_type), expr(_expr) {
    }

    void FunctionNode::accept(Visitor& visitor, bool forward) {
        visitor.visit(this);
        if (forward) {
            formals->accept(visitor, true);
            type->accept(visitor, true);
            expr->accept(visitor, true);
            visitor.leave(this);
        }
    }

    ClassVariableNode::ClassVariableNode(IdNode* i, TypeNode* _type, Node* e, const location loc) : Node(loc), id(i), type(_type), expr(e) {
    }

    ClassVariableNode::ClassVariableNode(IdNode* i, NativeNode* nat, const location loc) : Node(loc), id(i), type(nullptr), expr(nat) {
    }

    void ClassVariableNode::accept(Visitor& visitor, bool forward) {
        visitor.visit(this);
        if (forward) {
            id->accept(visitor, true);
            if (type) {
                type->accept(visitor, true);
            }
            if (expr) {
                expr->accept(visitor, true);
            }

            visitor.leave(this);
        }
    }

    ClassNode::ClassNode(const location & loc, TypeNode* _type, FormalsNode* form, ExtendNode* ext, ClassBodyNode* b) :
    Node(loc), type(_type), varFormals(form), body(b), extend(ext) {

    }

    void ClassNode::accept(Visitor& visitor, bool forward) {
        visitor.visit(this);
        if (forward) {
            type->accept(visitor, true);
            varFormals->accept(visitor, true);
            extend->accept(visitor, true);
            body->accept(visitor, true);
            visitor.leave(this);
        }
    }

    ExtendNode::ExtendNode(const location & loc, TypeNode* _type, ActualsNode* ac) : Node(loc), type(_type), actuals(ac), native(nullptr){
    }

    ExtendNode::ExtendNode(NativeNode* nat, const location loc) : Node(loc), type(nullptr), actuals(nullptr), native(nat) {
    }

    void ExtendNode::accept(Visitor& visitor, bool forward) {
        visitor.visit(this);
        if (forward) {
            if (native) {
                native->accept(visitor, true);
            } else {
                type->accept(visitor, true);
                actuals->accept(visitor, true);
            }
            visitor.leave(this);
        }
    }

    NativeNode::NativeNode(const location loc) : Node(loc) {
    }

    void NativeNode::accept(Visitor& visitor, bool forward) {
        visitor.visit(this);
        if (forward) {
            visitor.leave(this);
        }
    }

    NullNode::NullNode(const location & loc) : Node(loc) {
    }

    void NullNode::accept(Visitor& visitor, bool forward) {
        visitor.visit(this);
        if (forward) {
            visitor.leave(this);
        }
    }

    UnitNode::UnitNode(const location & loc) : Node(loc) {

    }

    void UnitNode::accept(Visitor& visitor, bool forward) {
        visitor.visit(this);
        if (forward) {
            visitor.leave(this);
        }
    }

    ThisNode::ThisNode(const location& loc) : Node(loc) {
    }

    void ThisNode::accept(Visitor& visitor, bool forward) {
        visitor.visit(this);
        if (forward) {
            visitor.leave(this);
        }
    }
}