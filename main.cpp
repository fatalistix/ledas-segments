#include <iostream>

/**
 * @name Vector3D
 * @brief Represents a 3D vector, contains x, y and z coordinates.
 *
 * @param x The x coordinate
 * @param y The y coordinate
 * @param z The z coordinate
 *
 * There are @c getter methods for each coordinate and @c operator== to compare two vectors.
 */
class Vector3D
{
public:
    Vector3D(const double x, const double y, const double z) : x(x), y(y), z(z) {}

    [[nodiscard]] double getX() const { return x; }
    [[nodiscard]] double getY() const { return y; }
    [[nodiscard]] double getZ() const { return z; }

    bool operator==(const Vector3D&) const = default;

private:
    const double x;
    const double y;
    const double z;
};

/**
 * @name Segment3D
 * @brief Represents a 3D segment, contains start and end points as Vector3D.
 *
 * @param start The start point of the segment
 * @param end The end point of the segment
 *
 * @throws std::invalid_argument if start and end are the same
 *
 * There are @c getter methods for each point.
 */
class Segment3D
{
public:
    Segment3D(const Vector3D& start, const Vector3D& end) : start(start), end(end)
    {
        if (start == end)
        {
            throw std::invalid_argument("Start and end points are the same");
        }
    }

    [[nodiscard]] Vector3D getStart() const { return start; }
    [[nodiscard]] Vector3D getEnd() const { return end; }

private:
    const Vector3D start;
    const Vector3D end;
};

/**
 * @name intersect
 * @brief Finds the intersection point between two 3D segments.
 *
 * @param segment1 The first segment
 * @param segment2 The second segment
 * @param precision The precision of the result
 *
 * @return The intersection point
 *
 * @throws std::invalid_argument if the segments do not have an intersection point.
 *
 * It builds the linear equation system for each of 3 dimensions:
 *
 * @latexonly
 *
 * \begin{cases}
 * a_1 * t + b_1 = a_2 * s + b_2 \\
 * c_1 * t + d_1 = c_2 * s + d_2 \\
 * e_1 * t + f_1 = e_2 * s + f_2 \\
 * \end{cases}
 *
 * @endlatexonly
 *
 * where (assuming, that x_1 and x_2 are the start and end points of the first segment):
 *
 * @latexonly
 *
 * a_1 = x_2 - x_1 \\
 * b_1 = x_1 \\
 * c_1 = y_2 - y_1 \\
 * d_1 = y_1 \\
 * e_1 = z_2 - z_1 \\
 * f_1 = z_1
 *
 * @endlatexonly
 *
 * and the same for the second segment.
 *
 * First 2 equations are used to find the intersection point. 3rd equation is used for checking the intersection to exist.
 */
Vector3D intersect(const Segment3D& segment1, const Segment3D& segment2, const double precision = 1e-6)
{
    const double a1 = segment1.getEnd().getX() - segment1.getStart().getX();
    const double b1 = segment1.getStart().getX();
    const double c1 = segment1.getEnd().getY() - segment1.getStart().getY();
    const double d1 = segment1.getStart().getY();
    const double e1 = segment1.getEnd().getZ() - segment1.getStart().getZ();
    const double f1 = segment1.getStart().getZ();

    const double a2 = segment2.getEnd().getX() - segment2.getStart().getX();
    const double b2 = segment2.getStart().getX();
    const double c2 = segment2.getEnd().getY() - segment2.getStart().getY();
    const double d2 = segment2.getStart().getY();
    const double e2 = segment2.getEnd().getZ() - segment2.getStart().getZ();
    const double f2 = segment2.getStart().getZ();

    const double s = (d2 * a1 - c1 * b2 + c1 * b1 - d1 * a1) / (c1 * a2 - c2 * a1);
    const double t = (a2 * s + b2 - b1) / a1;

    if (a1 * t + b1 - precision <= a2 * s + b2 && a2 * s + b2 <= a1 * t + b1 + precision &&
        c1 * t + d1 - precision <= c2 * s + d2 && c2 * s + d2 <= c1 * t + d1 + precision &&
        e1 * t + f1 - precision <= e2 * s + f2 && e2 * s + f2 <= e1 * t + f1 + precision)
    {
        return { a2 * s + b2, c2 * s + d2, e2 * s + f2 };
    }
    throw std::invalid_argument("Segments do not intersect");
}

int main()
{
    Vector3D x1 = { 3, 0, 1e-7 };
    Vector3D x2 = { 1, 0, 0 };
    Vector3D x3 = { 0, 1, 0 };
    Vector3D x4 = { 0, 4, 0 };
    Segment3D s1 = { x1, x2 };
    Segment3D s2 = { x3, x4 };
    Vector3D x = intersect(s1, s2);
    std::cout << x.getX() << " " << x.getY() << " " << x.getZ() << std::endl;
    return 0;
}


