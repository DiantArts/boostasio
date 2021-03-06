#pragma once

#include <Packet/APacket.hpp>



namespace client {



class Connection {

public:

    using PacketType = ::APacket;



public:

    // ------------------------------------------------------------------ *structors

    Connection(
        const ::std::string& host,
        const ::std::string& port,
        const ::std::string& name,
        const ::std::size_t pingFrequencyAsMs = 1000
    );

    ~Connection();



    // ------------------------------------------------------------------ methods

    auto isValid() const
        -> bool;

    auto getLatency() const
        -> ::std::size_t;



// ------------------------------------------------------------------ send

    void send(
        ::std::unique_ptr<::APacket>&& message
    );

    template <
        typename MessageType
    > void send(
        auto&&... args
    );



    // ------------------------------------------------------------------ receive

    void startReceive(
        ::std::function<void(::APacket&)>&& func
    );

    void stopReceive();



private:

    void startReceiveImpl();

    void prehandleMessage();


    // ------------------------------------------------------------------ connection

    void maintainConnection();



private:

    // ------------------------------------------------------------------ Threads
    ::std::unique_ptr<::std::thread> m_maintainConnectionThread;
    ::std::unique_ptr<::std::thread> m_receiverThread;

    // ------------------------------------------------------------------ connection
    ::boost::asio::io_context m_ioContext;
    ::boost::asio::ip::udp::endpoint m_endpoint;
    ::boost::asio::ip::udp::socket m_socket;
    ::std::uint8_t m_lastId; // important message IDs allow a packet loss check

    // ------------------------------------------------------------------ receive
    static inline constexpr ::std::size_t bufferLength{ 254 };
    ::std::array<::std::byte, Connection::bufferLength> m_buffer;
    ::std::unique_ptr<::std::function<void(::APacket&)>> m_userReceiveFunc;

    // ------------------------------------------------------------------ ping
    ::std::chrono::time_point<std::chrono::system_clock> m_pingTimepoint; // allows to get the server latency
    ::std::size_t m_latency;
    ::std::atomic<bool> m_isPingValidated;
    ::std::atomic<bool> m_isRunning{ true };
    const ::std::size_t m_pingFrequency;

    // ------------------------------------------------------------------ packetLoss
    ::std::queue<::std::unique_ptr<::APacket>> m_sentPackets;

};



} // namespace client

#include <Client/Connection.impl.hpp>
