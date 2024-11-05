//
// Created by blau08 on 05.11.2024.
//

#ifndef COMPONENTS_H
#define COMPONENTS_H
#include <optional>

#include "Components/Entities/RenderComponent.h"
#include "Components/Lighting/LightComponent.h"

/**
 * @brief Beinhaltet optionale Render- und Light-Komponenten.
 */
struct Components {
    std::optional<RenderComponent> renderComponent;
    std::optional<LightComponent> lightComponent;
};


#endif //COMPONENTS_H
