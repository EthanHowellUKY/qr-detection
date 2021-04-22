
#ifndef BAVQR_H
#define BAVQR_H

#pragma once

#include <qrencode.h>
#include <png.h>
#include <string>
#include <iostream>

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
    void encode_qr(const std::string &to_encode);
    void create_png(const std::string &file_name);

private:
    // -------------------------------- // 
	//        PRIVATE FUNCTIONS         //
	// -------------------------------- //
    void write_margin();
    void do_exit(const std::string &msg, const int &code);
    void open_png(const std::string &file_name);
    void close_png();

    // -------------------------------- // 
	//        PRIVATE VARIABLES         //
	// -------------------------------- //
    FILE *m_fp;
    QRcode *m_qr;
    png_structp m_png_ptr = NULL;
    png_infop m_info_ptr = NULL;
    unsigned char *m_row, *m_q, *m_p;

    int m_real_width, m_bit;
    int m_size = 5;
    int m_margin = 1;
    int m_dpi = 72;

    std::string m_toencode;
    std::string m_filename;
    bool        m_qr_isnull = true;
};

#endif // BAVQR_H
