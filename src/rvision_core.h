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

#ifndef RVISION_RVISION_H
#define RVISION_RVISION_H

#include <string>
#include <memory>
#include <stdexcept>
#include <boost/filesystem.hpp>

#include "event.h"
#include "notifiers.h"
#include "rest_api_factory.h"
#include "motion_engine.h"

namespace rvision {

    class invalid_core_state : public std::runtime_error {
    public:
        explicit invalid_core_state(const std::string &what_arg) : runtime_error(what_arg), m_reason(what_arg) {

        }

        virtual const char *what() const throw() {
            return m_reason.c_str();
        }

    private:
        std::string m_reason;
    };

    class core_error : public std::runtime_error {
    public:
        explicit core_error(const std::string &what_arg) : runtime_error(what_arg) {

        }
    };

    class rvision_core {
    public:

        rvision_core();

        void init(const boost::filesystem::path &config_file);

        void run();

        void finish();

    private:
        std::shared_ptr<rvision::notifiers<rvision::event>> m_notifiers;
        std::shared_ptr<rvision::motion_engine> m_motion_engine;
        rvision::rest_api_factory m_rest_api_factory;

    };

}


#endif //RVISION_RVISION_H
