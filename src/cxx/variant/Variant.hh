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
** Variant.hh
**
**        Created on: Feb 14, 2011
**            Author: fargie_s
**
*/

#ifndef VARIANT_HH_
#define VARIANT_HH_

#include "config.h"
#include <stdint.h>
#include <string>
#include <map>
#include <vector>
#include <list>

class Variant
{
    public:
        //! @brief Variant types
        typedef enum {
            VARIANT_NONE,
            VARIANT_BOOL,
            VARIANT_INT16,
            VARIANT_UINT16,
            VARIANT_INT32,
            VARIANT_UINT32,
            VARIANT_INT64,
            VARIANT_UINT64,
            VARIANT_STRING,
            VARIANT_MAP,
            VARIANT_VECTOR,
        } Type;

        //! @brief default constructor
        //! #VARIANT_NONE #Type will be used
        Variant();

        //! @name stdint constructors
        //@{
        Variant(const int16_t);
        Variant(const uint16_t);
        Variant(const int32_t);
        Variant(const uint32_t);
        Variant(const int64_t);
        Variant(const uint64_t);
        //@}

        Variant(const bool);

        //! @name string constructors
        //@{
        Variant(const std::string &);
        Variant(const char *);
        //@}

        Variant(const Variant &);

        //! @name complex types constructors
        //@{
        Variant(const std::map< std::string, Variant > &);
        Variant(const std::vector< Variant > &);
        //! @brief creates a #VARIANT_VECTOR
        Variant(const std::pair< Variant, Variant > &);
        //@}

        ~Variant();

        const Variant &operator= (const Variant &v);

        bool operator== (const Variant &v) const;
        bool operator!= (const Variant &v) const;

        //! @name stdint conversion
        //@{
        operator int16_t() const;
        operator uint16_t() const;
        operator int32_t() const;
        operator uint32_t() const;
        operator int64_t() const;
        operator uint64_t() const;
        //@}

        //! @name boolean conversion
        //@{
        operator bool() const;
        //@}

        //! @name string conversions
        //@{
        operator std::string &() const;
        //@}

        //! @name complex types conversions
        //@{
        operator std::map< std::string, Variant > &() const;
        operator std::vector< Variant > &() const;
        //! @brief uses #VARIANT_VECTOR Variant #Type (any 2 sized vector can be cast as a pair)
        operator std::pair< Variant, Variant > () const;
        //@}

        const std::type_info &typeinfo() const;
        const Variant::Type type() const;

    protected:
        const std::type_info *m_typeinfo;
        Variant::Type m_type;
        void *m_data;

        void delData();
};

#ifdef DBUS_CPP_FOUND

#include <dbus-c++/dbus.h>

namespace DBus {
template <> struct type< ::Variant>        { static std::string sig(){ return "v";  } };
}

DBus::MessageIter &operator << (DBus::MessageIter &iter, const Variant &val);
DBus::MessageIter &operator >> (DBus::MessageIter &iter, Variant &val);

#endif

#endif /* VARIANT_HH_ */
