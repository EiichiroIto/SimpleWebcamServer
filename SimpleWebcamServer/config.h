#define CONFIG_M5_CAMERA_X_BOARD 1

#define USE_APMODE
#define SSID "SimpleWebcamServer"

//#define USE_STAMODE
//#define SSID "YourAPName"

#define PASSWD "12345678"

#define WEBCAM_PORT 80

#define PIXEL_FORMAT PIXFORMAT_JPEG       //YUV422,GRAYSCALE,RGB565,JPEG
#define FRAME_SIZE FRAMESIZE_QVGA         //QQVGA-UXGA Do not use sizes above QVGA when not JPEG
