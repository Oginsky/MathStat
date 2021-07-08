#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QMap>
#include <QList>

#include "commonstruct.h"
#include "Stats/sample.h"


class QStringList;
class QString;
class QVariant;

namespace Core { struct CriterialInfo; }

class TaskManager {

private:
    TaskManager();

    static void handleShiftProblem(QMap<QString, QVariant>& configs, const QList<Sample>& samples, OutputData& result);
    static void handleSignRank(QMap<QString, QVariant>& configs, const QList<Sample>& samples, OutputData& result);

    static void handleDichotomicData(QMap<QString, QVariant>& configs, const QList<Sample>& samples, OutputData& result);

public:
    static void handle(const Core::CriterialInfo* criterial, QMap<QString, QVariant> configs,
                       const QList<Sample>& samples, OutputData& result);

};

#endif // TASKMANAGER_H
