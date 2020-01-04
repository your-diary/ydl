using namespace std;
#include <iostream>
#include <sstream>
#include <vector>

#define NDEBUG

namespace prm {

    const char *input_output_file = "./.ydl_video_id_list.txt";
    const char *base_command_1 = "get_video_id";
    const char *base_command_2 = "ydl";

    enum option_type { get_video_id, ydl };

    const string separator = "--";

    void print_usage(const char *program_name) {
        cout << "Usage:\n  " << program_name << " [<option(s) to `get_video_id`>] [-- <option(s) to `ydl`>]\n";
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

    #ifndef NDEBUG
        cout << "This is a debug (dry-run) mode.\n";
    #endif

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

    ostringstream command_str;
    command_str << prm::base_command_1 << " "
                << "'" << prm::input_output_file << "'";
    for (int i = 0; i < get_video_id_option_array.size(); ++i) {
        command_str << " " << "'" << get_video_id_option_array[i] << "'";
    }
    #ifndef NDEBUG
        cout << command_str.str() << "\n";
    #else
        cout << "Starting `" << prm::base_command_1 << "`...\n";
        int exit_status = system(command_str.str().c_str());
        if (exit_status != 0) {
            return exit_status;
        }
        cout << "\n";
    #endif

    command_str.str("");
    command_str << prm::base_command_2 << " "
                << "'" << prm::input_output_file << "'";
    for (int i = 0; i < ydl_option_array.size(); ++i) {
        command_str << " " << "'" << ydl_option_array[i] << "'";
    }
    #ifndef NDEBUG
        cout << command_str.str() << "\n";
    #else
        cout << "Starting `" << prm::base_command_2 << "`...\n";
        exit_status = system(command_str.str().c_str());
        return exit_status;
    #endif

}

