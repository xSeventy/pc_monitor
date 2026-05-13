#include "httplib.h"

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

using namespace std;

struct CpuTimes {
    long long user = 0;
    long long nice = 0;
    long long system = 0;
    long long idle = 0;
    long long iowait = 0;
    long long irq = 0;
    long long softirq = 0;
    long long steal = 0;

    long long total() const {
        return user + nice + system + idle + iowait + irq + softirq + steal;
    }

    long long idle_total() const {
        return idle + iowait;
    }
};

struct CpuUsage {
    double usage_percent = 0.0;
};

struct MemoryInfo {
    long long total_bytes = 0;
    long long available_bytes = 0;
    long long used_bytes = 0;
    double used_percent = 0.0;
};

struct LoadAverage {
    double one = 0.0;
    double five = 0.0;
    double fifteen = 0.0;
};

CpuTimes read_total_cpu_times() {
    ifstream file("/proc/stat");

    string label;
    CpuTimes times;

    file >> label
         >> times.user
         >> times.nice
         >> times.system
         >> times.idle
         >> times.iowait
         >> times.irq
         >> times.softirq
         >> times.steal;

    return times;
}

double calculate_cpu_percent(const CpuTimes& previous, const CpuTimes& current) {
    const long long total_delta = current.total() - previous.total();
    const long long idle_delta = current.idle_total() - previous.idle_total();

    if (total_delta <= 0) {
        return 0.0;
    }

    return 100.0 * static_cast<double>(total_delta - idle_delta) /
           static_cast<double>(total_delta);
}

CpuUsage read_cpu_usage() {
    const CpuTimes first = read_total_cpu_times();

    this_thread::sleep_for(std::chrono::milliseconds(200));

    const CpuTimes second = read_total_cpu_times();

    CpuUsage usage;
    usage.usage_percent = calculate_cpu_percent(first, second);

    return usage;
}

MemoryInfo read_memory_info() {
    ifstream file("/proc/meminfo");

    MemoryInfo info;

    string key;
    long long value_kb = 0;
    string unit;

    while (file >> key >> value_kb >> unit) {
        if (key == "MemTotal:") {
            info.total_bytes = value_kb * 1024;
        } else if (key == "MemAvailable:") {
            info.available_bytes = value_kb * 1024;
        }
    }

    info.used_bytes = info.total_bytes - info.available_bytes;

    if (info.total_bytes > 0) {
        info.used_percent =
            100.0 * static_cast<double>(info.used_bytes) /
            static_cast<double>(info.total_bytes);
    }

    return info;
}

LoadAverage read_load_average() {
    ifstream file("/proc/loadavg");

    LoadAverage load;

    file >> load.one >> load.five >> load.fifteen;

    return load;
}

string make_metrics_json(
    const CpuUsage& cpu,
    const MemoryInfo& memory,
    const LoadAverage& load
) {
    ostringstream json;

    json << fixed << setprecision(1);

    json << "{";

    json << "\"cpu\":{";
    json << "\"usagePercent\":" << cpu.usage_percent;
    json << "},";

    json << "\"memory\":{";
    json << "\"totalBytes\":" << memory.total_bytes << ",";
    json << "\"availableBytes\":" << memory.available_bytes << ",";
    json << "\"usedBytes\":" << memory.used_bytes << ",";
    json << "\"usedPercent\":" << memory.used_percent;
    json << "},";

    json << setprecision(2);

    json << "\"loadAverage\":{";
    json << "\"one\":" << load.one << ",";
    json << "\"five\":" << load.five << ",";
    json << "\"fifteen\":" << load.fifteen;
    json << "}";

    json << "}";

    return json.str();
}

int main() {
    httplib::Server server;

    server.Get("/health", [](const httplib::Request&, httplib::Response& response) {
        response.set_header("Access-Control-Allow-Origin", "*");
        response.set_content("{\"status\":\"ok\"}", "application/json");
    });

    server.Get("/metrics", [](const httplib::Request&, httplib::Response& response) {
        const CpuUsage cpu = read_cpu_usage();
        const MemoryInfo memory = read_memory_info();
        const LoadAverage load = read_load_average();

        const std::string body = make_metrics_json(cpu, memory, load);

        response.set_header("Access-Control-Allow-Origin", "*");
        response.set_content(body, "application/json");
    });

    constexpr int port = 8080;

    cout << "PC Monitor backend started\n";
    cout << "Health:  http://127.0.0.1:" << port << "/health\n";
    cout << "Metrics: http://127.0.0.1:" << port << "/metrics\n";

    server.listen("127.0.0.1", port);

    return 0;
}
