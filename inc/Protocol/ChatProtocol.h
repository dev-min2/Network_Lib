#pragma once
#include <string>
#include <cstdint>
using int64 = std::int64_t;
using int32 = std::int32_t;
using int16 = std::int16_t;
using int8 = std::int8_t;
using uInt64 = std::uint64_t;
using uInt32 = std::uint32_t;
using uInt16 = std::uint16_t;
using uInt8 = std::uint8_t;

enum class ChatProtocol
{
	CS_REQU_CHAT = 1000,
	SC_REQU_CHAT
};
#pragma pack(push,1)
/// <summary>
/// Ŭ�� - ����
/// Ŭ�� : �Է��� ä�ó���� ä���� ģ ������ ���̵�� �Բ� ��Ŷ�� �����Ѵ�.
/// ���� : Ŭ�󿡼� ���� �����͸� �״�� BroadCastó���Ѵ�.
/// </summary>
struct CS_REQU_CHAT_PACKET
{
	std::wstring name;
	std::wstring chatMsg;
};
struct SC_REQU_CHAT_PACKET
{
	std::wstring name;
	std::wstring chatMsg;
};
#pragma pack(pop)