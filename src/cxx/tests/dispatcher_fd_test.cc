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
** dispatcher_fd_test.cc
**
**        Created on: Nov 10, 2012
**   Original Author: Fargier Sylvain <fargier.sylvain@free.fr>
**
*/

#include <string>

#include <unistd.h>
#include <fcntl.h>

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "notifier/Dispatcher.hh"
#include "notifier/FDWatch.hh"

using namespace std;
using namespace notifier;

class Pipe : public FDWatch
{
public:
    Pipe(Dispatcher &disp) :
        m_disp(disp)
    {
        CPPUNIT_ASSERT(pipe(m_fd) == 0);
        CPPUNIT_ASSERT(fcntl(m_fd[0], F_SETFD, FD_CLOEXEC) == 0);
        CPPUNIT_ASSERT(fcntl(m_fd[1], F_SETFD, FD_CLOEXEC) == 0);
        CPPUNIT_ASSERT(fcntl(m_fd[1], F_SETFL, O_NONBLOCK) == 0);

        m_disp.addHandler(this, m_fd[0], FDWatch::INPUT);
    }

    ~Pipe()
    {
        m_disp.removeHandler(this, m_fd[0], FDWatch::INPUT);
        close(m_fd[0]);
        close(m_fd[1]);
    }

    void handle(int fd)
    {
        size_t len = m_buff.size();
        ssize_t readed;

        do {
            m_buff.resize(len + 1024);
            readed = read(fd, &(m_buff[len]), 1024);

            if (readed >= 0)
            {
                len += readed;
                m_buff.resize(len);
            }
            else
                m_buff.resize(len);
        }
        while (readed == 1024);
    }

    void send(const string &str)
    {
        write(m_fd[1], str.c_str(), str.length());
    }

    vector<char> &getBuff()
    {
        return m_buff;
    }
protected:
    int m_fd[2];
    vector<char> m_buff;
    Dispatcher &m_disp;
};

class TestDispatcherFD : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestDispatcherFD);
    CPPUNIT_TEST(simple);
    CPPUNIT_TEST_SUITE_END();

public:
    void simple()
    {
        Dispatcher disp;
        Pipe p(disp);

        p.send("test");
        CPPUNIT_ASSERT(disp.dispatch(0) == 1);
        CPPUNIT_ASSERT(p.getBuff().size() == 4);

        CPPUNIT_ASSERT(disp.dispatch(1) == -1);
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestDispatcherFD);
