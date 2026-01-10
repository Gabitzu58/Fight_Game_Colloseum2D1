#pragma once

#include <QMediaPlayer>
#include <QAudioOutput>
#include <QObject>

class SoundManager : public QObject
{
    Q_OBJECT

public:
    static SoundManager& instance();

    void playMusic();
    void stopMusic();
    void setVolume(float v);

private:
    explicit SoundManager(QObject* parent = nullptr);

    QMediaPlayer* music;
    QAudioOutput* audio;
};
