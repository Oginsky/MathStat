#ifndef COMMONSTRUCT_H
#define COMMONSTRUCT_H

#include <QMap>


enum class FLAG {
    NormalAproximate = 0, ImansAproximate, ProbValue, StatisticsInfo, ZeroCorrection,
};

unsigned qHash(FLAG flag);



struct Pair {
    QString name;
    FLAG flag;

    Pair(QString _name, FLAG _flag) :
        name(_name), flag(_flag) { }
    Pair() { }
};

class QString;

struct OutputData {

    struct CriterialHypoth {
        QString name;
        QMap<QString, QString> hypoth;

        CriterialHypoth(QString _name) : name(_name) { }
        CriterialHypoth() { }
        void addHypoth(const QString& name, const QString& value) {
            hypoth.insert(name, value);
        }
    };

    enum Type { Value = 0, Hypoth, Extra };

    void addHypoth(const QString& name, const CriterialHypoth& value) {
        hypoth.insert(name, value);
    }
    void addValue(const QString& name, const QString& value) {
         values.insert(name, value);
    }
    void addExtra(const QString& name, const QString& value) {
         extra.insert(name, value);
    }


    QMap<QString, CriterialHypoth> hypoth;
    QMap<QString, QString> values;
    QMap<QString, QString> extra;

};

#endif // COMMONSTRUCT_H
