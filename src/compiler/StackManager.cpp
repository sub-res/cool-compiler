/* 
 * File:   StackManager.cpp
 * Author: len
 * 
 * Created on December 4, 2015, 12:31 PM
 */

#include <map>
#include "StackManager.h"
#include "AsmContainer.h"

namespace Cool {

    StackManager::StackManager(AsmContainer & asmcont) : asmc(&asmcont) {
    }

    //should be called at least once before any other functions
    void StackManager::enterScope() {
        envs.push({&envs.top()});
    }

    void StackManager::exitScope() {
        envs.pop();
    }

    void StackManager::incr(unsigned c) {
        count += c;
    }

    void StackManager::decr(unsigned c) {
        count -= c;
    }

    void StackManager::pushVar(const std::string &reg_src, const std::string &refId, const std::string& comment) {
        push(reg_src, comment);
        envs.top().set(refId, count);
    }

    void StackManager::regVar(const std::string& refId) {
        incr(1);
        envs.top().set(refId, count);
    }

    void StackManager::store(const std::string& reg, unsigned offset, const std::string& comment) const {
        asmc->add("\tsw\t" + reg + "\t" + std::to_string(offset) + "($sp)", comment);
    }
    
    void StackManager::storeInVar(const std::string& src, const std::string& refId, const std::string& comment) const{
        store(src,getByteOffset(refId),comment);
    }

    void StackManager::load(const std::string& dest, const std::string& refId, const std::string& comment)const {
        asmc->add("\tlw\t" + dest + "\t" + std::to_string(getByteOffset(refId)) + "($sp)", comment);
    }

    void StackManager::push(const std::string& reg_src, const std::string& comment) {
        add(1);
        asmc->add("\tsw\t" + reg_src + "\t4($sp)\t\t", comment);
    }

    void StackManager::pop(const std::string& reg_dest, const std::string& comment) {
        asmc->add("\tlw\t" + reg_dest + "\t4($sp)\t\t", comment);
        pop(1);
    }

    void StackManager::pop() {
        pop(1);
    }

    void StackManager::pop(unsigned c) {
        if (c > 0) {
            asmc->add("\taddiu\t$sp\t$sp\t" + std::to_string(c * 4), "decrease stack size");
            decr(c);
        }
    }

    void StackManager::add(unsigned c, const std::string& comment) {
        if (c > 0) {
            asmc->add("\taddiu\t$sp\t$sp\t-" + std::to_string(c * 4) + "\t", comment);
            incr(c);
        }
    }

    size_t StackManager::getScopeCount() const {
        return envs.top().getCount();
    }

    size_t StackManager::getCount() const {
        return count;
    }

    unsigned StackManager::getByteOffset(const std::string& id) const {
        return getWordOffset(id) * 4;
    }

    unsigned StackManager::getWordOffset(const std::string& id) const {
        return getCount() + 1 - envs.top().get(id);
    }

    StackManager::Environment::Environment(Environment*env) {
       this->prev = env;
    }

    unsigned StackManager::Environment::get(const std::string& id) const{
        for (auto env = this; env; env = env->prev) {
            auto o = env->offsets.find(id);
            if (o != env->offsets.end()) {
                return o->second;
            }
        }
        throw std::logic_error(__FUNCTION__ + std::string("invalid request"));
    }

    size_t StackManager::Environment::getCount() const {
        return offsets.size();
    }

    void StackManager::Environment::set(const std::string& id, unsigned offset) {
        offsets[id] = offset;
    }
}