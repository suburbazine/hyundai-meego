#ifndef	UPDATE_PREPARATION_H_
#define	UPDATE_PREPARATION_H_

#include <QObject>

class UpdatePreparation : public QObject{
Q_OBJECT
public:
	UpdatePreparation(QObject * obj = 0);

	void tryToKillApplications();
	void sendUpdateInfoToMicom();
};

#endif
