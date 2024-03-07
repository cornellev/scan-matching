// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#pragma once

#include <SDL.h>
#include <vector>
#include "gui/view.h"
#include "util/keyboard.h"
#include "icp.h"

class LidarView final : public View {
    const std::vector<icp::Point> wall;
    icp::ICP icp;
    Keyboard keyboard;

public:
    /** Consructs a new lidar view visualizing applying ICP to `source`
     * targeting `destination`. */
    LidarView(const std::vector<icp::Point> wall,
        const std::vector<icp::Point> source,
        const std::vector<icp::Point> destination);
    ~LidarView() noexcept override;

    void on_event(const SDL_Event& event) override;
    void draw(SDL_Renderer* renderer, const SDL_Rect* frame,
        double dtime) override;
};
