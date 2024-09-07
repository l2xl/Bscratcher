#include "walletwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "config.hpp"

using namespace bscratcher;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    auto config = std::make_shared<Config>(argc, argv);


    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "scratcher_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    WalletWindow w(config);
    w.show();
    return a.exec();
}
