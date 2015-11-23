#include "huffman.h"

Huffman::Huffman()
{
    m_huff = new ActionsCenter();
    m_inputFilePath = m_outputFilePath = "";
    m_flag = QPair<bool, QString>();
}

int Huffman::compress(const QUrl input, const QUrl output)
{
   m_flag.first = input.isLocalFile();
   if(m_flag.first)
   {
       m_inputFilePath = input.toLocalFile();
       m_outputFilePath = output.toLocalFile();

       if((!m_inputFilePath.isEmpty()) & (m_inputFilePath != "/"))
           m_flag = m_huff->compIOPaths(m_inputFilePath,m_outputFilePath);
       else
           return -1;

       if(m_flag.first)
           return m_huff->compCompress(m_inputFilePath, m_flag.second);
       else
           return 0xFF;
   }
   else
   {
       qDebug() << "Sorry something much wrong occur, it isn't a local file\nMay you want give it another try\n\n";
       return -1;
   }
}

int Huffman:: decompress(const QUrl input, const QUrl output)
{
    m_flag.first = input.isLocalFile();

    if(m_flag.first)
    {
        m_inputFilePath = input.toLocalFile();
        m_outputFilePath = output.toLocalFile();

        if((!m_inputFilePath.isEmpty()) & (m_inputFilePath != "/"))
            m_flag = m_huff->decompIOPaths(m_inputFilePath, m_outputFilePath);
        else
            return -1;

        if(m_flag.first)
            return m_huff->decompDecompress(m_flag.second);
        else
            return 0xFF;
    }
    else
    {
        qDebug() << "Sorry something much wrong occur, it isn't a local file\nMay you want give it another try\n\n";
        return -1;
    }
}

int Huffman::consoleCompress(const QString &input, const QString &output)
{
    m_flag = m_huff->compIOPaths(input,output);

    if(m_flag.first)
        return m_huff->compCompress(input,m_flag.second);
    else
        return 0xFF;
}

int Huffman::consoleDecompress(const QString &input, const QString &output)
{
    m_flag = m_huff->decompIOPaths(input, output);

    if(m_flag.first)
        return m_huff->decompDecompress(m_flag.second);
    else
        return 0xFF;
}

