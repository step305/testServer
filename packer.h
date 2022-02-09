//
// Created by sanch on 04.01.2022.
//

#ifndef TESTSERVER_PACKER_H
#define TESTSERVER_PACKER_H

#include "data_types.h"
#include <memory>
#include <string>
#include <stdexcept>
#include <stdio.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/opencv.hpp>
#include "base64.h"

#include <iomanip>
#include <fcntl.h>
#include <unistd.h>
#include "thread"
#include "chrono"
#include <iostream>
#include "chrono"
#include <sys/stat.h>
#include <stdio.h>
#include <signal.h>

void pack_data(T_report_packet data,
               std::string & json);

#endif //TESTSERVER_PACKER_H
