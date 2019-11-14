//
// Created by wizard on 14.11.2019.
//

#include "PreparedType.h"
namespace phpconvert {
    const string &PreparedType::getId() const {
        return id;
    }

    void PreparedType::setId(const string &id) {
        PreparedType::id = id;
    }

    const string &PreparedType::getType() const {
        return type;
    }

    void PreparedType::setType(const string &type) {
        PreparedType::type = type;
    }

    const string &PreparedType::getTypeLower() const {
        return typeLower;
    }

    void PreparedType::setTypeLower(const string &typeLower) {
        PreparedType::typeLower = typeLower;
    }

    const string &PreparedType::getUsage() const {
        return usage;
    }

    void PreparedType::setUsage(const string &usage) {
        PreparedType::usage = usage;
    }

    const string &PreparedType::getAlias() const {
        return alias;
    }

    void PreparedType::setAlias(const string &alias) {
        PreparedType::alias = alias;
    }

    const string &PreparedType::getRaw() const {
        return raw;
    }

    void PreparedType::setRaw(const string &raw) {
        PreparedType::raw = raw;
    }

    const string &PreparedType::PreparedType::getReplace() const {
        return replace;
    }

    void PreparedType::setReplace(const string &replace) {
        PreparedType::replace = replace;
    }

    bool PreparedType::isMain1() const {
        return isMain;
    }

    void PreparedType::setIsMain(bool isMain) {
        PreparedType::isMain = isMain;
    }

    const string &PreparedType::getExtends() const {
        return extends;
    }

    void PreparedType::setExtends(const string &extends) {
        PreparedType::extends = extends;
    }

    const vector<string> &PreparedType::getImplements() const {
        return implements;
    }

    void PreparedType::setImplements(const vector<string> &implements) {
        PreparedType::implements = implements;
    }

}