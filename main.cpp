// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#include <cassert>
#include <iostream>
extern "C" {
#include <cmdapp/cmdapp.h>
}
#include "gui/window.h"
#include "sim/sim_config.h"
#include "sim/lidar_view.h"

void launch_gui() {
    Window window("Scan Matching", WINDOW_WIDTH, WINDOW_HEIGHT);

    LidarView* view = new LidarView();
    window.attach_view(view);

    std::cout << "SCAN MATCHING : ITERATIVE CLOSEST POINT\n";
    std::cout << "=======================================\n";
    std::cout << "- The points are generated randomly\n";
    std::cout << "- Press <SPACE> to advance one iteration\n";
    std::cout << "- Press the red <X> on the window to exit\n";

    window.present();
}

int main(int argc, const char** argv) {
    if (ca_init(argc, argv) != 0) {
        perror("ca_init");
        return 1;
    }

    ca_description("Driver program for Ethan's scan matching implementation.");
    ca_author("Ethan Uppal");
    ca_year(2024);
    ca_version(0, 0, 0);
    ca_versioning_info("All rights reserved.");

    ca_synopsis("[-h|-v]");
    ca_synopsis("[-g|-b METHOD]");

    bool* use_gui;
    bool* do_bench;
    const char* bench_method;
    assert(use_gui = ca_opt('g', "gui", "<g", NULL,
               "launch the interactive GUI"));
    assert(ca_opt('h', "help", "<h", NULL, "prints this info"));
    assert(ca_opt('v', "version", "<v", NULL, "prints version info"));
    assert(do_bench = ca_opt('b', "bench", ".METHOD", &bench_method,
               "benchmarks a given icp method"));

    if (argc == 1) {
        ca_print_help();
        return 1;
    } else if (ca_parse(NULL) != 0) {
        return 1;
    }

    if (*use_gui) {
        launch_gui();
    }
}
