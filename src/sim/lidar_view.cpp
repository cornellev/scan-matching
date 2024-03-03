// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#include <cassert>
#include <cstdlib>
#include "util/logger.h"
#include "util/keyboard.h"
#include "lidar_view.h"

LidarView::LidarView() {}

LidarView::~LidarView() {}

void LidarView::on_event(const SDL_Event& event) {
    keyboard.update(event);
}

void LidarView::draw(SDL_Renderer* renderer, const SDL_Rect* frame,
    double dtime) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
}
