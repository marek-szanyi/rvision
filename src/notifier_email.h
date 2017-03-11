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
#ifndef RVISION_NOTIFIER_EMAIL_H
#define RVISION_NOTIFIER_EMAIL_H

#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>
#include <boost/program_options.hpp>

#include "observer.h"
#include "notifiers.h"
#include "config_error.h"

namespace rvision {

    namespace po = boost::program_options;

    /**
     * @brief a class providing configuration which is obtained from a file
     */
    class notifier_email_file_conf {
    public:
        /**
         * @brief constructor
         */
        notifier_email_file_conf()
                : m_config_desc(po::options_description("Configuration for email notifier")),
                  m_config_map(po::variables_map()) {

            m_config_desc.add_options()
                    ("emails", po::value<std::string>()->required(), "comma separated emails to notify")
                    ("attachment_extra", po::value<bool>(), "send extra data as attachment")
                    ("subject", po::value<std::string>()->required(), "subject of the mail")
                    ("msg", po::value<std::string>(), "optional message of the notification");
        }

        /**
         * @brief read the configuration data from file
         * @param config full path to the config file
         * @throws rvision::config_error in case of invalid or missing file
         */
        void read(const boost::filesystem::path &config) {
            try {
                po::store(po::parse_config_file<char>(config.generic_string().c_str(), m_config_desc), m_config_map);
                po::notify(m_config_map);
            } catch (po::error &e) {
                throw rvision::config_error(e.what());
            }
        }

        /**
         * @return vector of email addresses
         */
        const std::vector<std::string> &emails() {
            if (m_emails.empty()) {
                using tokenizer = boost::tokenizer<boost::char_separator<char>>;
                boost::char_separator<char> sep(",");
                tokenizer tokens(m_config_map["emails"].as<std::string>(), sep);

                for (auto token_iter = tokens.begin(); token_iter != tokens.end(); ++token_iter) {
                    m_emails.push_back(*token_iter);
                }
            }
            return m_emails;
        }

        /**
         * @return true if extra data should be sent as attachment, false otherwise
         */
        bool attachment_extra() {
            return m_config_map["attachment_extra"].as<bool>();
        }

        /**
         * @return body of the email
         */
        std::string email_text() {
            return m_config_map["msg"].as<std::string>();
        }

        /**
         * @return the subject of the mail
         */
        std::string email_subject() {
            return m_config_map["subject"].as<std::string>();
        }

    private:
        boost::program_options::options_description m_config_desc;
        boost::program_options::variables_map m_config_map;
        std::vector<std::string> m_emails;
    };

    /**
     * @brief class email notifications about events
     * @tparam Temailer class providing the functionality for sending emails
     * @tparam Tconfig class providing configuration
     * @tparam Tvalue class holding information about the event
     */
    template<typename Temailer, typename Tconfig, typename Tvalue>
    class notifier_email : public observer<Tvalue> {
    public:

        /**
         * @brief constructor
         * @param email_sender a shared instance of the mail sender
         */
        notifier_email(std::shared_ptr<Temailer> email_sender) : m_email_sender(email_sender), m_config(Tconfig()) {
        }

        /**
         * Get the configuration
         * @return configuration class
         */
        Tconfig &configure() {
            return m_config;
        }

        /**
         * @brief callback for the event
         * @param value information about the event
         */
        void update(const Tvalue &value) override {
            std::string atch;
            if (m_config.attachment_extra()) {
                atch = value.to_b64();
            }

            for (auto &&email_addr : m_config.emails()) {
                m_email_sender->send_mail(email_addr, m_config.email_subject(), m_config.email_text(), atch);
            }
        }

    private:
        std::shared_ptr<Temailer> m_email_sender;
        Tconfig m_config;
    };

}

#endif //RVISION_NOTIFIER_EMAIL_H
