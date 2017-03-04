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
#include "rest_client.h"
#include "network_error.h"
#include <stdexcept>


using namespace rvision;

rest_client::rest_client() {

}

std::vector<uint8_t> rest_client::request_get(const std::string rest_uri, const std::string host,
                                              std::multimap<std::string, std::string> values) {
    return request_internal("GET", rest_uri, host, values);
}

std::vector<uint8_t> rest_client::request_post(const std::string rest_uri, const std::string host,
                                               std::multimap<std::string, std::string> values) {
    return request_internal("POST", rest_uri, host, values);
}

std::vector<uint8_t> rest_client::handle_response(std::shared_ptr<restbed::Response> &response) {
    std::cout << response->get_status_code() << "\n";
    if (response->get_status_code() != restbed::OK) {
        throw network_error("failed to get response for REST api call");
    }

    auto length = response->get_header("Content-Length", 0);

    restbed::Http::fetch(length, response);
    return response->get_body();
}

std::vector<uint8_t>
rest_client::request_internal(const std::string &method_type, const std::string rest_uri, const std::string host,
                              std::multimap<std::string, std::string> &values) {
    if (rest_uri.empty()) {
        throw std::invalid_argument("rest_uri");
    }
    if (host.empty()) {
        throw std::invalid_argument("host");
    }

    auto request = std::make_shared<restbed::Request>(restbed::Uri(rest_uri));
    request->set_method(method_type);
    request->set_header("Accept", "*/*");
    request->set_header("Host", host);
    request->set_header("Content-Type", "application/x-www-form-urlencoded");
    request->set_query_parameters(values);

    std::cout << std::string(reinterpret_cast<const char *>(restbed::Http::to_bytes(request).data())) << "\n";
    auto response = restbed::Http::sync(request);
    return handle_response(response);
}

