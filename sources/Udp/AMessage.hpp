// Every message is devided into multiple 250 chars messages at max.

#pragma once



namespace udp {



class AMessage {

public:

    struct Header {

        enum class Type : uint8_t {
            error = 0,
            latency,
            confirmation,
            exit,
            chatMessage
        };
        Header::Type type;

        static inline constexpr ::std::size_t maxSize{ 254 };
        const uint8_t size;
        const uint8_t position; // position of the message in the multi messages range
        const bool isImportant;

    };



public:

    // ------------------------------------------------------------------ *structors

    AMessage(
        AMessage::Header::Type type,
        ::std::size_t size,
        bool isImportant = false
    );

    AMessage(
        AMessage::Header::Type type,
        ::std::size_t size,
        uint8_t position,
        bool isImportant = false
    );

    virtual ~AMessage() = 0;



    // ------------------------------------------------------------------ informations

    [[ nodiscard ]] static constexpr auto getMaxSize()
        -> ::std::size_t;

    [[ nodiscard ]] static constexpr auto getMaxChars()
        -> ::std::size_t;

    [[ nodiscard ]] static constexpr auto getHeaderSize()
        -> ::std::size_t;

    [[ nodiscard ]] auto getHeader() const
        -> const AMessage::Header&;

    [[ nodiscard ]] auto getSize() const
        -> ::std::size_t;

    [[ nodiscard ]] auto getPosition() const
        -> uint8_t;

    [[ nodiscard ]] auto isImportant() const
        -> bool;

    [[ nodiscard ]] auto getMemoryAddress()
        -> void*;



protected:

    const AMessage::Header m_header;

};




} // namespace udp

#include <Udp/AMessage.impl.hpp>
