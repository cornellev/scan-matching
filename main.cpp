// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#include <cassert>
#include <iostream>
extern "C" {
#include <cmdapp/cmdapp.h>
#include <config/config.h>
}
#include <chrono>
#include "gui/window.h"
#include "sim/sim_config.h"
#include "sim/lidar_view.h"

void set_config_param(const char* var, const char* data, void* user_data) {
    if (strcmp(var, "n") == 0) {
        sim_config::n = std::stoi(data);
    } else if (strcmp(var, "scale") == 0) {
        sim_config::scale = std::stod(data);
    } else if (strcmp(var, "slope") == 0) {
        sim_config::slope = std::stod(data);
    } else if (strcmp(var, "intercept") == 0) {
        sim_config::intercept = std::stod(data);
    } else if (strcmp(var, "perturbation_range") == 0) {
        sim_config::perturbation_range = std::stod(data);
    } else if (strcmp(var, "x_displace") == 0) {
        sim_config::x_displace = std::stod(data);
    } else if (strcmp(var, "y_displace") == 0) {
        sim_config::y_displace = std::stod(data);
    } else if (strcmp(var, "angle_displace") == 0) {
        sim_config::angle_displace = std::stod(data);
    } else if (strcmp(var, "window_width") == 0) {
        sim_config::window_width = std::stoi(data);
    } else if (strcmp(var, "window_height") == 0) {
        sim_config::window_height = std::stoi(data);
    } else if (strcmp(var, "x_delta") == 0) {
        sim_config::x_delta = std::stoi(data);
    }
}

void read_config() {
    FILE* file = fopen("sim.conf", "r");
    if (!file) {
        perror("read_config: fopen");
        std::exit(1);
    }

    if (conf_parse_file(file, set_config_param, NULL) != 0) {
        perror("read_config: conf_parse_file");
        std::exit(1);
    }

    fclose(file);
}

void launch_gui() {
    Window window("Scan Matching", sim_config::window_width,
        sim_config::window_height);

    LidarView* view = new LidarView();
    window.attach_view(view);

    std::cout << "SCAN MATCHING : ITERATIVE CLOSEST POINT\n";
    std::cout << "=======================================\n";
    std::cout << "* The points are generated randomly\n";
    std::cout << "* Press <SPACE> to advance one iteration\n";
    std::cout << "* Press the red <X> on the window to exit\n";

    window.present();
}

void run_benchmark(const char* method, bool with_gui) {
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

    std::cout << "ICP ALGORITHM BENCHMARKING\n";
    std::cout << "=======================================\n";

    // very scuffed
    LidarView* view = new LidarView();
    std::unique_ptr<icp::ICP> icp = icp::ICP::from_method(method, 1000, 0.01);

    std::cout << "running...\r";
    std::cout.flush();
    int invocation_count = 100;
    std::vector<double> final_costs;
    using Clock = std::chrono::high_resolution_clock;
    std::chrono::nanoseconds elapsed_ns = std::chrono::nanoseconds::zero();
    for (int i = 0; i < invocation_count; i++) {
        view->construct_instance();
        icp->set_initial(icp::Transform());

        auto start = Clock::now();
        icp->converge(view->get_source(), view->get_dest(), 10);
        final_costs.push_back(icp->cost());
        auto end = Clock::now();

        elapsed_ns +=
            std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    }
    double elapsed = (double)elapsed_ns.count() / 1000000000.0;
    std::cout << "* Using method '" << method << "'\n";
    std::cout << "* Matching two clouds of n = " << (sim_config::n * 3 / 4)
              << " points\n";
    std::cout << "* " << invocation_count << " invocations, " << elapsed
              << "s in total, " << (elapsed / invocation_count)
              << " per invocation\n";
    std::cout << "* Greatest final cost was "
              << (*std::max_element(final_costs.begin(), final_costs.end()))
              << '\n';
    std::cout << "* Least final cost was "
              << (*std::min_element(final_costs.begin(), final_costs.end()))
              << '\n';
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
    ca_synopsis("[-g|-b METHOD] [-l]");

    bool* use_gui;
    bool* do_bench;
    bool* enable_log;

    const char* bench_method = "point_to_point";
    assert(use_gui = ca_opt('g', "gui", "<g", NULL,
               "launch the interactive GUI"));
    assert(ca_opt('h', "help", "<h", NULL, "prints this info"));
    assert(ca_opt('v', "version", "<v", NULL, "prints version info"));
    assert(do_bench = ca_opt('b', "bench", ".METHOD !@g", &bench_method,
               "benchmarks a given icp method"));  // for now disabled with -g
    assert(enable_log = ca_opt('l', "log", "", NULL, "enables debug logging"));

    if (argc == 1) {
        ca_print_help();
        return 1;
    } else if (ca_parse(NULL) != 0) {
        return 1;
    }

    Log.is_enabled = *enable_log;

    read_config();

    if (*do_bench) {
        run_benchmark(bench_method, *use_gui);
    } else if (*use_gui) {
        launch_gui();
    }
}
