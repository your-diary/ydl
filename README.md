# *ydl*, a front end for `youtube-dl`

See [README_ja.md](./README_ja.md) for a Japanese :jp: version.

## Index

1. [Introduction](#introduction)
    1. [What can *ydl* do?](#what-can-ydl-do)
    2. [Example 1](#example-1)
    3. [Example 2](#example-2)
2. [Usage](#usage)
    1. [`get_video_id`](#get_video_id)
    2. [`ydl`](#ydl)
    3. [`easy_ydl`](#easy_ydl)
3. [Build](#build)
    1. [Tested Environment](#tested-environment)
    2. [Requirement](#requirement)
    3. [Preparation](#preparation)
    4. [Installation](#installation)
4. [Q&A](#qa)

## Introduction

### What can *ydl* do?

*ydl* is a front end for [`youtube-dl`](https://ytdl-org.github.io/youtube-dl/index.html). ydl consists of two programs: `get_video_id` and `ydl`.

`get_video_id` constructs a list of video ids associated with a username, a channel id or a playlist id, and writes the list to a specified file. Each video id is appended to the file as long as it is not found in the file. This makes it possible, if you in the past called `get_video_id` with the same arguments, to *update* the file.

`ydl` reads a specified file to construct a list of video ids and then makes `youtube-dl` download videos according to the list. When a video is successfully downloaded, the video id corresponding to it is prepended with a number sign `#`, which indicates "you have already downloaded the video". By that, you can suspend and resume the operation (without re-downloading anything), or only download newly uploaded videos after updating the list file by `get_video_id`.

### Example 1

Say you want to track uploaded videos in [*Apple channel*](https://www.youtube.com/user/Apple/videos).

1. Make a directory and enter it.
```bash
$ mkdir Apple_video
$ cd Apple_video/
```

2. Retrieve video ids and write them to a file.

As the channel URL suggests, the username of the channel is `Apple`.
```bash
$ get_video_id --username Apple video_id_list.txt
Retrieving the playlist id...
Retrieving video ids...
    [1/7] Done.
    [2/7] Done.
    [3/7] Done.
    [4/7] Done.
    [5/7] Done.
    [6/7] Done.
    [7/7] Done.
Opening [ video_id_list.txt ]...
Appending 315 new video ids to the file...
Done.

$ cat video_id_list.txt
yBX-KpMoxYk Apple Special Event. September 10, 2013._2013-10-09T04:33:09.000Z
IbWOQWw1wkM Apple - Making the Mac Pro_2013-10-23T17:36:21.000Z
(snip)
6EE68bvpaIM Slofie in the rain with iPhone 11 — Apple_2019-12-28T20:00:00.000Z
BpOZ9oxNwMY Slofie to the beat with iPhone 11 — Apple_2019-12-28T20:00:00.000Z
```

3. Download videos according to the file.
```bash
$ ydl video_id_list.txt 
------------ [1/315] yBX-KpMoxYk start ------------
[youtube] yBX-KpMoxYk: Downloading webpage
[youtube] yBX-KpMoxYk: Downloading video info webpage
WARNING: Requested formats are incompatible for merge and will be merged into mkv.
[download] Destination: Apple Special Event. September 10, 2013._2013-10-09T04:33:09.000Z.f137.mp4
[download] 100% of 412.91MiB in 00:35
[download] Destination: Apple Special Event. September 10, 2013._2013-10-09T04:33:09.000Z.f251.webm
[download] 100% of 65.42MiB in 00:05
[ffmpeg] Merging formats into "Apple Special Event. September 10, 2013._2013-10-09T04:33:09.000Z.mkv"
Deleting original file Apple Special Event. September 10, 2013._2013-10-09T04:33:09.000Z.f137.mp4 (pass -k to keep)
Deleting original file Apple Special Event. September 10, 2013._2013-10-09T04:33:09.000Z.f251.webm (pass -k to keep)
------------- [1/315] yBX-KpMoxYk end -------------
------------ [2/315] IbWOQWw1wkM start ------------
[youtube] IbWOQWw1wkM: Downloading webpage
[youtube] IbWOQWw1wkM: Downloading video info webpage
WARNING: Requested formats are incompatible for merge and will be merged into mkv.
[download] Destination: Apple - Making the Mac Pro_2013-10-23T17:36:21.000Z.f137.mp4
[download]  81.5% of 24.82MiB at  8.83MiB/s ETA 00:00^C
ERROR: Interrupted by user
--------- [2/315] IbWOQWw1wkM interrupted ---------
```

Here you cancelled the operation by pressing Ctrl+c (i.e. sending `SIGINT` to the foreground process group) while downloading the second video. Since the first video has successfully been downloaded, its video id is commented out with `#`.
```bash
$ head -n 3 video_id_list.txt 
#yBX-KpMoxYk Apple Special Event. September 10, 2013._2013-10-09T04:33:09.000Z
IbWOQWw1wkM Apple - Making the Mac Pro_2013-10-23T17:36:21.000Z
4FunXnJQxYU Apple Special Event. October 22, 2013._2013-10-23T21:09:58.000Z
```

Thus, when you again execute `ydl` command, an operation starts with the second video.
```bash
$ ydl video_id_list.txt 
------------ [1/314] IbWOQWw1wkM start ------------
[youtube] IbWOQWw1wkM: Downloading webpage
[youtube] IbWOQWw1wkM: Downloading video info webpage
WARNING: Requested formats are incompatible for merge and will be merged into mkv.
[download] Resuming download at byte 21210704
[download] Destination: Apple - Making the Mac Pro_2013-10-23T17:36:21.000Z.f137.mp4
[download] 100% of 24.82MiB in 00:00
[download] Destination: Apple - Making the Mac Pro_2013-10-23T17:36:21.000Z.f251.webm
[download] 100% of 2.12MiB in 00:00
[ffmpeg] Merging formats into "Apple - Making the Mac Pro_2013-10-23T17:36:21.000Z.mkv"
Deleting original file Apple - Making the Mac Pro_2013-10-23T17:36:21.000Z.f137.mp4 (pass -k to keep)
Deleting original file Apple - Making the Mac Pro_2013-10-23T17:36:21.000Z.f251.webm (pass -k to keep)
------------- [1/314] IbWOQWw1wkM end -------------
------------ [2/314] 4FunXnJQxYU start ------------
```

4. Update the list and fetch newly uploaded videos.

A month has passed since the previous step. Now you want to fetch newly uploaded videos since then. Necessarily commands are exactly same as before.
```bash
$ get_video_id --username Apple video_id_list.txt
Retrieving the playlist id...
Retrieving video ids...
    [1/7] Done.
Opening [ video_id_list.txt ]...
Appending 3 new video ids to the file...
Done.

$ ydl video_id_list.txt
------------ [1/3] KEc3aGjN228 start ------------
[youtube] KEc3aGjN228: Downloading webpage
[youtube] KEc3aGjN228: Downloading video info webpage
[download] Destination: Group slofie on iPhone 11 — Apple_2019-12-28T20:00:04.000Z.f137.mp4
[download] 100% of 2.53MiB in 00:00
[download] Destination: Group slofie on iPhone 11 — Apple_2019-12-28T20:00:04.000Z.f140.m4a
[download] 100% of 238.88KiB in 00:00
[ffmpeg] Merging formats into "Group slofie on iPhone 11 — Apple_2019-12-28T20:00:04.000Z.mp4"
Deleting original file Group slofie on iPhone 11 — Apple_2019-12-28T20:00:04.000Z.f137.mp4 (pass -k to keep)
Deleting original file Group slofie on iPhone 11 — Apple_2019-12-28T20:00:04.000Z.f140.m4a (pass -k to keep)
------------- [1/3] KEc3aGjN228 end -------------
------------ [2/3] 6EE68bvpaIM start ------------
```

### Example 2

Although it is simple enough to use the combination of `get_video_id` and `ydl`, you may sometimes be so lazy that typing two commands, considering a good file name or pass it manually as an argument feels troublesome. If that the case, and if you never mind about a file name associated with the list, you can simply use `easy_ydl` command. For example, these two sets of commands are equivalent.
```bash
$ easy_ydl --username Apple
```
```bash
$ file="./.ydl_video_id_list.txt"
$ get_video_id --username Apple "${file}" && ydl "${file}"
```

And these two are equivalent.
```bash
$ easy_ydl
```
```bash
$ file="./.ydl_video_id_list.txt"
$ ydl "${file}"
```

The detail is explained in [Usage](#easy_ydl).

## Usage

See [Example 1](#example-1) and [Example 2](#example-2) for examples.

### `get_video_id`
```bash
$ get_video_id
Usage:
  get_video_id <playlist spec> <output file name>

<playlist spec>:
  --username <user name>         specify all uploaded videos by <user name>
  --channel-id <channel id>      specify all uploaded videos in <channel id>
  --playlist-id <playlist id>    specify all videos in <playlist id>
```

### `ydl`
```bash
$ ydl
Usage:
  ydl <input file name>
```

### `easy_ydl`

This command is implemented as below. If you don't understand the script (since you're not familiar with [*shell script*](https://en.wikipedia.org/wiki/Shell_script)), see [Example 2](#example-2) for examples.
```bash
$ cat easy_ydl
#!/usr/bin/env sh
file="./.ydl_video_id_list.txt"
if [ $# = 0 ]; then
    ydl "${file}"
else
    get_video_id "$1" "$2" "${file}" && ydl "${file}"
fi
```

## Build

### Tested Environment

The program is tested under

- [Arch Linux](https://www.archlinux.org/)

- [Arch Linux ARM](https://archlinuxarm.org/)

- [Linux Mint 19](https://linuxmint.com/)

- [OS X 10.10 Yosemite](https://en.wikipedia.org/wiki/OS_X_Yosemite)

### Requirement

- A POSIX-compatible shell (e.g. [bash](https://www.gnu.org/software/bash/))

- [GNU Make](https://www.gnu.org/software/make/)

- [g++](https://gcc.gnu.org/)

- [Python 3.x](https://www.python.org/)

- [youtube-dl](https://ytdl-org.github.io/youtube-dl/index.html)

### Preparation

1. Get an API key for [*YouTube Data API*](https://developers.google.com/youtube).

To use ydl, an API key for YouTube DATA API is needed. It is easily and freely got. See [here](https://developers.google.com/youtube/v3/getting-started#before-you-start) for brief instructions. After you get an key (like `AIpaSyCaXPx0utk8HmM4a8PbYp_qeu7wOknGT3U`), store it in `.ydl_api_key` file under your home directory.

```bash
$ echo "AIpaSyCaXPx0utk8HmM4a8PbYp_qeu7wOknGT3U" > ~/.ydl_api_key
```

2. Download this project.

You can use `git clone <URL of this page>` command or press a download button on this webpage.

3. Change shebang lines if needed.

There exists a [shebang](https://en.wikipedia.org/wiki/Shebang_(Unix)) line in `get_video_id` and `easy_ydl`.
```bash
$ head -n 1 get_video_id easy_ydl
==> get_video_id <==
#!/usr/bin/env python3

==> easy_ydl <==
#!/usr/bin/env sh
```

Their contents may be inappropriate for your environment.

- With low probability, `env` command is not located in `/usr/bin`. Check that with `which -a env` command or `type -a env` command. If the location is incorrect, replace `/usr/bin/env` with the correct path.

- With sufficiently high probability, a name of Python 3 interpreter is not `python3`. In that case, replace `python3` with the correct name (possibly `python`).

### Installation

- Install
```bash
$ make && make install
```

- Uninstall
```bash
$ make uninstall
```

## Q&A

- `make` fails with the error message "*Makefile:16: *** missing separator.  Stop.*". How can I solve it?

You may encounter this problem on macOS. In that case, first execute `brew install make` and then use `gmake` instead of `make`.

- Do I have to prepare *N* list files if I'd like to track *N* channels?

No. It is possible to associate multiple usernames, channel ids or playlist ids with a single list. Just execute `video_id_list` multiple times with different `<playlist spec>`s but with the same `<output file name>`.

- I'd like to execute ydl on a server. What is a good way?

Say you have a computer and a headless server which can be accessed by the computer via SSH. When a download operation takes more than a day (yes, it often does), you may want to leave it to the server, shutdown the computer and lay down on a bed. In such a situation, these commands are useful.
```bash
$ ssh <host>

$$ get_video_id --username Apple video_id_list.txt
Reading an API key from [ ~/.ydl_api_key ]
Retrieving the playlist id...
Retrieving video ids...
    [1/7] Done.
    [2/7] Done.
    [3/7] Done.
    [4/7] Done.
    [5/7] Done.
    [6/7] Done.
    [7/7] Done.
Opening [ video_id_list.txt ]...
Appending 315 new video ids to the file...
Done.

$$ ydl video_id_list.txt > ydl_log 2>&1 & #Execute as a background job and redirect all to `ydl_log` file.
[1] 24535

$$ disown #Now you can logout from the server without terminating the operation.
```

To see the progress,
```bash
$ ssh <host> #re-login 

$$ tail -f ydl_log
```

To cancel the operation,
```bash
$ ssh <host> #re-login

$$ ps -eH | grep -A 1 ydl #find a process id

$$ kill -SIGINT 24742 #kill `youtube-dl` (not `ydl`)
```

