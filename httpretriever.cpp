#include "httpretriever.h"

HttpRetriever::HttpRetriever(char *url)
    : m_successfulConnection(false), m_htmlContents("")
{
    m_url = url;
    m_connection = NULL;
    curl_global_init(CURL_GLOBAL_DEFAULT);

    initializeConnection();

    if (connectionSuccess())
    {
        // Retrieve content for the URL
        m_curlCode = curl_easy_perform(m_connection);
        if (m_curlCode != CURLE_OK)
        {
            fprintf(stderr, "Failed to get '%s' [%s]\n", m_url, m_errorBuffer);
        }
    }
    else
    {
        fprintf(stderr, "Connection initializion failed\n");
    }
}

HttpRetriever::~HttpRetriever()
{
    curl_easy_cleanup(m_connection);
}

bool HttpRetriever::connectionSuccess()
{
    return m_successfulConnection;
}

std::string HttpRetriever::getHtmlContents()
{
    return m_htmlContents;
}

bool HttpRetriever::initializeConnection()
{
    m_connection = curl_easy_init();

    if (m_connection == NULL)
    {
        fprintf(stderr, "Failed to create CURL connection\n");
        m_successfulConnection = false;
        return m_successfulConnection;
    }

    m_curlCode = curl_easy_setopt(m_connection, CURLOPT_ERRORBUFFER, m_errorBuffer);
    if (m_curlCode != CURLE_OK)
    {
        fprintf(stderr, "Failed to set error buffer [%d]\n", m_curlCode);
        m_successfulConnection = false;
        return m_successfulConnection;
    }

    m_curlCode = curl_easy_setopt(m_connection, CURLOPT_URL, m_url);
    if (m_curlCode != CURLE_OK)
    {
        fprintf(stderr, "Failed to set URL [%s]\n", m_errorBuffer);
        m_successfulConnection = false;
        return m_successfulConnection;
    }

    m_curlCode = curl_easy_setopt(m_connection, CURLOPT_FOLLOWLOCATION, 1L);
    if (m_curlCode != CURLE_OK)
    {
        fprintf(stderr, "Failed to set redirect option [%s]\n", m_errorBuffer);
        m_successfulConnection = false;
        return m_successfulConnection;
    }

    m_curlCode = curl_easy_setopt(m_connection, CURLOPT_WRITEFUNCTION, writer);
    if (m_curlCode != CURLE_OK)
    {
        fprintf(stderr, "Failed to set writer [%s]\n", m_errorBuffer);
        m_successfulConnection = false;
        return m_successfulConnection;
    }

    m_curlCode = curl_easy_setopt(m_connection, CURLOPT_WRITEDATA, &m_htmlContents);
    if (m_curlCode != CURLE_OK)
    {
        fprintf(stderr, "Failed to set write data [%s]\n", m_errorBuffer);
        m_successfulConnection = false;
        return m_successfulConnection;
    }

    m_successfulConnection = true;
    return m_successfulConnection;
}

int HttpRetriever::writer(char *data, size_t size, size_t nmemb, std::string *writerData)
{
    if (!writerData)
    {
        return 0;
    }

    writerData->append(data, size * nmemb);

    return size * nmemb;
}

void HttpRetriever::addImageLink(const QString &imageLink)
{
    m_imageLinks << imageLink;
}

void HttpRetriever::downloadImages()
{
    for (const auto &imageLink : m_imageLinks)
    {
        std::string str = imageLink.toStdString();
        const char* img_url = str.c_str();

        CURL *curl;
        FILE *fp;
        CURLcode res;
        QString outFileName = getFilePath(imageLink);
        QByteArray array = outFileName.toLatin1();
        char* outfilename = array.data();

        curl = curl_easy_init();
        if (curl)
        {
            fp = fopen(outfilename, "wb");
            curl_easy_setopt(curl, CURLOPT_URL, img_url);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            fclose(fp);
            m_savedImages << outFileName;
        }
    }
}

QString HttpRetriever::getFilePath(const QString &link)
{
    auto tempLink = link;
    tempLink = tempLink.remove(QString::fromStdString("http://static-numista.com/catalogue/photos/"), Qt::CaseInsensitive);
    return tempLink;
}

QStringList HttpRetriever::getSavedImages()
{
    return m_savedImages;
}

#if 0
    This code was used for extracting the links of the images from the coin pages.
    for (int i = 0; i < str.size(); ++i)
    {
        if(str.at(i).contains("catalogue/photos", Qt::CaseInsensitive))
        {
            QString pic;
            pic = str.at(i);
            pic = pic.remove(QString::fromStdString("\t<a href=\""), Qt::CaseInsensitive);
            pic = pic.remove(QRegExp("target.*$", Qt::CaseInsensitive));
            pic = pic.remove('"');
            pic = pic.left(pic.length()-1);
            qDebug() << pic;
            httpRetriever.addImageLink(pic);
        }
    }

    httpRetriever.downloadImages();
#endif
