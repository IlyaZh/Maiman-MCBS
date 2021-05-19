#ifndef PARSER_H
#define PARSER_H

#include <QXmlStreamReader>
#include <QFile>
#include <QAbstractItemModel>
#include <QDomDocument>
#include <QModelIndex>


class ParseItem;

class Parser: public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit Parser(QString filePath,QObject *parent = nullptr);
    ~Parser();

    QVariant data(const QModelIndex &index, int role) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    void attribFind(QString name);

    void clear();
private:

protected:
    QDomDocument domDocument;
    ParseItem *rootItem;
    QString filePath;
    QString attrib;
signals:
    void finished();
    void errorOccured(QString);
public slots:
    virtual void process();

};

typedef  struct {
    QString code;
    uint divider;
    uint interval;
    QString unit;
    bool tol;
    bool isTemperature;
} Command;
typedef  struct {
    QString min;
    QString max;
    QString value;
    QString real;
    QString name;

}ParamControls;
typedef struct {
    QString code;
    QString onCommand;
    QString offCommand;
    QString mask;
    QString name;
} BinaryOptions;
typedef struct {
    QString name;
    QString mask;
    QString code;
    QString onCommand;
    QString offCommand;
} Buttons;
typedef struct{
    QString name;
    QString code;
    QString mask;
    QString maskColor;
    QString defaultColor;
}Led;
typedef struct{
    QString code;
    QString name;
    uint min;
    uint max;
}CalibrationKoefs;
typedef struct{
    QString name;
    QString limitCode;
    QString minCode;
    QString maxCode;
    int minValue;
    int maxValue;
}limits;
typedef struct {
    QString devName;
    QString image;
    QString description;
    QString link;
    QString deviceID;

    QMap<QString, Command> commands;
    QMap<QString, ParamControls> pControls;
    QMap<QString, BinaryOptions> bOptions;
    QMap<QString, Buttons> button;
    QMap<QString, QMap<int,Led>> leds;
    QMap<QString, CalibrationKoefs> cKoef;
    QMap<QString, limits> lim;

    uint stopCommandDelay;
    uint minCommDelay;
    uint maxCommDelay;
} device_t;
#endif // PARSER_H
