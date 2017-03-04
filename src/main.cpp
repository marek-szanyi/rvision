#include <iostream>
#include <functional>
#include <chrono>
#include <iomanip>
#include <ctime>
#include "motion_engine.h"
#include "notificator_builder.h"


#include "rest_client.h"
#include "rest_api_123sms.h"
#include "notifier_sms.h"
#include "rvision_types.h"


int main() {
//    rvision::motion_engine motion_engine;
//    motion_engine.init();
//    auto callback = [](cv::Mat result) {
//        auto t = std::time(nullptr);
//        auto tm = *std::localtime(&t);
//        std::cout << "MOTION   " << tm.tm_sec << std::endl;
//    };
//    motion_engine.start(callback);
//    while(1) { std::cin;}
    /*  using rest_client_123 = rvision::rest_client<rvision::rest_api_123sms>;
      rvision::sms_sender<rest_client_123> sms_sender;
      sms_sender.init("./sms_rest_client.cfg");
      sms_sender.send("421908645412", "fuck the motion");

      using sms_sender_123 = rvision::sms_sender<rest_client_123>;

      std::shared_ptr<sms_sender_123> shared_sender = std::make_shared<sms_sender_123>();

      rvision::notifier_sms<sms_sender_123, rvision::notifier_sms_config, int> sms_notifier(shared_sender);

      sms_notifier.configure("./sms_rest_client.cfg");*/

    //auto notificator = rvision::notificator_builder::build_from_file_config(". / sms_rest_client.cfg");
//    rvision::notificator_builder_fileconf builder;
//    auto notificator = builder.make("./sms_rest_client.cfg");
    return 0;
}