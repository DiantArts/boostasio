#pragma once

// ------------------------------------------------------------------ informations

constexpr auto ::udp::AMessage::getMaxSize()
    -> ::std::size_t
{
    return AMessage::Header::maxSize;
}

constexpr auto ::udp::AMessage::getMaxChars()
    -> ::std::size_t
{
    return AMessage::Header::maxSize - 16;
}

constexpr auto ::udp::AMessage::getHeaderSize()
    -> ::std::size_t
{
    return 16;
}
