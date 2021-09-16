#pragma once

#include <Packet/Types.hpp>



// ------------------------------------------------------------------ send

template <
    typename MessageType
> void ::Buffer::send(
    ::boost::asio::ip::udp::endpoint endpoint,
    auto&&... args
)
{
    ::std::unique_ptr<::APacket> message{ ::std::make_unique<MessageType>(
        ::std::forward<decltype(args)>(args)...
    ) };

    // debug
    message->display("->");

    m_socket.send_to(
        ::boost::asio::buffer(message.get(), message->getSize()), m_endpoint = ::std::move(endpoint)
    );
    m_sentPackets.push(::std::move(message));
}

// template <
    // typename MessageType
// > void ::Buffer::send(
    // MessageType&& message,
    // ::boost::asio::ip::udp::endpoint endpoint
// )
// {
    // debug
    // message.display("->");

    // m_socket.send_to(
        // ::boost::asio::buffer(message.get(), message->getSize()), m_endpoint = ::std::move(endpoint)
    // )
    // m_sentPackets.push(::std::make_unique<::APacket>(::std::move(message)));
// }



template <
    typename MessageType
> void ::Buffer::reply(
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
// > void ::Buffer::reply(
    // MessageType&& message,
// )
// {}



// ------------------------------------------------------------------ receive

template <
    auto function
> void ::Buffer::startReceive()
{
    m_socket.async_receive_from(
        ::boost::asio::buffer(m_buffer, this->bufferLength),
        m_endpoint,
        ::boost::bind(
            &::Buffer::prehandleReceive<function>,
            this,
            ::boost::asio::placeholders::error,
            ::boost::asio::placeholders::bytes_transferred
        )
    );
}

template <
    auto function
> void ::Buffer::prehandleReceive(
    const boost::system::error_code& error,
    size_t bytesTransferred
)
{
    if (error) {
        ::std::cout << "[ERROR:" << __FUNCTION__ << "]: " << error.message() << ::std::endl;
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
        if (this->isPacketSent(packetLost.getId())) {
            m_sentPackets.front()->assignNewId();
            this->reply(::std::move(m_sentPackets.front()));
            m_sentPackets.pop();
            break;
        }
        break;

    } case ::packet::Header::Type::ping:
        // compute the latency and confirm that the client is still connected to the server
        this->clearSentPackets();
        this->reply<::packet::PingAck>();
        break;

    case ::packet::Header::Type::pingAck:
        // compute the latency and confirm that the client is still connected to the server
        this->clearSentPackets();
        break;

    case ::packet::Header::Type::error:
        ::std::cout << reinterpret_cast<::packet::Error*>(&m_buffer)->toString() << ::std::endl;
        break;

    default:
        // user defined behaviours
        function(message);
    }
}