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

public slots:
   void setParam1(bool value)              { param1 = value;            }
   void setParam2(const QString &value)    { param2 = value.toInt();    }
   void setParam3(const QString &value)    { param3 = value.toDouble(); }

   void setLucyN(const QString &value)    { lucyN = value.toDouble(); }
};

#endif
