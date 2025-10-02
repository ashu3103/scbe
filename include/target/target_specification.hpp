#pragma once

#include <string>
namespace scbe::Target {

enum class Arch {
    Unknwon,
    x86_64,
    AArch64
};

enum class Vendor {
    Unknown,
    PC,
};

enum class OS {
    Unknwon,
    Linux,
    Windows,
    Apple
};

enum class ABI {
    Unknwon,
    GNU,
    MSVC,
    Darwin
};

class TargetSpecification {
public:
    TargetSpecification(const std::string& targetString);

    Arch getArch() const { return m_arch; }
    Vendor getVendor() const { return m_vendor; }
    OS getOS() const { return m_os; }
    ABI getABI() const { return m_abi; }

private:
    static Arch parseArch(const std::string& archString);
    static Vendor parseVendor(const std::string& vendorString);
    static OS parseOs(const std::string& osString);
    static ABI parseABI(const std::string& abiString);
    
private:
    std::string m_targetString;
    Arch m_arch = Arch::Unknwon;
    Vendor m_vendor = Vendor::Unknown;
    OS m_os = OS::Unknwon;
    ABI m_abi = ABI::Unknwon;
};

}