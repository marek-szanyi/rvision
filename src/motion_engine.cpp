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
#include "motion_engine.h"
#include <opencv/cv.hpp>

using namespace rvision;

motion_detector_simple::motion_detector_simple()
        : m_result(cv::Mat()),
          m_prev_frame(cv::Mat()),
          m_current_frame(cv::Mat()),
          m_next_frame(cv::Mat()),
          m_d1(cv::Mat()),
          m_d2(cv::Mat()),
          m_motion(cv::Mat()),
          m_kernel_ero(cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2))),
          m_number_of_changes(0),
          m_number_of_sequence(0) {}

int motion_detector_simple::detection_algo(const cv::Mat &motion, cv::Mat &result, int x_start, int x_stop, int y_start,
                                           int y_stop, int max_deviation) {
    // calculate the standard deviation
    cv::Scalar mean, stddev;
    meanStdDev(motion, mean, stddev);
    // if not to much changes then the motion is real (neglect agressive snow, temporary sunlight)
    if (stddev[0] < max_deviation) {
        int number_of_changes = 0;
        int min_x = motion.cols, max_x = 0;
        int min_y = motion.rows, max_y = 0;
        // loop over image and detect changes
        for (int j = y_start; j < y_stop; j += 2) { // height
            for (int i = x_start; i < x_stop; i += 2) { // width
                // check if at pixel (j,i) intensity is equal to 255
                // this means that the pixel is different in the sequence
                // of images (prev_frame, current_frame, next_frame)
                if (static_cast<int>(motion.at<uchar>(j, i)) == 255) {
                    number_of_changes++;
                    if (min_x > i) min_x = i;
                    if (max_x < i) max_x = i;
                    if (min_y > j) min_y = j;
                    if (max_y < j) max_y = j;
                }
            }
        }
        if (number_of_changes) {
            //check if not out of bounds
            if (min_x - 10 > 0) min_x -= 10;
            if (min_y - 10 > 0) min_y -= 10;
            if (max_x + 10 < result.cols - 1) max_x += 10;
            if (max_y + 10 < result.rows - 1) max_y += 10;
            // draw rectangle round the changed pixel
        }
        return number_of_changes;
    }
    return 0;
}

bool motion_detector_simple::detect(const std::unique_ptr<cv::VideoCapture> &m_video_capture, cv::Mat &result) {

    *m_video_capture >> m_prev_frame;
    *m_video_capture >> m_current_frame;
    *m_video_capture >> m_next_frame;

    cv::cvtColor(m_next_frame, m_next_frame, CV_RGB2GRAY);
    cv::cvtColor(m_current_frame, m_current_frame, CV_RGB2GRAY);
    cv::cvtColor(m_prev_frame, m_prev_frame, CV_RGB2GRAY);

    m_prev_frame = m_current_frame;
    m_current_frame = m_next_frame;
    *m_video_capture >> m_next_frame;

    cv::cvtColor(m_next_frame, m_next_frame, CV_RGB2GRAY);

    cv::absdiff(m_prev_frame, m_next_frame, m_d1);
    cv::absdiff(m_next_frame, m_current_frame, m_d2);
    cv::bitwise_and(m_d1, m_d2, m_motion);
    cv::threshold(m_motion, m_motion, 35, 255, CV_THRESH_BINARY);
    cv::erode(m_motion, m_motion, m_kernel_ero);

    m_number_of_changes = detection_algo(m_motion, result, 10, m_current_frame.cols - 11, 350, 530, 20);
    if (m_number_of_changes >= 5) {
        result = m_current_frame;
        return true;
    }

    return false;
}

motion_engine::motion_engine() : m_video_capture(new cv::VideoCapture()), m_workthread(nullptr),
                                 m_worker_running(false) {}

void motion_engine::init() {
    if (!m_video_capture->open(0)) {
        throw std::runtime_error("failed to init webcam");
    }

//    if(!m_video_capture->set(cv::CAP_PROP_FRAME_WIDTH,640)) {
//        throw std::runtime_error("failed to set frame width");
//    }
//
//    if(!m_video_capture->set(cv::CAP_PROP_FRAME_HEIGHT,420)) {
//        throw std::runtime_error("failed to set frame height");
//    }
}

void motion_engine::start(std::function<void(cv::Mat)> on_motion_detected) {
    using std::placeholders::_1;
    auto fbind = std::bind(&motion_engine::worker_func, this, _1);
    m_workthread.reset(new std::thread(fbind, on_motion_detected));
}

void motion_engine::stop() {
    m_worker_running = false;
}

void motion_engine::worker_func(std::function<void(cv::Mat)> on_motion_detected) {
    m_worker_running = true;
    motion_detector_simple simple_detect;
    cv::Mat result;
    while (m_worker_running) {
        if (simple_detect.detect(m_video_capture, result)) {
            on_motion_detected(result);
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(500ms);
        }
    }
}
