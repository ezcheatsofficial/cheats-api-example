#pragma once
/*****************************************************************/ /**
 * \file   API.h
 * \brief
 * \author Oniel, creator
 * \date   May 2021
 *********************************************************************/
#pragma once
#define _SILENCE_CXX17_C_HEADER_DEPRECATION_WARNING
#include <boost/property_tree/json_parser.hpp>
#include <cpr/cpr.h>
#include <iostream>
#include <regex>

namespace ezcheats {
class API {
public:
    API(std::string apiUrl, std::string cheatId, std::string userSecretData);

    enum class RequestType {
        GET,
        POST
    };

    unsigned int onlineCount();
    void updateOnline();
    std::string userTimeLeft();
    std::string lastError();

private:
    // unique private cheat ID
    std::string m_id;
    // secret information about the user (HWID)
    std::string m_secret_data;
    // URL to send API requests to
    std::string m_apiUrl;
    std::string m_lastErrorMessage = "";
    //  Sending a request to the server, returns a response
    std::string request(RequestType type, const std::string& host,
        const std::string& body, const std::string& data);
};
}