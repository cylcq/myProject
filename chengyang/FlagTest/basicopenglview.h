#ifndef BASICOPENGLVIEW_H
#define BASICOPENGLVIEW_H

#include <QGLWidget>
#include <QMatrix4x4>

class BasicOpenGLView : public QGLWidget
{
    Q_OBJECT
public:
    explicit BasicOpenGLView(QWidget *parent = 0);
    void resizeCloth();
	void loadTexture();
    
signals:
    
public slots:

    void paintGL();
    void animateGL();

protected:
    void initializeGL();
    void resizeGL(int w, int h);

private:
	GLuint texture[1];
    QMatrix4x4 projectionMatrix;
    QMatrix4x4 viewMatrix;
    
};

#endif // BASICOPENGLVIEW_H
