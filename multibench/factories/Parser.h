#ifndef PARSER_H
#define PARSER_H

#include <QFile>
#include <QObject>

#include "treeitem.h"

class Parser : public QObject {
  Q_OBJECT
 public:
  explicit Parser(const QByteArray& dataArray, QObject* parent = nullptr);
  ~Parser() override;
  TreeItem* data();
  virtual bool start() = 0;
  virtual void stop();
  virtual QString errorString();

 protected:
  QByteArray m_data;
  TreeItem* m_tree = nullptr;
  bool m_stop = false;
  QString m_errorString;

  void makeError(const QString& msg);
  //    void readData(TreeItem* item, int level = 0); // debug only
};

#endif  // PARSER_H
