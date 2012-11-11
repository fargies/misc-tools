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
** timespec.hh
**
**        Created on: Nov 11, 2012
**   Original Author: Fargier Sylvain <fargier.sylvain@free.fr>
**
*/

#ifndef __TIMESPEC_HH__
#define __TIMESPEC_HH__

#include <stdint.h>
#include <time.h>

inline struct timespec &addMilisecs(struct timespec &ts, const uint32_t msecs)
{
    // uint32_t is definitely enough to store some seconds encoded as a nanosec
    uint32_t nsec = (msecs % 1000) * (1000 * 1000) + ts.tv_nsec;
    ts.tv_sec += msecs / 1000;

    if (nsec >= 1000000000) {
        ts.tv_sec += 1;
        nsec -= 1000000000;
    }
    ts.tv_nsec = nsec;
    return ts;
}

inline uint32_t toMilisecs(const struct timespec &ts)
{
    return ts.tv_sec * 1000 + ( ts.tv_nsec / 1000000 );
}

inline struct timespec &operator -= (struct timespec &t1, const struct timespec &t2)
{
    if (t1.tv_nsec < t2.tv_nsec) {
        t1.tv_sec -= (t2.tv_sec + 1);
        t1.tv_nsec = (t1.tv_nsec + 1000000000 - t2.tv_nsec);
    }
    else {
        t1.tv_sec -= t2.tv_sec;
        t1.tv_nsec -= t2.tv_nsec;
    }
    return t1;
}

inline struct timespec operator - (const struct timespec &t1, const struct timespec &t2) {
    struct timespec res = t1;
    return (res -= t2);
}

inline bool operator < (const struct timespec &t1, const struct timespec &t2)
{
    if (t1.tv_sec != t2.tv_sec)
    {
        return t1.tv_sec < t2.tv_sec;
    }
    else
        return t1.tv_nsec < t2.tv_nsec;
}

inline bool operator <= (const struct timespec &t1, const struct timespec &t2)
{
    if (t1.tv_sec != t2.tv_sec)
    {
        return t1.tv_sec <= t2.tv_sec;
    }
    else
        return t1.tv_nsec <= t2.tv_nsec;
}

inline bool operator == (const struct timespec &t1, const struct timespec &t2) {
    return (t1.tv_sec == t2.tv_sec) && (t1.tv_nsec == t2.tv_nsec);
}

#endif

