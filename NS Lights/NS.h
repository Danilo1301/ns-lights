#pragma once

#include "plugin.h"

class NS {
public:
    static bool m_ShowDebug;

    static void Init();
    static void Update();
    static void UpdateVehicleControllers(CVehicle* vehicle);
    static void Draw();

    NS();
};
