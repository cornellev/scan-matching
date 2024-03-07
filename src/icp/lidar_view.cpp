// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#include <cassert>
#include <cstdlib>
#include "util/logger.h"
#include "util/keyboard.h"
#include "lidar_view.h"
#include "geo/midpoint.h"

LidarView::LidarView(const std::vector<icp::Point> wall,
    const std::vector<icp::Point> source,
    const std::vector<icp::Point> destination)
    : wall(wall), icp(source, destination, 0.1) {}

LidarView::~LidarView() {}

void LidarView::on_event(const SDL_Event& event) {
    keyboard.update(event);

    if (keyboard.query(SDLK_SPACE)) {
        icp.iterate();
    }
}

void LidarView::draw(SDL_Renderer* renderer, const SDL_Rect* frame,
    double dtime) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
    for (const icp::Point& point: icp.source()) {
        SDL_DrawCircle(renderer, point.x, point.y, 5);
    }

    SDL_SetRenderDrawColor(renderer, 128, 128, 128, SDL_ALPHA_OPAQUE);
    for (const icp::Point& point: wall) {
        SDL_DrawCircle(renderer, point.x, point.y, 5);
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
    for (const icp::Point& point: icp.destination()) {
        SDL_DrawCircle(renderer, point.x, point.y, 5);
    }

    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 100);
    for (const icp::Point& point: icp.source()) {
        SDL_DrawCircle(renderer, point.x + icp.transform().dx,
            point.y + icp.transform().dy, 5);
    }
}
