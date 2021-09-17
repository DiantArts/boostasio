#pragma once

#include <Packet/Text.hpp>



namespace packet {



class ConnectionRequest
    : public ::packet::Text
{

public:

    // ------------------------------------------------------------------ *structors

    ConnectionRequest(
        const ::std::string& name
    );

    ~ConnectionRequest();

};



} // namespace packet
