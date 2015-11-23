#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "huffman.h"

int main(int argc, char *argv[])
{
    //Qt main gui app config
    QGuiApplication app(argc, argv);
    app.setApplicationName("Huffman");
    app.setApplicationVersion("1.0");

    //Qt additional app configs
    QQmlApplicationEngine engine;
    QQmlContext *huffRoot = engine.rootContext();

    //Qt command line parser config
    QCommandLineParser parser;
    parser.setApplicationDescription("Huffman is a simple lossless file compresser that you can uses it to shrinks file's size");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addPositionalArgument("inputFile.x",
                                 QCoreApplication::translate("main", "Yours Original file to be compressed"));

    parser.addPositionalArgument("outputFile.huff",
                                 QCoreApplication::translate("main", "Huffed file gened name"));

    parser.addPositionalArgument("workDir",
                                 QCoreApplication::translate("main", "Set the huffman's Work Dir"));

    QCommandLineOption compress("c",
                                QCoreApplication::translate("main", "Compress yours inputed file"),
                                QCoreApplication::translate("main", "inputFile.x"));

    parser.addOption(compress);

    QCommandLineOption output("o",
                               QCoreApplication::translate("main", "Save Huffed file w user's custom typed name"),
                               QCoreApplication::translate("main", "outputFile.huff"));

    parser.addOption(output);
    QCommandLineOption workDir("d", QCoreApplication::translate("main", "Decompress in workDir"),
                               QCoreApplication::translate("main","workDir"));

    parser.addOption(workDir);

    QCommandLineOption graphicUserInterface("gui",
                                            QCoreApplication::translate("main","Uses Huffman's Graphical Users Interface Option"));

    parser.addOption(graphicUserInterface);
    parser.process(app);

    //Dev main config
    Huffman huff;

    //Verifying inputed arguments
    qint64 argumentNum =  app.arguments().size();
    if(argumentNum == 5)
    {
        if(parser.isSet(compress))
            exit(huff.consoleCompress(parser.value(compress), parser.value(output)));
        else
        {
            qDebug() << parser.helpText();
            exit(0);
        }
    }
    else if(argumentNum == 4)
    {
        if(parser.isSet(workDir))
            exit(huff.consoleDecompress(app.arguments().at(1), parser.value(workDir)));
        else
        {
            qDebug() << parser.helpText();
            exit(0);
        }
    }
    else if(argumentNum == 3)
    {
        if(parser.isSet(compress))
            exit(huff.consoleCompress(parser.value(compress), QString("")));
        else
        {
            qDebug() << parser.helpText();
            exit(0);
        }
    }
    else if(argumentNum == 2)
    {
        if(parser.isSet(graphicUserInterface))
        {
            huffRoot->setContextProperty("_huff",&huff);
            engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
        }

        else if (!app.arguments().at(1).isEmpty())
            exit(huff.consoleDecompress(app.arguments().at(1), QString("")));

        else
        {
            qDebug() << parser.helpText();
            exit(0);
        }
    }

    else
    {
            qDebug() << parser.helpText();
            exit(0);
    }

    return app.exec();
}
