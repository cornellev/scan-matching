/*
 * @author Ethan Uppal
 * @copyright Copyright (C) 2024 Ethan Uppal. All rights reserved.
 */

#include <cassert>
#include <cstdlib>
#include <random>
#include "util/logger.h"
#include "util/keyboard.h"
#include "lidar_view.h"
#include "geo/midpoint.h"
#include "sim_config.h"

#define CIRCLE_RADIUS 3

LidarView::LidarView(std::vector<icp::Vector> source,
    std::vector<icp::Vector> destination, const std::string method)
    : source(source), destination(destination), keyboard(false), iterations{} {
    icp = icp::ICP::from_method(method);
    icp->begin(source, destination, icp::RBTransform());
}

LidarView::~LidarView() noexcept {
    icp.release();
}

void LidarView::on_event(const SDL_Event& event) {
    bool space_before = keyboard.query(SDLK_SPACE);
    bool d_before = keyboard.query(SDLK_d);
    keyboard.update(event);
    bool space_after = keyboard.query(SDLK_SPACE);
    bool d_after = keyboard.query(SDLK_d);

    if (!space_before && space_after) {
        is_iterating = !is_iterating;
    }
    if (!d_before && d_after) {
        std::cerr << "DEBUG PRINT:\n";
        std::cerr << icp->current_transform().to_string() << '\n';
        std::cerr << icp->calculate_cost() << '\n';
        std::cerr << iterations << '\n';
    }
}

void LidarView::draw(SDL_Renderer* renderer, const SDL_Rect* frame __unused,
    double dtime __unused) {
    if (sim_config::use_light_background) {
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    }
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
    for (const icp::Vector& point: destination) {
        SDL_DrawCircle(renderer, point[0], point[1], CIRCLE_RADIUS);
    }

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);
    for (const icp::Vector& point: source) {
        icp::Vector result = icp->current_transform().apply_to(point);
        SDL_DrawCircle(renderer, result[0], result[1], CIRCLE_RADIUS);
    }

    if (is_iterating) {
        icp->iterate();
        iterations++;
    }
}
