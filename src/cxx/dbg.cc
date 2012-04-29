/*
** Copyright (C) 2012 Fargier Sylvain <fargier.sylvain@free.fr>
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
** dbg.cc
**
**        Created on: Apr 14, 2012
**   Original Author: fargie_s
**
*/

#if !defined(NDEBUG)

#include <cstdio>
#include <cstring>
#include <ctype.h>

#include "dbg.hh"

#define DEBUG_INDENT_LEN (sizeof (DEBUG_INDENT_STR) - 1)
#define DEBUG_MAX_INDENT (DEBUG_BUFF_SIZE / DEBUG_INDENT_LEN / 2)
#define DEBUG_DATA_LEN 80

static char buffer[DEBUG_BUFF_SIZE];

Debug &Debug::operator <<(Debug::DebugMod m)
{
    switch (m)
    {
        case NL:
            putc('\n');
            flush();
            break;
        case INDENT:
            ++m_indent;
            break;
        case OUTDENT:
            --m_indent;
            break;
    };
    return *this;
}

Debug &Debug::operator <<(const char *str)
{
    if (str == NULL)
        puts(DEBUG_NULL_STR);
    else
        puts(str);
    return *this;
}

Debug &Debug::operator <<(const Quoted &f)
{
    if (!f.m_str)
        puts(DEBUG_NULL_STR);
    else
    {
        const char *str = f.m_str;
        if (m_pos && (m_pos + strlen(str) + 2) > DEBUG_BUFF_SIZE)
            flush();

        buffer[m_pos++] = '"';
        while (*str != '\0' && m_pos < DEBUG_BUFF_SIZE)
            buffer[m_pos++] = *(str++);
        if (m_pos < DEBUG_BUFF_SIZE)
            buffer[m_pos++] = '"';
    }
    return *this;
}

Debug &Debug::operator <<(int i)
{
    size_t buff_size = DEBUG_BUFF_SIZE - m_pos;
    size_t len = snprintf(&buffer[m_pos], buff_size, "%d", i);

    if (len >= buff_size)
    {
        flush();
        buff_size = DEBUG_BUFF_SIZE - m_pos;
        len = snprintf(&buffer[m_pos], buff_size, "%d", i);
        m_pos += (len > buff_size) ? buff_size : len;
    }
    else
        m_pos += len;
    return *this;
}

Debug &Debug::operator <<(unsigned int i)
{
    size_t buff_size = DEBUG_BUFF_SIZE - m_pos;
    size_t len = snprintf(&buffer[m_pos], buff_size, "%u", i);

    if (len >= buff_size)
    {
        flush();
        buff_size = DEBUG_BUFF_SIZE - m_pos;
        len = snprintf(&buffer[m_pos], buff_size, "%u", i);
        m_pos += (len > buff_size) ? buff_size : len;
    }
    else
        m_pos += len;
    return *this;
}

Debug &Debug::operator <<(char c)
{
    if (m_pos && (m_pos + 3) > DEBUG_BUFF_SIZE)
        flush();

    if ((m_pos + 3) <= DEBUG_BUFF_SIZE)
    {
        buffer[m_pos++] = '\'';
        buffer[m_pos++] = c;
        buffer[m_pos++] = '\'';
    }
    return *this;
}

Debug &Debug::operator <<(bool b)
{
    (b) ? puts(DEBUG_TRUE_STR) : puts(DEBUG_FALSE_STR);
    return *this;
}

Debug &Debug::operator <<(short s)
{
    this->operator <<(static_cast<unsigned int>(s));
    return *this;
}

Debug &Debug::operator <<(const DataFmt &f)
{
    const char *data = f.m_data;
    size_t remain = f.m_len;
    /* FIXME SAVE INDENT THEN RESTORE */
    if (m_pos)
        flush();

    while (remain)
    {
        m_pos += snprintf(&buffer[m_pos], 11, "%.8x  ",
                (int) (f.m_len - remain));

        /* we can print a whole line here */
        size_t count = remain;
        char *p = &buffer[m_pos];
        char *d = p + 16 * 3 + 1;
        *(d++) = '|';

        if (count > 16)
            count = 16;
        for (size_t i = 0; i < 16; ++i)
        {
            if (i < count)
            {
                unsigned char c = (*(data++));
                *(p++) = (c >> 4) + ((c > 0x9F) ? ('A' - 10) : '0');
                *(p++) = (c & 0x0F) + (((c & 0x0F) > 0x09) ? ('a' - 10) : '0');
                *(p++) = ' ';
                *(d++) = isprint(c) ? c : '.';
            }
            else
            {
                *(p++) = ' ';
                *(p++) = ' ';
                *(p++) = ' ';
            }
            if (i == 8)
                *(p++) = ' ';
        }
        remain -= count;
        *(d++) = '|';
        *(d++) = '\n';

        m_pos += 16 * 3 + 1 + count + 2 + 1;
        flush();
    }
    return *this;
}

void Debug::puts(const char *str)
{
    if (m_pos && (m_pos + strlen(str)) > DEBUG_BUFF_SIZE)
        flush();

    while (*str != '\0' && m_pos < DEBUG_BUFF_SIZE)
        buffer[m_pos++] = *(str++);
}

void Debug::putc(char c)
{
    if (m_pos == DEBUG_BUFF_SIZE)
        flush();

    buffer[m_pos++] = c;
}

void Debug::flush()
{
    if (m_pos)
    {
        fwrite(buffer, m_pos, 1, stderr);
        m_pos = 0;

        unsigned int indent = (m_indent > DEBUG_MAX_INDENT) ?
                DEBUG_MAX_INDENT : m_indent;
        char *tmp_buf = buffer;
        while (indent--)
        {
            memcpy(tmp_buf, DEBUG_INDENT_STR, DEBUG_INDENT_LEN);
            tmp_buf += DEBUG_INDENT_LEN;
            m_pos += DEBUG_INDENT_LEN;
        }
    }
}

char *Debug::buf()
{
    return buffer;
}

#ifdef DEBUG_STL
Debug &operator <<(Debug &d, const std::string &s)
{
    d << Debug::Quoted(s.c_str());
    return d;
}

#endif

#endif

