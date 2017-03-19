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

#include "rvision_core.h"
#include "rvision_types.h"

using namespace rvision;


rvision_core::rvision_core()
        : m_notifiers(nullptr),
          m_motion_engine(nullptr),
          m_rest_api_factory(rest_api_factory()) {

    using sms_notifier = rvision::notifier_sms<sender_sms_rest, rvision::notifier_sms_file_conf, rvision::event>;
    notifiers_event_register<sender_sms_rest> __sms_sender_registered(
            [=](boost::filesystem::path conf) -> rvision::observer<rvision::event> * {
                try {
                    auto sms_sender = m_rest_api_factory.create<rvision::sender_sms_rest>();
                    sms_notifier *notifier = new sms_notifier(sms_sender);
                    notifier->configure().read(conf);

                    return dynamic_cast<rvision::observer<rvision::event> *>(notifier);
                } catch (rvision::config_error &e) {
                    return nullptr;
                }
            }
    );

//    notifiers_event_register<sender_mail_rest> __mail_sender_registered(
//            [](boost::filesystem::path conf) -> rvision::observer<rvision::event>* {
//
//            }
//    );

}

void rvision_core::init(const boost::filesystem::path &config_file) {
    m_notifiers = rvision::notifiers_builder_fileconf::make(config_file);
    if (m_notifiers->count() == 0) {
        throw invalid_core_state("no registered notifiers!");
    }
    try {
        m_motion_engine.reset(new rvision::motion_engine());
        m_motion_engine->init();
    } catch (std::runtime_error &er) {
        throw invalid_core_state("motion engine initialization failed!");
    }
}

void rvision_core::run() {
    m_motion_engine->start([=](cv::Mat img) {
        try {
            event new_event;
            rvision::engine_tools::to_raw_png(img, new_event.data());
            m_notifiers->notify(new_event);
        } catch (std::runtime_error &re) {
            m_motion_engine->stop();
            throw core_error(re.what());
        }
    });
}

void rvision_core::finish() {
    m_motion_engine->stop();
}