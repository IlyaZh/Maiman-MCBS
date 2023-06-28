#ifndef CONNECTIONPOLLING_H
#define CONNECTIONPOLLING_H

#include <QObject>

class ConnectionPolling : public QObject {
  Q_OBJECT
 public:
  explicit ConnectionPolling(QObject *parent = nullptr);
  void reset();
  bool isNeedAction();
 signals:

 public slots:

 private:
  int counter = 0;
  size_t idx = 0;
};

#endif  // CONNECTIONPOLLING_H
