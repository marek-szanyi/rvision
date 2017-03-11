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

#include "test_data_config.h"

#include "../src/notifier_sms.h"
#include "../src/config_error.h"

class notifier_sms_file_conf_test : public ::testing::Test {

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

TEST_F(notifier_sms_file_conf_test, missing_or_invalid_config) {
    try {
        rvision::notifier_sms_file_conf file_conf;
        file_conf.read("/blalbla.conf");
        FAIL();
    } catch (rvision::config_error &e) {

    }
}

TEST_F(notifier_sms_file_conf_test, multiple_phone_numbers) {
    rvision::notifier_sms_file_conf file_conf;
    file_conf.read(std::string(TEST_DIR) + "/sms_file.conf");
    auto phones = file_conf.telephone_numbers();
    EXPECT_EQ(2, phones.size());
}