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

#ifndef RVISION_APPLICATION_H
#define RVISION_APPLICATION_H

#include "easylogging++.h"
#include "signal_handler.h"
#include "rvision_core.h"
#include <boost/program_options.hpp>


namespace rvision {
    namespace sys {

        /**
         * @brief application class implements the main subsystem of the rvision process, which can
         * run as either a daemon or as a command line application. The application part can be configured
         * either from command line or from a dedicated config file.
         */
        class application {
        public:
            /**
             * @brief constructor
             */
            application();

            /**
             * @brief runs the application main logic
             * @param argc passed from main function
             * @param argv passed from main function
             * @return the exit code of the application
             */
            int run(int argc, const char **argv);

        private:
            boost::program_options::options_description m_general_desc;
            boost::program_options::options_description m_specific_desc;
            boost::program_options::options_description m_cmdline_desc;
            boost::program_options::variables_map m_config_map;
            signal_handler<void(void)> m_signal_handler;
            int m_pid_fd;
            rvision::rvision_core m_rvision_core;

            /**
             * @brief tries to parse the command line arguments
             * @param argc from main function
             * @param argv from main function
             */
            void try_parse_config(int argc, const char **argv);

            /**
             * @brief print the help message to the command line if the user requested so
             */
            void print_help();

            /**
             * @brief print the application version to the command line if the user requested so
             */
            void print_version();

            /**
             * @brief configure the logger to also log to a file (given as a command line argument)
             */
            void configure_file_logging();

            /**
             * @brief check whenever the application should run as daemon based on config
             * @return true if yes, false otherwise
             */
            bool should_run_as_daemon();

            /**
             * @brief fork a daemon
             */
            void daemonize();

            /**
             * @brief run the main application logic
             * @throws app_error in case of execution error
             */
            void run_internal();

            /**
             * @brief handle SIGINT from OS
             */
            void handle_sigint();

            /**
             * @brief handle SIGHUP from OS
             */
            void handle_sighup();

            /**
             * @brief handle SIGHUP from OS
             */
            void handle_sigterm();

            /**
             * @brief cleanup lock file created when running as daemon
             */
            void cleanup_lockfile();
        };

    }
}

#endif //RVISION_APPLICATION_H
