#pragma once

template <
    typename MessageType
> void server::Connection::send(
    const ::boost::asio::ip::udp::endpoint& endpoint,
    auto&&... args
)
{
    MessageType message{ ::std::forward<decltype(args)>(args)... };

    // display the message sent
    message.display("->");

    // actual reply
    m_socket.async_send_to(
        ::boost::asio::buffer(&message, message.getSize()),
        endpoint,
        ::boost::bind(
            &Connection::startReceiveImpl,
            this
        )
    );
}

template <
    typename MessageType
> void server::Connection::reply(
    auto&&... args
)
{
    this->send<MessageType>(m_lastSenderEndpoint, ::std::forward<decltype(args)>(args)...);
}
