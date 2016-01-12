#include "gui/MainWindow.h"

#include "algorithms/Algorithm.h"
#include "algorithms/Greenify.h"
#include "algorithms/Lucy.h"
#include "algorithms/LucyOMP.h"
#include "algorithms/Cartoonize.h"
#include "algorithms/CartoonizeOMP.h"

#include "algorithms/Parameters.h"

#include <memory>

#include <QApplication>
#include <QLineEdit>
#include <QLabel>

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    // structure containing parameters you can set
    Parameters params;

    // Create main window
    MainWindow window(params);





    // instantiate algorithms
    std::shared_ptr<Algorithm> greenify(new Greenify);
    //------------------------------------
    // YOU SHOULD ADD SOMETHING HERE..
    //------------------------------------
    std::shared_ptr<Algorithm> lucy(new Lucy);
    std::shared_ptr<Algorithm> lucyomp(new LucyOMP);
    std::shared_ptr<Algorithm> cartoonize(new Cartoonize);
    std::shared_ptr<Algorithm> cartoonizeomp(new CartoonizeOMP);

    // add algorithms to gui
    window.addAlgorithm("Greenify", greenify.get());
    //------------------------------------
    // YOU SHOULD ADD SOMETHING HERE..
    //------------------------------------
    window.addAlgorithm("Lucy", lucy.get());
    window.addAlgorithm("LucyOMP", lucyomp.get());
    window.addAlgorithm("Cartoonize", cartoonize.get());
    window.addAlgorithm("CartoonizeOMP", cartoonizeomp.get());






    // Example code for adding some parameters to the GUI

    // add param 1
    QCheckBox *checkBox_params1 = window.addCheckBox("Param1:");
    checkBox_params1->setChecked(params.param1);
    MainWindow::connect(checkBox_params1, SIGNAL(toggled(bool)), &params, SLOT(setParam1(bool)));

    // add param 2
    QLineEdit *lineEdit_params2 = window.addIntLineEdit("Param2:");
    lineEdit_params2->setText(QString::number(params.param2));
    MainWindow::connect(lineEdit_params2, SIGNAL(textChanged(const QString&)), &params, SLOT(setParam2(QString)));

    // add param 3
    QLineEdit *lineEdit_param3 = window.addDoubleLineEdit("Param3:");
    lineEdit_param3->setText(QString::number(params.param3));
    MainWindow::connect(lineEdit_param3, SIGNAL(textChanged(const QString&)), &params, SLOT(setParam3(QString)));



    //------------------------------------
    // YOU SHOULD ADD SOMETHING HERE..
    //------------------------------------
    // new parameters

    // Lucy Parameter
    QLineEdit *lineEdit_lucyN = window.addIntLineEdit("Lucy - Anzahl Iterationen:");
    lineEdit_lucyN->setText(QString::number(5));
    MainWindow::connect(lineEdit_lucyN, SIGNAL(textChanged(const QString&)), &params, SLOT(setLucyN(QString)));

    // Cartoonize Parameter
    QLineEdit *lineEdit_filterSize = window.addIntLineEdit("Cartoonize - Filtergroesse:");
    lineEdit_filterSize->setText(QString::number(15));
    MainWindow::connect(lineEdit_filterSize, SIGNAL(textChanged(const QString&)), &params, SLOT(setFilterSize(QString)));

    
    QLineEdit *lineEdit_sigmaD = window.addIntLineEdit("Cartoonize - Sigma D:");
    lineEdit_sigmaD->setText(QString::number(7));
    MainWindow::connect(lineEdit_sigmaD, SIGNAL(textChanged(const QString&)), &params, SLOT(setSigmaD(QString)));

    QLineEdit *lineEdit_sigmaR = window.addIntLineEdit("Cartoonize - Sigma R:");
    lineEdit_sigmaR->setText(QString::number(23));
    MainWindow::connect(lineEdit_sigmaR, SIGNAL(textChanged(const QString&)), &params, SLOT(setSigmaR(QString)));

    QLineEdit *lineEdit_tau = window.addIntLineEdit("Cartoonize - Tau:");
    lineEdit_tau->setText(QString::number(50));
    MainWindow::connect(lineEdit_tau, SIGNAL(textChanged(const QString&)), &params, SLOT(setTau(QString)));


    // finally show the main window
    window.show();

    // Run main event loop
    return app.exec();
}
