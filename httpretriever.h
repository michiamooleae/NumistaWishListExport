#ifndef HTTPRETRIEVER_H
#define HTTPRETRIEVER_H

#include <string>

#include <curl/curl.h>

#include <QStringList>

class HttpRetriever
{
  public:
    HttpRetriever(char *url);
    ~HttpRetriever();
    bool connectionSuccess();
    std::string getHtmlContents();
    void addImageLink(const QString& link);
    void downloadImages();
    QString getFilePath(const QString& link);
    QStringList getSavedImages();
  private:
    bool initializeConnection();
    static int writer(char *data, size_t size, size_t nmemb, std::string *writerData);
  private:
    QStringList m_imageLinks;
    bool m_successfulConnection;
    CURL *m_connection;
    CURLcode m_curlCode;
    char *m_url;
    char m_errorBuffer[CURL_ERROR_SIZE];
    std::string m_htmlContents;
    QStringList m_savedImages;
};

#endif // HTTPRETRIEVER_H
