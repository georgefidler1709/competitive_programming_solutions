#include <iostream>
#include <vector>
#include <sstream>

void fillTape(int tape_size, int track_idx, const std::vector<int>& tracks, std::vector<int>& selected_tracks, int& best_size, std::vector<int>& best_tracks) {
    if(tape_size < best_size) {
        best_size = tape_size;
        best_tracks = selected_tracks;
    }
    if(track_idx == tracks.size()) return;
    int curr_track = tracks[track_idx];
    if(tape_size - curr_track >= 0) {
        selected_tracks.push_back(curr_track);
        fillTape(tape_size - curr_track, track_idx + 1, tracks, selected_tracks, best_size, best_tracks);
        selected_tracks.pop_back();
    }
    fillTape(tape_size, track_idx + 1, tracks, selected_tracks, best_size, best_tracks);
}


int main() {
    std::string input;
    while(std::getline(std::cin, input)) {
        std::stringstream ss(input);
        std::vector<int> tracks;
        int num_tracks;
        int tape_size;
        ss >> tape_size >> num_tracks;
        int ti;
        while(ss >> ti) tracks.push_back(ti);
        std::vector<int> selected_tracks;
        std::vector<int> best_tracks;
        int best_size = tape_size;
        fillTape(tape_size, 0, tracks, selected_tracks, best_size, best_tracks);
        for(auto i : best_tracks) std::cout << i << " ";
        std::cout << "sum:" << tape_size - best_size << "\n";
    }
}