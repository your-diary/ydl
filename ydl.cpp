using namespace std;
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <csignal>

namespace prm {

    const char comment_char = '#';

    const string URL_prefix = "https://www.youtube.com/watch?v=";

    volatile sig_atomic_t has_SIGINT_caught = false;
//     volatile sig_atomic_t has_SIGUSR1_caught = false;

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

    if (argc != 2) {
        cout << "Usage:\n  " << argv[0] << " <input file name>" << "\n";
        return 1;
    }

    vector<string> video_id_list;
    vector<string> title_list;

    string video_id;
    string title;

    //parse the input file
    {

        ifstream ifs(argv[1]);
        if (!ifs) {
            cout << "Couldn't open the file [ " << argv[1] << " ].\n";
            return 1;
        }

        while (true) {

            getline(ifs, video_id, ' ');
            getline(ifs, title);

            if (!ifs) {
                break;
            }

            video_id_list.push_back(video_id);
            title_list.push_back(title);

        }

        ifs.close();

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

        exit_status_list[i] = system(command.str().c_str());

        if (exit_status_list[i] != 0 || prm::has_SIGINT_caught == true) {
            cout << "--------- [" << download_index << "/" << num_should_download << "] " << video_id_list[i] << " interrupted ---------\n";
            break;
        }

        cout << "------------- [" << download_index << "/" << num_should_download << "] " << video_id_list[i] << " end -------------\n";

    }
        
    //output to the input file
    {

        ofstream ofs(argv[1]);
        if (!ofs) {
            cout << "Couldn't open file [ " << argv[1] << " ].\n";
            return 1;
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

