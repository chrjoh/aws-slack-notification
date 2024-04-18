#include "client.hpp"

HttpClient::HttpClient() {
    curl_global_init(CURL_GLOBAL_ALL);
}

int HttpClient::doPostToSlack(std::string const& url, std::string const& token, std::string const& msg) {
    CURLcode ret;
    CURL* hnd;
    struct curl_slist* slist;
    std::string auth = "Authorization: Bearer " + token;
    slist = NULL;
    slist = curl_slist_append(slist, "Content-Type: application/json");
    slist = curl_slist_append(slist, auth.c_str());
    hnd = curl_easy_init();
    curl_easy_setopt(hnd, CURLOPT_BUFFERSIZE, 102400L);
    curl_easy_setopt(hnd, CURLOPT_URL, url.c_str());
    curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, msg.c_str());
    curl_easy_setopt(hnd, CURLOPT_POSTFIELDSIZE_LARGE, (curl_off_t)msg.length());
    curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, slist);
    curl_easy_setopt(hnd, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(hnd, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_2TLS);
    curl_easy_setopt(hnd, CURLOPT_FTP_SKIP_PASV_IP, 1L);
    curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);

    ret = curl_easy_perform(hnd);
    curl_easy_cleanup(hnd);
    curl_slist_free_all(slist);
    return ret;
}

nlohmann::json HttpClient::doGetSecretParameter(std::string const& url, std::string const& token, Params const& params) {
    CURLcode ret;
    CURL* hnd;
    struct curl_slist* slist;
    std::string resultBody{};
    auto curl_callback = static_cast<size_t (*)(void*, size_t, size_t, void*)>([](void* ptr, size_t size, size_t nmemb, void* resultBody) {
        *(static_cast<std::string*>(resultBody)) += std::string{(char*)ptr, size * nmemb};
        return size * nmemb;
    });

    hnd = curl_easy_init();
    std::string auth = "X-Aws-Parameters-Secrets-Token: " + token;

    std::string getUrl = url + paramsString(params);
    curl_easy_setopt(hnd, CURLOPT_URL, getUrl.c_str());
    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, curl_callback);
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, (void*)&resultBody);
    curl_easy_setopt(hnd, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    slist = NULL;
    slist = curl_slist_append(slist, "Accept: application/json");
    slist = curl_slist_append(slist, auth.c_str());
    curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, slist);

    ret = curl_easy_perform(hnd);
    nlohmann::json Doc;
    if (ret != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(ret));
    } else {
        Doc = nlohmann::json::parse(resultBody);
    }

    curl_easy_cleanup(hnd);
    curl_slist_free_all(slist);
    return Doc;
}

/*
std::string params_string(Params const &params)
auto str= get("where.com?"+ params_string({ {"q", "C++"}, { "img", "true" } }));
*/
std::string HttpClient::paramsString(Params const& params) {
    if (params.empty()) return "";
    Params::const_iterator pb = params.cbegin(), pe = params.cend();
    char* output = curl_easy_escape(NULL, pb->second.c_str(), pb->second.size());
    std::string data = "?" + pb->first + "=" + std::string(output);
    curl_free(output);
    ++pb;
    if (pb == pe) return data;
    for (; pb != pe; ++pb) {
        output = curl_easy_escape(NULL, pb->second.c_str(), pb->second.size());
        data += "&" + pb->first + "=" + std::string(output);
        curl_free(output);
    }
    return data;
}

HttpClient::~HttpClient() {
    curl_global_cleanup();
}