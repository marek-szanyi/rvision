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
#ifndef RVISION_REST_CLIENT_H
#define RVISION_REST_CLIENT_H

#include <memory>
#include <vector>
#include <restbed>
#include <stdexcept>
#include <boost/filesystem.hpp>
#include <iostream>

namespace rvision {

    class rest_client {
    public:

        rest_client();

        std::vector<uint8_t>
        request_get(const std::string rest_uri, const std::string host, std::multimap<std::string, std::string> values);

        std::vector<uint8_t> request_post(const std::string rest_uri, const std::string host,
                                          std::multimap<std::string, std::string> values);

    private:
        std::vector<uint8_t> handle_response(std::shared_ptr<restbed::Response> &response);

        std::vector<uint8_t>
        request_internal(const std::string &method_type, const std::string rest_uri, const std::string host,
                         std::multimap<std::string, std::string> &values);
    };
}

#endif //RVISION_REST_CLIENT_H
