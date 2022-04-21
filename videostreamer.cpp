#include "videostreamer.h"
#include <unistd.h>

static cv::VideoWriter writer;
static int codec = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');  // select desired codec (must be available at runtime)
static double fps = 30.0;                          // framerate of the created video stream
static bool statusRecording = 0;

VideoStreamer::VideoStreamer()
{
    connect(&tUpdate,&QTimer::timeout,this,&VideoStreamer::streamVideo);

}

VideoStreamer::~VideoStreamer()
{
    writer.release();
    tUpdate.stop();
    threadStreamer->requestInterruption();
}

void VideoStreamer::streamVideo()
{

    if(frame.data)
    {
        QImage img = QImage(frame.data,frame.cols,frame.rows,QImage::Format_RGB888).rgbSwapped();
        emit newImage(img);
    }
}

void VideoStreamer::catchFrame(cv::Mat emittedFrame)
{
    frame = emittedFrame;
}

void VideoStreamer::openVideoCamera(QString path)
{


    VideoStreamer* worker = new VideoStreamer();
    worker->moveToThread(threadStreamer);
    QObject::connect(threadStreamer,SIGNAL(started()),worker,SLOT(streamerThreadSlot()));
    QObject::connect(worker,&VideoStreamer::emitThreadImage,this,&VideoStreamer::catchFrame);
    threadStreamer->start();

    tUpdate.start(100);

}

void VideoStreamer::startRecording(QString path)
{
    std::string fileName = path.toStdString();

    std::cout<<fileName<<std::endl;

    statusRecording = 1;
    writer.open(fileName, codec, fps, cv::Size(1920,1080), 1);

}

void VideoStreamer::saveButton()
{
    statusRecording = 0;
    tUpdate.stop();
    threadStreamer->requestInterruption();
}

void VideoStreamer::streamerThreadSlot()
{

    Display* display;
    Window root;
    int x,y,width,height;
    XImage* img{nullptr};


    x = 0;
    y = 0;
    width = 1920;
    height = 1080;
    display = XOpenDisplay(nullptr);
    root = DefaultRootWindow(display);

    cv::Mat cvImg;
   // cv::Mat img;
    while (1) {
       // cap>>tempFrame;

        img = XGetImage(display, root, x, y, width, height, AllPlanes, ZPixmap);
        cvImg = cv::Mat(height, width, CV_8UC4, img->data);

        //std::cout<<(int)cvImg.at<cv::Vec3b>(cv::Point(500,500))[1]<<std::endl;


        cv::cvtColor(cvImg,cvImg,cv::COLOR_BGRA2BGR);

        if(statusRecording)
            writer.write(cvImg);







        usleep(100000);





        if(cvImg.data)
        {
            emit emitThreadImage(cvImg);

        }

        if(QThread::currentThread()->isInterruptionRequested())
        {
            writer.release();

            return;
        }

    }
}
