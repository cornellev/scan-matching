// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#include <cassert>
#include <cstdlib>
#include <random>
#include "util/logger.h"
#include "util/keyboard.h"
#include "lidar_view.h"
#include "geo/midpoint.h"
#include "sim_config.h"

LidarView::LidarView()
    : source(sim_config::n), wall(sim_config::n), keyboard(false) {
    icp = icp::ICP::from_method("point_to_point", sim_config::n * 3 / 4, 0.01);
    construct_instance();
    // icp->converge(source, destination, 1);
}

LidarView::~LidarView() noexcept {
    icp.release();
}

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

    // TODO: make less scuffed way to do this
    icp->iterate(source, source);
    double cx = icp->transform().cx;
    double cy = icp->transform().cy;
    icp->set_initial(icp::Transform());
    icp::Transform com_rot(0, 0, sim_config::angle_displace);
    com_rot.cx = cx;
    com_rot.cy = cy;
    for (icp::Point& point: source) {
        double new_x = com_rot.transform_x(point.x, point.y);
        double new_y = com_rot.transform_y(point.x, point.y);
        point.x = new_x;
        point.y = new_y;
    }

    double dest_move_x = WINDOW_WIDTH / 2 + sim_config::x_displace / 2
                         - sim_config::x_unshift;
    double dest_move_y = WINDOW_WIDTH / 2 + sim_config::y_displace / 2
                         - sim_config::y_unshift;

    for (icp::Point& point: destination) {
        point.x += dest_move_x;
        point.y += dest_move_y;
    }

    for (icp::Point& point:
        wall) {  // todo: fix this and LidarView to be better code. essentially
        // you are also shifting the wall to act as gray dots
        // underneath the destination so you see how close you are to
        // where you should be. if it works it works but should clean
        point.x += dest_move_x;
        point.y += dest_move_y;
    }
}

void LidarView::on_event(const SDL_Event& event) {
    bool before = keyboard.query(SDLK_SPACE);
    keyboard.update(event);
    bool after = keyboard.query(SDLK_SPACE);

    if (before && !after) {
        is_iterating = !is_iterating;
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
        SDL_DrawCircle(renderer, icp->transform().transform_x(point.x, point.y),
            icp->transform().transform_y(point.x, point.y), 5);
    }

    if (is_iterating) {
        icp->iterate(source, destination);
    }
}
