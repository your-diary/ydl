using namespace std;
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <csignal>

#define NDEBUG

namespace prm {

    const char comment_char = '#';
    const char prefix_for_option_array_element = '\'';
    const char prefix_for_youtube_dl_option_array_element = '"';

    const string URL_prefix = "https://www.youtube.com/watch?v=";

    volatile sig_atomic_t has_SIGINT_caught = false;
//     volatile sig_atomic_t has_SIGUSR1_caught = false;

    void print_usage(const char *program_name) {
        cout << "Usage:\n  " << program_name << " <input file name> [<option(s) to `youtube-dl`>]" << "\n";
    }

    void signal_handler(int signal_id) {
        if (signal_id == SIGINT) {
            prm::has_SIGINT_caught = true;
//         } else if (signal_id == SIGUSR1) {
//             prm::has_SIGUSR1_caught = true;
        }
    }

    //Repeatedly replace `old_part` with `new_part` in `str`.
    void replace(string &str, const string &old_part, const string &new_part) {
        string::size_type index = 0;
        while (true) {
            index = str.find(old_part, index);
            if (index == string::npos) {
                break;
            } else {
                str.replace(index, old_part.size(), new_part);
            }
            index += new_part.size();
        }
    }

    string make_up_title(string str) {

        //convert halfwidth slashes in `str` to fullwidth slashes
        {
            string old_part = "/";
            string new_part = "／";
            prm::replace(str, old_part, new_part);
        }

        //escape single quotes (replace ' with '"'"')
        {
            string old_part = "'";
            string new_part = "'\"'\"'";
            prm::replace(str, old_part, new_part);
        }

        return str;

    }

}

int main(int argc, char **argv) {

    signal(SIGINT, prm::signal_handler);
    signal(SIGUSR1, prm::signal_handler);

    if (argc == 1) {
        prm::print_usage(argv[0]);
        return 1;
    }

    const char *input_output_file = argv[1];

    if (input_output_file[0] == '-') { //-h, --help
        prm::print_usage(argv[0]);
        return 0;
    }

    #ifndef NDEBUG
        cout << "This is a debug (dry-run) mode.\n";
    #endif

    vector<string> option_list; //contains options to `get_video_id`
    vector<string> youtube_dl_option_list; //contains options to `youtube-dl`

    vector<string> video_id_list;
    vector<string> title_list;

    //parse the input file
    {

        ifstream ifs(input_output_file);
        if (!ifs) {
            cout << "Couldn't open the file [ " << input_output_file << " ].\n";
            return 1;
        }

        while (true) {

            string line;
            getline(ifs, line);
            if (!ifs) {
                break;
            }

            if (line[0] == prm::prefix_for_option_array_element) {
                option_list.push_back(line);
            } else if (line[0] == prm::prefix_for_youtube_dl_option_array_element) {
                youtube_dl_option_list.push_back(line);
            } else {
                if (line[0] == '\0') { //ignore empty lines
                    continue;
                }
                string::size_type index = line.find(' ');
                if (index == string::npos) {
                    cout << "An error occurred while parsing the line [ " << line << " ]\n";
                    return 1;
                }
                video_id_list.push_back(string(line.begin(), line.begin() + index));
                title_list.push_back(string(line.begin() + index + 1, line.end()));
            }

        }

        ifs.close();

    }

    //process `<option(s) to `youtube-dl`>`
    for (int i = 2; i < argc; ++i) {

        const string option_to_youtube_dl = prm::prefix_for_youtube_dl_option_array_element + string(argv[i]);

        if (find(youtube_dl_option_list.begin(), youtube_dl_option_list.end(), option_to_youtube_dl) == youtube_dl_option_list.end()) { //avoid duplication
            youtube_dl_option_list.push_back(option_to_youtube_dl);
        }

    }

    vector<int> exit_status_list(video_id_list.size(), 0);

    unsigned num_should_download = video_id_list.size();
    for (int i = 0; i < video_id_list.size(); ++i) {
        if (video_id_list[i][0] == prm::comment_char) {
            --num_should_download;
        }
    }
    unsigned download_index = 0;

    for (int i = 0; i < video_id_list.size(); ++i) {

        if (video_id_list[i][0] == prm::comment_char) {
            continue;
        }

        ++download_index;

        cout << "------------ [" << download_index << "/" << num_should_download << "] " << video_id_list[i] << " start ------------\n";

        string title = prm::make_up_title(title_list[i]);

        ostringstream command;
        command << "youtube-dl" << " "
                << "'" << prm::URL_prefix << video_id_list[i] << "'" << " "
                << "--output" << " " << "'" << title << ".%(ext)s" << "'";
        for (int i = 0; i < youtube_dl_option_list.size(); ++i) {
            command << " " << string(youtube_dl_option_list[i].begin() + 1, youtube_dl_option_list[i].end());
        }

        #ifndef NDEBUG
            cout << command.str() << "\n";
            exit_status_list[i] = 0;
        #else
            exit_status_list[i] = system(command.str().c_str());
        #endif

        if (exit_status_list[i] != 0 || prm::has_SIGINT_caught == true) {
            cout << "----------- [" << download_index << "/" << num_should_download << "] " << video_id_list[i] << " failed ------------\n";
            if (i == 0 && argc > 2) {
                cout << "Failed in downloading the very first video.\n";
                cout << "It is suspected a command-line argument is incorrect.\n";
                cout << "Exiting without touching the file [ " << input_output_file << " ]...\n";
                return 1;
            }
            break;
        }

        cout << "------------- [" << download_index << "/" << num_should_download << "] " << video_id_list[i] << " end -------------\n";

    }
        
    //output to the input file
    {

        #ifndef NDEBUG
            ofstream ofs("/dev/stdout");
        #else
            ofstream ofs(input_output_file);
        #endif
        if (!ofs) {
            cout << "Couldn't open file [ " << input_output_file << " ].\n";
            return 1;
        }

        for (const string &i : option_list) {
            ofs << i << "\n";
        }

        for (const string &i : youtube_dl_option_list) {
            ofs << i << "\n";
        }

        bool should_comment_out = true;
        for (int i = 0; i < video_id_list.size(); ++i) {
            if (exit_status_list[i] != 0) {
                should_comment_out = false;
            }
            if (should_comment_out && video_id_list[i][0] != prm::comment_char) {
                ofs << prm::comment_char;
            }
            ofs << video_id_list[i] << " " << title_list[i] << "\n";
        }

        ofs.close();

    }

}

