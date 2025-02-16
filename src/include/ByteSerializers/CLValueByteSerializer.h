#pragma once
#include "ByteSerializers/BaseByteSerializer.h"
#include "Types/CLValue.h"
#include <boost/variant/get.hpp>
namespace Casper {
struct CLValueByteSerializer : public BaseByteSerializer {
  ///
  CBytes ToBytes(CLValue source) {
    // ByteQueue byte_queue;
    CBytes bytes;
    // serialize data length (4 bytes)
    //
    WriteInteger(bytes, source.bytes.size());
    // std::cout << "CLValueByteSerializer::ToBytes: source.bytes.size() = "
    //          << source.bytes.size() << std::endl;
    // serialize data
    //

    // std::cout << "CLValueByteSerializer::ToBytes: source.bytes = "
    //           << hexEncode(source.bytes) << std::endl;
    WriteBytes(bytes, source.bytes);
    // std::cout << "CLValueByteSerializer::ToBytes: source.bytes.size() = "
    //          << source.bytes.size() << std::endl;
    // serialize type and inner types (if any) recursively
    //
    // std::cout << "hexencode to bytes:" << hexEncode(bytes) << std::endl;
    // nlohmann::json jjj;
    // to_json(jjj, source.parsed);
    // std::cout << "CLValueByteSerializer::ToBytes: source.parsed = "
    //  << jjj.dump(2) << std::endl;

    CLTypeToBytes(bytes, source.cl_type, source.parsed.parsed);
    // std::cout << "CLValueByteSerializer::ToBytes: after CLTypeToBytes"
    //           << hexEncode(bytes) << std::endl;
    return bytes;
  }

  void CLTypeToBytes(CBytes& sb, CLType innerType, CLTypeParsedRVA parsed) {
    // std::cout << "CLTypeToBytes1: " << std::endl;
    int type_idx = innerType.type.which();
    SPDLOG_DEBUG("CLTypeToBytes idx: {}", type_idx);

    nlohmann::json j;
    to_json(j, innerType);
    // std::cout << "CLTypeToBytes json: " << j.dump(2) << std::endl;

    if (type_idx == 0) {
      CLTypeEnum type = boost::get<CLTypeEnum>(innerType.type);
      WriteByte(sb, static_cast<uint8_t>(type));
    } else if (type_idx == 1) {
      throw std::runtime_error("CLTypeToBytes: type_idx = 1 not implemented");
    } else if (type_idx == 2) {
      WriteByte(sb, 17);
      std::map<CLTypeRVA, CLTypeRVA> mp =
          boost::get<std::map<CLTypeRVA, CLTypeRVA>>(innerType.type);
      if (parsed.which() == 16) {
        CLTypeToBytes(sb, mp.begin()->first, parsed);
        CLTypeToBytes(sb, mp.begin()->second, parsed);
      } else {
        std::map<CLTypeParsedRVA, CLTypeParsedRVA> mp2 =
            boost::get<std::map<CLTypeParsedRVA, CLTypeParsedRVA>>(parsed);

        CLTypeToBytes(sb, mp.begin()->first, mp2.begin()->first);
        CLTypeToBytes(sb, mp.begin()->second, mp2.begin()->second);
      }
    } else if (type_idx == 3) {
      auto inner_type_rva =
          boost::get<std::map<std::string, CLTypeRVA>>(innerType.type);
      // std::cout << "3-1" << std::endl;
      std::string inner_type_name = inner_type_rva.begin()->first;
      // std::cout << "3-2" << std::endl;
      CLTypeRVA inner_type_rva_value = inner_type_rva.begin()->second;
      // std::cout << "3-3" << std::endl;
      if (inner_type_name == "Option") {
        WriteByte(sb, 13);
        // if (parsed.which() != 14) {
        // std::cout << "3-option" << std::endl;
        CLTypeToBytes(sb, inner_type_rva_value, parsed);
        // }
      } else if (inner_type_name == "List") {
        WriteByte(sb, 14);
        // std::cout << "3-4" << std::endl;

        // Option None, parsed is nullptr case

        // std::cout << "3-5" << std::endl;

        // TODO: Check if this is correct, maybe whole list can be serialized
        CLTypeToBytes(sb, inner_type_rva_value, parsed);
        // std::cout << "3-6" << std::endl;

      } else if (inner_type_name == "Result") {
        WriteByte(sb, 16);
        std::map<std::string, CLTypeRVA> inner_type_rva2 =
            boost::get<std::map<std::string, CLTypeRVA>>(inner_type_rva_value);

        CLTypeToBytes(sb, inner_type_rva2.at("Ok"), parsed);
        CLTypeToBytes(sb, inner_type_rva2.at("Err"), parsed);

      } else {
        throw std::runtime_error("CLTypeToBytes: inner_type_name = " +
                                 inner_type_name + " not implemented");
      }

    } else if (type_idx == 4) {
      auto inner_type_rva =
          boost::get<std::map<std::string, std::vector<CLTypeRVA>>>(
              innerType.type);
      std::string inner_type_name = inner_type_rva.begin()->first;
      if (inner_type_name == "Tuple1") {
        WriteByte(sb, 18);
        CLTypeToBytes(sb, inner_type_rva.begin()->second.at(0), parsed);
      } else if (inner_type_name == "Tuple2") {
        WriteByte(sb, 19);
        CLTypeToBytes(sb, inner_type_rva.begin()->second.at(0), parsed);
        CLTypeToBytes(sb, inner_type_rva.begin()->second.at(1), parsed);

      } else if (inner_type_name == "Tuple3") {
        WriteByte(sb, 20);
        CLTypeToBytes(sb, inner_type_rva.begin()->second.at(0), parsed);
        CLTypeToBytes(sb, inner_type_rva.begin()->second.at(1), parsed);
        CLTypeToBytes(sb, inner_type_rva.begin()->second.at(2), parsed);
      } else {
        throw std::runtime_error("Unknown inner type with idx:4 not tuple");
      }
    } else if (type_idx == 5) {
      // std::cout << "CLTypeToBytes: type_idx = 5 " << std::endl;
      auto inner_type_rva =
          boost::get<std::map<std::string, int32_t>>(innerType.type);
      // std::cout << "after get inner_type_rva" << std::endl;
      std::string inner_type_name = inner_type_rva.begin()->first;
      // std::cout << "after get inner_type_name" << std::endl;
      if (inner_type_name == "ByteArray") {
        WriteByte(sb, 15);
        WriteInteger(sb, inner_type_rva.begin()->second);
      } else {
        throw std::runtime_error("Unknown type with idx:5 not bytearray");
      }
    }
  }
};

}  // namespace Casper
