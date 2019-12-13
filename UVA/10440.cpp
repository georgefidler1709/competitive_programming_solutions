#include <iostream>
#include <vector>

int main() {
    int cases;
    std::cin >> cases;

    for(long long cur_case = 0; cur_case < cases; ++cur_case) {
        long long ferry_cap, trip_time, num_cars;
        std::cin >> ferry_cap >> trip_time >> num_cars;
        std::vector<long long>car_times{};
        for(long long i = 0; i < num_cars; ++i) {
            long long cur_car;
            std::cin >> cur_car;
            car_times.push_back(cur_car);
        }

        long long min_time = car_times[car_times.size() - 1] + trip_time;
        long long min_trips = 1;

        std::vector<long long>leaving_times{};
        leaving_times.push_back(car_times[car_times.size() - 1]);
        for(long long curr_leaving_idx = num_cars - ferry_cap - 1; curr_leaving_idx >= 0; curr_leaving_idx -= ferry_cap) {
            leaving_times.insert(leaving_times.begin(), car_times[curr_leaving_idx]);
            ++min_trips;
        }

        long long time_deficit = 0;
        for(long long i = 0; i < leaving_times.size() - 1; ++i) {
            time_deficit += (leaving_times[i] + (2 * trip_time)) - leaving_times[i + 1];
            if(time_deficit < 0) time_deficit = 0;
        }

        if(time_deficit > 0) min_time += time_deficit;
        std::cout << min_time << " " << min_trips << "\n";
    }
}