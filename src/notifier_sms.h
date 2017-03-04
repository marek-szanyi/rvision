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
#ifndef RVISION_NOTIFIER_SMS_H
#define RVISION_NOTIFIER_SMS_H

#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>
#include <boost/program_options.hpp>

#include "observer.h"
#include "notificator.h"


namespace rvision {

    namespace po = boost::program_options;

    class notifier_sms_file_conf {
    public:
        notifier_sms_file_conf()
                : m_config_desc(po::options_description("Configuration for sms notifier")),
                  m_config_map(po::variables_map()) {

            m_config_desc.add_options()
                    ("min_interval", po::value<size_t>(), "minimal interval between notifications")
                    ("telnums", po::value<std::string>(), "phone numbers to notify")
                    ("msg", po::value<std::string>(), "optional message of the notification");
        }

        void read(const boost::filesystem::path &config) {
            po::store(po::parse_config_file<char>(config.generic_string().c_str(), m_config_desc), m_config_map);
            po::notify(m_config_map);
        }

        size_t min_interval() {
            return m_config_map["min_interval"].as<size_t>();
        }

        const std::vector<std::string> &telephone_numbers() {
            if (m_phone_numbers.empty()) {
                using tokenizer = boost::tokenizer<boost::char_separator<char>>;
                boost::char_separator<char> sep(",");
                tokenizer tokens(m_config_map["telnums"].as<std::string>(), sep);

                for (auto token_iter = tokens.begin(); token_iter != tokens.end(); ++token_iter) {
                    m_phone_numbers.push_back(*token_iter);
                }
            }
            return m_phone_numbers;
        }

        std::string msg() {
            return m_config_map["msg"].as<std::string>();
        }


    private:
        boost::program_options::options_description m_config_desc;
        boost::program_options::variables_map m_config_map;
        std::vector<std::string> m_phone_numbers;
    };


    template<typename Tsmsender, typename Tconfig, typename Tvalue>
    class notifier_sms : public observer<Tvalue> {
    public:

        notifier_sms(std::shared_ptr<Tsmsender> sms_sender)
                : m_sms_sender(sms_sender), m_config(Tconfig()),
                  m_last_notif_time(std::chrono::steady_clock::time_point()) {

        }

        void configure(const boost::filesystem::path &config) {
            m_config.read(config);
        }

        void update(const Tvalue &value) override {
            if (allow_update()) {
                for (auto &&telnum : m_config.telephone_numbers()) {
                    m_sms_sender->send_sms(telnum, m_config.msg());
                }
                m_last_notif_time = std::chrono::steady_clock::now();
            }
        }

    private:
        std::shared_ptr<Tsmsender> m_sms_sender;
        Tconfig m_config;
        std::chrono::steady_clock::time_point m_last_notif_time;

        bool allow_update() {
            bool result = false;
            if (m_config.min_interval() != 0) {
                auto time_now = std::chrono::steady_clock::now();
                auto new_interval = std::chrono::duration_cast<std::chrono::milliseconds>(
                        time_now - m_last_notif_time).count();
                if (new_interval >= m_config.min_interval()) {
                    result = true;
                }
            }

            return result;
        }
    };
}


#endif //RVISION_NOTIFIER_SMS_H
