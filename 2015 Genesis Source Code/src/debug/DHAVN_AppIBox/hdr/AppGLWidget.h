#ifndef APPGLWIDGET_H
#define APPGLWIDGET_H

#include <QtOpenGL/QGLWidget>

// Singleton
class AppGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit AppGLWidget(QWidget *parent = 0);

    static AppGLWidget* instance();
    static void release();

protected:
    void initializeGL();

signals:
    void initialize(QGLWidget*);

public slots:

private:
    static AppGLWidget* m_self;
};

#endif // APPGLWIDGET_H
