#include <pch.hpp>
#include <Packet/Loss.hpp>



// ------------------------------------------------------------------ *structors

::packet::Loss::Loss(
    const ::std::uint8_t lostPacketId
)
    : ::APacket{ ::APacket::Header::Type::packetLoss, sizeof(::std::uint8_t), false, 0 }
    , m_lostPacketId{ lostPacketId }
{}

::packet::Loss::~Loss() = default;



// ------------------------------------------------------------------ comparison

auto ::packet::Loss::operator<(const ::std::uint8_t id) const
    -> bool
{
    return m_lostPacketId < id;
}

auto ::packet::Loss::operator<=(const ::std::uint8_t id) const
    -> bool
{
    return m_lostPacketId <= id;
}

auto ::packet::Loss::operator==(const ::std::uint8_t id) const
    -> bool
{
    return m_lostPacketId == id;
}

auto ::packet::Loss::operator!=(const ::std::uint8_t id) const
    -> bool
{
    return m_lostPacketId != id;
}

auto ::packet::Loss::operator>=(const ::std::uint8_t id) const
    -> bool
{
    return m_lostPacketId >= id;
}

auto ::packet::Loss::operator>(const ::std::uint8_t id) const
    -> bool
{
    return m_lostPacketId > id;
}



// ------------------------------------------------------------------ get

auto ::packet::Loss::getLostPacketId() const
    -> ::std::uint8_t
{
    return m_lostPacketId;
}
