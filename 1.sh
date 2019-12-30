API_key="AIzaSyC0XPb0udk8HMMEa8PbYp_qZu7WONnGt3U"
num_max_result="50"
json_parser="python3 ./parse_json.py"

get_uploaded_videos_playlist_id() {
    if [[ $# != 2 ]]; then
        echo "Number of arguments is incorrect."
        return 1
    elif [[ $1 == "--username" ]]; then
        local by_what="forUsername=${2}"
    elif [[ $1 == "--channel-id" ]]; then
        local by_what="id=${2}"
    else
        echo "An invalid option [ $1 ] was passed."
        return 1
    fi
    curl --silent "https://www.googleapis.com/youtube/v3/channels?part=contentDetails&${by_what}&maxResults=${num_max_result}&key=${API_key}" --header 'Accept: application/json' --compressed | "${json_parser}" "['items'][0]['contentDetails']['relatedPlaylists']['uploads']"
}
# get_uploaded_videos_playlist_id --username GoogleDevelopers
# get_uploaded_videos_playlist_id --channel-id "UC_x5XG1OV2P6uZZ5FSM9Ttw"

get_all_video_id_in_playlist() {
    if [[ $# != 1 ]]; then
        echo "Number of arguments is incorrect."
        return 1
    fi
    local playlist_id=$1
    curl --silent "https://www.googleapis.com/youtube/v3/playlistItems?part=status&maxResults=${num_max_result}&playlistId=${playlist_id}&key=${API_key}" --header 'Accept: application/json' --compressed | python3 "./print_video_ids.py"
}

get_all_video_id_in_playlist UU_x5XG1OV2P6uZZ5FSM9Ttw


# channel_id="UC_x5XG1OV2P6uZZ5FSM9Ttw" #GoogleDevelopers
# channel_id="UCtKDFB-NVLHqIXM9y-C4-Mw"
# curl "https://www.googleapis.com/youtube/v3/playlists?part=snippet&channelId=${channel_id}&maxResults=${num_max_result}&key=${API_key}" --header 'Accept: application/json' --compressed

#Get a channel ID from a username.
# username="GoogleDevelopers"
# curl -s "https://www.googleapis.com/youtube/v3/channels?part=snippet&forUsername=${username}&key=${API_key}" --header 'Accept: application/json' --compressed | "${json_parser}" "['items'][0]['id']"

#Uploaded videos list is a just special kind of playlist.
#See |https://developers.google.com/youtube/v3/docs/playlistItems?authuser=1| for the detail.

