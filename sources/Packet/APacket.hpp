// Every message is devided into multiple 250 chars messages at max.

#pragma once



namespace packet {



class APacket {

public:

    struct Header {

        enum class Type : ::std::uint8_t {
            error = 0,
            ping,
            confirmation,
            exit,
            text,
            syn,
            packetLoss
        };
        Header::Type type;

        static inline ::std::uint8_t packetIdGenerator{ 0 };
        mutable ::std::uint8_t packetId; // 0 = not important, else, ID equal to last ID + 1

        const ::std::uint8_t packetPosition; // position of the message in the multi messages range

        const ::std::uint8_t bodySize;

    };



public:

    // ------------------------------------------------------------------ *structors

    APacket(
        APacket::Header::Type type,
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
        static_assert(sizeof(APacket::Header) <= APacket::getMaxHeaderSize(), "APacket::Header too big");
        return sizeof(APacket::Header);
    }

    [[ nodiscard ]] auto getSize() const
        -> ::std::size_t;

    [[ nodiscard ]] auto getBodySize() const
        -> ::std::size_t;

    [[ nodiscard ]] auto getHeader() const
        -> const APacket::Header&;

    [[ nodiscard ]] auto getType() const
        -> APacket::Header::Type;

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

    const APacket::Header m_header;



private:

    // ------------------------------------------------------------------ idGenerator

    static auto generatePacketId()
        -> ::std::uint8_t;

};




} // namespace packet

using APacket = ::packet::APacket;
