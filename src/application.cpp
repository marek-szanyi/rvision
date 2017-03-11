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
          m_config_map(po::variables_map()) {

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

    } catch (app_exception &e) {
        if (e.is_error()) {
            return EXIT_FAILURE;
        } else {
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