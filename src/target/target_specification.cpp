#include "target/target_specification.hpp"
#include <cassert>
#include <vector>

namespace scbe::Target {

std::vector<std::string> split(const std::string& s, const std::string& delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos)
    {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}

TargetSpecification::TargetSpecification(const std::string& targetString) : m_targetString(targetString) {
    std::vector<std::string> splitted = split(targetString, "-");
    assert(splitted.size() >= 3 && "Invalid target string");
    if(splitted.size() < 3) return;

    m_arch = parseArch(splitted[0]);
    if(splitted.size() >= 4) {
        m_vendor = parseVendor(splitted[1]);
        m_os = parseOs(splitted[2]);
        m_abi = parseABI(splitted[3]);
        return;
    }
    m_os = parseOs(splitted[1]);
    m_abi = parseABI(splitted[2]);
}

Arch TargetSpecification::parseArch(const std::string& archString) {
    if(archString == "x86_64") return Arch::x86_64;
    else if(archString == "aarch64") return Arch::AArch64;
    return Arch::Unknwon;
}

Vendor TargetSpecification::parseVendor(const std::string& vendorString) {
    if(vendorString == "PC" || vendorString == "pc") return Vendor::PC;
    return Vendor::Unknown;
}

OS TargetSpecification::parseOs(const std::string& osString) {
    if(osString == "linux") return OS::Linux;
    else if(osString == "windows") return OS::Windows;
    else if(osString == "apple") return OS::Apple;
    return OS::Unknwon;
}

ABI TargetSpecification::parseABI(const std::string& abiString) {
    if(abiString == "gnu") return ABI::GNU;
    else if(abiString == "msvc") return ABI::MSVC;
    else if(abiString == "darwin") return ABI::Darwin;
    return ABI::Unknwon;
}

}