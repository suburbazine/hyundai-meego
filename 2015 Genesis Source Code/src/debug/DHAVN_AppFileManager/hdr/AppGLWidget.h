#ifndef APPGLWIDGET_H
#define APPGLWIDGET_H

#include <QGLWidget>

// Singleton
class AppGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit AppGLWidget(QWidget *parent = 0);

    static AppGLWidget* instance();
//{added by junam 2013.04.03 for ISV77703
    static AppGLWidget* instanceFront();
    static AppGLWidget* instanceRear();
    static void selectInstance(bool isFront);
//}added by junam

protected:
    void initializeGL();

signals:
    void initialize(QGLWidget*);

public slots:

private:
    static AppGLWidget* m_self;
    //{added by junam 2013.04.03 for ISV77703
    static AppGLWidget* m_front;
    static AppGLWidget* m_rear;
    //}added by junam
};

#endif // APPGLWIDGET_H
