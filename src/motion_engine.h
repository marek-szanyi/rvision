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
#ifndef RVISION_MOTION_ENGINE_H
#define RVISION_MOTION_ENGINE_H

#include <memory>
#include <vector>
#include <functional>
#include <thread>
#include <atomic>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

namespace rvision {

    class motion_engine {
    public:
        motion_engine();

        void init();

        void start(std::function<void(cv::Mat)> on_motion_detected);

        void start_async(std::function<void(cv::Mat)> on_motion_detected);

        void stop();

    private:
        void worker_func(std::function<void(cv::Mat)> on_motion_detected);


    private:
        std::unique_ptr<cv::VideoCapture> m_video_capture;
        std::unique_ptr<std::thread> m_workthread;
        std::atomic_bool m_worker_running;
    };

    class motion_detector_simple {
    public:
        motion_detector_simple();

        bool detect(const std::unique_ptr<cv::VideoCapture> &m_video_capture, cv::Mat &result);

        int detection_algo(const cv::Mat &motion, cv::Mat &result, int x_start, int x_stop, int y_start, int y_stop,
                           int max_deviation);

    private:
        cv::Mat m_result;
        cv::Mat m_prev_frame;
        cv::Mat m_current_frame;
        cv::Mat m_next_frame;
        cv::Mat m_d1, m_d2, m_motion;
        cv::Mat m_kernel_ero;
        int m_number_of_changes, m_number_of_sequence = 0;
    };

    namespace engine_tools {
        void to_raw_png(const cv::Mat &input, std::vector<uchar> &output);
    }
}


#endif //RVISION_MOTION_ENGINE_H
