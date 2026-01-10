#ifndef SETTINGSOVERLAY_H
#define SETTINGSOVERLAY_H

#include <QWidget>

class SettingsOverlay : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsOverlay(QWidget* parent = nullptr);

    signals:
        void backPressed();
};

#endif
