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
#ifndef RVISION_NOTIFICATOR_BUILDER_H
#define RVISION_NOTIFICATOR_BUILDER_H

#include <memory>
#include <unordered_map>
#include <functional>

#include "observer.h"
#include "notifiers.h"


#include <boost/filesystem.hpp>


namespace rvision {

    namespace notifiers_builder {

        template<typename T>
        inline size_t __get_clazz_uid() {
            return typeid(T).hash_code();
        }

        template<typename Tdata, typename Tconfig>
        class builder {
        public:

            inline static std::unordered_map<size_t, std::function<rvision::observer<Tdata> *(const Tconfig &)>> &
            get() {
                static std::unordered_map<size_t, std::function<rvision::observer<Tdata> *(const Tconfig &)>> creators;
                return creators;
            };

            template<typename Type>
            static void add(std::function<rvision::observer<Tdata> *(const Tconfig &)> creator) {
                get()[__get_clazz_uid<Type>()] = creator;
            }

            std::shared_ptr<rvision::notifiers<Tdata>> make(const Tconfig &config) {
                std::shared_ptr<rvision::notifiers<Tdata>> notificator = std::make_shared<rvision::notifiers<Tdata>>();
                for (auto &creator : get()) {
                    auto new_notifier = creator.second(config);
                    notificator->attach(new_notifier);
                }

                return notificator;
            }


        };

        template<typename Ttype, typename Tdata, typename Tconfig>
        class register_notifier {
            int &count(void) {
                static int x = 0;
                return x;
            } //counts the number of callers per derived
        public:
            register_notifier(std::function<rvision::observer<Tdata> *(const Tconfig &)> creator) {
                if ((count())++ == 0) { //only initialize on the first caller of that class T
                    builder<Tdata, Tconfig>::template add<Ttype>(creator);
                }
            }
        };

    }
}


#endif //RVISION_NOTIFICATOR_BUILDER_H
