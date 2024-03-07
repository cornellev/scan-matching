// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#include <cassert>
#include <cstdlib>
#include <random>
#include "util/logger.h"
#include "util/keyboard.h"
#include "lidar_view.h"
#include "geo/midpoint.h"
#include "sim_config.h"

LidarView::LidarView(): wall(sim_config::n), source(sim_config::n) {
    icp = icp::ICP::point_to_point(sim_config::n * 3 / 4, 0.1);
    construct_instance();
}

LidarView::~LidarView() {}

void LidarView::construct_instance() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-sim_config::perturbation_range,
        sim_config::perturbation_range);

    // Create a line of points for the wall
    for (int i = 0; i < sim_config::n; ++i) {
        double x = i * sim_config::scale;
        double y = sim_config::slope * x + sim_config::intercept + dis(gen);
        wall[i] = {x, y};
    }

    // Sample the first 3/4th and the last 3/4th as our LiDAR scans
    source.assign(wall.begin(), wall.begin() + sim_config::n * 3 / 4);
    destination.assign(wall.begin() + sim_config::n / 4, wall.end());

    // Separate the scans to simulate robot movement
    for (icp::Point& point: source) {
        point.x += WINDOW_WIDTH / 2 - sim_config::x_displace / 2
                   - sim_config::x_unshift;
        point.y += WINDOW_WIDTH / 2 - sim_config::y_displace / 2
                   - sim_config::y_unshift;
    }
    for (icp::Point& point: destination) {
        point.x += WINDOW_WIDTH / 2 + sim_config::x_displace / 2
                   - sim_config::x_unshift;
        point.y += WINDOW_WIDTH / 2 + sim_config::y_displace / 2
                   - sim_config::y_unshift;
    }
    for (icp::Point& point:
        wall) {  // todo: fix this and LidarView to be better code. essentially
                 // you are also shifting the wall to act as gray dots
                 // underneath the destination so you see how close you are to
                 // where you should be. if it works it works but should clean
        point.x += WINDOW_WIDTH / 2 + sim_config::x_displace / 2
                   - sim_config::x_unshift;
        point.y += WINDOW_WIDTH / 2 + sim_config::y_displace / 2
                   - sim_config::y_unshift;
    }
}

void LidarView::on_event(const SDL_Event& event) {
    keyboard.update(event);

    if (keyboard.query(SDLK_SPACE)) {
        icp->iterate(source, destination);
    }
}

void LidarView::draw(SDL_Renderer* renderer, const SDL_Rect* frame,
    double dtime) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
    for (const icp::Point& point: source) {
        SDL_DrawCircle(renderer, point.x, point.y, 5);
    }

    SDL_SetRenderDrawColor(renderer, 128, 128, 128, SDL_ALPHA_OPAQUE);
    for (const icp::Point& point: wall) {
        SDL_DrawCircle(renderer, point.x, point.y, 5);
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
    for (const icp::Point& point: destination) {
        SDL_DrawCircle(renderer, point.x, point.y, 5);
    }

    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 100);
    for (const icp::Point& point: source) {
        SDL_DrawCircle(renderer, point.x + icp->transform().dx,
            point.y + icp->transform().dy, 5);
    }
}
