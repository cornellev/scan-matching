// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#pragma once

#include <SDL.h>
#include <vector>
#include "gui/view.h"
#include "util/keyboard.h"
#include "icp/icp.h"

class LidarView final : public View {
    std::vector<icp::Point> source;
    std::vector<icp::Point> destination;
    std::vector<icp::Point> wall;
    std::unique_ptr<icp::ICP> icp;
    Keyboard keyboard;

    void construct_instance();

public:
    /** Consructs a new lidar view visualizing ICP. */
    LidarView();
    ~LidarView() noexcept override;

    void on_event(const SDL_Event& event) override;
    void draw(SDL_Renderer* renderer, const SDL_Rect* frame,
        double dtime) override;
};
