//
// Created by Philip on 5/11/2020.
//

#include "math_util.h"

#include <utility>


namespace utils::math {
    std::vector<int *> generated_binomial_coeffs;
    int max_generated_degree = 0;

    int binomial_coeff(int n, int k) {
        while (n >= max_generated_degree) {
            int s = max_generated_degree;
            int *coeffs = new int[s + 1];
            coeffs[0] = 1;
            for (int i = 1, prev = s - 1; i < s; i++)
                coeffs[i] = generated_binomial_coeffs[prev][i - 1] + generated_binomial_coeffs[prev][i];
            coeffs[s] = 1;
            generated_binomial_coeffs.push_back(coeffs);
            max_generated_degree++;
            //delete[] coeffs;
        }

        return generated_binomial_coeffs[n][k];
    }

    std::vector<glm::vec2> generate_bezier_curve(std::vector<glm::vec2> control_points, double step_size) {
        if (control_points.size() <= 2) {
            return control_points;
        }

        std::vector<float> points;
        for (auto p: control_points) {
            points.push_back(p.x);
            points.push_back(p.y);
        }

        std::vector<float> curve = generate_bezier_curve(points, step_size, 2);

        std::vector<glm::vec2> returnPoints;
        for (int i = 0; i < curve.size() - 1; i += 2)
            returnPoints.emplace_back(curve[i], curve[i + 1]);

        return returnPoints;
    }

    std::vector<glm::vec3> generate_bezier_curve(std::vector<glm::vec3> control_points, double step_size) {
        if (control_points.size() <= 2) {
            return control_points;
        }

        auto points = std::vector<float>();
        for (auto p: control_points) {
            points.push_back(p.x);
            points.push_back(p.y);
            points.push_back(p.z);
        }

        std::vector<float> curve = generate_bezier_curve(points, step_size, 3);

        std::vector<glm::vec3> returnPoints;
        for (int i = 0; i < curve.size() - 2; i += 3)
            returnPoints.emplace_back(curve[i], curve[i + 1], curve[i + 2]);

        return returnPoints;
    }

    std::vector<float> generate_bezier_curve(std::vector<float> control_points, double step_size, int dimension) {
        if (control_points.size() <= 2) {
            return control_points;
        }

#ifdef BEZIER_POLYNOMIAL
        return bezierPolynomial(std::move(controlPoints), stepSize, dimension);
#else
        return bezier_deCasteljau(std::move(control_points), step_size, dimension);
#endif
    }

    std::vector<float> bezier_polynomial(std::vector<float> points, double step_size, int dimension) {
        // add first point to curve
        std::vector<float> curve;
        curve.insert(std::end(curve), std::begin(points), std::begin(points) + dimension);
        int n = points.size() / dimension - 1;
        double t = step_size;

        while (t < 1.0) {
            // setup output vector
            std::vector<float> v;
            v.reserve(dimension);
            for (int i = 0; i < dimension; i++)
                v.push_back(0);

            double subT = 1.0 - t;
            for (int k = 0; k <= n; k++) {
                int subK = n - k;
                double coeff = binomial_coeff(n, k) * std::pow(subT, subK) * std::pow(t, k);
                // round to zero at threshold=0.001
                coeff = coeff > 0.001 ? coeff : 0;
                for (int i = 0; i < dimension; i++)
                    v[i] += points[k * dimension + i] * coeff;
            }

            curve.insert(std::end(curve), std::begin(v), std::end(v));
            t += step_size;
        }

        // add last point to curve
        curve.insert(std::end(curve), std::end(points) - dimension, std::end(points));

        return curve;
    }

    std::vector<float> bezier_deCasteljau(std::vector<float> points, double step_size, int dimension) {
        // add first point to curve
        std::vector<float> curve;
        curve.insert(std::end(curve), std::begin(points), std::begin(points) + dimension);
        double t = step_size;
        while (t < 1.0) {
            auto subCurve = deCasteljau_kernel(points, t, dimension);
            curve.insert(std::end(curve), std::begin(subCurve), std::end(subCurve));
            t += step_size;
        }

        return curve;
    }

    std::vector<float> deCasteljau_kernel(std::vector<float> points, double t, int dimension) {
        if (points.size() <= dimension)
            return points;

        std::vector<float> curve;
        for (int i = 0; i < points.size() - dimension; i += dimension) {
            for (int j = 0; j < dimension; j++)
                curve.push_back((1 - t) * points[i + j] + t * points[i + dimension + j]);
        }
        return deCasteljau_kernel(curve, t, dimension);
    }

    std::vector<glm::vec3> generate_sphere(double radius, double phi_step, double theta_step) {
        auto surfacePoints = std::vector<glm::vec3>();
        for (double phi = 0.0; phi <= 0.5 * 3.14159265358979; phi += phi_step) {
            for (double theta = 0.0; theta < 3.14159265358979; theta += theta_step) {
                surfacePoints.emplace_back(radius * glm::sin(theta) * glm::cos(phi),
                                           radius * glm::sin(theta) * glm::sin(phi), radius * glm::cos(theta));
            }
        }

        return surfacePoints;
    }

    std::vector<glm::vec3> generate_cylinder(double radius, double length, double length_step, double phi_step) {
        auto surfacePoints = std::vector<glm::vec3>();
        for (double z = 0.0; z <= length; z += length_step) {
            for (double phi = 0.0; phi < 3.14159265358979; phi += phi_step) {
                surfacePoints.emplace_back(z * glm::cos(phi), z * glm::sin(phi), z);
            }
        }
        return surfacePoints;
    }

    std::vector<glm::vec3> generate_box(double width, double height, double length) {
        return {
                {0, 0, 0},
                {width, 0, 0},
                {width, height, 0},
                {0, height, 0},
                {0, 0, length},
                {width, 0, length},
                {width, height, length},
                {0, height, length}
        };
    }

    std::vector<double> generate_parabola(std::vector<double> x, double a, double b, double c) {
        std::vector<double> y;
        for (auto i: x)
            y.push_back(a * i * i + b * i + c);
        return y;
    }

    double compute_parabola_x(glm::vec2 focus, double directrix_y, double y, bool right_zero) {
        if(focus.y == directrix_y)
            return focus.x;
        auto root = glm::sqrt(
                2 * y * focus.y - 2 * y * directrix_y - focus.y * focus.y + directrix_y * directrix_y);
        return right_zero ? focus.x + root : focus.x - root;
    }

    double compute_parabola_y(glm::vec2 focus, double directrix_y, double x) {
        return 0.5 * (glm::pow(x - focus.x, 2) / (focus.y - directrix_y) + (focus.y + directrix_y));
    }

    double compute_parabolic_collision_x(glm::vec2 focus1, glm::vec2 focus2, double directrix_y) {
        double x1 = focus1.x, y1 = focus1.y, x2 = focus2.x, y2 = focus2.y;
        double d1 = 1.0 / (2.0 * (directrix_y - y1));
        double d2 = 1.0 / (2.0 * (directrix_y - y2));
        double a = d1 - d2;
        double b = 2.0 * (x1 * d1 - x2 * d2);
        double c = (y1 * y1 + x1 * x1 - directrix_y * directrix_y) * d1 -
                   (y2 * y2 + x2 * x2 - directrix_y * directrix_y) * d2;
        double delta = b * b - 4.0 * a * c;
        return (b - std::sqrt(delta)) / (2.0 * a);
    }

    double compute_parabola_slope(glm::vec2 focus, double directrix_y, double x) {
        if(focus.y == directrix_y)
            return std::numeric_limits<double>::infinity();
        return (x - focus.x) / (focus.y - directrix_y);
    }

    glm::vec2 compute_line_collision(glm::vec2 origin1, double slope1, glm::vec2 origin2, double slope2) {
        assert(slope1 != slope2);
        if(slope1 == std::numeric_limits<double>::infinity())
            return glm::vec2(origin1.x, origin2.y + (origin1.x - origin2.x) * slope2);
        if(slope2 == std::numeric_limits<double>::infinity())
            return glm::vec2(origin2.x, origin1.y + (origin2.x - origin1.x) * slope1);
        auto x = (slope1 * origin1.x - slope2 * origin2.x + origin2.y - origin1.y) / (slope1 - slope2);
        auto y = slope1 * (x - origin1.x) + origin1.y;
        return glm::vec2(x, y);
    }
} // namespace utils::math
