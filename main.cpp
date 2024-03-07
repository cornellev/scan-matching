// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#include <iostream>
#include <random>
#include <algorithm>
#include "gui/window.h"
#include "icp/lidar_view.h"

#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 700

int main() {
    Window window("Scan Matching", WINDOW_WIDTH, WINDOW_HEIGHT);

    // Number of points
    const int n = 40;

    // Spacing of points
    const double scale = 10;

    // Wall parameters
    const double slope = -1.0;
    const double intercept = 0.0;

    // Make wall slightly shaky
    const double perturbation_range = 10.0;
    const double x_displace = 100;
    const double y_displace = 50;

    const double x_unshift = n * scale / 2;
    const double y_unshift = slope * x_unshift;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-perturbation_range,
        perturbation_range);

    // Create a line of points for the wall
    std::vector<icp::Point> wall(n);
    for (int i = 0; i < n; ++i) {
        double x = i * scale;
        double y = slope * x + intercept + dis(gen);
        wall[i] = {x, y};
    }

    // Sample the first 3/4th and the last 3/4th as our LiDAR scans
    std::vector<icp::Point> source(wall.begin(), wall.begin() + n * 3 / 4);
    std::vector<icp::Point> destination(wall.begin() + n / 4, wall.end());

    // Separate the scans to simulate robot movement
    for (icp::Point& point: source) {
        point.x += WINDOW_WIDTH / 2 - x_displace / 2 - x_unshift;
        point.y += WINDOW_WIDTH / 2 - y_displace / 2 - y_unshift;
    }
    for (icp::Point& point: destination) {
        point.x += WINDOW_WIDTH / 2 + x_displace / 2 - x_unshift;
        point.y += WINDOW_WIDTH / 2 + y_displace / 2 - y_unshift;
    }
    for (icp::Point& point:
        wall) {  // todo: fix this and LidarView to be better code. essentially
                 // you are also shifting the wall to act as gray dots
                 // underneath the destination so you see how close you are to
                 // where you should be. if it works it works but should clean
        point.x += WINDOW_WIDTH / 2 + x_displace / 2 - x_unshift;
        point.y += WINDOW_WIDTH / 2 + y_displace / 2 - y_unshift;
    }

    LidarView* view = new LidarView(wall, source, destination);
    window.attach_view(view);

    std::cout << "SCAN MATCHING : ITERATIVE CLOSEST POINT\n";
    std::cout << "=======================================\n";
    std::cout << "- The points are generated randomly\n";
    std::cout << "- Press <SPACE> to advance one iteration\n";
    std::cout << "- Press the red <X> on the window to exit\n";

    window.present();
}
