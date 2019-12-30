import sys
import subprocess
import json

import math

API_key = "AIzaSyC0XPb0udk8HMMEa8PbYp_qZu7WONnGt3U"
num_max_result = "50"

#Example:
# print(convert_username_to_channel_id("GoogleDevelopers")) #=> "UC_x5XG1OV2P6uZZ5FSM9Ttw"
def convert_username_to_channel_id(username):

    command = [
        'curl',
        '-s',
        'https://www.googleapis.com/youtube/v3/channels?part=snippet&forUsername=' + username + '&key=' + API_key,
        '--header',
        'Accept: application/json',
        '--compressed',
    ]
    output = subprocess.check_output(command)
    json_dict = json.loads(output)

    return json_dict['items'][0]['id']

#"Uploaded Videos" is just a special kind of playlist (see |https://developers.google.com/youtube/v3/docs/playlistItems?authuser=1| for the detail).
#This function returns the id of the playlist which belongs to a channel specified by `id`.
#Example:
# print(get_uploaded_videos_playlist_id("username", "GoogleDevelopers")) #=> "UU_x5XG1OV2P6uZZ5FSM9Ttw"
# print(get_uploaded_videos_playlist_id("channel_id", "UC_x5XG1OV2P6uZZ5FSM9Ttw")) #same as above
def get_uploaded_videos_playlist_id(id_type, id):

    if (id_type == "username"):
        by_what = "forUsername=" + id
    elif (id_type == "channel_id"):
        by_what = "id=" + id
    else:
        print("The value of `id_type` is incorrect")
        return

    command = [
        'curl',
        '--silent',
        'https://www.googleapis.com/youtube/v3/channels?part=contentDetails&' + by_what + '&maxResults=' + num_max_result + '&key=' + API_key,
        '--header',
        'Accept: application/json',
        '--compressed',
    ]
    output = subprocess.check_output(command)
    json_dict = json.loads(output)

    if (json_dict['items'] == []):
        print("There exists no channel specified by [ {} ].".format(id))
        return

    return json_dict['items'][0]['contentDetails']['relatedPlaylists']['uploads']

#This function returns a list of ids of videos in a playlist specified by `playlist_id`.
#Note that the first element of the list corresponds to the **latest** video.
#Per request, at most `num_max_result` ids can be got, thus this function may send multiple requests that takes a while.
#Example:
# print(get_all_video_id_in_playlist("UU_x5XG1OV2P6uZZ5FSM9Ttw")) #=> list of video ids
def get_all_video_id_in_playlist(playlist_id, should_show_progress = False):

    video_id_list = []

    page_info = ""

    if (should_show_progress):
        num_total_result = 0
        num_page = 0
        i = -1

    while (True):

        command = [
            'curl',
            '--silent',
            'https://www.googleapis.com/youtube/v3/playlistItems?part=contentDetails' + page_info + '&maxResults=' + num_max_result + '&playlistId=' + playlist_id + '&key=' + API_key,
            '--header',
            'Accept: application/json',
            '--compressed'
        ]
        output = subprocess.check_output(command)
        json_dict = json.loads(output)

        if (should_show_progress):

            i += 1

            if (i == 0):
                num_total_result = int(json_dict['pageInfo']['totalResults'])
                num_page = math.ceil(num_total_result / int(num_max_result))

            if (num_page > 1):
                if (i == 0):
                    print("Retrieving video ids...")
                print("    [{}/{}] Done.".format(i + 1, num_page))
            else:
                print(num_page)

        for video in json_dict['items']:
            video_id_list.append(video['contentDetails']['videoId'])

        if ('nextPageToken' in json_dict):
            page_info = '&pageToken=' + json_dict['nextPageToken']
        else:
            break

    return video_id_list

#Example:
# print(get_video_title("5X13ZffwtjE")) #=> "Nina77「完全少女論」"
def get_video_title(video_id, should_append_uploaded_date = False):

    command = [
        'curl',
        '--silent',
        'https://www.googleapis.com/youtube/v3/videos?part=snippet&id=' + video_id + '&key=' + API_key,
        '--header',
        'Accept: application/json',
        '--compressed',
    ]
    output = subprocess.check_output(command)
    json_dict = json.loads(output)

    title = json_dict['items'][0]['snippet']['title']
    if (should_append_uploaded_date):
        title += "_" + json_dict['items'][0]['snippet']['publishedAt']

    return title

if (__name__ == "__main__"):

    usage_message = "Usage: python3 " + sys.argv[0] + " {{--username <user name>} | {--channel-id <channel id>}}"

    if (len(sys.argv) != 3):
        print(usage_message)
        sys.exit(0)

    if (sys.argv[1] == "--username"):
        id_type = "username"
    elif (sys.argv[1] == "--channel-id"):
        id_type = "channel_id"
    else:
        print("Invalid option [ {} ] was passed.".format(sys.argv[1]))
        print(usage_message)
        sys.exit(1)

    playlist_id = get_uploaded_videos_playlist_id(id_type, sys.argv[2])
    if (playlist_id == None):
        sys.exit(1)
    video_id_list = get_all_video_id_in_playlist(playlist_id)
    for i in video_id_list:
        print(i + " " + get_video_title(i, True))

