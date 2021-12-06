//
// Created by Philip on 5/11/2020.
//

#ifndef UTILS_MATH_UTIL_H
#define UTILS_MATH_UTIL_H

#include <bullet/LinearMath/btVector3.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Quadtree.h>
#include <fmt/format.h>
#include <functional>
#include <glm/glm.hpp>
#include <string>
#include <vector>


namespace utils::math {
	typedef CGAL::Simple_cartesian<double> Kernel;
	typedef Kernel::Point_2 Point_2;
	typedef CGAL::Quadtree<Kernel, std::vector<Point_2>> Quadtree;

    struct rect {
        double x, y, w, h;
    };

    // Convenience wrapper for quadtree
    class PointFinder {
    public:
    	explicit PointFinder(std::vector<Point_2> sites);

    	explicit PointFinder(std::vector<double> coords);

    	Point_2 operator[](const Point_2& p) const;

    	Point_2 find_closest(const Point_2& p) const;

    	[[nodiscard]] const Quadtree& get_quadtree() const;

    	[[nodiscard]] const std::vector<Point_2>& get_points() const;
    private:
    	std::vector<Point_2> m_points{};
    	Quadtree m_collision_tree;

    	static std::vector<Point_2> convert_to_point_2(std::vector<double> coords);
    };

    int binomial_coeff(int n, int k);

    double compute_parabola_y(glm::vec2 focus, double directrix_y, double x);

    double compute_parabolic_collision_x(glm::vec2 left, glm::vec2 right, double directrix_y);

    glm::vec3 bt2glm(const btVector3& vec);

    btVector3 glm2bt(const glm::vec3& vec);

    bool in_rect(glm::vec2 p, rect r);

    bool check_overflow(double val);

    bool check_overflow(float val);

    bool on_segment(glm::vec2 p, glm::vec2 q, glm::vec2 r);

    int orientation(glm::vec2 p, glm::vec2 q, glm::vec2 r);

    bool do_intersect(glm::vec2 p1, glm::vec2 q1, glm::vec2 p2, glm::vec2 q2);

    glm::vec2 compute_triangle_circumcenter(glm::vec2 a, glm::vec2 b, glm::vec2 c);

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

namespace std {
	template <>
	struct hash<glm::vec2> {
		size_t operator()(const glm::vec2& p) const {
			return hash<std::string>()(fmt::format("{}:{}", p.x, p.y));
		}
	};

	template <>
	struct hash<glm::vec3> {
		size_t operator()(const glm::vec3& p) const {
			return hash<std::string>()(fmt::format("{}:{}:{}", p.x, p.y, p.z));
		}
	};

	template <>
	struct hash<utils::math::Point_2> {
		size_t operator()(const utils::math::Point_2& p) const {
			return hash<std::string>()(fmt::format("{}:{}", p.x(), p.y()));
		}
	};
} // namespace std

#endif //UTILS_MATH_UTIL_H
