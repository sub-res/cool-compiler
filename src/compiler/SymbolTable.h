/* 
 * File:   SymbolTable.h
 * Author: len
 *
 * Created on November 20, 2015, 6:04 PM
 */

#ifndef SYMBOLTABLE_H
#define	SYMBOLTABLE_H

#include <map>
#include <vector>
#include "Visitor.h"
#include "DispTab.h"
#include "IntConst.h"
#include "BoolConst.h"
#include "StrConst.h"

namespace Cool {

    class UsageChecker;
    class Environment;
    
    class SymbolTable {
    public:
        
        SymbolTable(const Environment & environment, const UsageChecker *  const usageChecker = nullptr);
                
        int getDefStrConst() const;
        int getDefIntConst() const;
        int getDefBoolConst() const;

        const std::map<std::string, DispTab>& getDispTabs()const;
        const std::map<int, Assembly::StrConst> & getStrConsts()const;
        const std::map<int, Assembly::IntConst>& getIntConsts()const;
        const std::map<int, Assembly::BoolConst>& getBoolConsts()const;

        const std::map<int, int> & getCNameTabs()const;
        const Environment& env;

        //attribute table
        int getAttrWordOffset(const std::string & cname, const std::string & attr) const;
        int getAttrByteOffset(const std::string & cname, const std::string & attr) const;

        const Assembly::BoolConst& getBoolConst(int i) const;
        const Assembly::IntConst& getIntConst(int i) const;
        const Assembly::StrConst& getStrConst(int i) const;

        const DispTab& getDispTab(const std::string & name)const;
        int getDispWordOffset(const std::string & cname, const std::string & fname) const;
        int getDispByteOffset(const std::string & cname, const std::string & fname) const;

        int getCNameIndex(const std::string & cname)const;

        int getConstStrId(const std::string & value)const;
        int getConstBoolId(const bool value)const;
        int getConstIntId(const int value)const;
        const Assembly::StrConst& getFileConst(const std::string & fname)const;
        
        void build();
    private:
        const UsageChecker * const usageChecker;

        bool dispTabExists(const std::string & cname) const;

        void addFileConst(const std::string & fname);
        //insert class name in class name tab
        int insertCName(const std::string & cname);
        //DispTab
        void createDispTab(const ClassNode*);
        //creates a new bool constant and returns the key
        int createBoolConst(const bool v);
        //creates a new string constant and returns the key
        int createStrConst(const std::string v);
        //creates a new int constant and returns the key
        int createIntConst(const int v);

        int defInt, defStr, defBool;

        std::map<std::string, DispTab>dispTabs;
        std::map<int, Assembly::IntConst>intConsts;
        std::map<int, Assembly::StrConst>strConsts;
        std::map<int, Assembly::BoolConst>boolConsts;
        std::map<std::string, int>filenames;

        std::vector<std::string> classNameTab;
        std::map<int, int>nameLinks;

        const static std::string strLabel;
        const static std::string intLabel;
        const static std::string boolLabel;

        unsigned intC  = 0, strC = 0, boolC = 0, c = 0;

        void destroy();

        std::string getDispTabLabel(const std::string& cname) const {
            return cname + "_dispTab";
        }

        class SymbolLinker : public Visitor {
        public:
            SymbolLinker(SymbolTable& table);
            void visit(ClassNode*node);
            void visit(StringNode*node);
            void visit(IntNode*node);
            void visit(BoolNode*node);
            void link(const std::map<std::string, ClassNode*>&classes);
        private:
            SymbolTable& symTable;
        };
    };
}
#endif	/* SYMBOLTABLE_H */

