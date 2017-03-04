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
#include "notificator.h"
#include "notificator_builder.h"
#include "event.h"

namespace rvision {

//    using notifier_sms_123 = rvision::notifier_sms<rvision::rest_api_123sms, rvision::notifier_sms_file_conf, rvision::event>;
//
//    using notifier_email_pop = rvision::notifier_email<rvision::rest_api_mailgate, rvision::notifier_email_file_conf, rvision::event>;
//
//    using notificator_builder_fileconf = rvision::notificator_builder::builder<rvision::event,boost::filesystem::path>;
//
//    template<typename T>
//    using register_fileconf_notifier = rvision::notificator_builder::register_notifier<T, rvision::event,boost::filesystem::path>;
//
//    register_fileconf_notifier<rvision::notifier_sms_123> _notifier_sms_123_registered([](boost::filesystem::path conf) -> rvision::observer<rvision::event>*
//                                                                                       {
//
//    });
}

#endif //RVISION_RVISION_TYPES_H
