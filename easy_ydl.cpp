using namespace std;
#include <iostream>
#include <vector>

namespace prm {

    enum option_type { get_video_id, ydl };

    const string separator = "--";

    void print_usage(const char *program_name) {
        cout << "Usage:\n";
    }

}

int main(int argc, char **argv) {

    if (argc > 1) { //-h, --help
        const string &first_option = argv[1];
        if (first_option == "-h" || first_option == "--help") {
            prm::print_usage(argv[0]);
            return 0;
        }
    }

    vector<string> get_video_id_option_array;
    vector<string> ydl_option_array;

    prm::option_type opt_type = prm::get_video_id;

    for (int i = 1; i < argc; ++i) {
        const string option = argv[i];
        if (option == prm::separator) {
            opt_type = prm::ydl;
            continue;
        }
        if (opt_type == prm::get_video_id) {
            get_video_id_option_array.push_back(option);
        } else if (opt_type == prm::ydl) {
            ydl_option_array.push_back(option);
        }
    }

    for (int i = 0; i < get_video_id_option_array.size(); ++i) {
        cout << get_video_id_option_array[i] << "\n";
    }
    cout << "---\n";
    for (int i = 0; i < ydl_option_array.size(); ++i) {
        cout << ydl_option_array[i] << "\n";
    }

// file="./.ydl_video_id_list.txt"
// get_video_id "${file}" "$@" && ydl "${file}"

}

#if 0

#endif

