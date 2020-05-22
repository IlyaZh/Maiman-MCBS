#ifndef DEVICEDESCRIPTION_H
#define DEVICEDESCRIPTION_H

#include <QObject>
#include <QPointer>
#include <QPixmap>

class DeviceDescription
{
public:
    DeviceDescription(QPixmap *pic, QString desc, QString link) : m_Photo(pic), m_Description(desc), m_Link(link) {}
    ~DeviceDescription() {
        if(m_Photo != nullptr) { delete m_Photo; }
    }
    const QPixmap* photo() { return m_Photo; }
    QString description() { return m_Description; }
    QString link() { return m_Link; }
private:
    QPixmap* m_Photo;
    QString m_Description;
    QString m_Link;
};

#endif // DEVICEDESCRIPTION_H

