#pragma once

#include <Udp/Packet/APacket.hpp>


namespace udp::packet { class Text; }



namespace udp {



class Client {

public:

    // ------------------------------------------------------------------ *structors

    Client(
        const ::std::string& host,
        const ::std::string& port
    );

    ~Client();



    // ------------------------------------------------------------------ run

    void run();



    // ------------------------------------------------------------------ send

    void startSend();

    void handleSend(
        const ::udp::APacket& message
    );



    // ------------------------------------------------------------------ receive

    void startReceive();

    void handleResponse();



private:

    ::boost::asio::io_context m_ioContext;
    ::boost::asio::ip::udp::endpoint m_endpoint;
    ::boost::asio::ip::udp::socket m_socket;

    static inline constexpr ::std::size_t bufferLength{ 254 };
    ::std::array<::std::byte, Client::bufferLength> m_buffer;

};



} // namespace udp
