#ifndef _ObjectFactory_H
#define _ObjectFactory_H

#include "Tmx/TmxPropertySet.h"
#include "object.h"

class ObjectFactory {
public:
    static Object* create_object(const char *name,
                                 const char *type,
                                 int x, int y,
                                 int w, int h,
                                 Object::Direction dir,
                                 const Tmx::PropertySet &prop);
    static Object* create_object(const char *name,
                                 const char *type,
                                 int x, int y,
                                 int w, int h,
                                 const Tmx::PropertySet &prop);
    static Object* create_object(const char *name,
                                 const char *type,
                                 int x, int y,
                                 int w = 0, int h = 0,
                                 Object::Direction dir = Object::Right);
};

#endif

