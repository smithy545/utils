//
// Created by Philip on 5/11/2020.
//

#ifndef UTILS_MATH_UTIL_H
#define UTILS_MATH_UTIL_H

#include <glm/glm.hpp>
#include <vector>


namespace utils::math {
    struct rect {
        double x, y, w, h;
    };

    int binomial_coeff(int n, int k);

    double compute_parabola_y(glm::vec2 focus, double directrix_y, double x);

    double compute_parabolic_collision_x(glm::vec2 left, glm::vec2 right, double directrix_y);

    bool in_rect(glm::vec2 p, rect r);

    bool check_overflow(double val);

    bool check_overflow(float val);

    bool on_segment(glm::vec2 p, glm::vec2 q, glm::vec2 r);

    int orientation(glm::vec2 p, glm::vec2 q, glm::vec2 r);

    bool do_intersect(glm::vec2 p1, glm::vec2 q1, glm::vec2 p2, glm::vec2 q2);

    glm::vec2 compute_triangle_circumcenter(glm::vec2 a, glm::vec2 b, glm::vec2 c);

    std::vector<glm::vec2> triangulate(const std::vector<double> &coords);

    /* 2-D utils */
    std::vector<glm::vec2> generate_bezier_curve(std::vector<glm::vec2> control_points, double step_size);

    std::vector<double> generate_parabola(std::vector<double> x, double a = 1, double b = 0, double c = 0);

    /* 3-D utils */
    std::vector<glm::vec3> generate_bezier_curve(std::vector<glm::vec3> control_points, double step_size);

    std::vector<glm::vec3> generate_sphere(double radius, double phi_step, double theta_step);

    std::vector<glm::vec3>
    generate_cylinder(double radius, double length, double length_step, double phi_step);

    std::vector<glm::vec3> generate_box(double width, double height, double length);
    // TODO: std::vector<glm::vec3> generate_ovaloid();

    /* N-D utils */
    std::vector<float> generate_bezier_curve(std::vector<float> control_points, double step_size, int dimension);
    // TODO: int* generate_binomial_matrix(int n, int k);

    // generate bezier curve for given control points

    // via polynomial function
    std::vector<float> bezier_polynomial(std::vector<float> points, double step_size, int dimension);

    // geometrically
    std::vector<float> bezier_deCasteljau(std::vector<float> points, double step_size, int dimension);

    std::vector<float> deCasteljau_kernel(std::vector<float> points, double t, int dimension);
    // via matrix operations
    // TODO: std::vector<float> bezier_matrix(std::vector<float> control_points, double step_size, int dimension);
} // namespace utils::math

#endif //UTILS_MATH_UTIL_H
