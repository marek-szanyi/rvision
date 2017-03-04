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
#include "gtest/gtest.h"

#include "../src/rest_api_123sms.h"
#include "../src/config_error.h"

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

class mock_config {
public:

    void read(const boost::filesystem::path &config) {

    }

    std::string api_key() const {
       return "asdskfdf12";
    }

    std::string base_url() const {
       return "http://rest.sk";
    }

    std::string login() const {
        return "test";
    }

    std::string sender_name() const {
        return "test coder";
    }

    std::string host() const {
       return "rest.sk";
    }
};


// The fixture for testing class Foo.
class rest_api_123sms_test: public ::testing::Test {

protected:

    // Code here will be called immediately after the constructor (right
    // before each test).
    void SetUp() override {

    }

    // Code here will be called immediately after each test (right
    // before the destructor).
    void TearDown() override {

    }

    mock_rest_client m_mock_rest_Client;
};

TEST_F(rest_api_123sms_test, sending_no_config) {
    std::shared_ptr<mock_rest_client> rest_client = std::make_shared<mock_rest_client>();
    rvision::rest_api_123sms<mock_rest_client,rvision::config_file_123sms> rest_api_123sms(rest_client);
    try {
        rest_api_123sms.send_sms("lala", "test");
        FAIL();
    } catch (rvision::config_error &ce) { }
}

TEST_F(rest_api_123sms_test, sending_with_config_no_address) {
    std::shared_ptr<mock_rest_client> rest_client = std::make_shared<mock_rest_client>();
    rvision::rest_api_123sms<mock_rest_client,mock_config> rest_api_123sms(rest_client);
    std::string empty;

    try {
        rest_api_123sms.configure("/conf.conf");
        rest_api_123sms.send_sms(empty, "hello world");
        FAIL();
    } catch (std::invalid_argument &ia) { }
}

TEST_F(rest_api_123sms_test, sending_with_conf_and_address) {
    std::shared_ptr<mock_rest_client> rest_client = std::make_shared<mock_rest_client>();
    rvision::rest_api_123sms<mock_rest_client,mock_config> rest_api_123sms(rest_client);

    rest_api_123sms.configure("/conf.conf");
    rest_api_123sms.send_sms("+421905554123", "hello world");
}