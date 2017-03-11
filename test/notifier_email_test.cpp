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

#include "gtest/gtest.h"
#include <iostream>
#include <memory>

#include "../src/notifier_email.h"

class mock_email_sender {
public:
    std::string m_attachment;

    bool send_mail(
            const std::string to,
            const std::string subject,
            const std::string text,
            const std::string &attachment) {
        std::cout << " ~~~~~~~~~~~~ " << to << " ~~~~~~~~~ \n";
        std::cout << " ~~~~~~~~~~~~ " << attachment << " ~~~~~~~~~ \n";
        m_attachment = attachment;
    }
};

class mock_notifier_config_attch_enabled {
public:

    std::vector<std::string> m_emails;

    mock_notifier_config_attch_enabled() {
        m_emails = {"me@to.you", "you@to.me"};
    }

    void read(const boost::filesystem::path &config) {
    }


    const std::vector<std::string> &emails() {
        return m_emails;
    }

    bool is_attachment_extra_enabled = true;

    bool attachment_extra() {
        return is_attachment_extra_enabled;
    }

    std::string email_text() {
        return "mock mail text";
    }

    std::string email_subject() {
        return "mock subject";
    }
};

class mock_notifier_config {
public:

    std::vector<std::string> m_emails;

    mock_notifier_config() {
        m_emails = {"me@to.you", "you@to.me"};
    }

    void read(const boost::filesystem::path &config) {
    }


    const std::vector<std::string> &emails() {
        return m_emails;
    }

    bool is_attachment_extra_enabled = true;

    bool attachment_extra() {
        return is_attachment_extra_enabled;
    }

    std::string email_text() {
        return "mock mail text";
    }

    std::string email_subject() {
        return "mock subject";
    }
};

// The fixture for testing class notifier_email.
class notifier_email_test : public ::testing::Test {

protected:

    // Code here will be called immediately after the constructor (right
    // before each test).
    void SetUp() override {
        m_mock_sender.reset(new mock_email_sender());
    }

    // Code here will be called immediately after each test (right
    // before the destructor).
    void TearDown() override {

    }

    std::shared_ptr<mock_email_sender> m_mock_sender;
};

class mock_notification_value {
public:

    const std::string to_b64() const {
        return "this was mocked";
    }
};

TEST_F(notifier_email_test, test_sending_attachment) {
    rvision::notifier_email<mock_email_sender, mock_notifier_config, mock_notification_value> notifier_email(
            m_mock_sender);
    mock_notification_value avalue;
    notifier_email.configure().is_attachment_extra_enabled = true;
    notifier_email.update(avalue);
    EXPECT_EQ(avalue.to_b64(), m_mock_sender->m_attachment);
}

TEST_F(notifier_email_test, test_not_sending_attachment) {
    rvision::notifier_email<mock_email_sender, mock_notifier_config, mock_notification_value> notifier_email(
            m_mock_sender);
    mock_notification_value avalue;
    notifier_email.configure().is_attachment_extra_enabled = false;
    notifier_email.update(avalue);
    EXPECT_EQ("", m_mock_sender->m_attachment);
}