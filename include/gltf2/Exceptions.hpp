#pragma once

#include <string>
#include <exception>

namespace gltf2 {

class MisformattedException: public std::exception {
public:
    explicit MisformattedException(const char* key, const char* what) {
        _what = std::string("Misformated file: '") + key + "' " + what;
    }
    explicit MisformattedException(const std::string& key, const std::string& what) {
        _what = "Misformated file: '" + key + "' " + what;
    }
    virtual ~MisformattedException() throw() {}

    virtual const char* what() const throw() {
        return _what.c_str();
    }

protected:
    std::string _what;
};


class MisformattedExceptionNotNumber: public MisformattedException {
public:
    explicit MisformattedExceptionNotNumber(const char* key) : MisformattedException(key, "is not a number") {}
    explicit MisformattedExceptionNotNumber(const std::string& key) : MisformattedException(key, "is not a number") {}
    virtual ~MisformattedExceptionNotNumber() throw() {}
};

class MisformattedExceptionNotBoolean: public MisformattedException {
public:
    explicit MisformattedExceptionNotBoolean(const char* key) : MisformattedException(key, "is not a boolean") {}
    explicit MisformattedExceptionNotBoolean(const std::string& key) : MisformattedException(key, "is not a boolean") {}
    virtual ~MisformattedExceptionNotBoolean() throw() {}
};

class MisformattedExceptionNotString: public MisformattedException {
public:
    explicit MisformattedExceptionNotString(const char* key) : MisformattedException(key, "is not a string") {}
    explicit MisformattedExceptionNotString(const std::string& key) : MisformattedException(key, "is not a string") {}
    virtual ~MisformattedExceptionNotString() throw() {}
};

class MisformattedExceptionNotArray: public MisformattedException {
public:
    explicit MisformattedExceptionNotArray(const char* key) : MisformattedException(key, "is not an array") {}
    explicit MisformattedExceptionNotArray(const std::string& key) : MisformattedException(key, "is not an array") {}
    virtual ~MisformattedExceptionNotArray() throw() {}
};

class MisformattedExceptionNotGoodSizeArray: public MisformattedException {
public:
    explicit MisformattedExceptionNotGoodSizeArray(const char* key) : MisformattedException(key, "is not the good size") {}
    explicit MisformattedExceptionNotGoodSizeArray(const std::string& key) : MisformattedException(key, "is not the good size") {}
    virtual ~MisformattedExceptionNotGoodSizeArray() throw() {}
};

class MisformattedExceptionNotObject: public MisformattedException {
public:
    explicit MisformattedExceptionNotObject(const char* key) : MisformattedException(key, "is not an array") {}
    explicit MisformattedExceptionNotObject(const std::string& key) : MisformattedException(key, "is not an array") {}
    virtual ~MisformattedExceptionNotObject() throw() {}
};

class MisformattedExceptionIsRequired: public MisformattedException {
public:
    explicit MisformattedExceptionIsRequired(const char* key) : MisformattedException(key, "is required") {}
    explicit MisformattedExceptionIsRequired(const std::string& key) : MisformattedException(key, "is required") {}
    virtual ~MisformattedExceptionIsRequired() throw() {}
};

}
