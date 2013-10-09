#ifndef VIEDOPLAYER_H
#define VIEDOPLAYER_H

#include <QtGui/QMainWindow>
#include "ui_viedoplayer.h"

class ViedoPlayer : public QMainWindow
{
	Q_OBJECT

public:
	ViedoPlayer(QWidget *parent = 0, Qt::WFlags flags = 0);
	~ViedoPlayer();

private:
	Ui::ViedoPlayerClass ui;
};

#endif // VIEDOPLAYER_H
