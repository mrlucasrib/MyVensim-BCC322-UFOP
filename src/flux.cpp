//
// Created by lucas on 11/09/2020.
//

#include "flux.h"
#include <lua.hpp>

Flux::Flux() {
    this->source = NULL;
    this->target = NULL;
}

Flux::Flux(System *from, System *to) {
    this->source = from;
    this->target = to;
}

System *Flux::getSource() {
    return this->source;
}

void Flux::setTarget(System *s) {
    this->target = s;
}

System *Flux::getTarget() {
    return this->target;
}

void Flux::clearSource() {
    this->source = NULL;
}

void Flux::clearTarget() {
    this->target = NULL;
}


double Flux::execute() {
    double result = 0;
    lua_State *L = luaL_newstate();
//    Add math Library
    luaL_requiref(L, "math", luaopen_math, 1);
//    Send variable to lua script
    lua_pushnumber(L, this->getSource()->getValue());
    lua_setglobal(L, "source");
    lua_pushnumber(L, this->getTarget()->getValue());
    lua_setglobal(L, "target");
    int r = luaL_dostring(L, this->formula);

    if (r == LUA_OK) {
        lua_getglobal(L, "result");

        if (lua_isnumber(L, -1)) {
            result = (double) lua_tonumber(L, -1);
        } else {
            const char *errmsg = lua_tostring(L, -1);
        }

    } else {
        const char *errmsg = lua_tostring(L, -1);
    }

    lua_close(L);


    return result;
}

void Flux::setFormula(const char *formula) {
    this->formula = formula;
}
