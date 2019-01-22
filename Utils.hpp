//
// Created by lirfu on 18.01.19..
//

#ifndef PROJEKT_UTILS_HPP
#define PROJEKT_UTILS_HPP

#import <GL/glut.h>
#include <eigen3/Dense>
#include <cfloat>

struct fac {
    std::vector<uint> vertices;

    uint operator[](uint index) const {
        return vertices[index];
    }
};

typedef struct bound {
    GLdouble minx = DBL_MAX, maxx = DBL_MIN, miny = DBL_MAX, maxy = DBL_MIN, minz = DBL_MAX, maxz = DBL_MIN;
} bound;


namespace racgra {
    GLuint window_;
    GLuint win_width_ = 600, win_height_ = 600, win_x_ = 100, win_y_ = 100;
    Eigen::Vector3f camera_(1.0, 1.2, 1.4);
    Eigen::Vector3f lookat_(0.0, 0.0, 0.0);
    Eigen::Vector3f lookup_(0.0, 0.0, 1.0);
    double angle_ = 0;
    double fovy_ = 1.0;
    GLdouble near_ = 0.5, far_ = 20.0;
    bool wire_ = true;
    double amount_ = 1;

    void reshape(int w, int h) {
        win_width_ = (uint) w;
        win_height_ = (uint) h;
        glViewport(0, 0, win_width_, win_height_);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glClearColor(0.89f, 0.87f, 0.86f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        gluPerspective(fovy_, (float) win_width_ / win_height_, near_, far_);
        gluLookAt(camera_[0], camera_[1], camera_[2],
                  lookat_[0], lookat_[1], lookat_[2],
                  lookup_[0], lookup_[1], lookup_[2]);
        glRotated(angle_, lookup_[0], lookup_[1], lookup_[2]);
    }

    void redisplay_all(void) {
        glutSetWindow(window_);
        reshape(win_width_, win_height_);
        glutPostRedisplay();
    }

    void zoom(bool in, double amount) {
        Eigen::Vector3f dist = lookat_ - camera_;
        // Movement vector.
        Eigen::Vector3f v(dist);
        v.normalize();
        v *= amount;
        // End if movement is larger than distance.
        dist -= v;
        if (in && dist.norm() < 0.1) return;
        // Move.
        in ? camera_ += v : camera_ -= v;
    }

    void draw_origin() {
        glPushMatrix();
        glLoadIdentity();
        // X
        glLineWidth(3);
        glBegin(GL_LINES);
        glColor3f(1, 0, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(1, 0, 0);
        // Y
        glColor3f(0, 1, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 1, 0);
        // Z
        glColor3f(0, 0, 1);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, 1);
        glEnd();
        glLineWidth(1);
        glPopMatrix();
    }

    void camera_control(char c, double amount) {
        switch (c) {
            // Z-axis
            case 'w':
                camera_[2] += amount;
                break;
            case 's':
                camera_[2] -= amount;
                break;
                // X-axis
            case 'a':
                camera_[0] += amount;
                break;
            case 'd':
                camera_[0] -= amount;
                break;
                // Y-axis
            case 'q':
                camera_[1] += amount;
                break;
            case 'e':
                camera_[1] -= amount;
                break;
                // FOVY
            case 'u':
                fovy_ -= 10 * amount;
                break;
            case 'o':
                fovy_ += 10 * amount;
                break;
                // Center zoom
            case 'k':
                zoom(false, amount_);
                break;
            case 'i':
                zoom(true, amount_);
                break;
                // Rotate around center.
            case 'l':
                angle_ += amount;
                break;
            case 'j':
                angle_ -= amount;
                break;
                // Wire or some polygon view.
            case 'x':
                wire_ = !wire_;
                break;
            default:
                return;
        }
        racgra::redisplay_all();
    }

    void mouse_wheel(int button, int dir, int x, int y) {
        switch (button) {
            case GLUT_LEFT_BUTTON:
                angle_ -= amount_;
                break;
            case GLUT_RIGHT_BUTTON:
                angle_ += amount_;
                break;
            case 3:
                zoom(true, amount_);
                break;
            case 4:
                zoom(false, amount_);
                break;
        }
    }
};


#endif //PROJEKT_UTILS_HPP
