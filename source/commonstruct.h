#ifndef COMMONSTRUCT_H
#define COMMONSTRUCT_H

#include <QMap>

class QString;

/*


        { {"Ранговая форма", FLAG::Ranks}, {"Поправка Йейтса на непрерывность", FLAG::ChiSquareCorrection} }
 * */

namespace Order {

    // Дихотомические данные : оценка вероятности
    const QString HodgesLehmanEstimate ("Оценка Ходжеса и Лемана");
    const QString CopperPearsonInterval ("Интервал Коппера Пирсона (Бета распределение)");
    const QString WaldApproximation ("Приближение Вальда");
    const QString WilsonInterval ("Интервал Вилсона");

    // Дихотомические данные : Сравнение вероятностей
    const QString AsymptoticDifferenceInterval ("Асимптотический интервал для разности");
    const QString ProbabilityRatioEstimate ("Оценка отношения вероятностей");
    const QString ProbabilityRatioConfidenceInterval ("Доверительный интервал для отношения вероятностей");

    // Двухвыборочная задача о сдвиге : Критерий Манна-Уитни
    const QString r_Ratio ("r-отношение");
    const QString KliffDelta ("Дельта Клиффа");
    const QString DominatedProbability ("Вероятнсоть превосходства");
    const QString A_ValueWardDelani ("А-величина Варга и Делани");

}


enum class FLAG {
    Another = 0,
    NormalAproximate, ImansAproximate,
    ProbValue, EffectSize, StatisticsInfo,
    PointEstimate, ConfidenceInterval, Ranks,
    ZeroCorrection, ChiSquareCorrection, ContinuityCorrection
};

unsigned qHash(FLAG flag);



struct Pair {
    QString name;
    FLAG flag;

    Pair(QString _name, FLAG _flag = FLAG::Another) :
        name(_name), flag(_flag) { }
    Pair() { }
};


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
    void addValue(const QString& name, double value) {
         values.insert(name, QString::number(value));
    }
    void addExtra(const QString& name, const QString& value) {
         extra.insert(name, value);
    }


    QMap<QString, CriterialHypoth> hypoth;
    QMap<QString, QString> values;
    QMap<QString, QString> extra;

};

#endif // COMMONSTRUCT_H
