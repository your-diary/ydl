# *ydl*, a cross-platform front end for `youtube-dl`

日本語版 :jp: が[README_ja.md](./README_ja.md)から見られます。 (See [README_ja.md](./README_ja.md) for a Japanese :jp: version.)

This document is for ydl <ins>**v1.1.x**</ins>. Refer to another document for an explanation about another version of ydl.

## Index

1. [Introduction](#introduction)
    1. [What is *ydl*?](#what-is-ydl)
    2. [I'm a Beginner / I Have No Time to Read This Long Documentation](#step_by_step_documentation)
    3. [Example 1](#example-1)
    4. [Example 2](#example-2)
2. [Usage](#usage)
    1. [`get_video_id`](#get_video_id)
    2. [`ydl`](#ydl)
    3. [`easy_ydl`](#easy_ydl)
3. [Instruction File](#instruction-file)
    1. [About](#about)
    2. [Syntax](#syntax)
4. [Build](#build)
    1. [Tested Environment](#tested-environment)
    2. [Requirement](#requirement)
    3. [Preparation](#preparation)
    4. [Installation](#installation)
5. [Q&A](#qa)
6. [Contribution](#contribution)

## Introduction

### What is *ydl*?

*ydl* is a front end for [`youtube-dl`](https://ytdl-org.github.io/youtube-dl/index.html). ydl is cross-platform (see [Tested Environment](#tested-environment)) and works with no GUI. ydl consists of two programs: `get_video_id` and `ydl`.

`get_video_id` constructs a list of video ids associated with a username, a channel id or a playlist id (hereafter, we call them *target*s), and writes the list to a specified file. Each video id is appended to the file as long as it is not found in the file. This makes it possible, if you in the past called `get_video_id` with the same arguments, to *update* the file.

`ydl` reads a specified file to construct a list of video ids and then makes `youtube-dl` download videos according to the list. When a video is successfully downloaded, the video id corresponding to it is prepended with a number sign `#`, which indicates "you have already downloaded the video". By that, you can suspend and resume the operation (without re-downloading anything), or only download newly uploaded videos after updating the list file by `get_video_id`.

<a name="step_by_step_documentation"></a>
### I'm a Beginner / I Have No Time to Read This Long Documentation

We offer a step by step documentation for beginners, which is completely independent of this exhaustive documentation. How to set up environment, how to install `ydl` and how to use it are explained in detail with dozens of screenshots. Visit our [website](http://ynn-note.com/articles/ydl/index.html) to read it.

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
$ get_video_id id.txt --username Apple
Reading an API key from [ ~/.ydl_api_key ]
----- [1/1] Apple (username) start -----
Retrieving the playlist id...
Retrieving video ids...
    [1/7] Done.
    [2/7] Done.
    [3/7] Done.
    [4/7] Done.
    [5/7] Done.
    [6/7] Done.
    [7/7] Done.
------ [1/1] Apple (username) end ------
Opening [ id.txt ]...
Appending 314 new video ids to the file...
Done.
```

`video_id_list` writes information about target (`'--username Apple`) in addition to the retrieved video ids.
```bash
$ cat id.txt
'--username Apple
yBX-KpMoxYk Apple Special Event. September 10, 2013._2013-10-09T04:33:09.000Z
IbWOQWw1wkM Apple - Making the Mac Pro_2013-10-23T17:36:21.000Z
(snip)
6EE68bvpaIM Slofie in the rain with iPhone 11 — Apple_2019-12-28T20:00:00.000Z
BpOZ9oxNwMY Slofie to the beat with iPhone 11 — Apple_2019-12-28T20:00:00.000Z
```

3. Download videos according to the file.
```bash
$ ydl id.txt
------------ [1/314] yBX-KpMoxYk start ------------
[youtube] yBX-KpMoxYk: Downloading webpage
[youtube] yBX-KpMoxYk: Downloading video info webpage
WARNING: Requested formats are incompatible for merge and will be merged into mkv.
[download] Destination: Apple Special Event. September 10, 2013._2013-10-09T04:33:09.000Z.f137.mp4
[download] 100% of 412.91MiB in 00:50
[download] Destination: Apple Special Event. September 10, 2013._2013-10-09T04:33:09.000Z.f251.webm
[download] 100% of 65.42MiB in 00:08
[ffmpeg] Merging formats into "Apple Special Event. September 10, 2013._2013-10-09T04:33:09.000Z.mkv"
Deleting original file Apple Special Event. September 10, 2013._2013-10-09T04:33:09.000Z.f137.mp4 (pass -k to keep)
Deleting original file Apple Special Event. September 10, 2013._2013-10-09T04:33:09.000Z.f251.webm (pass -k to keep)
------------- [1/314] yBX-KpMoxYk end -------------
------------ [2/314] IbWOQWw1wkM start ------------
[youtube] IbWOQWw1wkM: Downloading webpage
[youtube] IbWOQWw1wkM: Downloading video info webpage
WARNING: Requested formats are incompatible for merge and will be merged into mkv.
[download] Destination: Apple - Making the Mac Pro_2013-10-23T17:36:21.000Z.f137.mp4
[download]  79.4% of 24.82MiB at  3.39MiB/s ETA 00:01^C
ERROR: Interrupted by user
----------- [2/314] IbWOQWw1wkM failed ------------
```

Here you cancelled the operation by pressing Ctrl+c (i.e. sending `SIGINT` to the foreground process group) while downloading the second video. Since the first video has successfully been downloaded, its video id is commented out with `#`.
```bash
$ head -n 4 id.txt
'--username Apple
#yBX-KpMoxYk Apple Special Event. September 10, 2013._2013-10-09T04:33:09.000Z
IbWOQWw1wkM Apple - Making the Mac Pro_2013-10-23T17:36:21.000Z
4FunXnJQxYU Apple Special Event. October 22, 2013._2013-10-23T21:09:58.000Z
```

Thus, when you again execute `ydl` command, an operation starts with the second video.
```bash
$ ydl id.txt
------------ [1/313] IbWOQWw1wkM start ------------
[youtube] IbWOQWw1wkM: Downloading webpage
[youtube] IbWOQWw1wkM: Downloading video info webpage
WARNING: Requested formats are incompatible for merge and will be merged into mkv.
[download] Resuming download at byte 20649728
[download] Destination: Apple - Making the Mac Pro_2013-10-23T17:36:21.000Z.f137.mp4
[download] 100% of 24.82MiB in 00:00
[download] Destination: Apple - Making the Mac Pro_2013-10-23T17:36:21.000Z.f251.webm
[download] 100% of 2.12MiB in 00:00
[ffmpeg] Merging formats into "Apple - Making the Mac Pro_2013-10-23T17:36:21.000Z.mkv"
Deleting original file Apple - Making the Mac Pro_2013-10-23T17:36:21.000Z.f137.mp4 (pass -k to keep)
Deleting original file Apple - Making the Mac Pro_2013-10-23T17:36:21.000Z.f251.webm (pass -k to keep)
------------- [1/313] IbWOQWw1wkM end -------------
------------ [2/313] 4FunXnJQxYU start ------------
```

4. Update the list and fetch newly uploaded videos.

A month has passed since the previous step. Now you want to fetch newly uploaded videos since then. Necessarily commands are quite simple.
```bash
$ get_video_id id.txt #no need to pass target information
Reading an API key from [ ~/.ydl_api_key ]
----- [1/1] Apple (username) start -----
Retrieving the playlist id...
Retrieving video ids...
    [1/7] Done.
------ [1/1] Apple (username) end ------
Opening [ id.txt ]...
Appending 3 new video ids to the file...
Done.

$ ydl id.txt
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
$ get_video_id "${file}" --username Apple && ydl "${file}"
```

And these two are equivalent.
```bash
$ easy_ydl
```
```bash
$ file="./.ydl_video_id_list.txt"
$ get_video_id "${file}" && ydl "${file}"
```

The detail is explained in [Usage](#easy_ydl).

## Usage

See [Example 1](#example-1) and [Example 2](#example-2) for examples.

See [Instruction File](#instruction-file) for the detail about a instruction file.

### `get_video_id`

`get_video_id` constructs a list of video ids associated with one or more targets (i.e. a username, a channel id or a playlist id) and writes it to the specified instruction file `<output file name>`. A target is specified with `--username`, `--channel-id` or `--playlist-id` option. For example, when you pass `--username Apple` option, all ids of videos uploaded by the user `Apple` are retrieved.

In addition to video ids, options passed to `get_video_id` are stored in the file. These options are read when you in the future execute `get_video_id` with the same file, which removes troublesomeness of passing the same arguments every time. So, actually, `<output file name>` is also a input file name.

Videos ids and options are written wisely without duplication.

Usage:
```bash
$ get_video_id --help
Usage:
  get_video_id <output file name> [<option(s)>]

Option:
  --username <user name>         track all uploaded videos by <user name>
  --channel-id <channel id>      track all uploaded videos in <channel id>
  --playlist-id <playlist id>    track all videos in <playlist id>
  --with-time                    append uploaded time to a video title
  --no-time                      cancel `--with-time`
  --verbose                      enable verbose output
  -h, --help                     show this help
```

### `ydl`

`ydl` reads a specified instruction file `<input file name>` to construct a list of video ids and then makes `youtube-dl` download videos according to the list. The file is usually prepared with `get_video_id` in advance. When a video is successfully downloaded, the video id corresponding to it is prepended with a number sign `#`, which indicates "you have already downloaded the video". By that, you can suspend and resume the operation (without re-downloading anything), or only download newly uploaded videos after updating the list file by `get_video_id`.

Download is done with normal (i.e. with no option specified) `youtube-dl`. However, you can by-pass any and any number of arguments to `youtube-dl` by passing them as `<option(s) to youtube-dl>`. These options are appended to the instruction file and, when you in the future execute `ydl`, are automatically read. Exceptionally, when downloading the very first video fails, the options are not appended because the failure may due to an invalid option you've specified. If there was no exception like that, you would be annoyed manually to remove the invalid option from an instruction file.

Usage:
```bash
$ ydl --help
Usage:
  ydl <input file name> [<option(s) to `youtube-dl`>]
```

### `easy_ydl`

`easy_ydl` is just a wrapper of `get_video_id` and `ydl`. Whilst you shall specify an instruction file name in using `get_video_id` or `ydl`, no such an argument is needed to `easy_ydl` since it uses (i.e. creates or modifies) `.ydl_video_id_list.txt` file under a current directory as an instruction file.

Generally, these two sets of commands are equivalent.
```bash
easy_ydl A B C -- D E
```
```bash
file="./.ydl_video_id_list.txt"
get_video_id "${file}" A B C && ydl "${file}" D E
```

In other words, options before a double hyphen `--` are passed to `get_video_id` and those after it are passed to `ydl`. See [Example 2](#example-2) for examples.

Usage:
```bash
$ easy_ydl --help
Usage:
  easy_ydl [<option(s) to `get_video_id`>] [-- <option(s) to `ydl`>]
```

## Instruction File

### About

All commands composing ydl do their work according to an *instruction file*. This file includes these information to name a few.
- Targets the file is tracking.
- Options passed to `get_video_id`.
- Options passed to `youtube-dl`.
- Retrieved video ids.

Except when you execute `easy_ydl` command, an instruction file to which the commands refer is specified as an command-line argument. See [Usage](#usage) about that. Though the file is usually created or modified by the commands, you can do them manually.

### Syntax

Syntax of an instruction file is simple.

- `'<string>` (Example: `'--username Apple`, `'--with-time`)

When you execute `get_video_id`, `<string>` is interpreted as an option in addition to command-line arguments.

- `"<string>` (Example: `"--extract-audio`, `"--audio-format mp3`)

When you execute `ydl`, `<string>` is passed to `youtube-dl` as it is. One useful technique is to specify `--extract-audio` which allows you to get only audio.

- `<video id> <title>` (Example: `4FunXnJQxYU Apple Special Event. October 22, 2013.`)

When you execute `ydl`, a video whose id is `<video id>` is downloaded with its file name set to `<title>.<extension>` where `<extension>` is an appropriate extension like `mp4`, `webm` or `mkv`. Note it is not rare that `<video id>` starts with a hyphen "-". This is the reason why an additional character like a single quote is prepended to store options. In many cases, `<title>` contains spaces.

- `#<video id> <title>` (Example: `#4FunXnJQxYU Apple Special Event. October 22, 2013.`)

This line lets ydl know which video you've already downloaded. When you execute `ydl`, a video whose id is `<video id>` is skipped. When you execute `get_video_id`, the same video id to `<video id>` is not appended to an instruction file and information about the video (e.g. the title) is not retrieved, which makes operations much faster. Do not remove the line with no reason.

## Build

### Tested Environment

The program is tested under

- [Arch Linux](https://www.archlinux.org/)

- [Arch Linux ARM](https://archlinuxarm.org/)

- [Linux Mint 19](https://linuxmint.com/)

- [OS X 10.10 Yosemite](https://en.wikipedia.org/wiki/OS_X_Yosemite)

- Windows 10 with [WSL](https://docs.microsoft.com/en-us/windows/wsl/install-win10) installed

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

3. Change a shebang line if needed.

There exists a [shebang](https://en.wikipedia.org/wiki/Shebang_(Unix)) line in `get_video_id`.
```bash
$ head -n 1 get_video_id
#!/usr/bin/env python3
```

Its contents may be inappropriate for your environment.

- With low probability, `env` command is not located in `/usr/bin`. Check that with `which -a env` command or `type -a env` command. If the location is incorrect, replace `/usr/bin/env` with the correct path.

- With relatively high probability, a name of Python 3 interpreter is not `python3`. In that case, replace `python3` with the correct name (possibly `python`).

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

- `make` fails with the error message "*Makefile:16: *** missing separator.  Stop.*".

You may encounter this problem on macOS. In that case, first execute `brew install make` and then use `gmake` instead of `make`.

- `get_video_id` succeeds but downloading videos through `ydl` always fails.

That may be because `youtube-dl` on your environment is old. Try updating it to a newer version.

- Do I have to prepare *N* list files if I'd like to track *N* channels?

No. It is possible to associate multiple usernames, channel ids or playlist ids with a single list. Just execute `video_id_list` multiple times with different targets but with the same `<output file name>` or, more easily, execute `video_id_list` with two or more options which specify targets (i.e. `--username`, `--channel-id` and so on).

- I'd like to execute ydl on a server. What is a good way?

Say you have a computer and a headless server which can be accessed by the computer via SSH. When a download operation takes more than a day (yes, it often does), you may want to leave it to the server, shutdown the computer and lay down on a bed. In such a situation, these commands are useful.
```bash
$ ssh <host>

$$ get_video_id id.txt --username Apple
Reading an API key from [ ~/.ydl_api_key ]
----- [1/1] Apple (username) start -----
Retrieving the playlist id...
Retrieving video ids...
    [1/7] Done.
    [2/7] Done.
    [3/7] Done.
    [4/7] Done.
    [5/7] Done.
    [6/7] Done.
    [7/7] Done.
------ [1/1] Apple (username) end ------
Opening [ id.txt ]...
Appending 314 new video ids to the file...
Done.

$$ ydl id.txt > ydl_log 2>&1 & #Execute as a background job and redirect all to `ydl_log` file.
[1] 1377

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

## Contribution

Contributions are all welcome.

When you would like to suggest a new feature or have found a bug, if you have a GitHub :octocat: account, please send a pull request or open an issue. If you don't, please send a mail :email: to [takahashi.manatsu@gmail.com](mailto:takahashi.manatsu@gmail.com).

Also, translating this document is very much appreciated.

