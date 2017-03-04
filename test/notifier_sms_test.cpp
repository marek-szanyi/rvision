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
#include <iostream>
#include <chrono>
#include <thread>
#include "gtest/gtest.h"


#include "../src/notifier_sms.h"


class mock_sms_sender {
public:
    size_t m_send_counter;

    bool send_sms(const std::string &address, const std::string &msg) {
        std::cout << " ~~~~~~~~~~~~ " << address << " ~~~~~~~~~ \n";
        ++m_send_counter;
        return true;
    }
};

class mock_notifier_config {
public:

    mock_notifier_config() {
        m_phone_numbers = {"+4219084545", "+421645454546"};
    }

    void read(const boost::filesystem::path &config) {
    }


    size_t min_interval() {
        return 1000;
    }

    std::vector<std::string> m_phone_numbers;

    const std::vector<std::string> &telephone_numbers() {
        return m_phone_numbers;
    }

    std::string msg() {
        return "test msg";
    }
};

// The fixture for testing class Foo.
class notifier_sms_test : public ::testing::Test {

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

TEST_F(notifier_sms_test, simple_notify) {
    std::shared_ptr<mock_sms_sender> mock_sender = std::make_shared<mock_sms_sender>();
    rvision::notifier_sms<mock_sms_sender, mock_notifier_config, int> notifier_sms(mock_sender);

    mock_sender->m_send_counter = 0;
    notifier_sms.update(123);
    EXPECT_EQ(2, mock_sender->m_send_counter);
}

TEST_F(notifier_sms_test, interval_check) {
    std::shared_ptr<mock_sms_sender> mock_sender = std::make_shared<mock_sms_sender>();
    rvision::notifier_sms<mock_sms_sender, mock_notifier_config, int> notifier_sms(mock_sender);

    mock_sender->m_send_counter = 0;

    notifier_sms.update(123);
    notifier_sms.update(123);
    notifier_sms.update(123);
    notifier_sms.update(123);
    EXPECT_EQ(2, mock_sender->m_send_counter);
}

TEST_F(notifier_sms_test, interval_check_wait) {
    std::shared_ptr<mock_sms_sender> mock_sender = std::make_shared<mock_sms_sender>();
    rvision::notifier_sms<mock_sms_sender, mock_notifier_config, int> notifier_sms(mock_sender);

    mock_sender->m_send_counter = 0;

    notifier_sms.update(123);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    notifier_sms.update(123);
    EXPECT_EQ(4, mock_sender->m_send_counter);
}
