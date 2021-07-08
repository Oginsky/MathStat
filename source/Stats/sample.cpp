#include "sample.h"

#include <algorithm>


double SampleCharacteristic::expectation()   const { return _expectation;  }
double SampleCharacteristic::variance()      const { return _variance;     }
double SampleCharacteristic::correct_var()   const { return _correct_var;  }
double SampleCharacteristic::median()        const { return _median;       }
double SampleCharacteristic::span()          const { return _span;         }
double SampleCharacteristic::min()           const { return _min;          }
double SampleCharacteristic::max()           const { return _max;          }

Sample::Sample() {
    _size = 0;
}
Sample::Sample(QString name) {
    _name = name;
    _size = 0;
}
Sample::Sample(QVector<double>& data, QString name) {
    _name = name;
    sampleData = data;
    _size = data.size();
    countCharacteristic();
}


QVector<double>& Sample::getSampleData() { return sampleData; }
void Sample::setData(const QVector<double>& data) {
    sampleData = data;
    _size = data.size();
    countCharacteristic();
}

void Sample::countCharacteristic() {
    if(_size == 0) return;

    for(auto value: sampleData) _characters._expectation += value;
    _characters._expectation /= _size;

    for(auto value: sampleData) _characters._variance += (value - _characters._expectation)*(value - _characters._expectation);
    _characters._variance /= _size;

    _characters._correct_var = ( (double)_size/(double)(_size-1) ) * _characters._variance;

    QVector<double> tmp = sampleData;
    std::sort(tmp.begin(), tmp.end());

    size_t med_idx = _size / 2;
    _characters._median = (_size%2 == 1) ? (tmp[med_idx]) : ( (tmp[med_idx] + tmp[med_idx-1]) / 2.0);

    _characters._min = tmp.front();
    _characters._max = tmp.back();
    _characters._span = _characters._max - _characters._min;
}

void Sample::setName(const QString& name) {
    _name = name;
}


SampleCharacteristic Sample::characters() const { return _characters; }
SampleCharacteristic& Sample::characters()      { return _characters; }


size_t Sample::size()   const { return _size; }
QString Sample::name()  const { return _name; }

double& Sample::operator[](size_t index) {
    return sampleData[index];
}

double Sample::operator[](size_t index) const {
    return sampleData[index];
}

const Sample Sample::operator+(const Sample& right) {
    Sample result;
    result.sampleData = this->sampleData + right.sampleData;
    result._size = this->_size + right._size;
}


void Sample::push_back(double value) {
    _size++;
    sampleData.push_back(value);
}

void Sample::reserve(size_t size) {
    sampleData.reserve(size);
}

void Sample::resize(size_t size) {
    this->_size = size;
    sampleData.resize(size);
}

void Sample::makeSort() {
    std::sort(sampleData.begin(), sampleData.end(), std::greater<double>());
}


const QVector<double>& Sample::data() const { return sampleData; }


