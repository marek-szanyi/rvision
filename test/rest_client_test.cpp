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

#include <stdexcept>
#include "gtest/gtest.h"

#include "../src/rest_client.h"
#include "../src/network_error.h"


// The fixture for testing class Foo.
class rest_client_test: public ::testing::Test {

protected:

    // Code here will be called immediately after the constructor (right
    // before each test).
    void SetUp() override {

    }

    // Code here will be called immediately after each test (right
    // before the destructor).
    void TearDown() override {

    }
};

TEST_F(rest_client_test, test_empty_params_request_get) {
    rvision::rest_client rest_client;
    std::string empty_arg;
    std::multimap<std::string, std::string> empty_values;
    try {
        rest_client.request_get(empty_arg,empty_arg, empty_values);
        FAIL();
    } catch (std::invalid_argument &ia) {

    }
}

TEST_F(rest_client_test, test_empty_params_request_post) {
    rvision::rest_client rest_client;
    std::string empty_arg;
    std::multimap<std::string, std::string> empty_values;
    try {
        rest_client.request_post(empty_arg,empty_arg, empty_values);
        FAIL();
    } catch (std::invalid_argument &ia) {

    }
}

TEST_F(rest_client_test, test_invalid_url_for_requests) {
    rvision::rest_client rest_client;
    std::multimap<std::string, std::string> empty_values;
    try {
        rest_client.request_get("dhddlbla", "hahalalba", empty_values);
        FAIL();
    } catch (std::invalid_argument &ia) {

    }
}

TEST_F(rest_client_test, test_no_network) {
    rvision::rest_client rest_client;
    std::multimap<std::string, std::string> empty_values;
    try {
        rest_client.request_get("http://123sms.sk", "www.123sms.sk", empty_values);
        FAIL();
    } catch (rvision::network_error &ia) {

    }
}