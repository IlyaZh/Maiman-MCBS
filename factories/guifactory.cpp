#include "guifactory.h"

GUIfactory::GUIfactory(Parser *model)
{
    m_parser = model;
    start();
}

GUIfactory::~GUIfactory(){

}

void GUIfactory::start()
{
    if (m_parser != nullptr){
        QModelIndex indexFirst = m_parser->index(1, 0);
        QString check = indexFirst.data().toString();
        if (check == "GUI"){
            for (int iCount=0 ;iCount<=(m_parser->rowCount(indexFirst));iCount++ ) {
                QModelIndex indexID = SForward(iCount,0,indexFirst);
                if (indexID.data().toString() =="Device")
                    GUIReader(iCount);
            }
        }
    }
}

void GUIfactory::GUIReader(int row){
    QModelIndex indexID = SForward(row,0, m_parser->index(1, 0));
    device_t internalDevice;
    internalDevice.deviceID = AttributesValue("id",indexID).toString();

    ContentReader(findTag("Content",indexID),internalDevice);
    LimitsReader(findTag("Limits",indexID),internalDevice);
    CalibrationKoefReader(findTag("CalibrationKoefs",indexID),internalDevice);
    ParamControlReader(findTag("ParamControls",indexID),internalDevice);
    BinaryOptionsReader(findTag("BinaryOptions",indexID),internalDevice);
    ButtonsReader(findTag("Buttons",indexID),internalDevice);
    LedsReader(findTag("Leds",indexID),internalDevice);

    m_device.append(internalDevice);
}

QModelIndex GUIfactory::indexStepBack(int row,int column)
{
    return m_parser->parent(m_parser->index(row, column, m_parser->index(1, 0)));
}

QModelIndex GUIfactory::SForward(int row,int column,QModelIndex parent){

    return m_parser->index(row, column, parent);
}

QString GUIfactory::findDeviceID(QString id,int* row){
    QString findID = "id=" + id ;
    for (int iCount=0 ;iCount<=(m_parser->rowCount( m_parser->index(1, 0)));iCount++ ) {
        QModelIndex index = m_parser->index(iCount, 1,m_parser->index(1, 0));
        QString text = index.data(Qt::DisplayRole).toString();
        if(text.contains(findID, Qt::CaseInsensitive)){
            *row = iCount;
            return text;
        }
    }
    return QString();
}
QModelIndex GUIfactory::findTag(QString tag, QModelIndex parent){
    QModelIndex index;
    for (int iCount=0 ;iCount<=(m_parser->rowCount(parent));iCount++ ) {
        index = SForward(iCount,0,parent);
        if (index.data().toString() == tag)
            return index;
    }
    return QModelIndex();
}
QVariant GUIfactory::AttributesValue(QString attribName,QModelIndex index){
    m_parser->attribFind(attribName);
    return index.siblingAtColumn(3).data(Qt::DisplayRole);

}

void GUIfactory::ContentReader(QModelIndex index,device_t device)
{
    device.image = SForward(0,0,findTag("Image",index)).siblingAtColumn(2).data().toString();
    device.description = SForward(0,0,findTag("Description",index)).siblingAtColumn(2).data().toString();
    device.link = SForward(0,0,findTag("Link",index)).siblingAtColumn(2).data().toString();
}

void GUIfactory::LimitsReader(QModelIndex index,device_t device)
{
    limits internalLimits;
    for (int iCount=0 ;iCount<=(m_parser->rowCount(index));iCount++ ) {
        QModelIndex indexLimits = SForward(iCount,0,index);
        if(indexLimits.data().toString() == "Limit"){
            internalLimits.maxValue =  AttributesValue("maxValue",indexLimits).toInt();
            internalLimits.minValue =  AttributesValue("minValue",indexLimits).toInt();
            internalLimits.limitCode =  AttributesValue("limitCode",indexLimits).toString();
            internalLimits.maxCode = AttributesValue("maxCode",indexLimits).toString();
            internalLimits.minCode = AttributesValue("minCode",indexLimits).toString();
            internalLimits.name = SForward(0,0,indexLimits).siblingAtColumn(2).data().toString();

            device.lim.insert(internalLimits.name,internalLimits);
        }
    }
}

void GUIfactory::CalibrationKoefReader(QModelIndex index,device_t device)
{
    CalibrationKoefs internalKoefs;
    for (int iCount=0 ;iCount<=(m_parser->rowCount(index));iCount++ ) {
        QModelIndex indexCalibrate = SForward(iCount,0,index);
        if(indexCalibrate.data().toString() == "Calibrate"){
            internalKoefs.max =  AttributesValue("max",indexCalibrate).toUInt();
            internalKoefs.min =  AttributesValue("min",indexCalibrate).toUInt();
            internalKoefs.code =  AttributesValue("code",indexCalibrate).toString();
            internalKoefs.name = SForward(0,0,indexCalibrate).siblingAtColumn(2).data().toString();

            device.cKoef.insert(internalKoefs.code,internalKoefs);
        }
    }
}

void GUIfactory::ParamControlReader(QModelIndex index,device_t device)
{
    ParamControls internalParam;
    for (int iCount=0 ;iCount<=(m_parser->rowCount(index));iCount++ ) {
        QModelIndex indexParamContrl = SForward(iCount,0,index);
        if(indexParamContrl.data().toString() == "Param"){
            internalParam.max =  AttributesValue("max",indexParamContrl).toString();
            internalParam.min =  AttributesValue("min",indexParamContrl).toString();
            internalParam.value =  AttributesValue("value",indexParamContrl).toString();
            internalParam.real = AttributesValue("real",indexParamContrl).toString();
            internalParam.name = SForward(0,0,indexParamContrl).siblingAtColumn(2).data().toString();

            device.pControls.insert(internalParam.name,internalParam);
        }
    }
}

void GUIfactory::BinaryOptionsReader(QModelIndex index,device_t device)
{
    BinaryOptions internalBOptions;
    for (int iCount=0 ;iCount<=(m_parser->rowCount(index));iCount++ ) {
        QModelIndex indexBOptions = SForward(iCount,0,index);
        if(indexBOptions.data().toString() == "CheckBox"){
            internalBOptions.code =  AttributesValue("code",indexBOptions).toString();
            internalBOptions.onCommand =  AttributesValue("onCommand",indexBOptions).toString();
            internalBOptions.offCommand =  AttributesValue("offCommand",indexBOptions).toString();
            internalBOptions.mask = AttributesValue("mask",indexBOptions).toString();
            internalBOptions.name = SForward(0,0,indexBOptions).siblingAtColumn(2).data().toString();

            device.bOptions.insert(internalBOptions.name,internalBOptions);
        }
    }
}

void GUIfactory::ButtonsReader(QModelIndex index,device_t device)
{
    Buttons internalButtons;
    for (int iCount=0 ;iCount<=(m_parser->rowCount(index));iCount++ ) {
        QModelIndex indexButtons = SForward(iCount,0,index);
        if(indexButtons.data().toString() == "Button"){
            internalButtons.code =  AttributesValue("code",indexButtons).toString();
            internalButtons.onCommand =  AttributesValue("onCommand",indexButtons).toString();
            internalButtons.offCommand =  AttributesValue("offCommand",indexButtons).toString();
            internalButtons.mask = AttributesValue("mask",indexButtons).toString();
            internalButtons.name = AttributesValue("name",indexButtons).toString();

            device.button.insert(internalButtons.name,internalButtons);
        }
    }
}

void GUIfactory::LedsReader(QModelIndex index,device_t device)
{
    Led internalLed;
    QString ledLabel;
    QMap<int,Led> LedMask;

    for (int iCount=0 ;iCount<=(m_parser->rowCount(index));iCount++ ) {
        QModelIndex indexLeds = SForward(iCount,0,index);
        if(indexLeds.data().toString() == "Led"){
            ledLabel = AttributesValue("label",indexLeds).toString();
            for (int count=0;count<=(m_parser->rowCount(indexLeds));count++){
                QModelIndex indexLedMask = SForward(count,0,indexLeds);
                if(indexLedMask.data().toString() == "LedMask"){
                    internalLed.code =  AttributesValue("code",indexLedMask).toString();
                    internalLed.maskColor =  AttributesValue("maskColor",indexLedMask).toString();
                    internalLed.defaultColor =  AttributesValue("defaultColor",indexLedMask).toString();
                    internalLed.mask = AttributesValue("mask",indexLedMask).toString();
                    internalLed.name = SForward(0,0,indexLedMask).siblingAtColumn(2).data().toString();

                    LedMask.insert(count,internalLed);
                }
            }
            device.leds.insert(ledLabel,LedMask);
        }
    }
}

device_t GUIfactory::getDevice(QString id)
{
    device_t getDevice;
    for ( auto &i : m_device ) {
        if (i.deviceID == id){
            getDevice = i;
        }
    }
    return getDevice;
}
// private slots
void GUIfactory::parsingFinished() {
//    QString error;
//    emit m_parser->errorOccured(error);
//    if(error == ""){
//        start();
//    }
//    else
//        qDebug()<<"Error: "<<error;
}

