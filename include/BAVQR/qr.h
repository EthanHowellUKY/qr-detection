
#ifndef BAVQR_H
#define BAVQR_H

#pragma once

#include <opencv2/objdetect.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc.hpp"
#include <zbar.h>
#include <qrencode.h>
#include <png.h>
#include <string>
#include <iostream>
#include <vector>

#define INCHES_PER_METER (100.0/2.54)

class QR
{
public:
    // -------------------------------- // 
	//           CONSTRUCTORS           //
	// -------------------------------- //
    QR();

    // -------------------------------- // 
	//            DESTRUCTOR            //
	// -------------------------------- //
    ~QR();

    // -------------------------------- // 
	//         PUBLIC FUNCTIONS         //
	// -------------------------------- //
    std::string decode_qr(cv::Mat &to_decode);
    std::string decode_qr(const std::string &img);
    void read_qr();
    void encode_qr(const std::string &to_encode);
    void create_png(const std::string &file_name);

    // ------------ GETTERS ----------- //
    std::string to_encode() const;
    std::string decoded_data() const;

    // ------------ SETTERS ----------- //
    void set_camera(const int &camera_id);

private:
    // -------------------------------- // 
	//        PRIVATE FUNCTIONS         //
	// -------------------------------- //
    void write_margin();
    void do_exit(const std::string &msg, const int &code);
    void open_png(const std::string &file_name);
    void close_png();
    void decode(cv::Mat &im);
    void display(cv::Mat &im);

    // -------------------------------- // 
	//        PRIVATE VARIABLES         //
	// -------------------------------- //
    typedef struct
    {
        std::string type;
        std::string data;
        std::vector<cv::Point> location;
    } DecodedObject;
    
    // File operation objects//
    FILE *m_fp;
    png_structp m_png_ptr = NULL;
    png_infop m_info_ptr = NULL;

    // QR Objects
    QRcode *m_qr;
    cv::QRCodeDetector m_decoder;
    zbar::ImageScanner m_scanner;

    // MISC Members
    unsigned char *m_row, *m_q, *m_p;

    int m_real_width, m_bit;
    int m_size = 5;
    int m_margin = 1;
    int m_dpi = 72;
    int m_camera = 0; // Rear camera
    int m_api_id = cv::CAP_ANY;

    DecodedObject   m_decoded;
    std::string     m_toencode;
    std::string     m_filename;
    bool            m_qr_isnull = true;

    double          m_delay = 31.25; // 32 fps
};

#endif // BAVQR_H
