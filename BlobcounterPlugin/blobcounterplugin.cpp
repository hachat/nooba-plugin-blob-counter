#include "blobcounterplugin.h"
#include <QtCore>

#include <QImage>
#include <QDebug>
//#include <QPoint>
//#include <QPointF>
// opencv includes
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>


BlobcounterPlugin::BlobcounterPlugin()
{

}

BlobcounterPlugin::~BlobcounterPlugin()
{

}

bool BlobcounterPlugin::procFrame( const cv::Mat &in, cv::Mat &out, ProcParams &params )
{
    Q_UNUSED(params)
    out = in;

    return true;
}

bool BlobcounterPlugin::init()
{
    //connect(this, SIGNAL(generateEvent(QList<DetectedEvent>)), this, SLOT(onCaptureEvent(QList<DetectedEvent>)));
    connect(this, SIGNAL(generateEvent(QList<DetectedEvent>)), &lineCrossDetector, SLOT(captureEvent(QList<DetectedEvent>)));
    connect(&lineCrossDetector, SIGNAL(generateEvent(QList<DetectedEvent>)), &lineCrossCounter, SLOT(captureEvent(QList<DetectedEvent>)));
    connect(&lineCrossCounter, SIGNAL(generateEvent(QList<DetectedEvent>)), this, SLOT(onCaptureEvent(QList<DetectedEvent>)));
    connect(&lineCrossCounter, SIGNAL(generateEvent(QList<DetectedEvent>)), &crossCountAnomalyNode, SLOT(captureEvent(QList<DetectedEvent>)));
    connect(&crossCountAnomalyNode, SIGNAL(generateEvent(QList<DetectedEvent>)), this, SLOT(onCaptureEvent(QList<DetectedEvent>)));

    createFrameViewer("CountingLine");

//    createIntParam("point1-x",500,1024,0);
//    createIntParam("point1-y",0,768,0);
//    createIntParam("point2-x",500,1024,0);
//    createIntParam("point2-y",700,768,0);

//    createPointParam("Point1",QPoint(500,0));
//    createPointParam("Point2",QPoint(500,700));

    createIntParam("time_window(frames)",300,10000,0);
    createIntParam("min_of_count_range",-1,100,-1);
    createIntParam("max_of_count_range",2,100,0);

    createLineParam("Counting Line","CountingLine",QColor(0,0,255));
    crossCountAnomalyNode.setTime_window(300);
    crossCountAnomalyNode.setMax_of_count_range(2);
    crossCountAnomalyNode.setMin_of_count_range(0);

    QStringList rangeoptions;
    rangeoptions << "Out Range(-Inf,min)U(max,Inf)"
                 << "In Range[min,max]";
    createMultiValParamRequest("Anomaly Region",rangeoptions);

    QStringList direction_options;
    direction_options << "R1=>R2"
                      << "R2=>R1";
    createMultiValParamRequest("Counting Direction",direction_options);

    crossCountAnomalyNode.setAnomal_range(false);
    return true;
}

bool BlobcounterPlugin::release()
{
    return true;
}

PluginInfo BlobcounterPlugin::getPluginInfo() const
{
    PluginInfo pluginInfo(
        "Blob Counter Plugin",
        0,
        1,
        "This counts the blobs moving across a predefined line",
        "Chathuranga Hettiarachchi");
    return pluginInfo;
}

void BlobcounterPlugin::onCaptureEvent(QList<DetectedEvent> captured_event){


    foreach(DetectedEvent e, captured_event){
        if(e.getIdentifier() == "<FONT COLOR='#ff0000'>OutOfPhase"){
            generateAlert("CountingLine","<FONT COLOR='#ff0000'>OutOfPhase",nooba::AmberAlert);
        }
        debugMsg(QString(e.getIdentifier() + " " + e.getMessage() + " %1").arg(e.getConfidence()));
    }
    return;
}

void BlobcounterPlugin::onIntParamChanged(const QString &varName, int val){
    if(varName == "point1-x"){
        lineCrossDetector.setPoint1_x(val);
        debugMsg("point1-x set to"  + QString("%1").arg(val));
    }
    else if(varName == "point1-y"){
        lineCrossDetector.setPoint1_y(val);
        debugMsg("point1-y set to "  + QString("%1").arg(val));
    }
    else if(varName == "point2-x"){
        lineCrossDetector.setPoint2_x(val);
        debugMsg("point2-x set to "  + QString("%1").arg(val));
    }
    else if(varName == "point2-y"){
        lineCrossDetector.setPoint2_y(val);
        debugMsg("point2-y set to "  + QString("%1").arg(val));
    }
    else if(varName == "time_window(frames)"){
        crossCountAnomalyNode.setTime_window(val);
        debugMsg("time_window(frames) set to "  + QString("%1").arg(val));
    }
    else if(varName == "min_of_count_range"){
        crossCountAnomalyNode.setMin_of_count_range(val);
        debugMsg("min_of_count_range set to "  + QString("%1").arg(val));
    }
    else if(varName == "max_of_count_range"){
        crossCountAnomalyNode.setMax_of_count_range(val);
        debugMsg("max_of_count_range set to "  + QString("%1").arg(val));
    }
}

void BlobcounterPlugin::onLineParamUpdated(const QString &varName, const QString frameViewerTitle, QLine line){
    Q_UNUSED(frameViewerTitle)


    if(varName == "Counting Line"){
        lineCrossDetector.setPoint1_x(line.x1());
        lineCrossDetector.setPoint1_y(line.y1());
        lineCrossDetector.setPoint2_x(line.x2());
        lineCrossDetector.setPoint2_y(line.y2());
        debugMsg(QString("Counting Line set to (%1,%2)- (%3,%4)").arg(line.x1()).arg(line.y1()).arg(line.x2()).arg(line.y2()));
    }
    //        lineCrossDetector.setPoint1_x(p.x());
    //        lineCrossDetector.setPoint1_y(p.y());
    //        debugMsg(QString("Point1 set to (%1,%2)").arg(p.x()).arg(p.y()));
    //    }
}

//void BlobcounterPlugin::onPointParamChanged(const QString& varName, const QPointF& val){
//    QPoint p;
//    p = val.toPoint();
//    if(varName == "Point1"){

//        lineCrossDetector.setPoint1_x(p.x());
//        lineCrossDetector.setPoint1_y(p.y());
//        debugMsg(QString("Point1 set to (%1,%2)").arg(p.x()).arg(p.y()));
//    }
//    else if(varName == "Point2"){
//        lineCrossDetector.setPoint2_x(p.x());
//        lineCrossDetector.setPoint2_y(p.y());
//        debugMsg(QString("Point2 set to (%1,%2)").arg(p.x()).arg(p.y()));
//    }
//    return;
//}

void BlobcounterPlugin::onMultiValParamChanged(const QString &varName, const QString &val){
    if(varName == "Anomaly Region"){
        if(val == "In Range[min,max]"){
            crossCountAnomalyNode.setAnomal_range(true);
            debugMsg("Anomaly Region set to In Range[min,max]");
        }
        else{
            //Out Range(-Inf,min)U(max,Inf)
            crossCountAnomalyNode.setAnomal_range(false);
            debugMsg("Anomaly Region set to Out Range(-Inf,min)U(max,Inf)");
        }
    }
    //Counting Direction
    //"R1=>R2"  "R2=>R1"
    if(varName == "Counting Direction"){
        if(val == "R1=>R2"){
            crossCountAnomalyNode.setDirection(CountAnomalyNode::R1_to_R2);
            debugMsg("Counting Direction set to R1=>R2");
        }
        else{
            crossCountAnomalyNode.setDirection(CountAnomalyNode::R2_to_R1);
            debugMsg("Counting Direction set to R2=>R1");
        }
    }
}

void BlobcounterPlugin::inputData(const QStringList &strList, QList<QImage> imageList){

    QList<DetectedEvent> receivedEvents;
    QStringList stringList = strList;
    foreach(QString str,stringList){
        //debugMsg("recv" + str);
        QList<QString> parameters = str.split(" ");
        receivedEvents.append(DetectedEvent(parameters.at(0),parameters.at(1),parameters.at(2).toFloat()));
    }

    QImage temp = imageList.at(0);

    cv::Mat lineviewer(temp.height(),temp.width(),CV_8UC3,(uchar*)temp.bits(),temp.bytesPerLine());
    cv::line(lineviewer,cv::Point(lineCrossDetector.getPoint1().x(),lineCrossDetector.getPoint1().y()),
             cv::Point(lineCrossDetector.getPoint2().x(),lineCrossDetector.getPoint2().y()),
             cv::Scalar(0,0,255),3,CV_AA);

    cv::Point r1_label_position = cv::Point(((lineCrossDetector.getPoint1().x() + lineCrossDetector.getPoint2().x())/2) + 40,((lineCrossDetector.getPoint1().y() + lineCrossDetector.getPoint2().y())/2) - 40);
    cv::Point r2_label_position = cv::Point(((lineCrossDetector.getPoint1().x() + lineCrossDetector.getPoint2().x())/2) - 40,((lineCrossDetector.getPoint1().y() + lineCrossDetector.getPoint2().y())/2) + 40);

    QPoint l_r1 = QPoint(r1_label_position.x,r1_label_position.y);

    if(lineCrossDetector.getBlobRegion(l_r1) == LineCrossingNode::BLOB_REGION_ONE){
        cv::putText(lineviewer,"R2",r1_label_position,cv::FONT_ITALIC,1,cv::Scalar(0,0,255),3);
        cv::putText(lineviewer,"R1",r2_label_position,cv::FONT_ITALIC,1,cv::Scalar(0,0,255),3);
    }
    else{
        cv::putText(lineviewer,"R1",r1_label_position,cv::FONT_ITALIC,1,cv::Scalar(0,0,255),3);
        cv::putText(lineviewer,"R2",r2_label_position,cv::FONT_ITALIC,1,cv::Scalar(0,0,255),3);
    }

    updateFrameViewer("CountingLine",convertToQImage(lineviewer));

    emit generateEvent(receivedEvents);
    return;
}


QImage BlobcounterPlugin::convertToQImage(cv::Mat &cvImg)
{
    if (cvImg.channels()== 1){
        QImage img((uchar*)cvImg.data, cvImg.cols, cvImg.rows, cvImg.step1(), QImage::Format_Indexed8);
        return img;
    }
    else{
        QImage img((uchar*)cvImg.data, cvImg.cols, cvImg.rows, cvImg.step1(), QImage::Format_RGB888);
        return img;
    }
}
// see qt4 documentation for details on the macro (Qt Assistant app)
// Mandatory  macro for plugins in qt4. Made obsolete in qt5
#if QT_VERSION < 0x050000
    Q_EXPORT_PLUGIN2(BlobcounterPlugin, BlobcounterPlugin);
#endif
