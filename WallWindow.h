/*
 *   Copyright 2014, Christian Muehlhaeuser <muesli@gmail.com>
 */

#ifndef WALLWINDOW_H
#define WALLWINDOW_H

#include <QFileInfoList>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "VideoProxy.h"

class WallWindow : public QGraphicsView
{
    Q_OBJECT
public:
    WallWindow();

public slots:
    void loadMedia( const QFileInfoList& files );

protected:
    void resizeEvent( QResizeEvent* event );

private slots:
    void onFocusRequested();
    void onFullPlaybackRequested();

    void layoutItems( bool touchZValue = true );

private:
    QGraphicsScene m_scene;
    QList<VideoProxy*> m_videos;
};

#endif
