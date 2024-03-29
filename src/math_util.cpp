/* Created by Philip on 5/11/2020.
MIT License

Copyright (c) 2021 Philip Arturo Smith

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#include <boost/iterator/function_output_iterator.hpp>
#include <random>
#include <utility>
#include <utils/math_util.h>


namespace utils::math {

using Kernel = CGAL::Simple_cartesian<double>;
using TreeTraits = CGAL::Search_traits_2 <Kernel>;
using  Neighbor_search = CGAL::Orthogonal_k_neighbor_search <TreeTraits>;

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
        delete[] coeffs;
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

double compute_parabola_y(glm::vec2 focus, double directrix_y, double x) {
    return 0.5 * (glm::pow(x - focus.x, 2) / (focus.y - directrix_y) + (focus.y + directrix_y));
}

double compute_parabolic_collision_x(glm::vec2 left, glm::vec2 right, double directrix_y) {
    // ORDER OF PARAMS MATTERS
    double x1 = right.x, y1 = right.y, x2 = left.x, y2 = left.y;
    if(y1 == directrix_y) {
        assert(y2 != directrix_y);
        return x1;
    } else if(y2 == directrix_y)
        return x2;
    else if(y1 == y2)
        return (x1 + x2)/2.0;
    double d1 = 1.0 / (2.0 * (directrix_y - y1));
    double d2 = 1.0 / (2.0 * (directrix_y - y2));
    double a = d1 - d2;
    double b = 2.0 * (x1 * d1 - x2 * d2);
    double c = (y1 * y1 + x1 * x1 - directrix_y * directrix_y) * d1 -
               (y2 * y2 + x2 * x2 - directrix_y * directrix_y) * d2;
    double delta = b * b - 4.0 * a * c;
    return (b - std::sqrt(delta)) / (2.0 * a);
}

std::vector<Point_2> query_closest(const std::vector<Point_2>& points, const Point_2& query, const std::size_t n) {
	Point_2 target{query};
	Neighbor_search::Tree tree(points.begin(), points.end());
	Neighbor_search search(tree, query, n);
	std::vector<Point_2> results;
	for(auto hit: search)
		results.push_back(hit.first);
	return results;
}

bool in_rect(glm::vec2 p, rect r) {
    return p.x >= r.x
       && p.y >= r.y
       && p.x < r.x + r.w
       && p.y < r.y + r.h;
}

bool in_rect(double x, double y, rect r) {
	return x >= r.x
       && y >= r.y
       && x < r.x + r.w
       && y < r.y + r.h;
}

bool in_rect(Point_2 p, rect r) {
	return p.x() >= r.x
       && p.y() >= r.y
       && p.x() < r.x + r.w
       && p.y() < r.y + r.h;
}

bool in_bounds(Point_2 p, bounds b) {
	return p.x() >= b.top_left.x
       && p.y() >= b.top_left.y
       && p.x() < b.bottom_right.x
       && p.y() < b.bottom_right.y;
}

bool in_bounds(double x, double y, bounds b) {
	return x >= b.top_left.x
       && y >= b.top_left.y
       && x < b.bottom_right.x
       && y < b.bottom_right.y;
}

bool in_bounds(glm::vec2 p, bounds b) {
	return p.x >= b.top_left.x
       && p.y >= b.top_left.y
       && p.x < b.bottom_right.x
       && p.y < b.bottom_right.y;
}

bool check_overflow(double val) {
    return (val + 1.0) == val;
}

bool check_overflow(float val) {
    return (val + 1.0f) == val;
}

// Reference: https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
// Given three colinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
bool on_segment(glm::vec2 p, glm::vec2 q, glm::vec2 r)
{
    return q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
           q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y);

}

// Reference: https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int orientation(glm::vec2 p, glm::vec2 q, glm::vec2 r)
{
    // See https://www.geeksforgeeks.org/orientation-3-ordered-points/
    // for details of below formula.
    int val = (q.y - p.y) * (r.x - q.x) -
              (q.x - p.x) * (r.y - q.y);

    if (val == 0) return 0;  // colinear

    return (val > 0)? 1: 2; // clock or counterclock wise
}

// Reference: https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
// The main function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.
bool do_intersect(glm::vec2 p1, glm::vec2 q1, glm::vec2 p2, glm::vec2 q2)
{
    // Find the four orientations needed for general and
    // special cases
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4)
        return true;

    // Special Cases
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o1 == 0 && on_segment(p1, p2, q1)) return true;

    // p1, q1 and q2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && on_segment(p1, q2, q1)) return true;

    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && on_segment(p2, p1, q2)) return true;

    // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == 0 && on_segment(p2, q1, q2)) return true;

    return false; // Doesn't fall in any of the above cases
}

// Reference: https://mapbox.github.io/delaunator : circumcenter function
glm::vec2 compute_triangle_circumcenter(glm::vec2 a, glm::vec2 b, glm::vec2 c) {
    auto ad = a[0] * a[0] + a[1] * a[1];
    auto bd = b[0] * b[0] + b[1] * b[1];
    auto cd = c[0] * c[0] + c[1] * c[1];
    auto D = 2 * (a[0] * (b[1] - c[1]) + b[0] * (c[1] - a[1]) + c[0] * (a[1] - b[1]));
    return {
        1.0 / D * (ad * (b[1] - c[1]) + bd * (c[1] - a[1]) + cd * (a[1] - b[1])),
        1.0 / D * (ad * (c[0] - b[0]) + bd * (a[0] - c[0]) + cd * (b[0] - a[0]))
    };
}

std::vector<Point_2> generate_points(unsigned int num_points, unsigned int width, unsigned int height) {
    assert(width > 0);
    assert(height > 0);
    std::random_device rd;
    std::set<Point_2> points;
    while (points.size() < num_points) {
        Point_2 v;
        do {
            v = Point_2(rd() % width, rd() % height);
        } while (points.contains(v));
        points.insert(v);
    }
    return {points.begin(), points.end()};
}

std::vector<Point_2> convert_to_point_2(const std::vector<double>& coords) {
    assert(coords.size() % 2 == 0);
    std::vector<Point_2> points;
    for(auto i = 0; i < coords.size(); i+= 2)
        points.emplace_back(coords[i], coords[i+1]);
    return points;
}

float uniform_random(float a, float b) {
    return a + (b - a) * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
}

} // namespace utils::math
