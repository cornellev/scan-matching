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
    std::cout << "* The points are generated randomly\n";
    std::cout << "* Press <SPACE> to advance one iteration\n";
    std::cout << "* Press the red <X> on the window to exit\n";

    window.present();
}

void run_benchmark(const char* method) {
    if (std::find(icp::ICP::registered_methods().begin(),
            icp::ICP::registered_methods().end(), method)
        == icp::ICP::registered_methods().end()) {
        std::cerr << "error: unknown ICP method '" << method
                  << "'. expected one of:\n";
        for (const std::string& registered_method:
            icp::ICP::registered_methods()) {
            std::cerr << "* " << registered_method << '\n';
        }
        std::exit(1);
    }

    // very scuffed
    LidarView* view = new LidarView();
    std::unique_ptr<icp::ICP> icp = icp::ICP::from_method(method, 1000, 0.01);
    icp->set_initial(icp::Transform());
    std::cout << "initial cost: " << icp->cost() << '\n';
    icp->converge(view->source, view->destination, 10);
    std::cout << "final cost: " << icp->cost() << '\n';
    delete view;
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
    const char* bench_method = "point_to_point";
    assert(use_gui = ca_opt('g', "gui", "<g", NULL,
               "launch the interactive GUI"));
    assert(ca_opt('h', "help", "<h", NULL, "prints this info"));
    assert(ca_opt('v', "version", "<v", NULL, "prints version info"));
    assert(do_bench = ca_opt('b', "bench", ".METHOD !@g", &bench_method,
               "benchmarks a given icp method"));  // for now disabled with -g

    if (argc == 1) {
        ca_print_help();
        return 1;
    } else if (ca_parse(NULL) != 0) {
        return 1;
    }

    if (*use_gui) {
        launch_gui();
    } else if (*do_bench) {
        run_benchmark(bench_method);
    }
}
