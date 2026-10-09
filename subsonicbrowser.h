/*
 * Fooyin Subsonic plugin
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <core/track.h>

#include <QJsonObject>
#include <QList>
#include <QPoint>
#include <QWidget>

#include <functional>
#include <map>
#include <memory>
#include <set>
#include <vector>

class QLabel;
class QTreeWidget;
class QTreeWidgetItem;

namespace Fooyin {
class NetworkAccessManager;
class PlaylistInteractor;
class SettingsManager;

namespace Subsonic {
class SubsonicClient;
class SubsonicDownloadManager;

enum class BrowserAction : uint8_t
{
    Play = 0,
    AddToPlaylist,
    PlayNext,
    AddToQueue,
};

/*!
 * The Subsonic artist/album/song browser, usable in a dialog or embedded in a layout widget.
 */
class SubsonicBrowser : public QWidget
{
    Q_OBJECT

public:
    SubsonicBrowser(std::shared_ptr<NetworkAccessManager> network, PlaylistInteractor* interactor,
                    SubsonicDownloadManager* downloads, SettingsManager* settings, QWidget* parent = nullptr);

    void refresh();
    void openSettings();

private:
    struct GatherState
    {
        QList<QJsonObject> songs;
        qint64 bytes{0};
        int pending{0};
        bool finished{false};
        BrowserAction action{BrowserAction::Play};
    };

    void onExpanded(QTreeWidgetItem* item);
    void ensureChildrenLoaded(QTreeWidgetItem* item, bool expand = true);
    [[nodiscard]] TrackList tracksForDrag(const QList<QTreeWidgetItem*>& items);
    void showContextMenu(const QPoint& pos);
    void playSelection(BrowserAction action);

    void loadChildren(QTreeWidgetItem* item, int kind, const QString& id, std::function<void()> onDone = {},
                      bool expand = true);
    void populateChildren(QTreeWidgetItem* item, int kind, const QJsonObject& object, bool expand);
    void addPlaceholder(QTreeWidgetItem* item);
    void gatherTracks(const QList<QTreeWidgetItem*>& items, const std::shared_ptr<GatherState>& state);
    void finishGather(const std::shared_ptr<GatherState>& state);
    void downloadAndPlay(const std::shared_ptr<GatherState>& state);
    void applyAction(const TrackList& tracks, BrowserAction action, bool startPlayback);
    [[nodiscard]] Track trackForSong(const QJsonObject& song, const QString& filepath) const;
    [[nodiscard]] QString cacheText(const QString& songId) const;
    [[nodiscard]] QTreeWidgetItem* findSongItem(const QString& songId) const;
    void updateCacheColumn(const QString& songId);
    void setStatus(const QString& text);

    std::shared_ptr<NetworkAccessManager> m_network;
    PlaylistInteractor* m_interactor;
    SubsonicDownloadManager* m_downloads;
    SettingsManager* m_settings;
    SubsonicClient* m_client;
    QLabel* m_status;
    QTreeWidget* m_tree;
    std::map<QTreeWidgetItem*, std::vector<std::function<void()>>> m_pendingLoads;
    std::set<QTreeWidgetItem*> m_loading;
    int m_generation{0};
};
} // namespace Subsonic
} // namespace Fooyin
