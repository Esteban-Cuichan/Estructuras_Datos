#include <curl/curl.h>
#include "Conexion.h"
#include <stdexcept>
#include <sstream>
#include <fstream>
using namespace std;

static size_t writeToString(void* ptr, size_t size, size_t nmemb, void* userdata) {
    size_t total=size*nmemb;
    string* str=reinterpret_cast<string*>(userdata);
    str->append(reinterpret_cast<char*>(ptr), total);
    return total;
}

static size_t writeToFile(void* ptr, size_t size, size_t nmemb, void* userdata) {
    FILE* stream=reinterpret_cast<FILE*>(userdata);
    if (!stream) return 0;
    size_t written=fwrite(ptr, size, nmemb, stream);
    return written;
}

Conexion::Conexion() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

string Conexion::postJson(const string& url, const string& jsonPayload) {
    CURL* curl=curl_easy_init();
    if (!curl) throw runtime_error("No se pudo inicializar CURL");

    string response;
    struct curl_slist* headers=nullptr;
    headers=curl_slist_append(headers, "Content-Type: application/json");
    headers=curl_slist_append(headers, "User-Agent: cpp-translator/1.0");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonPayload.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeToString);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

    CURLcode res=curl_easy_perform(curl);
    if (res!=CURLE_OK) {
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        ostringstream oss;
        oss<<"Error en la petición HTTP: "<<curl_easy_strerror(res);
        throw runtime_error(oss.str());
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    return response;
}

string Conexion::getUrl(const string& url) {
    CURL* curl=curl_easy_init();
    if (!curl) throw runtime_error("No se pudo inicializar CURL (GET)");

    string response;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeToString);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "cpp-translator/1.0");
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

    CURLcode res=curl_easy_perform(curl);
    if (res!=CURLE_OK) {
        curl_easy_cleanup(curl);
        ostringstream oss;
        oss<<"Error en GET HTTP: "<<curl_easy_strerror(res);
        throw runtime_error(oss.str());
    }

    curl_easy_cleanup(curl);
    return response;
}

bool Conexion::descargarArchivo(const string& url, const string& rutaDestino) {
    CURL* curl=curl_easy_init();
    if (!curl) return false;

    FILE* fp=nullptr;
#ifdef _WIN32
    fp=_wfopen(wstring(rutaDestino.begin(), rutaDestino.end()).c_str(), L"wb");
#else
    fp=fopen(rutaDestino.c_str(), "wb");
#endif
    if (!fp) {
        curl_easy_cleanup(curl);
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeToFile);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "cpp-translator/1.0");
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 15L);

    CURLcode res=curl_easy_perform(curl);
    fclose(fp);
    curl_easy_cleanup(curl);

    return (res==CURLE_OK);
}

Conexion::~Conexion() {
    curl_global_cleanup();
}