//
// Created by sanch on 04.01.2022.
// https://learnopencv.com/install-opencv-4-on-ubuntu-18-04/
// https://learnopencv.com/install-opencv-4-on-ubuntu-18-04/
//

#include "packer.h"

typedef unsigned char uchar;

template<typename ... Args>
std::string string_format( const std::string& format, Args ... args )
{
    int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    auto size = static_cast<size_t>( size_s );
    auto buf = std::make_unique<char[]>( size );
    std::snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}



void pack_data(T_report_packet data,
               std::string & json) {
    cv::Mat frame;
    frame = cv::imread("test.jpg", cv::IMREAD_COLOR);

    std::vector<uchar> buf;
    cv::Mat resized_down;
    cv::resize(frame, resized_down, cv::Size(640, 480), cv::INTER_LINEAR);
    frame = resized_down;
    cv::imencode(".jpg", frame, buf);
    auto *encoded_frame = reinterpret_cast<unsigned char*>(buf.data());
    std::string encoded_frame_str = base64_encode(encoded_frame, buf.size());

    json = "{";
    json = string_format("%s \"adc\": %0.2f,", json.c_str(), data.adc);
    json = string_format("%s \"yaw\": %0.2f,", json.c_str(), data.euler[0]);
    json = string_format("%s \"pitch\": %0.2f,", json.c_str(), data.euler[1]);
    json = string_format("%s \"roll\": %0.2f,", json.c_str(), data.euler[2]);
    json = string_format("%s \"frame\": \"%s\"", json.c_str(), encoded_frame_str.c_str());
    json = string_format("%s}", json.c_str());
    json = string_format("%20ld :: %s", strlen(json.c_str()), json.c_str());
}
