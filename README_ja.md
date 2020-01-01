# *ydl*, a front end for `youtube-dl`

この文書は、オリジナルの[README.md](./README.md)(英語)の日本語訳です。特に理由が無ければ、オリジナルを参照することをおすすめします。

## はじめに

### *ydl*とは

*ydl*は、[`youtube-dl`](https://ytdl-org.github.io/youtube-dl/index.html)のフロントエンドです。ydlは二つのプログラム`get_video_id`、`ydl`から構成されます。

`get_video_id`はユーザー名、チャンネルID、もしくはプレイリストIDに紐付けられたビデオIDのリストを構築し、それを指定ファイルに書き込みます。それぞれのビデオIDは、ファイル中に見つからない場合に限り、そのファイルに追記されます。よって、同じ引数を与えて過去に`get_video_id`を呼び出していれば、そのファイルを*更新*することができます。

`ydl`は、指定ファイルを読んでビデオIDのリストを構築し、そのリストに従って`youtube-dl`にビデオをダウンロードさせます。ビデオが正常にダウンロードされた場合、対応するビデオIDの先頭には番号記号`#`が付されます。これは「既にそのビデオをダウンロードした」ことを表します。これにより、(何を再ダウンロードすることもなく)操作の一時中断・再開をしたり、`get_video_id`によるリストファイルの更新を通して新たにアップロードされたビデオだけをダウンロードすることが可能になります。

### 例1<a name="example-1"></a>

[*Appleチャンネル*](https://www.youtube.com/user/Apple/videos)にアップロードされた動画を追跡したいものとします。

1. ディレクトリを生成し、そこへ移動する。
```bash
$ mkdir Apple_video
$ cd Apple_video/
```

2. ビデオIDを取得し、それをファイルに書き込む。

チャンネルのURLが示すように、チャンネルのユーザー名は`Apple`です。
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

3. ファイルに従って、ビデオをダウンロードする。
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

いま、Ctrl+cを押下して(つまりフォアグラウンドプロセスグループに`SIGINT`を送信して)、二つ目のビデオをダウンロードしている最中に操作をキャンセルしました。一つ目のビデオは正常にダウンロードされているため、そのビデオIDは`#`によりコメントアウトされます。
```bash
$ head -n 3 video_id_list.txt 
#yBX-KpMoxYk Apple Special Event. September 10, 2013._2013-10-09T04:33:09.000Z
IbWOQWw1wkM Apple - Making the Mac Pro_2013-10-23T17:36:21.000Z
4FunXnJQxYU Apple Special Event. October 22, 2013._2013-10-23T21:09:58.000Z
```

よって、再度`ydl`コマンドを実行したとき、操作は二つ目のビデオから開始します。
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

4. リストを更新し、新たにアップロードされた動画を取得する。

前の手順から一月が経ち、それ以降に新たにアップロードされたビデオを取得したくなりました。必要なコマンドは、前回と全く変わりません。
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

### 例2<a name="example-2"></a>

`get_video_id`と`ydl`の組み合わせを使うことは十分に簡単ですが、二つのコマンドを入力したり、良いファイル名を考えたり、あるいはそれを手動で引数として渡すことは、ときに面倒に思えるかも知れません。そんなとき、もしリストに紐付けられたファイル名を決して気にしないのならば、単に`easy_ydl`コマンドを使うことができます。[用法](#usage)で説明するように、
```bash
$ easy_ydl --username Apple
```

は次のコマンドと等価です。
```bash
$ file="./.ydl_video_id_list.txt"
$ get_video_id --username Apple "${file}" && ydl "${file}"
```

## 用法<a name="usage"></a>

例については[例1](#example-1)および[例2](#example-2)を参照してください。

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

このコマンドは、次のように実装されています。
```bash
$ cat easy_ydl
#!/usr/bin/env sh
file="./.ydl_video_id_list.txt"
get_video_id "$1" "$2" "${file}" && ydl "${file}"
```

## インストール

このプログラムは、以下の環境下でテストされています。

- [Arch Linux](https://www.archlinux.org/)

- [Arch Linux ARM](https://archlinuxarm.org/)

- [Linux Mint 19](https://linuxmint.com/)

### 要件

- POSIX互換のシェル(例えば[bash](https://www.gnu.org/software/bash/))

- [GNU Make](https://www.gnu.org/software/make/)

- [g++](https://gcc.gnu.org/)

- [Python 3.x](https://www.python.org/)

- [youtube-dl](https://ytdl-org.github.io/youtube-dl/index.html)

### 準備

1. [*YouTube Data API*](https://developers.google.com/youtube)のAPIキーを取得する。.

ydlを使用するには、YouTube DATA APIのAPIキーが必要です。これは無料で簡単に取得できます。簡潔な説明が[こちら](https://developers.google.com/youtube/v3/getting-started#before-you-start)から見られます。キー(例えば`AIpaSyCaXPx0utk8HmM4a8PbYp_qeu7wOknGT3U`)を取得したら、それをホームディレクトリ以下の`.ydl_api_key`ファイルに格納してください。

```bash
$ echo "AIpaSyCaXPx0utk8HmM4a8PbYp_qeu7wOknGT3U" > ~/.ydl_api_key
```

2. このプロジェクトをダウンロードする。

`git clone <URL of this page>`コマンドを使うか、このページのダウンロードボタンを押してください。

3. 必要であれば、シェバン行を書き換える

`get_video_id`と`easy_ydl`には、[シェバン](https://en.wikipedia.org/wiki/Shebang_(Unix))行があります。
```bash
$ head -n 1 get_video_id easy_ydl
==> get_video_id <==
#!/usr/bin/env python3

==> easy_ydl <==
#!/usr/bin/env sh
```

環境によっては、その内容が不適切な可能性があります。
- 低い確率で、`env`コマンドは`/usr/bin`に位置していません。その確認には、`which -a env`コマンドまたは`type -a env`コマンドを用いてください。位置が誤っていれば、`/usr/bin/env`を正しいパスで置き換えてください。

- 十分に高い確率で、Python 3インタープリタの名前は`python3`ではありません。その場合、`python3`を正しい名前(もしかすると`python`)に置き換えてください。


### インストール

- インストール
```bash
$ make && make install
```

- アンインストール
```bash
$ make uninstall
```

## Q&A

- *N*個のチャンネルを追跡するために、*N*個のリストファイルを用意する必要がありますか。

いいえ。複数のユーザー名、チャンネルID、ないしプレイリストIDを、単一のリストに結びつけることができます。異なる`<playlist spec>`を与えて、しかし同一の`<output file name>`と共に、`video_id_list`を複数回呼び出してください。

- サーバー上でydlを実行したいのですが、何か良い方法がありますか。

コンピューターと、そこからSSHによりアクセスできるヘッドレスサーバーを持っているものとします。ダウンロード操作が一日以上掛かるとき(これは珍しいことではありません)、操作をサーバーに任せ、コンピューターをシャットダウンし、ベッドに身を休めたいと思うかも知れません。そんなとき、以下のコマンドが便利です。
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

$$ ydl video_id_list.txt > ydl_log 2>&1 & #バックグランドジョブとして実行し、全てを`ydl_log`ファイルにリダイレクトする
[1] 24535

$$ disown #これで、操作を終了させることなく、サーバーからログアウトできるようになる
```

進捗を見るには、
```bash
$ ssh <host> #再ログイン

$$ tail -f ydl_log
```

操作をキャンセルするには、
```bash
$ ssh <host> #再ログイン

$$ ps -eH | grep -A 1 ydl #プロセスIDを見つける

$$ kill -SIGINT 24742 #(`ydl`ではなく)`youtube-dl`をキルする
```

