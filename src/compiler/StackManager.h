/* 
 * File:   StackManager.h
 * Author: len
 *
 * Created on December 4, 2015, 12:31 PM
 */

#ifndef STACKMANAGER_H
#define	STACKMANAGER_H

#include <map>
#include <stack>

namespace Cool {

    class AsmContainer;
    
    class StackManager {
    public:
        StackManager(AsmContainer & asmcont);
        size_t getScopeCount() const;
        unsigned getWordOffset(const std::string& id) const;
        unsigned getByteOffset(const std::string& id) const;
        void incr(unsigned c);
        void decr(unsigned c);
        void enterScope();
        void exitScope();

        void pushVar(const std::string& reg, const std::string& refId, const std::string& com = "");
        void regVar(const std::string& refId);

        void store(const std::string& reg, unsigned offset, const std::string& com = "") const;
        void storeInVar(const std::string& src, const std::string& refId,const std::string& com = "store at pos of var")const;

        void push(const std::string& reg, const std::string& comment = "");
        void load(const std::string& dest, const std::string& refId, const std::string& com = "load value at pos in stack ") const;
        void pop(const std::string&dest, const std::string& comment = "pop value and store in dest");
        void pop();
        void pop(unsigned c);
        void add(unsigned c, const std::string& com = "increase stack");

        size_t getCount() const;

    private:

        AsmContainer* asmc;

        size_t count = 0;

        class Environment {
        public:
            Environment(Environment* env);
            unsigned get(const std::string& id) const;
            void set(const std::string& id, unsigned offset);
            size_t getCount() const;
            Environment* prev;
            std::map<std::string, unsigned>offsets;
        };
        std::stack<Environment>envs;
    };
}
#endif	/* STACKMANAGER_H */

