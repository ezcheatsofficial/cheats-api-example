#include "API.h"

using namespace std;

/**
* Client API Constructor.
* 
* \param apiUrl - URL of the Private Cheat Server API
* \param cheatId - Private Cheat ID
* \param userSecretData - the user's secret key, for example, HWID
* 
* \throw Invalid URL format
*/
ezcheats::API::API(string apiUrl, string cheatId, string userSecretData)
    : m_id(cheatId)
    , m_secret_data(userSecretData)
{
    auto regexPattern = regex(
        "(?:(?:https?):\/\/)(?:\([-A-Z0-9+&@#\/%=~_|$?!:,.]*\)|[-A-Z0-9+&@#\/%=~_|$?!:,.])*(?:\([-A-Z0-9+&@#\/%=~_|$?!:,.]*\)|[A-Z0-9+&@#\/%=~_|$])");

    if (!regex_match(apiUrl, regexPattern)) {
        throw std::invalid_argument("Invalid URL format");
    }

    m_apiUrl = apiUrl;
}

/**
 * Sending an HTTP request with SSL handshake support.
 * 
 * \param type - RequestType type (GET or POST)
 * \param host - Host URL
 * \param body - request body passed to path, used in GET requests
 * \param data - string (raw data) in JSON format used in POST requests
 * \return 
 */
string ezcheats::API::request(RequestType type, const string& host,
    const string& body, const string& data)
{
    // TODO: implement error handling
    cpr::Response r = type == RequestType::GET ? cpr::Get(cpr::Url { host + body })
                                               : cpr::Post(cpr::Url { host + body },
                                                   cpr::Body { data },
                                                   cpr::Header { { "Content-Type", "application/json" } });
    return r.text;
}

/**
 * Getting the amount of the current online cheat.
 * 
 * \return integer - number of online users
 */
UINT ezcheats::API::onlineCount()
{
    string urlBody = "/api/app/online/" + m_id + "/";
    string result = this->request(RequestType::GET, m_apiUrl, urlBody, "");
    stringstream strStream(result);
    boost::property_tree::ptree pt;
    boost::property_tree::read_json(strStream, pt);

    UINT onlineCount = pt.get<int>("online");

    return onlineCount;
}

/**
 * Send the "online" status to the server so that 
 * the current user is counted in the online counter.
 * \see onlineCount
 */
void ezcheats::API::updateOnline()
{
    string urlBody = "/api/app/online/";
    string rawData = "{\"cheat_id\": \"" + m_id + "\", \"secret_data\": \"" + m_secret_data + "\"}";
    string _ = this->request(RequestType::POST, m_apiUrl, urlBody, rawData);
}

/** 
 * The status of the user's current subscription.
 * 
 * \return the string can have one of the following types:
 * 1. negative number if the subscription has ended;
 * 2. 'inactive', if the subscription is disabled;
 * 3. 'lifetime', if an infinite subscription is active.
 */
string ezcheats::API::userTimeLeft()
{
    string urlBody = "/api/app/time-left/" + m_id + "/" + m_secret_data + "/";
    string result = this->request(RequestType::GET, m_apiUrl, urlBody, "");
    stringstream strStream(result);
    boost::property_tree::ptree pt;
    boost::property_tree::read_json(strStream, pt);

    string timeLeft = pt.get<string>("time_left");
    string secretData = pt.get<string>("secret_data");

    if (secretData == m_secret_data)
        return timeLeft;
    return "";
}
