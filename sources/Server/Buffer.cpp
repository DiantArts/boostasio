#include <pch.hpp>
#include <Server/Buffer.hpp>
#include <Packet/Types.hpp>



// ------------------------------------------------------------------ *structors

::server::Buffer::Buffer(
    ::boost::asio::ip::udp::socket& socket
)
    : m_socket{ socket }
{}

::server::Buffer::~Buffer() = default;



// ------------------------------------------------------------------ errorHandling
