#pragma once

#include <Packet/APacket.hpp>



class Buffer {

public:

    // ------------------------------------------------------------------ *structors

    // server
    Buffer(
        const int port
    );

    // client
    Buffer(
        const ::std::string& host,
        const int port
    );

    ~Buffer();



    // ------------------------------------------------------------------ send

    template <
        typename MessageType
    > void send(
        ::boost::asio::ip::udp::endpoint endpoint,
        auto&&... args
    );

    template <
        typename MessageType
    > void reply(
        auto&&... args
    );

    template <
        typename MessageType
    > void send(
        MessageType&& message,
        ::boost::asio::ip::udp::endpoint endpoint
    );

    template <
        typename MessageType
    > void reply(
        MessageType&& message
    );

    void send(
        ::std::unique_ptr<::APacket>&& message,
        ::boost::asio::ip::udp::endpoint endpoint
    );

    void reply(
        ::std::unique_ptr<::APacket>&& message
    );



    // ------------------------------------------------------------------ packets

    void clearSentPackets();

    auto isPacketSent(
        ::std::uint8_t packetId
    ) -> bool;



    // ------------------------------------------------------------------ receive

    template <
        auto function
    > void startReceive();

    void stopReceive();



private:

    template <
        auto function
    > void startReceiveImpl();

    template <
        auto function
    > void prehandleReceive(
        const boost::system::error_code& error,
        size_t bytesTransferred
    );



private:

    // ------------------------------------------------------------------ base
    ::boost::asio::io_context m_ioContext;
    ::boost::asio::ip::udp::endpoint m_endpoint;
    ::boost::asio::ip::udp::socket m_socket;

    // ------------------------------------------------------------------ receive
    static inline constexpr ::std::size_t bufferLength{ 254 };
    ::std::array<::std::byte, Buffer::bufferLength> m_buffer;

    // ------------------------------------------------------------------ packetSafety
    ::std::uint8_t m_nextPacketId{ 1 };
    ::std::queue<::std::unique_ptr<::APacket>> m_sentPackets;

    // ------------------------------------------------------------------ ping
    // ::std::chrono::time_point<std::chrono::system_clock> m_pingTimepoint; // allows to get the server latency
    // ::std::size_t m_latency;
    // ::std::atomic<bool> m_isPingValidated;
    // ::std::atomic<bool> m_isRunning{ true };
    // const ::std::size_t m_pingFrequency;

};

#include <Buffer.impl.hpp>
