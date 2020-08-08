#ifndef	OSD_TEXT_WIDGET_H_
#define	OSD_TEXT_WIDGET_H_

#include <QWidget>
#include <QStringList>

class OSDTextWidget : public QWidget{
	Q_OBJECT
public:
	OSDTextWidget(QWidget * parent = 0);

	void setText(const QString &);
	void setAlignment(Qt::Alignment alignment);
        void setLayoutDirection(Qt::LayoutDirection direction);
	void setLineSpacing(int);
        void setTextColor(bool);

private:
	QStringList m_string;
	Qt::Alignment m_alignment;
        Qt::LayoutDirection m_layoutDirection;
	int m_lineSpacing;
        bool m_mode;


protected:
	void paintEvent(QPaintEvent *);
};


#endif
