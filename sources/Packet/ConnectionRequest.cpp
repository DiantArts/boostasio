#include <pch.hpp>
#include <Packet/ConnectionRequest.hpp>



::packet::ConnectionRequest::ConnectionRequest(
    const ::std::string& name
)
    : ::packet::Text{ name, ::packet::Header::Type::connectionRequest, false }
{}

::packet::ConnectionRequest::~ConnectionRequest() = default;
