/*
 *   Copyright (C) 2008 Ali Shah <caffein@gmail.com>
 *
 *   This file is part of the Qlix project on http://berlios.de
 *
 *   This file may be used under the terms of the GNU General Public
 *   License version 2.0 as published by the Free Software Foundation
 *   and appearing in the file COPYING included in the packaging of
 *   this file.  
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License version 2.0 for more details.
 *
 *   You should have received a copy of the GNU General Public License along
 *   with this program; if not, write to the Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <QtWidgets/QWidget>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QSpacerItem>
#include <QFileInfo>
#include <QtWidgets/QApplication>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QAction>
#include <QtWidgets/QActionGroup>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QDirModel>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QListView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTableView>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QSplitter>
#include <QDir>
#include <QtWidgets/QLabel>
#include "widgets/QlixPreferences.h"
#include <QSortFilterProxyModel>
#include "widgets/QMtpDevice.h"
#include "widgets/AlbumModel.h"
#include "widgets/DirModel.h"
#include "widgets/PlaylistModel.h"
#include "mtp/MtpObject.h"
#include "types.h"

/**
 * @class The DeviceExplorer widget shows two panes one of which will expose
 * the local filesystem, the other will expose the device database. In specific
 * instances it will display the preferences widget
 * and the device management widget. Over all this widget ties all 
 * functionality of Qlix into one main widget. It is the central widget 
 * that is set in QlixMainWindow
 */
class DeviceExplorer : public QWidget
{
Q_OBJECT
public:
  DeviceExplorer(QMtpDevice*, QWidget* parent);
  bool QueueState();
  void SetProgressBar(QProgressBar*);

public slots:
  void ShowFiles();
  void ShowAlbums();
  void ShowPlaylists();
  void ShowPreferences();
  void ShowDeviceManager();
  void ShowQueue(bool);
  void UpdateProgressBar(const QString&, count_t percent);
  void Beep(MTP::Track* in_track);

signals:

private slots:
  void TransferToDevice();
  void TransferFromDevice();
  void DeleteFromDevice();
  void SwitchFilesystemDir(const QModelIndex&);

private:
  enum ViewPort
  {
    AlbumsView,
    PlaylistView,
    FileView
  };
  void setupToolBars();
  void setupMenus();
  void setupDeviceView();
  void setupFileSystemView();
  void setupProgressBar();

  void setupAlbumTools();
  void setupPlaylistTools();
  void setupFileTools();
  void setupCommonTools();

  void setupConnections();
  void showAlbumTools();

  void showGeneralTools();
  void hideGeneralTools();

  void showPlaylistTools();
  void showFileTools();
  void hideAlbumTools();
  void hidePlaylistTools();
  void hideFileTools();

  void updateDeviceSpace();
  void clearActions();

  bool confirmDeletion();
  bool confirmContainerDeletion(MTP::GenericObject*);

//  QModelIndexList removeAlbumDuplicates(const QModelIndexList&);
  QModelIndexList removeIndexDuplicates(const QModelIndexList&,
                                        const QAbstractItemModel*);
  QGridLayout* _layout; 
  ViewPort _view;

//Filesystem representation
  QListView* _fsView;
  QDirModel* _fsModel;

//Device representations
  QMtpDevice* _device;
  QHeaderView* _horizontalHeader;

//Queue representation
  QListView* _queueView;

  QTreeView* _deviceView;
  QSortFilterProxyModel* _albumModel;
  AlbumModel* _unsortedAlbumModel;
  QSortFilterProxyModel* _plModel;
  QSortFilterProxyModel* _dirModel;

  QSplitter* _fsDeviceSplit;
  QSplitter* _queueSplit;
//Device manager and preferences (for now there are just labels
  QlixPreferences* _preferencesWidget;
  QLabel* _deviceManagerWidget;
//Used to figure out if we are displaying the preferences or devicemanager
//widget
  bool _otherWidgetShown;
  bool _queueShown;

  //Track toolbar and actions
  QToolBar* _tools;
  QSpacerItem* _progressBarSpacer;
  //Progress bar for space usage and transfers
  QProgressBar* _progressBar;



  //Playlist Actions
  QAction* _newPlaylist;
  QAction* _addToPlaylist;
  QAction* _showDeviceTracks;
  QAction* _showFSTracks;
  QAction* _deletePlaylist;
  QAction* _deleteFromPlaylist;
  QVector <QAction*> _playlistActionList;

  
  //Queue Actions
  QAction* _viewQueue;
  QAction* _hideQueue;

  //common to all fs views
  QActionGroup* _commonFSActions;
  QAction* _transferToDevice;

  //common to all objects
  QActionGroup* _commonDeviceActions;
  QAction* _transferFromDevice;
  QAction* _addToQueue;
  QAction* _sync;
  QAction* _deleteSeperator;
  QAction* _delete;

  //File Actions
  QAction* _newFolder;
  QVector <QAction*> _fileActionList;

};

