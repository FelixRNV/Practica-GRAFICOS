#include "principal.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QInputDialog>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QStringList  idiomas;
    idiomas << "Ingles" << "Español" << "Griego" << "Japónes";
    QString idiomaSelec = QInputDialog::getItem(NULL,"Idioma","Sleccione un idioma",idiomas);
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        QString baseName = "Calculadora_" + QLocale(locale).name();
        if (idiomaSelec=="Ingles")
            baseName = "MiPaint_en_US";
        if (idiomaSelec=="Japónes")
            baseName = "MiPaint_jp_JP";
        if (idiomaSelec=="Griego")
            baseName = "MiPaint_el_GR";
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    Principal w;
    w.show();
    return a.exec();
}
