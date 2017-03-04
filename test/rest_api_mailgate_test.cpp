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
#include <memory>
#include <vector>
#include "gtest/gtest.h"

#include "../src/rest_api_mailgate.h"

class mock_rest_client {
public:

    std::vector<uint8_t> m_toreturn;

    std::vector<uint8_t> request_get(const std::string rest_uri, const std::string host, std::multimap<std::string, std::string> values) {
        return m_toreturn;
    }

    std::vector<uint8_t> request_post(const std::string rest_uri, const std::string host, std::multimap< std::string, std::string > values) {
        return m_toreturn;
    }
};

class mock_config_file {
public:
    void read(const boost::filesystem::path &config) {

    }

    std::string base_url() {
        return "http://rest.sk";
    }

    std::string host() {
        return "rest.sk";
    }

    std::string address_from() {
        return "from@me.to";
    }
};


// The fixture for testing class Foo.
class rest_api_mailgate_test: public ::testing::Test {

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

TEST_F(rest_api_mailgate_test, send_no_config) {
    std::shared_ptr<mock_rest_client> mock_client = std::make_shared<mock_rest_client>();
    rvision::rest_api_mailgate<mock_rest_client,rvision::config_file_mailgate> rest_api_mailgate(mock_client);

    try {
        rest_api_mailgate.send_mail("me@to.you","subject", "msg text");
        FAIL();
    } catch (rvision::config_error &ce) { }
}

TEST_F(rest_api_mailgate_test, send_config_missing_to_address) {
    std::shared_ptr<mock_rest_client> mock_client = std::make_shared<mock_rest_client>();
    rvision::rest_api_mailgate<mock_rest_client,mock_config_file> rest_api_mailgate(mock_client);
    std::string empty;

    try {
        rest_api_mailgate.configure("./confi.conf");
        rest_api_mailgate.send_mail(empty, "lala", "nanan");
        FAIL();
    } catch (std::invalid_argument &ia) { }
}