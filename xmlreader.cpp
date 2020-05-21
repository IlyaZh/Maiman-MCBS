#include "xmlreader.h"
#include "devicecommand.h"
#include "devicesignedcommand.h"

xmlReader::xmlReader(QObject *parent) : QObject(parent), file(nullptr)
{

}

void xmlReader::setDeviceOptions(device_t &device, quint16 dev_id) {
    this->device = &device;
    device.hasLaser = false;
    device.hasTEC = false;
    device.laserOn = false;
    device.tecOn = false;
    device.startTECwithLaser = false;
    deviceId = dev_id;
    device.stopCommandDelay = STOP_COMMAND_DELAY_DEFAULT;
    device.minCommDelay = COM_COMMAND_MIN_SEND_DELAY;
    device.maxCommDelay = COM_COMMAND_MAX_SEND_DELAY;
}

void xmlReader::setDeviceList(QList<availableDev_t>& listPtr) {
    availableListPtr = &listPtr;
}

void xmlReader::setBaudsList(QList<uint> &baudList) {
    bauds = &baudList;
}

void xmlReader::setConfigFile(QString fileName) {
    filePath = fileName;
}

bool xmlReader::readFile() {
    file = new QFile();
    file->setFileName(filePath);
    if(!file->exists()) {
        emit errorHandler("CONFIG: " + file->errorString());
        return false;
    }
    if(!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit errorHandler("CONFIG: " + file->errorString());
        return false;
    }

    return true;
}

void xmlReader::parseCommonConfig(QString fileName, QList<availableDev_t>& deviceList, QList<uint> &baudList) {
    setConfigFile(fileName);
    setDeviceList(deviceList);
    setBaudsList(baudList);
    if(!readFile()) return;

    availableListPtr->clear();
    bauds->clear();
    xml.setDevice(file);

    QXmlStreamAttributes attrib = xml.attributes();
    QStringRef xname = xml.name();

    while(!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        xname = xml.name();
        if(token == QXmlStreamReader::StartElement) {
            if(xname == "CIDD") {
                attrib = xml.attributes();
                if(attrib.hasAttribute("id")) {
                    availableDev_t devStruct;
                    devStruct.id = static_cast<quint16>(attrib.value("id").toUInt(nullptr, 16));
                    devStruct.name = xml.readElementText();
                    availableListPtr->append(devStruct);
                }

            }
            else if(xname == "BaudRate") {
                attrib = xml.attributes();
                if(attrib.hasAttribute("value")) {
                    uint value = attrib.value("value").toUInt();
                    bauds->append(value);
                }
            }
        } else if(token == QXmlStreamReader::EndElement) {
            // Завершаем цикл чтения по прошествию соответствующего закрывающего тэга
            if(xname == "CommonIDDevices") break;
            if(xname == "Config") break;
        }

        if(xml.hasError()) {
            emit errorHandler("Program CONFIG: " + xml.errorString());
            break;
        }
    }

    if(bauds->isEmpty()) {
        writeToLog("Bauds is empty!");
    }

    emit endOfLoadingProgramConfig(!xml.hasError());

    file->close();
    file->deleteLater();
}

void xmlReader::parseDeviceConfig(QString fileName, device_t &devicePtr, quint16 dev_id) {
    setConfigFile(fileName);
    setDeviceOptions(devicePtr, dev_id);
    if(!readFile()) return;
    bool isDeviceFound = false;

    xml.setDevice(file);

    while(!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        if(token == QXmlStreamReader::StartElement) {
            QStringRef xname = xml.name();
            if (xname == "Device") {
                isDeviceFound = parseDevice();
                if(!isDeviceFound) xml.skipCurrentElement();
            }

            else if (xname == "Content") {
                parseContent();
            } else if (xname == "Limits") {
                parseLimits();
            } else if (xname == "CalibrationKoefs") {
                parseCalibration();
            } else if (xname == "Commands") {
                parseCommands();
            } else if (xname == "ParamControls") {
                parseControls();
            } else if (xname == "Leds") {
                parseLeds();
            }  else if (xname == "BinaryOptions") {
                parseBinaryOptions();
            } else if (xname == "Buttons") {
                parseButtons();
            } else {
//                xml.skipCurrentElement();
            }
        } else if(token == QXmlStreamReader::EndElement) {
            if(xml.name() == "Device" && isDeviceFound) break;
            if(xml.name() == "Config") break;
        }
    }

    // Обработчик команд для неизвестных устройств
    // (отсутствует)

    if(xml.hasError()) {
        emit errorHandler("Error in config at line (" + QString::number(xml.lineNumber()) + "): " + xml.errorString());
    } else {
        emit endOfLoadingConfig(isDeviceFound);
    }

    file->close();
    file->deleteLater();
}

bool xmlReader::parseDevice() {
    QXmlStreamAttributes attrib = xml.attributes();
    if(attrib.hasAttribute("id")) {
        quint16 currID = static_cast<quint16>(attrib.value("id").toUInt(nullptr, 16));
        if(currID != deviceId) return false;
        device->deviceID = currID;
        if (attrib.hasAttribute("name")) device->devName = attrib.value("name").toString();
        if(attrib.hasAttribute("minCommandDelayMs")) {
            device->minCommDelay = attrib.value("minCommandDelayMs").toUInt();
        }

        if(attrib.hasAttribute("maxCommandDelayMs")) {
            device->maxCommDelay = attrib.value("maxCommandDelayMs").toUInt();
        }

        if(attrib.hasAttribute("stopCommandDelayMs")) {
            device->stopCommandDelay = attrib.value("stopCommandDelayMs").toUInt();
        }

        return true;
    }
    return false;
}

void xmlReader::parseContent() {
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "Content")) {
        if(xml.name() == "Image") {
            device->image = xml.readElementText();
        } else if(xml.name() == "Description") {
            device->description = xml.readElementText();
        } else if(xml.name() == "Link") {
            device->link = xml.readElementText();
        }
        xml.readNext();
    }
}

void xmlReader::parseCommands() {
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "Commands")) {
        xml.readNext();
        if(!(xml.name() == "Command" && xml.tokenType() == QXmlStreamReader::StartElement)) continue;
        QXmlStreamAttributes attrib = xml.attributes();

        QString code = (attrib.hasAttribute("code")) ? attrib.value("code").toString() : "";
        if(code.isEmpty()) {
            writeToLog(QString("Parse incorrect command. Line number %1: %2").arg(xml.lineNumber()).arg(xml.text()));
            continue;
        }

        double divider = (attrib.hasAttribute("divider")) ? attrib.value("divider").toDouble() : 1;

        quint8 interval = (attrib.hasAttribute("interval")) ? attrib.value("interval").toUInt() : 1;
        interval = qBound(MIN_COM_INTERVAL_COUNTER, interval, MAX_COM_INTERVAL_COUNTER);

        bool isSigned = attrib.hasAttribute("isSigned");
        bool isTemperature = attrib.hasAttribute("isTemperature");

        QString unit = (attrib.hasAttribute("unit")) ? attrib.value("unit").toString() : "" ;
        unit.replace("(deg)", QString::fromRawData(new QChar('\260'), 1));

        if(isSigned) {
            device->commands.insert(code, new DeviceSignedCommand(code, unit, divider, interval, isTemperature));
        } else {
            device->commands.insert(code, new Command(code, unit, divider, interval, isTemperature));
        }

    }
}

void xmlReader::parseControls() {
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "ParamControls")) {
        if(xml.name() == "Param" && xml.tokenType() == QXmlStreamReader::StartElement)
            parseParam();
        xml.readNext();
    }
}

void xmlReader::parseLeds() {
    leds_t *led = nullptr;
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "Leds")) {
        if(xml.name() == "Led") {
            if(xml.tokenType() == QXmlStreamReader::StartElement) {
                led = new leds_t;
                led->label = "";
                QXmlStreamAttributes attrib = xml.attributes();
                if(attrib.hasAttribute("label")) {
                    led->label = attrib.value("label").toString();
                } else {
                    writeToLog(QString("Some led has no label. Line %1: %2").arg(xml.lineNumber()).arg(xml.text()));
                }
            } else if (xml.tokenType() == QXmlStreamReader::EndElement) {
                if(!led->label.isEmpty()) {
                    device->leds.append(*led);
                } else {
                    delete led;
                }
                led = nullptr;
            }
        } else if (xml.name() == "LedMask") {
            parseLedMask(led);
        }
        xml.readNext();
    }
}

void xmlReader::parseLedMask(leds_t* ledPtr) {
    if(ledPtr == nullptr) return;

    ledMask_t ledMask;
    ledMask.message.clear();
    ledMask.activeColor = LEDS_DEFAULT_ACTIVE_COLOR;
    ledMask.defaultColor = LEDS_DEFAULT_COLOR;
    QXmlStreamAttributes attrib = xml.attributes();
    Command* cmd = nullptr;

    if(attrib.hasAttribute("code")) {
        ledMask.command = attrib.value("code").toString();
        cmd = device->commands.value(attrib.value("code").toString(), nullptr);
    }
    if(attrib.hasAttribute("mask")) {
        ledMask.mask = attrib.value("mask").toUInt(nullptr, 16);
    }
    if(attrib.hasAttribute("maskColor")) {
        ledMask.activeColor.setNamedColor(attrib.value("maskColor").toString());
    }
    if(attrib.hasAttribute("defaultColor")) {
        ledMask.defaultColor.setNamedColor(attrib.value("defaultColor").toString());
    }

    QString maskMsg = xml.readElementText();
    if(!maskMsg.isEmpty()) ledMask.message = maskMsg;
    if(cmd != nullptr) {
        ledPtr->masks.append(ledMask);
    }
}

void xmlReader::parseParam() {
    QXmlStreamAttributes attrib = xml.attributes();

    QString title = "no name";

    Command *minCmd = nullptr;
    Command *maxCmd = nullptr;
    Command *realCmd = nullptr;
    Command *valueCmd = nullptr;

    if(attrib.hasAttribute("min")) {
        minCmd = device->commands.value(attrib.value("min").toString(), nullptr);
    }

    if(attrib.hasAttribute("max")) {
        maxCmd = device->commands.value(attrib.value("max").toString(), nullptr);
    }

    if(attrib.hasAttribute("value")) {
        valueCmd = device->commands.value(attrib.value("value").toString(), nullptr);
    }

    if(attrib.hasAttribute("real")) {
        realCmd = device->commands.value(attrib.value("real").toString(), nullptr);
    }

    title = xml.readElementText();


    if(realCmd != nullptr || (minCmd != nullptr && maxCmd != nullptr && valueCmd != nullptr)) {
        ParameterController* parameterController = new ParameterController(title, minCmd, maxCmd, valueCmd, realCmd);
        device->paramWidgets.append(parameterController);
    }

}

void xmlReader::parseLimits() {
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "Limits")) {
        xml.readNext();
        if(!(xml.name() == "Limit" && xml.tokenType() == QXmlStreamReader::StartElement)) continue;
        //    if(!currDeviceFound) return;
        QXmlStreamAttributes attrib = xml.attributes();

        double min, max;
        min = max = 0;
        Command *valueComm = nullptr;
        Command *minComm = nullptr;
        Command *maxComm = nullptr;

        DeviceLimit *deviceLimit = nullptr;
        QString title = xml.readElementText();

        if(attrib.hasAttribute("limitCode")) {
            valueComm = device->commands.value(attrib.value("limitCode").toString(), nullptr);
            if(valueComm != nullptr) {

                if(attrib.hasAttribute("minCode") && attrib.hasAttribute("maxCode")) {
                    minComm = device->commands.value(attrib.value("minCode").toString(), nullptr);
                    maxComm = device->commands.value(attrib.value("maxCode").toString(), nullptr);
                    if(minComm != nullptr && maxComm != nullptr)
                        deviceLimit = new DeviceLimit(title, valueComm, minComm, maxComm);
                } else if (attrib.hasAttribute("minCode") && attrib.hasAttribute("maxValue")) {
                    minComm = device->commands.value(attrib.value("minCode").toString(), nullptr);
                    max = attrib.value("maxValue").toDouble();
                    if(minComm != nullptr)
                        deviceLimit = new DeviceLimit(title, valueComm, minComm, max);
                } else if (attrib.hasAttribute("minValue") && attrib.hasAttribute("maxCode")) {
                    maxComm = device->commands.value(attrib.value("maxCode").toString(), nullptr);
                    min = attrib.value("minValue").toDouble();
                    if(maxComm != nullptr)
                        deviceLimit = new DeviceLimit(title, valueComm, min, maxComm);
                } else if (attrib.hasAttribute("minValue") && attrib.hasAttribute("maxValue")) {
                    max = attrib.value("maxValue").toDouble();
                    min = attrib.value("minValue").toDouble();
                    deviceLimit = new DeviceLimit(title, valueComm, min, max);
                }
            }
        }

        if(deviceLimit != nullptr) {
            device->limits.append(deviceLimit);
        }

    }
}

void xmlReader::parseCalibration() {
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "CalibrationKoefs")) {
        xml.readNext();
        if(!(xml.name() == "Calibrate" && xml.tokenType() == QXmlStreamReader::StartElement)) continue;
        QXmlStreamAttributes attrib = xml.attributes();
        calibration_t calibrate;
        calibrate.cmd = nullptr;
//        calibrate.title = "No name";
        calibrate.min = 95.00;
        calibrate.max = 105.00;

        if(attrib.hasAttribute("code") && attrib.hasAttribute("min") && attrib.hasAttribute("max")) {
            calibrate.cmd = device->commands.value(attrib.value("code").toString(), nullptr);
            calibrate.min = attrib.value("min").toDouble();
            calibrate.max = attrib.value("max").toDouble();
        }

        calibrate.title = xml.readElementText();
        if(calibrate.cmd == nullptr)
            writeToLog(QString("Incorrect calibrate tag. Line %1 : %2").arg(xml.lineNumber()).arg(xml.text()));
        else
            device->calCoefs.append(calibrate);

    }
}

void xmlReader::parseButtons() {
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "Buttons")) {
        xml.readNext();
        if(!(xml.name() == "Button" && xml.tokenType() == QXmlStreamReader::StartElement)) continue;
        doubleMaskCommand_t tmp;
        QXmlStreamAttributes attrib = xml.attributes();
        QString tmpName = "noname";
        tmp.mask = 0;

        //    btn->setStyleSheet("QPushButton {\n	color: #000;\n	border: 1px solid rgb(31,31,31);\n	border-radius: 4px;\n	padding: 3px 20px;\n	background-color: rgb(189, 1, 2);\n}\n\nQPushButton::checked {\n	background-color: rgb(0, 102, 52);\n}");

        if(attrib.hasAttribute("name")) {
            tmpName = attrib.value("name").toString();
            //        btn->setText(tmpName);
            if(tmpName == "laser") {
                device->hasLaser = true;
            } else if (tmpName == "tec") {
                device->hasTEC = true;
            }
        }

        if(attrib.hasAttribute("code")) {
            tmp.code = attrib.value("code").toString();
        }
        if(attrib.hasAttribute("mask")) {
            tmp.mask = attrib.value("mask").toUInt(nullptr, 16);
        }

        if(attrib.hasAttribute("onCommand")) {
            tmp.onCommand = attrib.value("onCommand").toString();
        }

        if(attrib.hasAttribute("offCommand")) {
            tmp.offCommand = attrib.value("offCommand").toString();
        }

        device->stateButtons.insert(tmpName, tmp);

    }
}

void xmlReader::parseBinaryOptions() {
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "BinaryOptions")) {
        xml.readNext();
        if(!(xml.name() == "CheckBox" && xml.tokenType() == QXmlStreamReader::StartElement)) continue;

        binOption_t binOption;
        QXmlStreamAttributes attrib = xml.attributes();

        if(attrib.hasAttribute("code")) {
            binOption.code = attrib.value("code").toString();
        }
        if(attrib.hasAttribute("offCommand")) {
            binOption.offCommand = attrib.value("offCommand").toString();
        }
        if(attrib.hasAttribute("onCommand")) {
            binOption.onCommand = attrib.value("onCommand").toString();
        }
        if(attrib.hasAttribute("mask")) {
            binOption.mask = attrib.value("mask").toUInt(nullptr, 16);
        } else {
            binOption.mask = 0;
        }


        QCheckBox *binOptionCb = new QCheckBox();
        QString label = xml.readElementText();
        binOptionCb->setText(label);
        binOption.label = label;
        binOptionCb->setStyleSheet("border: none;");
        binOption.checkBox = binOptionCb;
        device->binOptions.append(binOption);

    }
}

void xmlReader::writeToLog(QString msg) {
    emit logger("[INFO][xmlReader] " + msg);
}
