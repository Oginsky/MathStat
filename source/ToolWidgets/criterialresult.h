#ifndef CRITERIALRESULT_H
#define CRITERIALRESULT_H

#include <QWidget>

class OutputData;

class CriterialResult : public QWidget {
    Q_OBJECT
public:
    explicit CriterialResult(const OutputData& output, QWidget *parent = nullptr);

signals:

};

#endif // CRITERIALRESULT_H
