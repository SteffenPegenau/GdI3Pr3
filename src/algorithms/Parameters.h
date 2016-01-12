#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <QObject>

class Parameters : public QObject
{
Q_OBJECT

public:
   Parameters();

   bool   param1;
   int    param2;
   double param3;
   
   int lucyN;
   
   // Cartoonize Params
   int filterSize, sigmaD, sigmaR, tau;

public slots:
   void setParam1(bool value)              { param1 = value;            }
   void setParam2(const QString &value)    { param2 = value.toInt();    }
   void setParam3(const QString &value)    { param3 = value.toDouble(); }

   void setLucyN(const QString &value)    { lucyN = value.toDouble(); }
   
   void setFilterSize(const QString &value)    { filterSize = value.toInt(); }
   void setSigmaD(const QString &value)    { sigmaD = value.toInt(); }
   void setSigmaR(const QString &value)    { sigmaR = value.toInt(); }
   void setTau(const QString &value)    { tau = value.toInt(); }
};

#endif
