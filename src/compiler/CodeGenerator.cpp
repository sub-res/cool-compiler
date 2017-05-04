/* 
 * File:   CodeGenerator.cpp
 * Author: wessel
 * 
 * Created on November 18, 2015, 12:27 PM
 */
#include "CodeGenerator.h"
#include <iostream>
#include "FileIO.h"
#include "Config.h"
#include "Nodes.h"
#include "Types.h"
#include "DispTab.h"
#include "stack.hh"
#include "Optimizer.h"
#include "SymbolTable.h"
#include "Environment.h"
#include "MemManager.h"
#include "GraphBuilder.h"
#include <cctype>
#include <type_traits>
#include "ErrorType.h"

namespace Cool {

    CodeGenerator::CodeGenerator(Environment& _env) : env(_env), usageChecker(_env) , symTable([this](){
        if(env.config.library){ return SymbolTable(env,&usageChecker);}else{ return SymbolTable(env);}}()) {
    }

    void CodeGenerator::start() {
        if (env.config.inDebug) {
            std::cout << "Writing to file \'" << env.config.outfile << "\'." << std::endl;
        }

        const bool writeLib = env.config.library;


        if (!writeLib) { //only optimize if not library
            usageChecker.start();
          //  symTable = std::make_unique<SymbolTable>(env, &usageChecker);
        } else {
         //   symTable = std::make_unique<SymbolTable>(env);
        }
        //write globals, garbage collector dependencies
        env.asmc.add("#generated assembly output:\n\t.data\n\t.align\t2\n\t.globl\tclass_nameTab\n\t.globl\tMain_protObj\n\t.globl\tInt_protObj\n\t.globl\tString_protObj\n\t.globl\tBoolean_protObj\n\t.globl\tboolean_lit0\n\t.globl\tboolean_lit1\n\t.globl\tSymbol_protObj\n\t.globl\theap_start\n"

                "\t.globl\t_MemMgr_INITIALIZER\n_MemMgr_INITIALIZER:\n\t.word\t" + std::string(env.config.gc ? "_GenGC_Init" : "_NoGC_Init") +
                "\n\t.globl\t_MemMgr_COLLECTOR\n_MemMgr_COLLECTOR:\n\t.word\t" + std::string(env.config.gc ? "_GenGC_Collect" : "_NoGC_Collect") +
                "\n\t.globl\t_MemMgr_TEST\n_MemMgr_TEST:\n\t.word\t" + std::string(env.config.gcTest ? "1" : "0"));
        //end

        symTable.build();

        //write constants
        for (const auto & entry : symTable.getStrConsts()) {
            env.asmc.add(entry.second.getAsm());
        }
        for (const auto & entry : symTable.getIntConsts()) {
            env.asmc.add(entry.second.getAsm());
        }
        for (const auto & entry : symTable.getBoolConsts()) {
            env.asmc.add(entry.second.getAsm());
        }

        //print class_nameTab
        std::string clT = "class_nameTab:\n";
        unsigned i = 0;
        for (const auto & entry : symTable.getCNameTabs()) {
            const Assembly::StrConst& strConst = symTable.getStrConst(entry.second);
            clT += "\t.word\t" + strConst.label + "\t# " + std::to_string(i++) + " " + strConst.value + "\n";
        }
        env.asmc.add(clT);

        //write dispatch tables
        for (const auto & entry : symTable.getDispTabs()) {
            env.asmc.add(entry.second.getAsm());
        }

        //write object prototypes
        for (const auto & entry : env.getBasicClasses()) {
            //  if (writeLib || usageChecker.typeUsed(itt->first)) {
            writeProtoObj(entry.second);
        }

        for (const auto & entry : env.getUserClasses()) {
            if (writeLib || usageChecker.typeUsed(entry.first)) {
                writeProtoObj(entry.second);
            } else {
                env.errors.registerError(entry.second->loc, "class '" + entry.first + "' is never used", ErrorType::WARNING);
            }
        }

        //heap start and beginning of text segment
        env.asmc.add("heap_start:");
        env.asmc.add("\t.word\t0");
        env.asmc.add("\t.text");
        env.asmc.setTextSegStart(); //mark beginning of text segment
        env.asmc.add("\t.globl\tMain.Main");

        //start writing assembly code for functions
        AsmWriter asmw(env, env.asmc);
        asmw.start(symTable, writeLib ? nullptr : &usageChecker);

        optimizer.optimize(env.asmc);

        env.asmc.print(env.config.outfile, env.config.inDebug);
    }

    void CodeGenerator::writeProtoObj(const ClassNode* node) {
        const std::string & pobjlabel = node->getType() + "_protObj:";
        const ClassNode* temp = node;


        const std::string defStrL = symTable.getStrConst(symTable.getDefStrConst()).label;
        const std::string defIntL = symTable.getIntConst(symTable.getDefIntConst()).label;
        const std::string defBoolL = symTable.getBoolConst(symTable.getDefBoolConst()).label;

        std::vector<std::string> pobj;
        while (true) {
            std::vector<std::string> attr_temp;

            for (auto var : node->body->feature_vars) {
                const std::string id = var->getId();
                const std::string type = var->getType();
                if (type == Types::INT) {
                    attr_temp.push_back("\t.word\t" + defIntL + "\t#" + id);
                } else if (type == Types::STRING) {
                    attr_temp.push_back("\t.word\t" + defStrL + "\t#" + id);
                } else if (type == Types::BOOL) {
                    attr_temp.push_back("\t.word\t" + defBoolL + "\t\t#" + id);
                } else {
                    attr_temp.push_back("\t.word\t0\t\t#" + id);
                }
            }
            if (!attr_temp.empty()) {
                //attr_temp.insert(attr_temp.begin(), "\t#attributes:");
                pobj.insert(pobj.begin(), attr_temp.begin(), attr_temp.end());
            }
            if (node->extend->getType() == Types::NATIVE) {
                break;
            }
            node = env.getClass(node->extend->getType());
        }

        const decltype(pobj.size()) objSize = 3 + pobj.size();

        pobj.insert(pobj.begin(), "\t.word\t" + temp->getType() + "_dispTab"); //dispatch table
        pobj.insert(pobj.begin(), "\t.word\t" + std::to_string(objSize)); //placeholder, gotta calc size
        pobj.insert(pobj.begin(), "\t.word\t" + std::to_string(temp->stId)); //obj identifier
        pobj.insert(pobj.begin(), pobjlabel); //final label
        pobj.insert(pobj.begin(), "\t.word\t-1\n");

        for (decltype(pobj.size()) i = 0; i < pobj.size(); ++i) {
            env.asmc.add(pobj.at(i));
        }
    }


    //AsmWriter

    CodeGenerator::AsmWriter::AsmWriter(const Environment & env, AsmContainer & asmcont) : config(&env.config), asmc(&asmcont), mm(env, asmcont) {

    }

    CodeGenerator::AsmWriter::~AsmWriter() {
    }

    void CodeGenerator::AsmWriter::start(const SymbolTable & sym, const UsageChecker * const usageChecker) {

        symTable = &sym;

        asmc->add("create_int:", "Create new Int object for value in $a0");
        asmc->add("\taddiu\t$sp\t$sp\t-4");
        asmc->add("\tsw\t$ra\t4($sp)", "save return address");
        asmc->add("\tla\t$t0\tInt_protObj", "new object");
        asmc->add("\tsw\t$a0\t12($t0)", "assign the result to the new object");
        asmc->add("\tmove\t$a0\t$t0");
        asmc->add("\tjal\tAny.clone", "create copy of object");
        asmc->add("\tlw\t$ra\t4($sp)");
        asmc->add("\taddiu\t$sp\t$sp\t4");
        asmc->add("\tjr\t$ra");


        asmc->add("lteq_helper:", "less-than/equals\" helper");
        asmc->add("\taddiu\t$sp\t$sp\t-4");
        asmc->add("\tsw\t$ra\t4($sp)");

        asmc->add("\tlw\t$a0\t12($a0)");
        asmc->add("\tlw\t$a1\t12($a1)");
        asmc->add("\tsub\t$a1\t$a1\t$a0");
        asmc->add("\tblez\t$a1\t\tcreate_bool1", "if less-than/equal");
        asmc->add("\tj\tcreate_bool0", "if not equal");

        asmc->add("lt_helper:", "\"less-than\" helper");
        asmc->add("\taddiu\t$sp\t$sp\t-4");
        asmc->add("\tsw\t$ra\t4($sp)");

        asmc->add("\tlw\t$a0\t12($a0)");
        asmc->add("\tlw\t$a1\t12($a1)");
        asmc->add("\tsub\t$a1\t$a1\t$a0");
        asmc->add("\tbltz\t$a1\t\tcreate_bool1", "if less-than/equal");
        asmc->add("\tj\tcreate_bool0", "if not equal");

        //helper functions for creating boolean objects
        asmc->add("create_bool0:", "(creating boolean false)");
        asmc->add("\tla\t$a0\tboolean_lit0");
        asmc->add("\tjal\tAny.clone");
        asmc->add("\tj\tbool_helper_end");

        asmc->add("create_bool1:", "(creating boolean true)");
        asmc->add("\tla\t$a0\tboolean_lit1");
        asmc->add("\tjal\tAny.clone");
        asmc->add("\tj\tbool_helper_end");

        asmc->add("bool_helper_end:");
        asmc->add("\tlw\t$ra\t4($sp)");
        asmc->add("\taddiu\t$sp\t$sp\t4");
        asmc->add("\tjr\t$ra");

        asmc->add("negate:");
        asmc->add("\tlw\t$a0\t12($a0)");
        asmc->add("\tbeq\t$a0\t$zero\tnegTrue");
        asmc->add("\tla\t$a0\tboolean_lit0");
        asmc->add("\tjr\t$ra");
        asmc->add("negTrue:");
        asmc->add("\tla\t$a0\tboolean_lit1");
        asmc->add("\tjr\t$ra");

        for (const auto & entry : sym.env.getUserClasses()) {
            if (usageChecker && !usageChecker->typeUsed(entry.first)) {
                continue;
            }
            currclass = entry.first;
            for (auto func : entry.second->body->feature_funcs) {
                if (usageChecker && !usageChecker->functionUsed(*func)) {
                    continue;
                }
                func->accept(*this);
            }
        }

        for (const auto & entry : sym.env.getBasicClasses()) {
            currclass = entry.second->getType();
            std::vector<FunctionNode*> funcs = entry.second->body->feature_funcs;
            for (auto func : entry.second->body->feature_funcs) {
                if (func->expr->getType() != Types::NATIVE) {
                    func->accept(*this);
                }
            }
        }
    }

    void CodeGenerator::AsmWriter::writeArithmetic(const BinNode*node, const int o) {
        node->left->accept(*this);
        asmc->add("\tlw\t$a0\t12($a0)", "get Int value from object");
        mm.sm.push("$a0", "save left operand of arithmetic op. on stack "); //increase stack and store $a0 on top

        node->right->accept(*this);
        asmc->add("\tlw\t$a0\t12($a0)", "get Int value from object");

        mm.sm.pop("$t0", "store first Int in $t0");

        switch (o) {
            case 0: //addition
                asmc->add("\tadd\t$t0\t$t0\t$a0", "add $a0 to $t0 and store result in $t0");
                break;
            case 1: //subtraction
                asmc->add("\tsub\t$t0\t$t0\t$a0", "subtract $t0 from $a0 and store result in $t0");
                break;
            case 2: //multiplication
                asmc->add("\tmul\t$t0\t$t0\t$a0", "multiply $t0 by $a0 and store result in $t0");
                break;
            case 3: //division
                asmc->add("\tli\t$t1\t" + std::to_string(node->loc.begin.line), "linenumber");
                asmc->add("\tla\t$a1\t" + symTable->getFileConst(*node->loc.begin.filename).label, "filename");
                asmc->add("\tbeq\t$a0\t$zero\t_divide_abort");
                asmc->add("\tdiv\t$t0\t$t0\t$a0", "divide $t0 by $a0 and store result in $t0");
                break;
        };

        asmc->add("", "Create new Int object for result:");
        asmc->add("\tla\t$a0\tInt_protObj", "new object");
        asmc->add("\tsw\t$t0\t12($a0)", "assign the result to the new object");
        asmc->add("\tjal\tAny.clone", "create copy of object");
    }

    void CodeGenerator::AsmWriter::visit(FunctionCallNode*node) {

        const std::string & fname = node->getId();
        const auto argCount = node->actuals->getCount();
        if (argCount > 0) {
            mm.sm.add(argCount, "incr stack size for arguments for function " + fname + ". Called from \' " + currclass + "\'");
            for (auto i = decltype(argCount){0}; i < argCount; ++i) {
                Node * arg = node->actuals->expressions[i];
                arg->accept(*this);
                ifIntWrite(arg);
                mm.sm.store("$a0", (argCount - i)*4, "push arg " + std::to_string(i) + " for function " + fname + " on the stack");
            }
        }
        asmc->add("\tmove\t$a0\t$s0", "store self in $a0 (some funcs require it)");
        asmc->add("\tlw\t$t1\t8($s0)", "load label of disptab (class " + currclass + ")");
        asmc->add("\tlw\t$t1\t" + std::to_string(symTable->getDispByteOffset(currclass, fname)) + "($t1)", "load label of function " + fname);
        asmc->add("\tjalr\t$t1", "call function " + fname);
        mm.sm.decr(argCount);
    }

    void CodeGenerator::AsmWriter::visit(ObjectFunctionCallNode*node) {
        mm.sm.push("$s0", "store \'this\' (" + currclass + ")"); //store "this"

        node->expr->accept(*this); //load variable in a0 
        ifIntWrite(node->expr);
        const std::string & fname = node->getId();

        //asmc->add("", "Dispatch to func \'" + fname + "\' on $a0");

        const auto argCount = node->actuals->getCount();
        if (argCount > 0) {
            mm.sm.add(argCount + 1, "incr stack for " + std::to_string(argCount) + " args for func call \'" + fname + "\'");
            mm.sm.store("$a0", 4, "store object for dispatch on stack (arguments beneath)");
            for (auto i = decltype(argCount){0}; i < argCount; ++i) {
                Node*arg = node->actuals->expressions[i];
                arg->accept(*this);
                ifIntWrite(arg);
                mm.sm.store("$a0", (argCount - i + 1)*4, "push arg on the stack");
            }
            mm.sm.pop("$a0", "get object for dispatch");
        }

        //null check (if expr is ThisNode this is unnecessary but keep it for now...)
        asmc->add("", "Check if null dispatch");
        asmc->add("\tli\t$t1\t" + std::to_string(node->expr->loc.begin.line), "linenumber");
        asmc->add("\tla\t$a1\t" + symTable->getFileConst(*node->loc.begin.filename).label, "\t#filename");
        asmc->add("\tbeq\t$a0\t$zero\t_dispatch_abort", "abort if null");

        asmc->add("\tmove\t$s0\t$a0", "overwrite \'this\' pointer with callee");
        asmc->add("\tlw\t$t1\t8($a0)", "get label of disptab");
        asmc->add("\tlw\t$t1\t" + std::to_string(symTable->getDispByteOffset(node->expr->getType(), fname)) + "($t1)");
        asmc->add("\tjalr\t$t1", "call function " + fname);
        mm.sm.decr(argCount);
        mm.sm.pop("$s0"); //restore 'this' pointer
    }

    void CodeGenerator::AsmWriter::visit(FunctionNode* node) {
        mm.enterScope();
        asmc->add(currclass + "." + node->getId() + ":");
        const auto fCount = node->formals->formals.size();
        for (auto i = decltype(fCount){0}; i < fCount; ++i) {
            mm.sm.regVar(node->formals->formals[i]->getId());
        }
        mm.sm.push("$ra", "save return addres on top of arguments"); //store return address, 


        asmc->add("", "function implementation:");
        node->expr->accept(*this);

        //expr result is expected in a0
        asmc->add("", "cleanup");

        mm.sm.pop("$ra"); //retrieve return address
        mm.sm.pop(node->formals->formals.size()); //clean args from stack
        mm.exitScope();
        asmc->add("\tjr\t$ra", "return from " + node->getId());
    }

    void CodeGenerator::AsmWriter::visit(BlockNode* node) {
        mm.enterScope();
        for (auto expr : node->nodes) {
            expr->accept(*this);
        }
        mm.sm.pop(mm.sm.getScopeCount());
        mm.exitScope();
    }

    void CodeGenerator::AsmWriter::visit(ObjectNode*node) {
        const std::string & id = node->getId();
        const auto attrOffset = symTable->getAttrByteOffset(currclass, id);
        if (attrOffset >= 0) { //class attribute
            asmc->add("\tlw\t$a0\t" + std::to_string(attrOffset) + "($s0)", "load value (at pos in object) (id: " + id + ")");
        } else {
            //mm.sm.load("$a0", id, "#load value (at pos in stack) (id: " + id + ")");
            mm.load("$a0", id, "load value (id: " + id + ")");
        }
        if (node->getType() == Types::INT) {
            //asmc->add("\tlw\t$a0\t12($a0)");
        }
    }

    void CodeGenerator::AsmWriter::visit(IdNode* node) {
        std::cout << "TEST: " << node->getId() << " ,type: " << node->getType() << std::endl;
        const std::string& id = node->getId();
        const auto attrOffset = symTable->getAttrByteOffset(currclass, id);
        if (attrOffset >= 0) { //class attribute
            asmc->add("\tlw\t$a0\t" + std::to_string(attrOffset) + "($s0)", "load value (at pos in object) (id: " + id + ")");
        } else {
            //mm.sm.load("$a0", id, "#load value (at pos in stack) (id: " + id + ")");
            mm.load("$a0", id, "load value (id: " + id + ")");
        }
        if (node->getType() == Types::INT) {
            //asmc->add("\tlw\t$a0\t12($a0)");
        }
    }

    void CodeGenerator::AsmWriter::visit(BoolNode* node) {

        asmc->add("\tla\t$a0\t" + std::string(node->val ? "boolean_lit1" : "boolean_lit0"), "load address of bool const");
        asmc->add("\tjal\tAny.clone");
    }

    void CodeGenerator::AsmWriter::visit(IntNode* node) {
        asmc->add("\tla\t$a0\t" + symTable->getIntConst(node->stId).label, "load address of int const");
        //asmc->add("\tlw\t$a0\t12($a0)"); //value
        asmc->add("\tjal\tAny.clone");
    }

    void CodeGenerator::AsmWriter::visit(StringNode*node) {
        asmc->add("\tla\t$a0\t" + symTable->getStrConst(node->stId).label, "load address of string const");
        asmc->add("\tjal\tAny.clone");
    }

    void CodeGenerator::AsmWriter::visit(AssignNode* node) {
        const std::string & id = node->getId();
        node->expr->accept(*this);

        ifIntWrite(node->expr);

        const auto attrOffset = symTable->getAttrByteOffset(currclass, id);
        asmc->add("", "Assignment of " + id);

        if (attrOffset >= 0) { //class attribute
            const std::string strOff = std::to_string(attrOffset);
            asmc->add("\tsw\t$a0\t" + strOff + "($s0)", "Assign to class attribute \'" + id + "\'");
            if (config->gc) {
                asmc->add("\taddiu\t$a1\t$s0\t" + strOff, "set pointer to changed loc for gc");
                asmc->add("\tjal\t_GenGC_Assign", "notify gc on assignment");
            }
        } else {
            //mm.sm.storeInVar("$a0", id, "#Assign to local var \'" + id + "\'");
            mm.store("$a0", id, "Assign to local var \'" + id + "\'");
        }
        asmc->add("\tmove\t$a0\t$zero", "Assignment returns unit (null)");
    }

    void CodeGenerator::AsmWriter::visit(VariableDefinitionNode*node) {
        node->expr->accept(*this);
        ifIntWrite(node->expr);
        //mm.sm.pushVar("$a0", node->getId(), "#Store new local var \'" + node->getId() + "\' on stack");
        mm.pushVar("$a0", *node, "Store new local var \'" + node->getId() + "\' on stack");
    }

    void CodeGenerator::AsmWriter::visit(UnaryMinusNode* node) {
        node->expr->accept(*this);
        asmc->add("\tlw\t$t0\t12($a0)");
        asmc->add("\tsub\t$t0\t$zero\t$t0");
        //asmc->add("\tsub\t$a0\t$zero\t$a0");
        asmc->add("\tsw\t$t0\t12($a0)");
    }

    void CodeGenerator::AsmWriter::visit(NegateNode*node) {
        node->expr->accept(*this);
        //writeLine("\tlw\t$t0\t12($a0)");
        //writeLine("\tli\t$t1\t-1");

        // writeLine("\txori\t$t0\t$t0\t1");

        asmc->add("\tjal\tnegate");
    }

    void CodeGenerator::AsmWriter::visit(WhileNode*node) {
        const auto ctr_local = whilectr++;
        asmc->add("_while" + std::to_string(ctr_local) + ":");
        node->cond->accept(*this);

        asmc->add("\tlw\t$a0\t12($a0)", "get bool value for cond");
        asmc->add("\tbeq\t$a0\t$zero\t_endwhile" + std::to_string(ctr_local), "jump to end if cond = false");
        node->expr->accept(*this);

        asmc->add("\tj\t_while" + std::to_string(ctr_local));
        asmc->add("\n_endwhile" + std::to_string(ctr_local) + ":");
        asmc->add("\tmove\t$a0\t$zero", "While returns unit (null)");
    }

    void CodeGenerator::AsmWriter::visit(IfNode*node) {
        const auto ctr_local = ifctr++;
        node->cond->accept(*this);
        asmc->add("\tlw\t$a0\t12($a0)", "get boolean value");
        asmc->add("\tbeq\t$a0\t$zero\t_else" + std::to_string(ctr_local), "go to else if condition = false");
        node->tBranch->accept(*this);
        asmc->add("\tj\t_endif" + std::to_string(ctr_local));
        asmc->add("_else" + std::to_string(ctr_local) + ":");
        node->fBranch->accept(*this);
        asmc->add("_endif" + std::to_string(ctr_local) + ":");
    }

    void CodeGenerator::AsmWriter::visit(NewNode*node) {
        mm.sm.push("$s0", "store \'this\'t (" + currclass + ")"); //store "this"

        const std::string type = node->getType();
        const auto argCount = node->actuals->getCount();
        if (argCount > 0) {
            mm.sm.add(argCount, "incr stack for " + std::to_string(argCount) + "actuals for constructor of \'" + type + "\'");
            for (auto i = decltype(argCount){0}; i < argCount; ++i) {
                Node*arg = node->actuals->expressions[i];
                arg->accept(*this);
                ifIntWrite(arg);
                mm.sm.store("$a0", (argCount - i) * 4, "push arg on stack");
            }

        }
        asmc->add("\tla\t$a0\t" + node->getType() + "_protObj");
        asmc->add("\tjal\tAny.clone");
        asmc->add("\tmove\t$s0\t$a0", "overwrite \'this\' pointer");
        asmc->add("\tlw\t$t1\t8($s0)", "get label of disptab for class \'" + type + "\'");
        asmc->add("\tlw\t$t1\t" + std::to_string(symTable->getDispByteOffset(type, type)) + "($t1)", "get label of func \'" + type + "\'");
        asmc->add("\tjalr\t$t1", "call constructor for " + type);
        mm.sm.decr(argCount);

        mm.sm.pop("$s0");
    }

    void CodeGenerator::AsmWriter::visit(SuperNode*node) {
        const std::string& fname = node->getId();
        const std::string& classType = node->getClass();
        const auto argCount = node->actuals->getCount();
        if (argCount > 0) {
            mm.sm.add(argCount, "incr stack for " + std::to_string(argCount) + " actuals for supercall to \'" + fname + "\' in " + classType);
            for (auto i = decltype(argCount){0}; i < argCount; ++i) {
                Node*arg = node->actuals->expressions[i];
                arg->accept(*this);
                ifIntWrite(arg);
                mm.sm.store("$a0", (argCount - i)*4, "push arg on stack)");
            }
        }

        asmc->add("\tla\t$t1\t" + classType + "_dispTab", "get label of disptab of \'" + classType + "\'");
        asmc->add("\tlw\t$t1\t" + std::to_string(symTable->getDispByteOffset(classType, fname)) + "($t1)", "get label of func \'" + fname + "\'");
        asmc->add("\tjalr\t$t1", "call function " + fname + "");
        mm.sm.decr(argCount);
    }

    void CodeGenerator::AsmWriter::visit(MatchNode*node) {
        const auto ctr_local = matchctr++;

        const std::string & matchId = std::to_string(ctr_local);
        const std::string & matchLabel = "_match" + matchId;
        const std::string & caseNull = matchLabel + "_caseNull";
        const std::string & caseAny = matchLabel + "_caseAny";
        const std::string & endNull = "_endmatchnull" + matchId;
        const std::string & end = "_endmatch" + matchId;
        const std::string & caseAbort = matchLabel + "_abort";



        asmc->add(matchLabel + ":");
        node->expr->accept(*this);
        ifIntWrite(node->expr);


        bool hasNull = false;
        for (const auto c : node->cases) {
            if (c->type->getType() == Types::_NULL) {
                hasNull = true;
                break;
            }
        }



        asmc->add("\tbeq\t$a0\t$zero\t" + caseNull);
        //store expression on stack but let individual cases register it at stackmanager
        asmc->add("\taddiu\t$sp\t$sp\t-4");
        asmc->add("\tsw\t$a0\t4($sp)");
        asmc->add("\tlw\t$t0\t0($a0)", "get type ID");


        for (const auto nCase : node->cases) {
            const std::string currcase = nCase->type->getType();
            if (currcase == Types::ANY) {
                asmc->add("\tj\t" + caseAny);
            } else {
                if (!(currcase == Types::_NULL && hasNull)) {
                    asmc->add("", "matches for case " + currcase);

                    std::vector<std::string> types;
                    for (const auto & entry : symTable->env.getAllClasses()) {
                        types.push_back(entry.first);
                    }

                    for (const auto & type : types) {
                        if (symTable->env.ltEqInherits(type, currcase)) {
                            asmc->add("\tbeq\t$t0\t" + std::to_string(symTable->getCNameIndex(type)) + "\t" + matchLabel + "_case" + currcase, "matching with type " + type);
                        }
                    }
                }
            }
        }


        asmc->add(caseAbort + ":");
        asmc->add("\tli\t$t1\t" + std::to_string(node->expr->loc.begin.line), "linenumber");
        asmc->add("\tla\t$a1\t" + symTable->getFileConst(*node->loc.begin.filename).label, "filename");
        asmc->add("\tj\t_case_abort", "abort if no case matches");


        for (auto cNode : node->cases) {
            const std::string caseType = cNode->type->getType();
            asmc->add(matchLabel + "_case" + caseType + ":");
            cNode->accept(*this);

            if (caseType == Types::_NULL) {
                asmc->add("\tj\t" + endNull);
            } else {
                asmc->add("\tj\t" + end);
            }
        }

        if (!hasNull) {
            asmc->add(caseNull + ":");
            asmc->add("\tj\t" + caseAbort);
        }

        asmc->add(end + ":");
        asmc->add("\taddiu\t$sp\t$sp\t4"); //pop expr from stack
        asmc->add(endNull + ":");
    }

    /*Note for CaseNode: 
     * expression to be matched is already pushed on the stack but not registered yet, 
     * since the name could vary between the cases 
     */
    void CodeGenerator::AsmWriter::visit(CaseNode*node) {
        mm.enterScope();
        if (node->type->getType() != Types::_NULL) { //no need to register if null, since null isn't an id
            mm.sm.regVar(node->getId());
            node->expr->accept(*this);
            mm.sm.decr(1); //unregister the expression for next case
        } else {
            node->expr->accept(*this);
        }
        mm.exitScope();
    }

    void CodeGenerator::AsmWriter::visit(ParenthesesExpressionNode*node) {
        node->expr->accept(*this);
    }

    void CodeGenerator::AsmWriter::visit(LtEqNode*node) {
        node->left->accept(*this);
        mm.sm.push("$a0", "save left operand of <= on stack");
        node->right->accept(*this);
        mm.sm.pop("$a1", "pop and save left operand in $a1");
        mm.sm.push("$ra", "save return address");
        asmc->add("\tjal\tlteq_helper");
        mm.sm.pop("$ra", "restore return address");
    }

    void CodeGenerator::AsmWriter::visit(LtNode*node) {
        node->left->accept(*this);
        mm.sm.push("$a0", "save left operand of < on stack");
        node->right->accept(*this);
        mm.sm.pop("$a1", "pop and save left operand in $a1");
        mm.sm.push("$ra", "save return address");
        asmc->add("\tjal\tlt_helper");
        mm.sm.pop("$ra", "restore return address");
    }

    void CodeGenerator::AsmWriter::visit(MultiplyNode*node) {
        writeArithmetic(node, 2);
    }

    void CodeGenerator::AsmWriter::visit(AddNode*node) {
        writeArithmetic(node, 0);
    }

    void CodeGenerator::AsmWriter::visit(DivideNode*node) {
        writeArithmetic(node, 3);
    }

    void CodeGenerator::AsmWriter::visit(SubtractNode*node) {
        writeArithmetic(node, 1);
    }

    void CodeGenerator::AsmWriter::visit(ThisNode*node) {
        asmc->add("\tmove\t$a0\t$s0", "store \'this\' (" + node->getType() + ") in $a0 ");
    }

    void CodeGenerator::AsmWriter::visit(NullNode*node) {
        asmc->add("\tmove\t$a0\t$zero", "null");
    }

    void CodeGenerator::AsmWriter::ifIntWrite(const Node* node) {
        if (node->getType() == Types::INT) {
            //  mm.sm.push("$ra", "save return address");
            //asmc->add("\tjal\tcreate_int");
            //mm.sm.pop("$ra", "restore return address");
        }
    }
}