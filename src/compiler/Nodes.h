/* 
 * File:   Nodes.h
 * Author: wessel
 *
 * Created on October 7, 2015, 12:57 PM
 */

#ifndef NODES_H
#define NODES_H

#include <vector>
#include <map>
#include <string>
#include "location.hh"
#include "Types.h"

// Every node for the syntaxtree of Cool are defined here
namespace Cool {
    class Visitor;
    class position;

    // Node is the root base class for all other nodes
    class Node {
    public:
        const location loc;
        virtual ~Node() = default;
        virtual void accept(Visitor & visitor, bool forward = false) = 0;

        virtual const std::string& getType() const {
            throw std::runtime_error("unrecognized type");
        }
        
    protected:
        explicit Node(const location & _loc);
    };

    /*empty expression*/
    class UnitNode : public Node {
    public:
        explicit UnitNode(const location & loc);
        void accept(Visitor&visitor, bool forward = false);

        const std::string& getType() const {
            return Types::UNIT;
        }
    };

    /*Keyword: native*/
    class NativeNode : public Node {
    public:
        explicit NativeNode(const location loc = location());
        void accept(Visitor&visitor, bool forward = false);

        const std::string& getType() const {
            return Types::NATIVE;
        }
    };

    /*Object Identifyer*/
    class IdNode : public Node {
    public:
        IdNode(const std::string& id, const location loc = location());

        void accept(Visitor&visitor, bool forward = false);

        const std::string& getId() const {
            return id;
        }
        const std::string& id;
    };

    class ObjectNode : public Node {
        friend class Analyser;
        friend class Intermediater;
    public:
        explicit ObjectNode(const std::string& id, const location loc = location());
        
        void accept(Visitor&visitor, bool forward = false);
        
        const std::string& getType() const {
            return type;
        }

        const std::string& getId() const {
            return *id;
        }
    protected:
        const std::string* id;
        std::string type;
    };

    /*Type Identifier*/
    class TypeNode : public Node {
    public:
        explicit TypeNode(const std::string& id, const location loc = location());

        void accept(Visitor&visitor, bool forward = false);

        const std::string& getType() const {
            return type_id;
        }
        const std::string& type_id;
    };

    /*Actual Node links to the vector of expressions*/
    class ActualsNode : public Node {
    public:
        std::vector<Node*>expressions;
        explicit ActualsNode(const location loc = location(), const std::vector<Node*> & exprs = std::vector<Node*>());

        void accept(Visitor&visitor, bool forward = false);

        decltype(expressions.size()) getCount() const {
            return expressions.size();
        }
    };

    class FormalNode : public Node {
    public:
        ObjectNode * const id;
        TypeNode * const type;
        FormalNode(ObjectNode*id, TypeNode*, const location loc = location());

        void accept(Visitor&visitor, bool forward = false);

        const std::string& getType()const {
            return type->getType();
        }

        const std::string& getId()const {
            return id->getId();
        }
    };

    /*Formal Node links to the vector of arguments*/
    class FormalsNode : public Node {
    public:
        std::vector<FormalNode*>formals; //ID : TYPE
        explicit FormalsNode(const location & loc, const std::vector<FormalNode*> & formals = std::vector<FormalNode*>());

        void accept(Visitor&visitor, bool forward = false);

        decltype(formals.size()) getCount() const {
            return formals.size();
        }
    };

    /*The Extend Node belongs to the ClassNode and is optional*/
    class ExtendNode : public Node {
    public:
        TypeNode * const type;
        ActualsNode * const actuals;
        NativeNode * const native;
        ExtendNode(const location & loc, TypeNode*, ActualsNode*actuals);
        explicit ExtendNode(NativeNode*native, const location loc = location());

        void accept(Visitor&visitor, bool forward = false);

        const std::string& getType() const {
            return native ? Types::NATIVE : type->getType();
        }
    };

    /*Block Node links to a vector of the statements inside the block*/
    class BlockNode : public Node {
    public:
        std::vector<Node*>nodes; //last = expr
        explicit BlockNode(const std::vector<Node*> & _nodes = std::vector<Node*>(), const location loc = location());

        void accept(Visitor&visitor, bool forward = false);

        const std::string& getType() const {
            return nodes.empty() ? Types::UNIT : nodes.back()->getType();
        }
    };

    /*Function declaration inside a Feature*/
    class FunctionNode : public Node {
    public:
        const bool isOverride;
        const std::string& id;
        FormalsNode * const formals;
        TypeNode * const type;
        Node * const expr;
        int symId;

        FunctionNode(bool _override, const std::string& _id, FormalsNode*form, TypeNode*, Node*_expr, const location loc = location());

        void accept(Visitor&visitor, bool forward = false);

        const std::string& getType() const {
            return type->getType();
        }

        const std::string& getId() const {
            return id;
        }
    };

    /*Variable declaration inside a Feature*/
    class ClassVariableNode : public Node {
    public:
        IdNode * const id;
        TypeNode * const type; //if null, expr = native
        Node * const expr;
        ClassVariableNode(IdNode*id, TypeNode*typeId, Node*expr, const location loc = location());
        ClassVariableNode(IdNode*id, NativeNode*expr, const location loc = location()); //for native

        void accept(Visitor&visitor, bool forward = false);

        const std::string& getType() const {
            return type ? type->getType() : Types::NATIVE;
        }

        const std::string& getId() const {
            return id->getId();
        }

    };

    /*The Class Body Node links to a vector of features*/
    class ClassBodyNode : public Node {
    public:
        std::vector<Node*>features;
        std::vector<ClassVariableNode*>feature_vars;
        std::vector<FunctionNode*>feature_funcs;
        std::vector<BlockNode*> feature_blocks;
        ClassBodyNode(const std::vector<Node*> & _features, const std::vector<ClassVariableNode*> & vars, const std::vector<FunctionNode*> & funcs, const std::vector<BlockNode*> &blocks, const location loc = location());

        void accept(Visitor&visitor, bool forward = false);
    };

    /*The Class Declaration Node is the root of the abstract syntax tree*/
    class ClassNode : public Node {
    public:

        int stId;
        TypeNode * const type;
        FormalsNode * const varFormals;
        ClassBodyNode * const body;
        ExtendNode * const extend; //optional

        ClassNode(const location & loc, TypeNode*, FormalsNode *varFormals, ExtendNode *extend, ClassBodyNode*body);

        void accept(Visitor&visitor, bool forward = false);

        const std::string& getType() const {
            return type->getType();
        }
    };



    /******************PRIMARY NODES**************************/

    /*Primary Node for Int*/
    class IntNode : public Node {
    public:
        IntNode(const location& loc, int i);
        int stId;
        const int val;
        void accept(Visitor&visitor, bool forward = false);

        const std::string& getType() const {
            return Types::INT;
        }
    };

    /*Primary Node for String*/
    class StringNode : public Node {
    public:
        int stId;
        const std::string& val;
        StringNode(const location& loc, const std::string& s);

        void accept(Visitor&visitor, bool forward = false);

        const std::string& getType() const {
            return Types::STRING;
        }
    };

    /*Primary Node for Bool*/
    class BoolNode : public Node {
    public:
        int stId;
        const bool val;
        BoolNode(const location & loc, bool b);
        void accept(Visitor&visitor, bool forward = false);

        const std::string& getType() const {
            return Types::BOOL;
        }
    };

    /*Primary Node for This*/
    class ThisNode : public Node {
        friend class Analyser;
        friend class Intermediater;
    public:
        explicit ThisNode(const location & loc);
        void accept(Visitor&visitor, bool forward = false);

        const std::string& getType()const {
            return type;
        }
    protected:
        std::string type;
    };

    /*Primary Node for null*/
    class NullNode : public Node {
    public:
        explicit NullNode(const location & loc);
        void accept(Visitor&visitor, bool forward = false);

        const std::string& getType() const {
            return Types::_NULL;
        }
    };

    /*****************EXPRESSION NODES**************************/

    /*If statement, syntax: if(condition) expr1 else expr2 */
    class IfNode : public Node {
        friend class Analyser;
    public:
        Node * const cond;
        Node * const tBranch;
        Node * const fBranch;
        IfNode(const location & loc, Node* cond, Node* tB, Node* fB);

        void accept(Visitor&visitor, bool forward = false);

        const std::string& getType() const {
            return return_type;
        }
    protected:
        std::string return_type;
    };

    /*While statement, syntax: while (condition) expr */
    class WhileNode : public Node {
    public:
        Node * const cond;
        Node * const expr;
        const position end;
        WhileNode(const location & loc, position end, Node* e1, Node* e2);

        void accept(Visitor&visitor, bool forward = false);

        const std::string& getType() const {
            return Types::UNIT;
        }
    };

    /*Unary Minus, syntax: -expr */
    class UnaryMinusNode : public Node {
    public:
        Node * const expr;
        UnaryMinusNode(const location & loc, Node* r);

        void accept(Visitor&visitor, bool forward = false);

        const std::string& getType() const {
            return Types::INT;
        }
    };

    /*Negation, syntax: !expr */
    class NegateNode : public Node {
    public:
        Node * const expr;
        NegateNode(const location & loc, Node* r);

        void accept(Visitor&visitor, bool forward = false);

        const std::string& getType() const {
            return Types::BOOL;
        }
    };

    /***********************FORMALS********************************/

    /*declaration assign node, syntax is: id : TypeId = expr*/
    class VariableDefinitionNode : public Node {
    public:
        IdNode * const id;
        TypeNode * const type;
        Node * const expr;
        VariableDefinitionNode(const location & loc, IdNode * _id, TypeNode*, Node*_expr);

        void accept(Visitor&visitor, bool forward = false);

        const std::string& getType() const {
            return type->getType();
        }

        const std::string& getId() const {
            return id->getId();
        }
    };

    /*Assignment, syntax: id = value*/
    class AssignNode : public Node {
    public:
        ObjectNode * const id;
        Node * const expr;
        AssignNode(ObjectNode* l, Node* r, const location loc = location());
        void accept(Visitor&visitor, bool forward = false);

        const std::string& getType() const {
            return Types::UNIT;
        }

        const std::string& getId()const {
            return id->getId();
        }
    };

    //merge w/ FunctionNode?

    /*Super keyword, syntax: super.id(actuals)*/
    class SuperNode : public Node {
        friend class Analyser;
    public:
        IdNode * const id;
        ActualsNode * const actuals;
        const ClassNode * supClass;
        std::string type = Types::UNDEFINED;
        SuperNode(const location & loc, IdNode* oid, ActualsNode* ac);

        void accept(Visitor&visitor, bool forward = false);

        const std::string& getType() const {
            return type;
        }

        const std::string& getId() const {
            return id->getId();
        }

        const std::string& getClass()const {
            return supClass ? supClass->getType() : Types::UNDEFINED;
        }
    };

    /*syntax: .function(actuals)*/
    class FunctionCallNode : public Node {
        friend class Analyser;
    public:
        std::string type = Types::UNDEFINED;
        const ClassNode * pClass;
        IdNode * const id;
        ActualsNode * const actuals;
        FunctionCallNode(const location & loc, IdNode* oid, ActualsNode* ac);

        void accept(Visitor&visitor, bool forward = false);

        const std::string& getType() const {
            return type;
        }

        const std::string& getId() const {
            return id->getId();
        }

        const std::string& getClass()const {
            return pClass ? pClass->getType() : Types::UNDEFINED;
        }
    };

    /*syntax: new TypeId(actuals)*/
    class NewNode : public Node {
    public:
        TypeNode * const type;
        ActualsNode * const actuals;
        NewNode(const location & loc, TypeNode* tid, ActualsNode* ac);

        void accept(Visitor&visitor, bool forward = false);

        const std::string& getType() const {
            return type->getType();
        }
    };

    /*Enclosed expression within parantheses, syntax: (expr) */
    class ParenthesesExpressionNode : public Node {
    public:
        Node * const expr;
        ParenthesesExpressionNode(const location & loc, Node* ex);

        void accept(Visitor&visitor, bool forward = false);

        const std::string& getType() const {
            return expr->getType();
        }
    };

    /*syntax: id.Function(actuals) */
    class ObjectFunctionCallNode : public Node {
    public:
        Node * const expr; //object 
        IdNode * const id; //function name
        ActualsNode * const actuals; // (arguments)
        std::string type = Types::UNDEFINED;

        ObjectFunctionCallNode(Node* ex, IdNode* oid, ActualsNode* ac, const location loc = location());

        void accept(Visitor&visitor, bool forward = false);

        const std::string& getType() const {
            return type; //func ? func->getType() : Types::UNDEFINED;
        }

        const std::string & getId() const {
            return id->getId();
        }
    };

    /*single case for match node, syntax is: id : TypeId => block */
    class CaseNode : public Node {
    public:
        IdNode * const id;
        TypeNode * const type;
        BlockNode * const expr;
        CaseNode(const location & loc, IdNode*id, TypeNode*, BlockNode*);

        void accept(Visitor&visitor, bool forward = false);

        const std::string& getType() {
            return expr->getType();
        }

        const std::string & getId() {
            return id->getId();
        }
    };

    /*Match Node that links to the expr to be matched and a vector of cases, syntax: expr match { cases } */
    class MatchNode : public Node {
    public:
        Node * const expr;
        std::vector<CaseNode*> cases;
        std::string type;
        MatchNode(const location & loc, Node* ex, const std::vector<CaseNode*> & ca);

        void accept(Visitor&visitor, bool forward = false);

        const std::string& getType() const {
            return type;
        }
    };

    /*Abstract Base Node for Binary Expressions*/
    class BinNode : public Node {
    public:
        Node * const left;
        Node * const right;
        BinNode(const location & loc, Node* l, Node* r);

        virtual void accept(Visitor& visitor) {
        };
    };

    /*  binary operator <= */
    class LtEqNode : public BinNode {
    public:
        LtEqNode(const location & loc, Node* l, Node* r);
        void accept(Visitor&visitor, bool forward = false);

        const std::string& getType() const {
            return Types::BOOL;
        }
    };

    /* binary operator < */
    class LtNode : public BinNode {
    public:
        LtNode(const location & loc, Node* l, Node* r);
        void accept(Visitor&visitor, bool forward = false);

        const std::string& getType() const {
            return Types::BOOL;
        }
    };

    /*binary operator * */
    class MultiplyNode : public BinNode {
    public:
        MultiplyNode(const location & loc, Node* l, Node* r);
        void accept(Visitor&visitor, bool forward = false);

        const std::string& getType() const {
            return Types::INT;
        }
    };

    /*binary operator / */
    class DivideNode : public BinNode {
    public:
        DivideNode(const location & loc, Node* l, Node* r);
        void accept(Visitor&visitor, bool forward = false);

        const std::string& getType() const {
            return Types::INT;
        }
    };

    /*binary operator + */
    class AddNode : public BinNode {
    public:
        AddNode(const location & loc, Node* l, Node* r);
        void accept(Visitor&visitor, bool forward = false);

        const std::string& getType() const {
            return Types::INT;
        }
    };

    /*binary operator - */
    class SubtractNode : public BinNode {
    public:
        SubtractNode(const location & loc, Node* l, Node* r);
        void accept(Visitor&visitor, bool forward = false);

        const std::string& getType() const {
            return Types::INT;
        }
    };

}
#endif /* NODES_H */

