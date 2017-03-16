/**
 * Copyright (c) 2017, Marek Szanyi (marek@memdump.sk)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "application.h"
#include "rvision_config.h"
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include <functional>
#include <fcntl.h>


namespace rvision {
    namespace sys {
        class app_exception : public std::runtime_error {
        public:
            explicit app_exception(bool is_error, const std::string &what_arg = "n/a") : m_is_error(is_error),
                                                                                         runtime_error(what_arg) {

            }

            bool is_error() {
                return m_is_error;
            }

        private:
            bool m_is_error;

        };
    }
}

using namespace rvision::sys;
namespace po = boost::program_options;

application::application()
        : m_general_desc(po::options_description("Rvision application configuration")),
          m_specific_desc(po::options_description("Rvision specific configuration")),
          m_cmdline_desc(po::options_description()),
          m_config_map(po::variables_map()),
          m_pid_fd(-1) {

    m_general_desc.add_options()
            ("version", "prints the application version")
            ("help", "print help message");
    m_specific_desc.add_options()
            ("daemon", "run as daemon process")
            ("conf", po::value<std::string>()->default_value("./rvision.conf"), "full path to the core config file")
            ("logfile", po::value<std::string>(), "log to custom log file");
    m_cmdline_desc.add(m_general_desc).add(m_specific_desc);


    el::Loggers::reconfigureAllLoggers(el::ConfigurationType::ToStandardOutput, "true");
    el::Loggers::getLogger("rvision_app");
#define ALOG(LEVEL) CLOG(LEVEL,"rvision_app")
}

int application::run(int argc, const char **argv) {

    try {
        try_parse_config(argc, argv);

        print_help();

        print_version();

        configure_file_logging();

        if (should_run_as_daemon()) {
            daemonize();
        }

        using namespace std::placeholders;
        m_signal_handler.register_signal(std::bind(&application::handle_sigint, this), SIGINT);
        m_signal_handler.register_signal(std::bind(&application::handle_sighup, this), SIGHUP);
        m_signal_handler.register_signal(std::bind(&application::handle_sigterm, this), SIGTERM);

        run_internal();

    } catch (app_exception &e) {
        if (e.is_error()) {
            ALOG(ERROR) << "application failed to run because: " << e.what();
            return EXIT_FAILURE;
        } else {
            ALOG(WARNING) << "application failed to run because: " << e.what();
            return EXIT_SUCCESS;
        }
    }

    return EXIT_SUCCESS;
}

void application::try_parse_config(int argc, const char **argv) {
    try {
        po::store(po::command_line_parser(argc, argv).options(m_cmdline_desc).run(), m_config_map);
        po::notify(m_config_map);

        ALOG(INFO) << "command line configuration parsed.";
    } catch (po::error &e) {
        ALOG(WARNING) << e.what();
        print_help();
        throw app_exception(true, e.what());
    }


}

void application::print_help() {
    if (m_config_map.count("help") > 0) {
        std::cout << m_cmdline_desc << "\n";
        throw app_exception(false);
    }
}

void application::print_version() {
    if (m_config_map.count("version") > 0) {
        std::cout << "rvision application: " << RVISION_MAJOR_VERSION << "." << RVISION_MINOR_VERSION << " ["
                  << RVISION_BUILD_TYPE << "]" << "\n";
        throw app_exception(false);
    }
}

void application::configure_file_logging() {
    if (m_config_map.count("logfile") > 0) {
        el::Loggers::reconfigureAllLoggers(el::ConfigurationType::ToFile, "true");
        el::Loggers::reconfigureAllLoggers(el::ConfigurationType::Filename, m_config_map["logfile"].as<std::string>());
        ALOG(INFO) << "loggers also log to file: " << m_config_map["logfile"].as<std::string>();
    }
}

bool application::should_run_as_daemon() {
    auto result = false;
    try {
        result = m_config_map.count("daemon") > 0;
    } catch (...) {

    }
    return result;
}

void application::cleanup_lockfile() {
    /* Unlock and close lockfile */
    if (m_pid_fd != -1) {
        lockf(m_pid_fd, F_ULOCK, 0);
        close(m_pid_fd);

        unlink("/tmp/rvision_daemon.lock");
    }

}

void application::handle_sigint() {
    ALOG(INFO) << "stopping daemon (received SIGINT) ...";

    cleanup_lockfile();

    /* Reset signal handling to default behavior */
    signal(SIGINT, SIG_DFL);
}

void application::handle_sighup() {
    ALOG(INFO) << "handling sighup ___";
}

void application::handle_sigterm() {
    ALOG(INFO) << "stopping daemon (received SIGTERM) ...";

    cleanup_lockfile();

    /* Reset signal handling to default behavior */
    signal(SIGTERM, SIG_DFL);
}

void application::daemonize() {
    pid_t pid = 0;

    signal(SIGHUP, SIG_IGN);

    /* Fork off the parent process */
    pid = fork();


    /* An error occurred */
    if (pid < 0) {
        throw app_exception(true, "pid failed");
    }

    /* Success: Let the parent terminate */
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    /* On success: The child process becomes session leader */
    if (setsid() < 0) {
        throw app_exception(true, "setsid failed");
    }

    /* Ignore signal sent from child to parent process */
    signal(SIGCHLD, SIG_IGN);

    /* Fork off for the second time*/
    pid = fork();

    /* An error occurred */
    if (pid < 0) {
        throw app_exception(true, "pid failed");
    }

    /* Success: Let the parent terminate */
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    /* Set new file permissions */
    umask(0);

    /* Change the working directory to the root directory */
    /* or another appropriated directory */
    chdir("/");

    /* Reopen stdin (fd = 0), stdout (fd = 1), stderr (fd = 2) */
    stdin = fopen("/dev/null", "r");
    stdout = fopen("/dev/null", "w+");
    stderr = fopen("/dev/null", "w+");

    /* Try to write PID of daemon to lockfile */
    m_pid_fd = open("/tmp/rvision_daemon.lock", O_RDWR | O_CREAT, 0640);
    if (m_pid_fd < 0) {
        /* Can't open lockfile */
        throw app_exception(true, "failed to open lockfile");
    }
    if (lockf(m_pid_fd, F_TLOCK, 0) < 0) {
        /* Can't lock file */
        throw app_exception(true, "can't lock file");
    }

    /* Write PID to lockfile */
    auto pidstr = std::to_string(getpid()).c_str();
    write(m_pid_fd, pidstr, strlen(pidstr));
}


void application::run_internal() {
    ALOG(INFO) << "run_internal ___";
}