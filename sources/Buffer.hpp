#pragma once

#include <Packet/APacket.hpp>



class Buffer {

public:

    static inline constexpr ::std::size_t bufferLength{ 254 };



public:

    // ------------------------------------------------------------------ *structors

    Buffer(
        ::boost::asio::io_context ioContext,
        const ::std::string& host,
        const int port
    );

    Buffer(
        ::boost::asio::io_context ioContext,
        int port
    );

    ~Buffer();



    // ------------------------------------------------------------------ packets

    void clearSentPackets();

    auto isPacketSent(
        ::std::uint8_t packetId
    ) -> bool;



    // ------------------------------------------------------------------ getters

    auto get()
        -> ::std::array<::std::byte, Buffer::bufferLength>&;

    operator ::APacket&();


    // ------------------------------------------------------------------ send

    template <
        typename MessageType
    > void send(
        ::boost::asio::ip::udp::endpoint endpoint,
        auto&&... args
    );

    template <
        typename MessageType
    > void send(
        MessageType&& message,
        ::boost::asio::ip::udp::endpoint endpoint
    );

    void send(
        ::std::unique_ptr<::APacket>&& message,
        ::boost::asio::ip::udp::endpoint endpoint
    );



    template <
        typename MessageType
    > void reply(
        auto&&... args
    );

    template <
        typename MessageType
    > void reply(
        MessageType&& message
    );

    void reply(
        ::std::unique_ptr<::APacket>&& message
    );



    // ------------------------------------------------------------------ receive

    template <
        auto function
    > void startReceive();



private:

    template <
        auto function
    > void prehandleReceive(
        const boost::system::error_code& error,
        size_t bytesTransferred
    );



private:

    // ------------------------------------------------------------------ receive
    ::boost::asio::ip::udp::endpoint m_endpoint;
    ::boost::asio::ip::udp::socket m_socket;
    ::std::array<::std::byte, Buffer::bufferLength> m_buffer;

    // ------------------------------------------------------------------ packetLoss
    ::std::queue<::std::unique_ptr<::APacket>> m_sentPackets;
    ::std::uint8_t m_nextPacketId{ 1 };

};

#include <Buffer.impl.hpp>
