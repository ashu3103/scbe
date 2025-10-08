#pragma once

#include "pass.hpp"

#include <vector>

namespace scbe {

struct PassGroup {
    std::vector<Ref<Pass>> m_passes;
    bool m_repeat = false;
};

class PassManager {
public:
    void addRun(std::vector<Ref<Pass>> passes, bool repeat) { m_groups.push_back({std::move(passes), repeat}); }

    void run(Unit& unit);

protected:
    std::vector<PassGroup> m_groups;
};

}