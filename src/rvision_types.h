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
#ifndef RVISION_RVISION_TYPES_H
#define RVISION_RVISION_TYPES_H

#include <boost/filesystem.hpp>

#include "rest_client.h"
#include "rest_api_123sms.h"
#include "rest_api_mailgate.h"
#include "notifier_sms.h"
#include "notifier_email.h"
#include "notifiers.h"
#include "notifiers_builder.h"
#include "event.h"

namespace rvision {

    /**
     * @brief send SMS messages by using a REST service(123sms), configured from a file
     */
    using sender_sms_rest = rest_api_123sms<rest_client,config_file_123sms>;

    /**
     * @brief send e-mails by using a REST service(mailgate), configured from a file
     */
    using sender_mail_rest = rest_api_mailgate<rest_client,config_file_mailgate>;

    /**
     * @brief a builder for creating notifiers which are configured from config files
     */
    using notifiers_builder_fileconf = rvision::notifiers_builder::builder<rvision::event,boost::filesystem::path>;

    /**
     * @brief register of notifiers which are configured from config files
     */
    template<typename T, typename K>
    using notifiers_register_fileconf = rvision::notifiers_builder::register_notifier<T, K, boost::filesystem::path>;

    /**
     * @brief register of notifiers which are notifying rvision::event and are configured from files
     */
    template<typename T>
    using notifiers_event_register = notifiers_register_fileconf<T, event>;

}

#endif //RVISION_RVISION_TYPES_H
