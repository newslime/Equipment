/* $Id: b_ace.cpp,v 1.1 2004/08/14 14:54:52 fukasawa Exp $ */

/*============================================================================*/
/**
 *  @file    b_ace.cpp
 *
 *  @author  Fukasawa Mitsuo
 *
 *
 *    Copyright (C) 2001-2004 BEE Co.,Ltd. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
/*============================================================================*/

#include "c_wrapper.h"
#include "b_ace.h"
#include "ace/Get_Opt.h"
#include "b_time.h"

//
// prototypes
//
class log_manager;
class Event_Server;

int ace_parse_args(int argc, char * argv[]);

//
static Event_Server * _event_server = NULL;

static u_short _port_number = ACE_DEFAULT_SERVER_PORT;
static char *  _host_name   = ACE_DEFAULT_SERVER_HOST;

ACE_Object_Manager * ace_object_manager;

//
// Log manager
//
class log_manager
{
public:
    log_manager() : m_log_stream(0), m_output_stream(0) {}
    ~log_manager()
    {
        if (m_log_stream)
        {
            m_log_stream->close();
            delete m_log_stream;
        }
    }

    const BCHAR * progname()     { return m_prog_name; }
    void  progname(BCHAR * name) {  m_prog_name = name; }

    int to_daemon(const BCHAR * prog_name = _TX(""),
                  const BCHAR * logger_key = ACE_DEFAULT_LOGGER_KEY)
    {
        m_prog_name = prog_name;
        return ACE_LOG_MSG->open(prog_name, ACE_Log_Msg::LOGGER, logger_key);
    }

    int to_syslog(const BCHAR * prog_name = _TX(""))
    {
        m_prog_name = prog_name;
        return ACE_LOG_MSG->open(m_prog_name, ACE_Log_Msg::SYSLOG, prog_name);
    }

    int to_ostream(ACE_OSTREAM_TYPE * output)
    {
        m_output_stream = output;
        ACE_LOG_MSG->msg_ostream(this->m_output_stream);
        ACE_LOG_MSG->clr_flags(ACE_Log_Msg::STDERR | ACE_Log_Msg::LOGGER);
        ACE_LOG_MSG->set_flags(ACE_Log_Msg::OSTREAM | ACE_Log_Msg::VERBOSE_LITE);
        return 0;
    }

    int to_file(const char * filename)
    {
        m_log_stream = new std::ofstream();
        m_log_stream->open(filename, ios::out | ios::app);
        return this->to_ostream((ACE_OSTREAM_TYPE *)m_log_stream);
    }

    int to_file()
    {
        string filename(m_prog_name);
        string timestr;
        b_time::makeDateName(time(0), timestr);
        filename += ACE_TEXT("_");
        filename += timestr;

        m_log_stream = new std::ofstream();
        m_log_stream->open(filename.c_str(), ios::out | ios::app);
        return this->to_ostream((ACE_OSTREAM_TYPE *)m_log_stream);
    }

    int to_stderr(void)
    {
        ACE_LOG_MSG->clr_flags(ACE_Log_Msg::OSTREAM | ACE_Log_Msg::LOGGER);
        ACE_LOG_MSG->set_flags(ACE_Log_Msg::STDERR | ACE_Log_Msg::VERBOSE_LITE);
        return 0;
    }

    ACE_Log_Msg_Callback * to_callback(ACE_Log_Msg_Callback * callback)
    {
        ACE_Log_Msg_Callback * previous = ACE_LOG_MSG->msg_callback(callback);
        if (callback == 0)
            ACE_LOG_MSG->clr_flags(ACE_Log_Msg::MSG_CALLBACK);
        else
            ACE_LOG_MSG->set_flags(ACE_Log_Msg::MSG_CALLBACK);
        return previous;
    }

private:
    const BCHAR * m_prog_name;
    std::ofstream *    m_log_stream;
    ACE_OSTREAM_TYPE * m_output_stream;
};

typedef ACE_Singleton<log_manager, ACE_Null_Mutex> LogManager;

#define LOG_MANAGER   LogManager::instance()


//
// Event Server in program
//
class Event_Server : public ACE_Event_Handler
{
public:
    // Constructor.
    Event_Server(void) {}

    // Shutdown the <ACE_Reactor>'s event loop when a SIGINT is received.
    Event_Server(int argc, char * argv[])
    {
        if (ace_parse_args(argc, argv) == -1)
        {
            ACE_ERROR((LM_ERROR, "%p\n", "parse_args"));
            return ;
        }
        m_sig_set.sig_add(SIGINT);
        m_sig_set.sig_add(SIGQUIT);
#ifdef _HP_UX
        m_sig_set.sig_add(SIGPIPE);
#endif

        // Register to handle the SIGINT SIGQUIT or SIGPIPE signals.
        if (ACE_Reactor::instance()->register_handler(m_sig_set, this) == -1)
        {
            ACE_ERROR((LM_ERROR, "%p\n", "register_handler"));
        }
    }

    // Run the event-loop for the event server.
    int svc(void)
    {
        // Perform the main event loop waiting for the user to type ^C.
        ACE_Reactor::run_event_loop();
        return 0;
    }

private:
    int handle_close(ACE_HANDLE, ACE_Reactor_Mask)
    {
        ace_fini();
        return 0;
    }

    // Close down via SIGINT, SIGQUIT or SIGPIPE.
    int handle_signal(int signum, siginfo_t *, ucontext_t *)
    {
        ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t) received signal %S\n"), signum));

#ifdef _HP_UX
        if (signum == SIGPIPE)
        {   // Disconnect Logger
            LOG_MANAGER->to_file();
            return 0;
        }
#endif

        ace_fini();
        return 0;
    }

private:
    ACE_Sig_Set m_sig_set;

};


//
// Initial Logger
//
int ace_parse_args(int argc, char * argv[])
{
    ACE_Get_Opt get_opt(argc, argv, "h:p:");
    for (int c; (c = get_opt()) != -1; )
    {
        switch (c)
        {
        case 'h':
            _host_name = get_opt.opt_arg();
            break;
        case 'p':
            _port_number = ACE_OS::atoi(get_opt.opt_arg());
            break;
        default:
            ACE_ERROR((LM_ERROR,
                ACE_TEXT("usage: %n [-h host_name] [-p portnum] \n")));
            return -1;
        }
    }
    return 0;
}

//
// Initial Logger
//
void ace_log_init(const ACE_TCHAR * prog_name, const ACE_TCHAR * param)
{
    int result;

#if defined(_DEBUG) && defined(WIN32) && defined(LEAK_CHECK)
    memdbg_init("memdebug.log");
#endif

    char * logger_key = ACE_OS::getenv("ACE_LOGGER_KEY");
    if ((logger_key == NULL) || (strlen(logger_key) == 0))
    {
        logger_key = ACE_DEFAULT_LOGGER_KEY;
    }

    if (param == NULL || _tcscmp(param, _TX("logger")) == 0)
    {
        result = LOG_MANAGER->to_daemon(prog_name, logger_key);
    }
    else if (_tcscmp(param, _TX("stderr")) == 0)
    {
        result = LOG_MANAGER->to_stderr();
    }
    else
    {
        result = LOG_MANAGER->to_file(param);
    }
    if (result != -1)
    {
        ACE_Trace::stop_tracing();   // disable trace

        // LOG_MANAGER->to_callback(TimestampCallback::instance());
//#ifdef _HP_UX
//        ACE_OS::signal(SIGPIPE, SIG_IGN);
//#endif
    }

    ACE_ERROR((LM_ERROR, ACE_TEXT("(%P|%t) %s start\n"), prog_name));
}

void ace_error_log(const char * fmt, ...)
{
    va_list arglist;
    va_start(arglist, fmt);

    ACE_ERROR((fmt, LM_ERROR, arglist));

    va_end(arglist);
    return ;
}

void ace_debug_log(const char * fmt, ...)
{
    va_list arglist;
    va_start(arglist, fmt);

    ACE_DEBUG((fmt, LM_DEBUG, arglist));

    va_end(arglist);
    return ;
}

void ace_hexdump(const char * top, size_t size)
{
    ACE_HEX_DUMP((LM_DEBUG, top, size));
}

void ace_disable_debug_log()
{
    ACE_Log_Msg::disable_debug_messages();
}

void ace_enable_debug_log()
{
    ACE_Log_Msg::enable_debug_messages();
}

//
// Initialize ACE wrapeers
//
int ace_init(int argc, char * argv[])
{
    ACE::init();
    ace_object_manager = ACE_Object_Manager::instance();

    ace_log_init(argv[0], argv[1]);

    //if (ACE_Service_Config::open(argv[0]) == -1 && errno != ENOENT)
    //{
    //    ACE_ERROR((LM_ERROR, ACE_TEXT("%p\n"), ACE_TEXT("open")));
    //    return -1;
    //}

    // Initialize the <Event_Server>.
    _event_server = new Event_Server(argc, argv);

    return 0;
}

int ace_run_loop()
{
    int result = 0;

    // Run the event server's event-loop.
#if 0
    result = _event_server->svc();
#else
    ACE_Reactor::run_event_loop();
#endif
    return result;
}


static fini_handler _fini_handler = NULL;

void ace_fini_handler(fini_handler fh)
{
    _fini_handler = fh;
}

void ace_fini()
{
#ifdef _HP_UX
    ACE_OS::signal(SIGRTMIN, SIG_IGN);
#endif

    if (_fini_handler != NULL)
    {
        _fini_handler();
    }

    ACE_Reactor::end_event_loop();
    //ACE_Reactor::instance()->end_reactor_event_loop();

    ACE_Proactor::end_event_loop();

    ACE::fini();
}

