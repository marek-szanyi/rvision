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

#ifndef RVISION_REST_API_FACTORY_H
#define RVISION_REST_API_FACTORY_H

#include <memory>

#include "rest_client.h"

namespace rvision {

    /**
     * @brief factory for creating instances of classes which use the rest_client class
     */
    class rest_api_factory {
    public:

        /**
         * @brief constructor
         */
        rest_api_factory() : m_rest_client(new rest_client()) {

        }

        /**
         * @brief create a new instance of type TClient
         * @tparam TClient type of the instance
         * @return shared_ptr of the new instance
         */
        template<typename TClient>
        std::shared_ptr<TClient> create() {
            return std::make_shared<TClient>(m_rest_client);
        }

        /**
         * @brief create a new instance of type TClient and also configures the newly created instance
         * @tparam TClient type of the instance
         * @tparam TConf type of the configuration input
         * @param input configuration
         * @return shared_ptr of the new instance
         */
        template<typename TClient, typename TConf>
        std::shared_ptr<TClient> create(const TConf &input) {
            std::shared_ptr<TClient> client = std::make_shared(m_rest_client);
            client->configure().read(input);
            return client;
        };

    private:
       std::shared_ptr<rvision::rest_client> m_rest_client;

    };

}


#endif //RVISION_REST_API_FACTORY_H
