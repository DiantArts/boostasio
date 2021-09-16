#pragma once

#include <Packet/Types.hpp>



// ------------------------------------------------------------------ send

template <
    typename MessageType
> void ::client::Connection::send(
    ::boost::asio::ip::udp::endpoint endpoint,
    auto&&... args
)
{
    ::std::unique_ptr<::APacket> message{ ::std::make_unique<MessageType>(
        ::std::forward<decltype(args)>(args)...
    ) };

    // debug
    message->display("->");

    m_socket.send_to(::boost::asio::buffer(message.get(), message->getSize()), endpoint);
    m_sentPackets.push(::std::move(message));
}

// template <
    // typename MessageType
// > void ::client::Connection::send(
    // MessageType&& message,
    // ::boost::asio::ip::udp::endpoint endpoint
// )
// {
    // debug
    // message.display("->");

    // m_socket.send_to(::boost::asio::buffer(&message, message.getSize()), endpoint);
    // m_sentPackets.push(::std::make_unique<::APacket>(::std::move(message)));
// }

template <
    typename MessageType
> void ::client::Connection::reply(
    auto&&... args
)
{
    ::std::unique_ptr<::APacket> message{ ::std::make_unique<MessageType>(
        ::std::forward<decltype(args)>(args)...
    ) };

    // debug
    message->display("->");

    m_socket.send_to(::boost::asio::buffer(message.get(), message->getSize()), m_endpoint);
    m_sentPackets.push(::std::move(message));
}

// template <
    // typename MessageType
// > void ::client::Connection::reply(
    // MessageType&& message
// )
// {
    // debug
    // message.display("->");

    // m_socket.send_to(::boost::asio::buffer(&message, message.getSize()), m_endpoint);
    // m_sentPackets.push(::std::make_unique<::APacket>(::std::move(message)));
// }



// ------------------------------------------------------------------ receive

template <
    auto function
> void ::client::Connection::startReceive()
{
    this->startReceiveImpl<function>();
    m_ioContext.run();
}

template <
    auto function
> void ::client::Connection::startReceiveImpl()
{
    m_socket.async_receive_from(
        ::boost::asio::buffer(m_buffer, this->bufferLength),
        m_endpoint,
        ::boost::bind(
            &client::Connection::prehandleReceive<function>,
            this,
            ::boost::asio::placeholders::error,
            ::boost::asio::placeholders::bytes_transferred
        )
    );
}

template <
    auto function
> void ::client::Connection::prehandleReceive(
    const boost::system::error_code& error,
    size_t bytesTransferred
)
{
    if (error) {
        ::std::cout << "[ERROR:" << __FUNCTION__ << "]: " << error.message() << ::std::endl;
        this->startReceiveImpl<function>();
        return;
    }

    // display the received message
    auto& message = *reinterpret_cast<::APacket*>(&m_buffer);
    message.display("<-");

    // packet loss check
    if (message.getId() != m_nextPacketId) {
        this->reply<::packet::Loss>(message.getId());
        ++m_nextPacketId;
        return;
    } else if (m_nextPacketId == 255) {
        m_nextPacketId = 1;
    } else {
        ++m_nextPacketId;
    }

    // basic behaviors
    switch (message.getType()) {

    case ::packet::Header::Type::packetLoss: {
        // search for the lost packet in the most recent sent packet
        auto& packetLost{ *reinterpret_cast<::packet::Loss*>(&m_buffer) };
        while (!m_sentPackets.empty()) {
            if (packetLost.getLostPacketId() == m_sentPackets.front()->getId()) {
                m_sentPackets.front()->assignNewId();
                this->reply(::std::move(m_sentPackets.front()));
                m_sentPackets.pop();
                break;
            }
            m_sentPackets.pop();
        }
        break;

    } case ::packet::Header::Type::ping:

        // compute the latency and confirm that the client is still connected to the server
        // m_isPingValidated = true;
        // m_latency = ::std::chrono::duration_cast<::std::chrono::milliseconds>(
            // std::chrono::high_resolution_clock::now() - m_pingTimepoint
        // ).count();

        this->clearSentPackets();
        this->reply<::packet::PingAck>();
        break;

    case ::packet::Header::Type::pingAck:

        // compute the latency and confirm that the client is still connected to the server
        // m_isPingValidated = true;
        // m_latency = ::std::chrono::duration_cast<::std::chrono::milliseconds>(
            // std::chrono::high_resolution_clock::now() - m_pingTimepoint
        // ).count();

        this->clearSentPackets();
        break;

    case ::packet::Header::Type::error:
        ::std::cout << reinterpret_cast<::packet::Error*>(&m_buffer)->toString() << ::std::endl;
        break;
    default:
        // user defined behaviours
        function(message);
    }

    // special interaction
    this->startReceiveImpl<function>();
}
