#include <QApplication>
#include <QQmlApplicationEngine>
#include "opencvimageprovider.h"
#include "videostreamer.h"
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qRegisterMetaType<cv::Mat>("cv::Mat");

    VideoStreamer videoStreamer;

    OpencvImageProvider *liveImageProvider(new OpencvImageProvider);

    engine.rootContext()->setContextProperty("VideoStreamer",&videoStreamer);
    engine.rootContext()->setContextProperty("liveImageProvider",liveImageProvider);

    engine.addImageProvider("live",liveImageProvider);

    const QUrl url(QStringLiteral("qrc:/main.qml"));

    engine.load(url);

    QObject::connect(&videoStreamer,&VideoStreamer::newImage,liveImageProvider,&OpencvImageProvider::updateImage);

    return app.exec();
}
