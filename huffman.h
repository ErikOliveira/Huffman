#ifndef HUFFMAN_H
#define HUFFMAN_H
#include <QObject>
#include <QUrl>
#include "actionscenter.h"


class Huffman: public QObject
{
    Q_OBJECT
    ActionsCenter *m_huff;
    QString m_inputFilePath;
    QString m_outputFilePath;
    QPair <bool, QString> m_flag;
public:
    Huffman();
    Q_INVOKABLE int compress(const QUrl input, const QUrl output);
    Q_INVOKABLE int decompress(const QUrl input, const QUrl output);

    int consoleCompress(const QString &input, const QString &output);
    int consoleDecompress(const QString &input, const QString &output);
};

#endif // HUFFMAN_H
