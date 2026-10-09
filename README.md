# foosonic

A [fooyin](https://fooyin.org) plugin (named **foosonic**) that browses and streams music from a
Subsonic-compatible server (Subsonic, Navidrome, Airsonic, Gonic, ...).

## Features

- Browse artists → albums → songs in a tree (loaded lazily).
- Add a selection (song, album or artist) to the current playlist.
- Play a selection (replaces the current playlist).
- Token authentication (`u`/`t`/`s`) and JSON responses.
- Server URL and credentials stored in fooyin's settings.

## Build

Requires fooyin development files (installed headers + `FooyinConfig.cmake`)
and Qt 6.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## Install

System-wide (writes to fooyin's plugin directory):

```sh
sudo cmake --install build
```

Or per-user, no root:

```sh
mkdir -p ~/.local/lib/fooyin/plugins
cp build/fyplugin_foosonic.so ~/.local/lib/fooyin/plugins/
```

Restart fooyin, then open **View → Foosonic Browser**.

## Usage

The browser is available in two places:

- **View → Foosonic Browser** — opens it in a window.
- **Layout widget** — add **Foosonic Browser** from the layout editing menu (under *Internet*) to embed
  it in your layout like the library tree.

Expand an artist to load its albums and an album to load its songs. **Drag** artists, albums or songs
onto a playlist to add them, **right-click** for a context menu (Play / Add to Playlist / Play Next /
Add to Queue), or **double-click** a song to play it. The only toolbar button is **Refresh**
(server settings live under **Settings → Foosonic**).

In download-first mode the tracks are added to the playlist immediately; the first track is fetched in
the background and playback starts as soon as it is ready.

## Settings

All configuration lives in fooyin's settings under **Settings → Foosonic** (also reachable via the
**Server...** button in the browser):

- **Server URL / Username / Password** with a **Test Connection** button.
- **Playback mode**:
  - *Download first* — **default**. Tracks play right away (streamed) and are fetched to the local
    cache in the background; each entry is swapped to its cached file once downloaded, which enables
    **seeking** and avoids servers/proxies that cut long-lived streams. Only the current track plus
    the prefetch window are fetched (not the whole selection).
  - *Stream* — never cache; always play the remote URL (experimental/testing; not seekable).
- **Stream while downloading** — **on by default**. Play immediately and switch to the cached file
  at the same position. Turn off to hold playback until the track has finished downloading.
- **Prefetch ahead** — how many upcoming tracks to fetch ahead of playback (default 1).
- **Stream format** (Original / MP3 / Opus / AAC) and **Max bitrate** for transcoding.
- **Download cache folder**, plus **Browse** and **Clear Cache**.
- **Cache size limit** — oldest cached files are evicted once the cache grows past this size
  (default Unlimited).

The browser shows a **Cache** column (✓ cached, ↓ downloading) and a status area with the active
download, its progress, and the current cache count/size.

In download-first mode tracks are added to the playlist immediately. Cached tracks are added as local
files (seekable); uncached tracks are added as remote streams and are promoted to their cached file
in place once downloaded, so playback never has to wait or error. Only the current track and the
prefetch window are downloaded, keeping bandwidth low.

## Notes

- The password is stored in fooyin's settings file in plain text.
- Track sizes are shown in the browser's **Size** column and logged under the `fy.subsonic` category
  (`QT_LOGGING_RULES="fy.subsonic.debug=true"` for per-request detail).
