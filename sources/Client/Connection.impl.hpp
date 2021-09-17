#pragma once

template <
    typename MessageType
> void client::Connection::send(
    auto&&... args
)
{
    auto message{ ::std::make_unique<MessageType>(::std::forward<decltype(args)>(args)...) };

    // debug
    ::std::cout << '\r';
    message->display("->");
    ::std::cout << "Input: " << ::std::flush;

    m_socket.send_to(::boost::asio::buffer(message.get(), message->getSize()), m_endpoint);
    m_sentPackets.push(::std::move(message));
}
