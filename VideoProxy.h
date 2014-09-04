/*
 *   Copyright 2014, Christian Muehlhaeuser <muesli@gmail.com>
 */

#ifndef VIDEOPROXY_H
#define VIDEOPROXY_H

class QPropertyAnimation;

#include <QFileInfo>
#include <QGraphicsProxyWidget>

#include <phonon/AudioOutput>
#include <phonon/MediaObject>

class VideoProxy : public QGraphicsProxyWidget
{
    Q_OBJECT
public:
    VideoProxy( QGraphicsItem* parent = 0, Qt::WindowFlags wFlags = 0 );

    QRectF boundingRect() const;

public slots:
    void loadMedia( const QFileInfo& info );

    void animatedResize( const QRect& finRect );

signals:
    void requestFullPlayback();
    void requestFocus();
    void forward();
    void rewind();

protected:
    void hoverEnterEvent( QGraphicsSceneHoverEvent* event );
    void hoverLeaveEvent( QGraphicsSceneHoverEvent* event );

    void mouseDoubleClickEvent( QGraphicsSceneMouseEvent* event );
    void mousePressEvent( QGraphicsSceneMouseEvent* event );
    void mouseReleaseEvent( QGraphicsSceneMouseEvent* event );

    void wheelEvent( QGraphicsSceneWheelEvent* event );

private slots:
    void resetZValue();

private:
    Phonon::MediaObject* m_media;
    Phonon::AudioOutput* m_audio;

    QPropertyAnimation* m_animation;
};

#endif
