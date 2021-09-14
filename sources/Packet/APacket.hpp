// Every message is devided into multiple 250 chars messages at max.

#pragma once



#include <Packet/Header.hpp>



namespace packet {



class APacket {

public:

    // ------------------------------------------------------------------ *structors

    APacket(
        packet::Header::Type type,
        ::std::size_t bodySize,
        bool isImportant,
        const ::std::size_t position = 0
    );

    virtual ~APacket() = 0;



    // ------------------------------------------------------------------ informations

    [[ nodiscard ]] static inline constexpr auto getMaxSize()
        -> ::std::size_t
    {
        return 64;
    }

    [[ nodiscard ]] static inline constexpr auto getMaxHeaderSize()
        -> ::std::size_t
    {
        return 12;
    }

    [[ nodiscard ]] static inline constexpr auto getMaxBodySize()
        -> ::std::size_t
    {
        return APacket::getMaxSize() - APacket::getMaxHeaderSize();
    }

    [[ nodiscard ]] static inline constexpr auto getHeaderSize()
        -> ::std::size_t
    {
        static_assert(sizeof(packet::Header) <= APacket::getMaxHeaderSize(), "packet::Header too big");
        return sizeof(packet::Header);
    }

    [[ nodiscard ]] auto getSize() const
        -> ::std::size_t;

    [[ nodiscard ]] auto getBodySize() const
        -> ::std::size_t;

    [[ nodiscard ]] auto getHeader() const
        -> const packet::Header&;

    [[ nodiscard ]] auto getType() const
        -> packet::Header::Type;

    [[ nodiscard ]] auto getPosition() const
        -> ::std::uint8_t;

    [[ nodiscard ]] auto getId() const
        -> ::std::uint8_t;

    [[ nodiscard ]] auto isImportant() const
        -> bool;

    [[ nodiscard ]] auto getMemoryAddress()
        -> void*;



    // ------------------------------------------------------------------ newId

    void assignNewId();



    // ------------------------------------------------------------------ debug

    void display(
        const char direction[2]
    ) const;



protected:

    const packet::Header m_header;



private:

    // ------------------------------------------------------------------ idGenerator

    static auto generatePacketId()
        -> ::std::uint8_t;

};



} // namespace packet

using APacket = ::packet::APacket;
