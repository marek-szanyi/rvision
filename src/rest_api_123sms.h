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
#ifndef RVISION_REST_API_123SMS_H
#define RVISION_REST_API_123SMS_H

#include <string>
#include <memory>
#include <map>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include "rest_client.h"
#include "config_error.h"

namespace rvision {

    namespace po = boost::program_options;

    class config_file_123sms {
    public:
        config_file_123sms() : m_config_desc(po::options_description("Configuration for 123SMS REST API")),
                               m_config_map(po::variables_map()) {
            m_config_desc.add_options()
                    ("login", po::value<std::string>()->default_value(std::string()), "login to 123sms service provider")
                    ("api_key", po::value<std::string>()->default_value(std::string()), "the api key associated with the login")
                    ("base_url", po::value<std::string>(), "base url for all REST calls")
                    ("host", po::value<std::string>(), "REST API host")
                    ("sender_name", po::value<std::string>()->default_value(std::string()), "name shown as sender in the sms messages");
        }

        void read(const boost::filesystem::path &config) {
            po::store(po::parse_config_file<char>(config.generic_string().c_str(), m_config_desc), m_config_map);
            po::notify(m_config_map);
        }

        std::string api_key() const {
            try {
                return m_config_map["api_key"].as<std::string>();
            } catch (boost::bad_any_cast &bc) {
                throw config_error("api_key");
            }
        }

        std::string base_url() const {
            try {
                return m_config_map["base_url"].as<std::string>();
            } catch (boost::bad_any_cast &bc) {
                throw config_error("base_url");
            }
        }

        std::string login() const {
            try {
                return m_config_map["login"].as<std::string>();
            } catch (boost::bad_any_cast &bc) {
                throw config_error("login");
            }
        }

        std::string sender_name() const {
            try {
                return m_config_map["sender_name"].as<std::string>();
            } catch (boost::bad_any_cast &bc) {
                throw config_error("sender_name");
            }
        }

        std::string host() const {
            try {
                return m_config_map["host"].as<std::string>();
            } catch (boost::bad_any_cast &bc) {
                throw config_error("host");
            }
        }

    private:
        boost::program_options::options_description m_config_desc;
        boost::program_options::variables_map m_config_map;
    };

    template<typename TClient, typename TConfig>
    class rest_api_123sms {
    public:
        rest_api_123sms(std::shared_ptr<TClient> rest_client)
                : m_config(), m_rest_client(rest_client) {
        }

        TConfig &configure() {
            return m_config;
        }


        bool send_sms(const std::string &address, const std::string &msg) {
            if (address.empty()) {
                throw std::invalid_argument("address");
            }

            std::multimap<std::string, std::string> values;
            values.emplace("recipient", address);
            values.emplace("message", msg);
            values.emplace("username", m_config.login());
            values.emplace("password", m_config.api_key());
            values.emplace("sender", m_config.sender_name());
            try {
                m_rest_client->request_post(m_config.base_url(), m_config.host(), values);
            } catch (...) {
                return false;
            }

            return true;
        }

    private:
        TConfig m_config;
        std::shared_ptr<TClient> m_rest_client;
    };
}


#endif //RVISION_REST_API_123SMS_H
