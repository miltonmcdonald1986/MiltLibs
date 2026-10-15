#include <array>
#include <print>

#include <glm/vec2.hpp>

#include <milt/nurbs/nurbs.hpp>

int main() {
    // C(u) = (1, 0) + (0, 2) u + (1, 1) u^2
    const std::array coefficients{glm::dvec2{1.0, 0.0}, glm::dvec2{0.0, 2.0}, glm::dvec2{1.0, 1.0}};

    for (const double u : {0.0, 0.25, 0.5, 0.75, 1.0}) {
        const glm::dvec2 c = milt::nurbs::horner1(coefficients, u);
        std::println("C({:.2f}) = ({:.4f}, {:.4f})", u, c.x, c.y);
    }
}
