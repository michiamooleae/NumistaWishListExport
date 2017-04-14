#include "httpretriever.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <list>
#include <iterator>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <QDebug>
#include <QStringList>
#include <QString>

int main()
{
    //@todo The private access web page download directly from Numista is not implemented yet.
    std::ifstream input("/Users/skyfire/Desktop/Numista.html");
    std::string entryTag = "<tr id=\"";
    const int SKIP_LINES = 7; // We need to skip 7 lines until we reach any useful information

    for (std::string line; std::getline(input, line);)
    {
        if (line.find(entryTag) != std::string::npos)
        {
            for (int i = 0; i < SKIP_LINES; ++i)
            {
                std::getline(input,line);
            }

            std::string countryLine;
            std::getline(input,countryLine);
            std::string countryName = countryLine.substr(countryLine.find("title")+7, countryLine.find("><")-countryLine.find("title")-8);
            std::cout << countryName << std::endl;

            std::string linkLine;
            std::getline(input,linkLine);
            std::string linkStr = linkLine.substr(linkLine.find_first_of("=")+2, linkLine.find_first_of(">")-linkLine.find_first_of("=")-3);
            std::cout << linkStr << std::endl;
            std::string coinName = linkLine.substr(linkLine.find_first_of(">")+1);
            coinName.erase(coinName.find("<"));
            std::cout << coinName << std::endl;

            std::string KMline;
            std::getline(input,KMline);
            size_t KMpos = KMline.find(";");
            if (KMpos != std::string::npos)
            {
                std::string KMnum = KMline.substr(KMpos+1, KMline.find("<")-KMpos-1);
                std::cout << KMnum << std::endl;
            }
            else
            {
                std::cout << "No KM# entry provided." << std::endl;
            }
        }
    }


//    HttpRetriever httpRetriever("http://en.numista.com/catalogue/pieces64430.html");
//    auto contents = httpRetriever.getHtmlContents();
//    QString html = QString::fromUtf8(contents.c_str());
//    QStringList str = html.split("\n");

//    for (int i = 0; i < str.size(); ++i)
//    {
//        if(str.at(i).contains("catalogue/photos", Qt::CaseInsensitive))
//        {
//            QString pic;
//            pic = str.at(i);
//            pic = pic.remove(QString::fromStdString("\t<a href=\""), Qt::CaseInsensitive);
//            pic = pic.remove(QRegExp("target.*$", Qt::CaseInsensitive));
//            pic = pic.remove('"');
//            pic = pic.left(pic.length()-1);
//            qDebug() << pic;
//            httpRetriever.addImageLink(pic);
//        }
//    }


    return EXIT_SUCCESS;
}
