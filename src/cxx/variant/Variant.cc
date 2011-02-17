/*
** Copyright (C) 2010 Fargier Sylvain <fargier.sylvain@free.fr>
**
** This software is provided 'as-is', without any express or implied
** warranty.  In no event will the authors be held liable for any damages
** arising from the use of this software.
**
** Permission is granted to anyone to use this software for any purpose,
** including commercial applications, and to alter it and redistribute it
** freely, subject to the following restrictions:
**
** 1. The origin of this software must not be misrepresented; you must not
**    claim that you wrote the original software. If you use this software
**    in a product, an acknowledgment in the product documentation would be
**    appreciated but is not required.
** 2. Altered source versions must be plainly marked as such, and must not be
**    misrepresented as being the original software.
** 3. This notice may not be removed or altered from any source distribution.
**
** Variant.cc
**
**        Created on: Feb 14, 2011
**            Author: fargie_s
**
*/

#include <assert.h>
#include <stdlib.h>
#include <iostream>

#include "Variant.hh"

Variant::Variant()
    : m_type(VARIANT_NONE),
      m_data(NULL)
{
}

Variant::Variant(const bool i)
    : m_type(VARIANT_BOOL),
      m_data(new bool(i))
{ }

Variant::Variant(const int16_t i)
    : m_type(VARIANT_INT16),
      m_data(new int16_t(i))
{ }

Variant::Variant(const uint16_t i)
    : m_type(VARIANT_UINT16),
      m_data(new uint16_t(i))
{ }

Variant::Variant(const int32_t i)
    : m_type(VARIANT_INT32),
      m_data(new int32_t(i))
{ }

Variant::Variant(const uint32_t i)
    : m_type(VARIANT_UINT32),
      m_data(new uint32_t(i))
{ }

Variant::Variant(const uint64_t i)
    : m_type(VARIANT_UINT64),
      m_data(new uint64_t(i))
{ }

Variant::Variant(const int64_t i)
    : m_type(VARIANT_INT64),
      m_data(new int64_t(i))
{ }

Variant::Variant(const std::string &s)
    : m_type(VARIANT_STRING),
      m_data(new std::string(s))
{ }

Variant::Variant(const char *s)
    : m_type(VARIANT_STRING),
      m_data(new std::string(s))
{
}

Variant::Variant(const std::map< std::string, Variant > &m)
    : m_type(VARIANT_MAP),
      m_data(new std::map< std::string, Variant >(m))
{
}

Variant::Variant(const std::vector< Variant > &v)
    : m_type(VARIANT_VECTOR),
      m_data(new std::vector< Variant >(v))
{ }

Variant::Variant(const std::pair< Variant, Variant > &v)
    : m_type(VARIANT_VECTOR)
{
    std::vector< Variant > *vect = new std::vector< Variant >();
    vect->push_back(v.first);
    vect->push_back(v.second);
    m_data = vect;
}

Variant::Variant(const Variant &v)
    : m_type(VARIANT_NONE)
{
    /* use the operator = */
    if (v.m_type != VARIANT_NONE)
        this->operator=(v);
    else
        m_data = NULL;
}

void Variant::delData()
{
    switch (m_type)
    {
        case VARIANT_STRING:
            delete (std::string *)m_data;
            break;;
        case VARIANT_MAP:
            delete (std::map<std::string, Variant> *)m_data;
            break;;
        case VARIANT_VECTOR:
            delete (std::vector<Variant> *)m_data;
            break;;
        case VARIANT_BOOL:
            delete (bool *)m_data;
            break;;
        case VARIANT_UINT16:
            delete (uint16_t *)m_data;
            break;;
        case VARIANT_INT16:
            delete (int16_t *)m_data;
            break;;
        case VARIANT_UINT32:
            delete (uint32_t *)m_data;
            break;;
        case VARIANT_INT32:
            delete (int32_t *)m_data;
            break;;
        case VARIANT_INT64:
            delete (int64_t *)m_data;
            break;;
        case VARIANT_UINT64:
            delete (uint64_t *)m_data;
            break;;
        case VARIANT_NONE:
            break;;
    }
    m_data = NULL;
    m_type = VARIANT_NONE;
}

#define VARIANT_COPY(src, dest, type) \
    do { \
        dest = new type(*(type *) src); \
    } while (0); \

const Variant &Variant::operator= (const Variant &v)
{
    if (this != &v) {
        delData();
        m_type = v.m_type;
        switch (v.m_type)
        {
            case VARIANT_NONE:
                m_data = v.m_data;
                break;;
            case VARIANT_BOOL:
                VARIANT_COPY(v.m_data, m_data, bool);
                break;;
            case VARIANT_UINT16:
                VARIANT_COPY(v.m_data, m_data, uint16_t);
                break;;
            case VARIANT_INT16:
                VARIANT_COPY(v.m_data, m_data, int16_t);
                break;;
            case VARIANT_UINT32:
                VARIANT_COPY(v.m_data, m_data, uint32_t);
                break;;
            case VARIANT_INT32:
                VARIANT_COPY(v.m_data, m_data, int32_t);
                break;;
            case VARIANT_INT64:
                VARIANT_COPY(v.m_data, m_data, int64_t);
                break;;
            case VARIANT_UINT64:
                VARIANT_COPY(v.m_data, m_data, uint64_t);
                break;;
            case VARIANT_STRING:
                VARIANT_COPY(v.m_data, m_data, std::string);
                break;;
            case VARIANT_MAP:
                m_data = new std::map<std::string, Variant>(*(const std::map<std::string, Variant> *) v.m_data);
                break;;
            case VARIANT_VECTOR:
                m_data = new std::vector<Variant>(*(const std::vector<Variant> *) v.m_data);
                break;;
        }
    }
    return *this;
}

#define VARIANT_COMPARE(v1,v2,type) \
    ( *(type *)v1 == *(type *)v2 )

bool Variant::operator == (const Variant &v) const
{
    if (v.m_type != m_type)
        return false;

    switch (m_type) {
        case VARIANT_NONE:
            return true;
        case VARIANT_BOOL:
            return VARIANT_COMPARE(m_data, v.m_data, bool);
        case VARIANT_UINT16:
            return VARIANT_COMPARE(m_data, v.m_data, uint16_t);
        case VARIANT_INT16:
            return VARIANT_COMPARE(v.m_data, m_data, int16_t);
        case VARIANT_UINT32:
            return VARIANT_COMPARE(m_data, v.m_data, uint32_t);
        case VARIANT_INT32:
            return VARIANT_COMPARE(v.m_data, m_data, int32_t);
        case VARIANT_INT64:
            return VARIANT_COMPARE(v.m_data, m_data, int64_t);
        case VARIANT_UINT64:
            return VARIANT_COMPARE(v.m_data, m_data, uint64_t);
        case VARIANT_STRING:
            return VARIANT_COMPARE(v.m_data, m_data, std::string);
        case VARIANT_MAP:
            return *(std::map<std::string,Variant>*)m_data == *(std::map<std::string,Variant>*)v.m_data;
            //VARIANT_COMPARE(v.m_data, m_data, std::map<std::string,Variant>);
        case VARIANT_VECTOR:
            return VARIANT_COMPARE(v.m_data, m_data, std::vector<Variant>);
    };
    return false;
}

bool Variant::operator != (const Variant &v) const
{
  return ! this->operator == (v);
}

Variant::~Variant()
{
    delData();
}

const Variant::Type Variant::type() const
{
    return m_type;
}

Variant::operator bool() const
{
    assert(m_type == VARIANT_BOOL);
    return *(bool *) m_data;
}

Variant::operator uint16_t() const
{
    assert(m_type == VARIANT_UINT16);
    return *(uint16_t *) m_data;
}

Variant::operator int16_t() const
{
    assert(m_type == VARIANT_INT16);
    return *(int16_t *) m_data;
}

Variant::operator uint32_t() const
{
    assert(m_type == VARIANT_UINT32);
    return *(uint32_t *) m_data;
}

Variant::operator int32_t() const
{
    assert(m_type == VARIANT_INT32);
    return *(int32_t *) m_data;
}

Variant::operator uint64_t() const
{
    assert(m_type == VARIANT_UINT64);
    return *(uint64_t *) m_data;
}

Variant::operator int64_t() const
{
    assert(m_type == VARIANT_INT64);
    return *(int64_t *) m_data;
}

Variant::operator std::string &() const
{
    assert(m_type == VARIANT_STRING);
    return *((std::string *) m_data);
}

Variant::operator std::map<std::string, Variant> &() const
{
    assert(m_type == VARIANT_MAP);
    return *(std::map<std::string, Variant> *) m_data;
}

Variant::operator std::vector<Variant> &() const
{
    assert(m_type == VARIANT_VECTOR);
    return *(std::vector<Variant> *) m_data;
}

Variant::operator std::pair<Variant, Variant> () const
{
    assert(m_type == VARIANT_VECTOR);
    const std::vector< Variant > *ptr = (std::vector< Variant > *) m_data;

    assert(ptr->size() == 2);

    return std::pair< Variant, Variant >((*ptr)[0], (*ptr)[1]);
}

#ifdef DBUS_CPP_FOUND

static inline void DBusMessageIterToVariant(::DBus::MessageIter &iter, Variant &var)
{

#define TO_DBUS(src,dest,type) \
    do { \
        type i;\
        src >> i; \
        dest = i; \
    } while (0);

    switch (iter.type()) {
        case 'b':
            TO_DBUS(iter,var, bool);
            break;
        case 'q':
            TO_DBUS(iter,var, uint16_t);
            break;
        case 'n':
            TO_DBUS(iter,var, int16_t);
            break;
        case 'i':
            TO_DBUS(iter,var,int32_t);
            break;;
        case 'u':
            TO_DBUS(iter,var,uint32_t);
            break;;
        case 'x':
            TO_DBUS(iter,var,int64_t);
            break;;
        case 't':
            TO_DBUS(iter,var,uint64_t);
            break;;
        case 's':
        {
            //std::string s;
            //iter >> s;
            //var = s;
            var = iter.get_string();
            iter++;
            break;;
        }
        case 'v':
        {
            DBus::MessageIter viter = iter.recurse();
            DBusMessageIterToVariant(viter, var);
            iter++;
            break;;
        }
        case 'a':
        {
            if (iter.array_type() == 'e') { // map type
                //TODO this might be enhanced
                std::map<std::string, Variant> map;

                for (iter = iter.recurse(); !iter.at_end(); ++iter) {
                    DBus::MessageIter eit = iter.recurse();
                    std::string s;
                    Variant v;

                    try {
                        eit >> s;
                    } catch (DBus::Error dbErr) {
                        assert(0 && "Only std::string maps are supported with Variant");
                        return;
                    }

                    DBusMessageIterToVariant(eit, v);
                    map[s] = v;
                }
                var = map;
            }
            else { // array type
                //TODO this might be enhanced
                std::vector<Variant> vect;

                iter = iter.recurse();
                while (!iter.at_end()) {
                    Variant v;

                    DBusMessageIterToVariant(iter, v);
                    vect.push_back(v);
                }
                var = vect;
            }
            break;;
        }

        default:
            std::cerr << "Unhandled DBus::Variant type '" << iter.type() << "'" << std::endl ;
            iter++;
            break;;
    }
}

DBus::MessageIter &operator << (DBus::MessageIter &iter, const Variant &var)
{
    DBus::MessageIter d_it;

    switch (var.type()) {
        case Variant::VARIANT_BOOL:
            d_it = iter.new_variant(DBus::type<bool>::sig().c_str());
            d_it << (bool) var;
            break;;

        case Variant::VARIANT_UINT16:
            d_it = iter.new_variant(DBus::type<uint16_t>::sig().c_str());
            d_it << (uint16_t) var;
            break;;

        case Variant::VARIANT_INT16:
            d_it = iter.new_variant(DBus::type<int16_t>::sig().c_str());
            d_it << (int16_t) var;
            break;;

        case Variant::VARIANT_UINT32:
            d_it = iter.new_variant(DBus::type<uint32_t>::sig().c_str());
            d_it << (uint32_t) var;
            break;;

        case Variant::VARIANT_INT32:
            d_it = iter.new_variant(DBus::type<int32_t>::sig().c_str());
            d_it << (int32_t) var;
            break;;

        case Variant::VARIANT_UINT64:
            d_it = iter.new_variant(DBus::type<uint64_t>::sig().c_str());
            d_it << (uint64_t) var;
            break;;

        case Variant::VARIANT_INT64:
            d_it = iter.new_variant(DBus::type<int64_t>::sig().c_str());
            d_it << (int64_t) var;
            break;;

        case Variant::VARIANT_STRING:
            d_it = iter.new_variant(DBus::type<std::string>::sig().c_str());
            d_it << var.operator std::string&();
            break;;

        case Variant::VARIANT_VECTOR:
        {
            d_it = iter.new_variant(DBus::type<std::vector<DBus::Variant> >::sig().c_str());
            d_it << var.operator std::vector<Variant>&();
            break;;
        }

        case Variant::VARIANT_MAP:
        {
          d_it = iter.new_variant(DBus::type<std::map<std::string, DBus::Variant> >::sig().c_str());
          d_it << var.operator std::map<std::string, Variant>&();
          break;;
        }

        case Variant::VARIANT_NONE:
            break;;
    };
    iter.close_container(d_it);
    return iter;
}

DBus::MessageIter &operator >> (DBus::MessageIter &iter, Variant &var)
{
  DBusMessageIterToVariant(iter, var);
  return iter;
}

#endif
