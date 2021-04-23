
#include "BAVQR/qr.h"

// -------------------------------- // 
//        PRIVATE FUNCTIONS         //
// -------------------------------- //
void QR::write_margin()
{
    memset(m_row, 0xff, (m_real_width + 7) / 8);
    for (int ii=0; ii<m_margin*m_size; ii++)
    {
        png_write_row(m_png_ptr, m_row);
    }
}

void QR::do_exit(const std::string &msg, const int &code)
{
    std::cout << msg << '\n';
    if (m_fp != NULL) { fclose(m_fp); }
    if (m_row != NULL) { free(m_row); }
    QRcode_free(m_qr);
    exit(code);
}

void QR::open_png(const std::string &file_name)
{
    // Initialize write structure
    m_png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (m_png_ptr == NULL) { do_exit("Could not initialize PNG ptr", -2); }

    // Initialize info structure
    m_info_ptr = png_create_info_struct(m_png_ptr);
    if (m_info_ptr == NULL) { do_exit("Could not initialize info_ptr", -3); }

    // Exception handler
    if (setjmp(png_jmpbuf(m_png_ptr))) { png_destroy_write_struct(&m_png_ptr, &m_info_ptr); do_exit("Could not set up Exception handling", 1); }

    m_fp = fopen(file_name.c_str(), "wb");
    if (m_fp == NULL) { do_exit("Could not open file: " + file_name, -4); }

    png_init_io(m_png_ptr, m_fp);

    // Write header (8 bit colour depth)
    png_set_IHDR(m_png_ptr, m_info_ptr, m_real_width, m_real_width,
            1, PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    png_set_pHYs(m_png_ptr, m_info_ptr,
        m_dpi * INCHES_PER_METER,
        m_dpi * INCHES_PER_METER,
        PNG_RESOLUTION_METER);

    png_write_info(m_png_ptr, m_info_ptr);
}

void QR::close_png()
{
    if (m_png_ptr != NULL && m_info_ptr != NULL)
    {
        png_write_end(m_png_ptr, m_info_ptr);
        png_destroy_write_struct(&m_png_ptr, &m_info_ptr);
    }
    if (m_fp != NULL) { fclose(m_fp); }
    if (m_row != NULL) { free(m_row); }
    if (!m_qr_isnull) { QRcode_free(m_qr); m_qr_isnull = true; }
}

// -------------------------------- // 
//           CONSTRUCTORS           //
// -------------------------------- //
QR::QR() {}

// -------------------------------- // 
//            DESTRUCTOR            //
// -------------------------------- //
QR::~QR() { }

// -------------------------------- // 
//         PUBLIC FUNCTIONS         //
// -------------------------------- //
std::string QR::decode_qr(const cv::Mat &to_decode) { return m_decoder.detectAndDecode(to_decode); }
std::string QR::decode_qr(const std::string &img)
{
    cv::Mat input_image = cv::imread(img);
    return decode_qr(input_image);
}

void QR::read_qr()
{
    cv::Mat frame;
    cv::VideoCapture cap;
    cap.open(m_camera, m_api_id);
    if (!cap.isOpened())
    {
        std::cout << "Unable to open camera" << '\n';
        m_decoded = "-1";
    }
    else
    {
        while (true)
        {
            cap >> frame;
            m_decoded = decode_qr(frame);
            cv::imshow("Live", frame);
            if (m_decoded != "") { break; }
            if (cv::waitKey(m_delay) >= 0) { break; }
        }
    }
}

void QR::encode_qr(const std::string &to_encode)
{
    m_qr = QRcode_encodeString(to_encode.c_str(), 4, QR_ECLEVEL_Q, QR_MODE_8, 1);
    m_qr_isnull = false;
    m_real_width = (m_qr->width + m_margin * 2) * m_size;
    m_row = (unsigned char *)malloc((m_real_width + 7) / 8);
    if (m_row == NULL) { do_exit("Could not allocate row memory", -1); }
}

void QR::create_png(const std::string &file_name)
{
    // Open the PNG File
    open_png(file_name);

    // Write the upper margin
    write_margin();

    m_p = m_qr->data;
    for (int ii=0; ii < m_qr->width; ii++)
    {
        m_bit = 7;
        memset(m_row, 0xff, (m_real_width + 7) / 8);
        m_q = m_row;
        m_q += m_margin*m_size / 8;
        m_bit = 7 - (m_margin*m_size % 8);
        for (int jj=0; jj < m_qr->width; jj++)
        {
            for (int kk=0; kk<m_size; kk++)
            {
                *m_q ^= (*m_p & 1) << m_bit;
                m_bit--;
                if (m_bit < 0)
                {
                    m_q++;
                    m_bit=7;
                }
            }
            m_p++;
        }
        for (int kk=0; kk<m_size; kk++)
        {
            png_write_row(m_png_ptr, m_row);
        }
    }

    // Write the bottom margin
    write_margin();

    // Close the PNG File
    close_png();
}

// ------------ GETTERS ----------- //
std::string QR::to_encode() const { return m_toencode; }
std::string QR::decoded_data() const { return m_decoded; }

// ------------ SETTERS ----------- //
void QR::set_camera(const int &camera_id) { m_camera = camera_id; }
