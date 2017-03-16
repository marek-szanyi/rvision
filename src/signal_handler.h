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

#ifndef RVISION_SIGNAL_HANDLER_H
#define RVISION_SIGNAL_HANDLER_H

#include <signal.h>
#include <functional>
#include <unordered_map>

namespace rvision {
    namespace sys {

        /**
         * @brief register of system signals and handlers
         * @tparam Tfunc type of handler function
         */
        template<typename Tfunc>
        class signal_handler {
        public:

            /**
             * @brief register a signal handler for a given signal
             * @param handler the handler of the signal
             * @param sign OS signal
             */
            void register_signal(std::function<Tfunc> handler, int sign) {
                signal_handler<Tfunc>::m_handler_map[sign] = handler;
                signal(sign, signal_handler::handle_signal);
            }

        private:
            static void handle_signal(int signal) {
                auto result = signal_handler<Tfunc>::m_handler_map.find(signal);
                if (result != signal_handler<Tfunc>::m_handler_map.end()) {
                    signal_handler<Tfunc>::m_handler_map[signal]();
                }
            }

        private:
            static std::unordered_map<int, std::function<Tfunc>> m_handler_map;
        };

        template<typename Tfunc> std::unordered_map<int, std::function<Tfunc>> signal_handler<Tfunc>::m_handler_map;
    }
}

#endif //RVISION_SIGNAL_HANDLER_H
