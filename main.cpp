// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#include <iostream>
#include <memory>
#include "gui/window.h"
#include "sim/lidar_view.h"

#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 700

int main() {
    Window window("Scan Matching", WINDOW_WIDTH, WINDOW_HEIGHT);

    LidarView* view = new LidarView();
    window.attach_view(view);

    window.present();
}
