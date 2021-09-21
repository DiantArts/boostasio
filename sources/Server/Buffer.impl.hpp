#pragma once



// ------------------------------------------------------------------ send

template <
    typename MessageType
> void ::server::Buffer::sendTo(
    const ::boost::asio::ip::udp::endpoint& endpoint,
    auto&&... args
)
{
    MessageType message{ ::std::forward<decltype(args)>(args)... };
    message.display("->"); // debug
    m_socket.send_to(::boost::asio::buffer(&message, message.getSize()), endpoint);
    // m_socket.async_send_to(
        // ::boost::asio::buffer(&message, message.getSize()),
        // endpoint,
        // ::boost::bind(
            // &Buffer::startReceive,
            // this
        // )
    // );
}

template <
    typename MessageType
> void ::server::Buffer::sendTo(
    const ::boost::asio::ip::udp::endpoint& endpoint,
    const MessageType& message
)
{
    message.display("->"); // debug
    m_socket.send_to(::boost::asio::buffer(&message, message.getSize()), endpoint);
    // m_socket.async_send_to(
        // ::boost::asio::buffer(&message, message.getSize()),
        // endpoint,
        // ::boost::bind(
            // &Buffer::startReceive,
            // this
        // )
    // );
}

template <
    typename MessageType
> void ::server::Buffer::sendTo(
    const ::boost::asio::ip::udp::endpoint& endpoint,
    MessageType&& message
)
{
    message.display("->"); // debug
    m_socket.send_to(::boost::asio::buffer(&message, message.getSize()), endpoint);
    // m_socket.async_send_to(
        // ::boost::asio::buffer(&message, message.getSize()),
        // endpoint,
        // ::boost::bind(
            // &Buffer::startReceive,
            // this
        // )
    // );
}



template <
    typename MessageType
> void ::server::Buffer::reply(
    auto&&... args
)
{
    this->sendTo<MessageType>(m_lastSenderEndpoint, ::std::forward<decltype(args)>(args)...);
}

template <
    typename MessageType
> void ::server::Buffer::reply(
    const MessageType& message
)
{
    this->sendTo<MessageType>(message, m_lastSenderEndpoint);
}

template <
    typename MessageType
> void ::server::Buffer::reply(
    MessageType&& message
)
{
    this->sendTo<MessageType>(m_lastSenderEndpoint, ::std::forward<MessageType>(message));
}




// ------------------------------------------------------------------ receive

template <
    void(::server::Server::*function)(::APacket& message)
> void ::server::Buffer::startReceive(
    ::server::Server& server
)
{
    m_socket.async_receive_from(
        ::boost::asio::buffer(m_buffer, this->bufferLength),
        m_lastSenderEndpoint,
        ::boost::bind(
            // &Buffer::prehandleReceive<Class, function>,
            &Buffer::prehandleReceive<function>,
            this,
            ::std::ref(server),
            ::boost::asio::placeholders::error,
            ::boost::asio::placeholders::bytes_transferred
        )
    );
    DEBUG_FUNC;
}

template <
    void(::server::Server::*function)(::APacket& message)
> void ::server::Buffer::prehandleReceive(
    ::server::Server& server,
    const boost::system::error_code& error,
    ::std::size_t bytesTransferred
)
{
    auto& message{ *reinterpret_cast<::APacket*>(&m_buffer) };
    if (!this->handleReceiveRoutine(error, bytesTransferred)) {
    } else if (message.getType() == ::packet::Header::Type::ping) {
        this->reply(message);
        DEBUG_MSG("ping");
    } else {
        (server.*function)(message); // user defined behaviours
    }
    this->startReceive<function>(server); // start receiving again
}
