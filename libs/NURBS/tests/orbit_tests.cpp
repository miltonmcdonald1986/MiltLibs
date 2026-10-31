#include "framework/orbit.hpp"

#include <numbers>

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>
#include <gtest/gtest.h>

namespace {

using milt::runner::max_orbit_pitch;
using milt::runner::Orbit;

constexpr double tolerance = 1e-12;

void expect_near(glm::dvec3 actual, glm::dvec3 expected) {
    EXPECT_NEAR(actual.x, expected.x, tolerance);
    EXPECT_NEAR(actual.y, expected.y, tolerance);
    EXPECT_NEAR(actual.z, expected.z, tolerance);
}

constexpr Orbit orbit{
    .center = {1.0, 2.0, 3.0},
    .pan = {0.0, 0.0, 0.0},
    .max_pan = 2.0,
    .distance = 5.0,
    .yaw = 0.6,
    .pitch = 0.4,
    .fovy = 4.0,
    .min_fovy = 1.0,
    .max_fovy = 8.0,
    .center_shift = 0.25,
};

} // namespace

TEST(Orbit, TowardEyeFromTheFront) {
    expect_near(milt::runner::toward_eye(0.0, 0.0), {0.0, -1.0, 0.0});
}

TEST(Orbit, YawSwingsTowardPlusX) {
    expect_near(milt::runner::toward_eye(std::numbers::pi / 2.0, 0.0), {1.0, 0.0, 0.0});
}

TEST(Orbit, PitchRaisesTheEye) {
    expect_near(milt::runner::toward_eye(0.3, std::numbers::pi / 2.0), {0.0, 0.0, 1.0});
}

TEST(Orbit, TowardEyeIsAUnitVector) {
    EXPECT_NEAR(glm::length(milt::runner::toward_eye(0.6, 0.4)), 1.0, tolerance);
}

TEST(Orbit, ScreenRightIsHorizontalAndAcrossTheView) {
    const glm::dvec3 right = milt::runner::screen_right(0.6);
    EXPECT_NEAR(glm::length(right), 1.0, tolerance);
    EXPECT_EQ(right.z, 0.0);
    EXPECT_NEAR(glm::dot(right, milt::runner::toward_eye(0.6, 0.4)), 0.0, tolerance);
    // Right, up (z) and toward the eye make a right-handed frame, as a camera's do.
    EXPECT_NEAR(glm::dot(glm::cross(right, {0.0, 0.0, 1.0}), milt::runner::toward_eye(0.6, 0.0)),
                1.0, tolerance);
}

TEST(Orbit, TargetIsShiftedLeftOfTheCenter) {
    // center_shift * fovy = 1 world unit, against screen_right.
    expect_near(milt::runner::orbit_target(orbit),
                orbit.center - milt::runner::screen_right(orbit.yaw));
}

TEST(Orbit, EyeIsDistanceFromTheTargetTowardTheEye) {
    const glm::dvec3 offset = milt::runner::orbit_eye(orbit) - milt::runner::orbit_target(orbit);
    expect_near(offset, milt::runner::toward_eye(orbit.yaw, orbit.pitch) * orbit.distance);
}

TEST(Orbit, RotatedAddsTheAngles) {
    const Orbit turned = milt::runner::rotated(orbit, {.yaw = 0.5, .pitch = -0.25});
    EXPECT_DOUBLE_EQ(turned.yaw, 1.1);
    EXPECT_DOUBLE_EQ(turned.pitch, 0.15);
    EXPECT_EQ(turned.center, orbit.center);
    EXPECT_EQ(turned.fovy, orbit.fovy);
}

TEST(Orbit, RotatedClampsThePitch) {
    EXPECT_EQ(milt::runner::rotated(orbit, {.yaw = 0.0, .pitch = 10.0}).pitch, max_orbit_pitch);
    EXPECT_EQ(milt::runner::rotated(orbit, {.yaw = 0.0, .pitch = -10.0}).pitch, -max_orbit_pitch);
}

TEST(Orbit, ZoomedScalesTheFovy) {
    EXPECT_DOUBLE_EQ(milt::runner::zoomed(orbit, 0.5).fovy, 2.0);
}

TEST(Orbit, ZoomedClampsTheFovy) {
    EXPECT_EQ(milt::runner::zoomed(orbit, 0.01).fovy, orbit.min_fovy);
    EXPECT_EQ(milt::runner::zoomed(orbit, 100.0).fovy, orbit.max_fovy);
}

TEST(Orbit, ScreenUpIsAcrossTheViewAndUpAtLevel) {
    const glm::dvec3 up = milt::runner::screen_up(0.6, 0.4);
    EXPECT_NEAR(glm::length(up), 1.0, tolerance);
    EXPECT_NEAR(glm::dot(up, milt::runner::toward_eye(0.6, 0.4)), 0.0, tolerance);
    EXPECT_NEAR(glm::dot(up, milt::runner::screen_right(0.6)), 0.0, tolerance);
    expect_near(milt::runner::screen_up(0.6, 0.0), {0.0, 0.0, 1.0});
}

TEST(Orbit, PannedMovesThePivotAgainstTheDrag) {
    // fovy 4 over 400 pixels is 0.01 world units per pixel, so a drag of (30, 40) pixels
    // moves the pivot 0.3 left and 0.4 up the screen.
    const Orbit moved = milt::runner::panned(orbit, {30.0, 40.0}, 400.0);
    expect_near(moved.pan, milt::runner::screen_up(orbit.yaw, orbit.pitch) * 0.4 -
                               milt::runner::screen_right(orbit.yaw) * 0.3);
    expect_near(milt::runner::orbit_pivot(moved), orbit.center + moved.pan);
    EXPECT_EQ(moved.center, orbit.center);
}

TEST(Orbit, PannedStopsAtMaxPan) {
    const Orbit moved = milt::runner::panned(orbit, {100000.0, 0.0}, 400.0);
    EXPECT_NEAR(glm::length(moved.pan), orbit.max_pan, tolerance);
    expect_near(moved.pan, -milt::runner::screen_right(orbit.yaw) * orbit.max_pan);
}

TEST(Orbit, TargetFollowsThePan) {
    const Orbit moved = milt::runner::panned(orbit, {30.0, 40.0}, 400.0);
    expect_near(milt::runner::orbit_target(moved) - milt::runner::orbit_target(orbit), moved.pan);
}

TEST(Orbit, PinchedPansWithTheMidpoint) {
    // Both fingers move by (30, 40), so their spread is unchanged and only the pan happens.
    const milt::runner::FingerMove move{
        .before = {.first = {100.0, 100.0}, .second = {200.0, 100.0}},
        .after = {.first = {130.0, 140.0}, .second = {230.0, 140.0}},
    };
    const Orbit moved = milt::runner::pinched(orbit, move, 400.0);
    expect_near(moved.pan, milt::runner::panned(orbit, {30.0, 40.0}, 400.0).pan);
    EXPECT_DOUBLE_EQ(moved.fovy, orbit.fovy);
}

TEST(Orbit, PinchedZoomsInAsTheFingersSpread) {
    // The spread doubles about a fixed midpoint, so the view halves in size without panning.
    const milt::runner::FingerMove move{
        .before = {.first = {150.0, 100.0}, .second = {250.0, 100.0}},
        .after = {.first = {100.0, 100.0}, .second = {300.0, 100.0}},
    };
    const Orbit moved = milt::runner::pinched(orbit, move, 400.0);
    EXPECT_DOUBLE_EQ(moved.fovy, orbit.fovy / 2.0);
    expect_near(moved.pan, orbit.pan);
}

TEST(Orbit, PinchedOnlyPansWhenTheFingersMeet) {
    const milt::runner::FingerMove move{
        .before = {.first = {100.0, 100.0}, .second = {100.0, 100.0}},
        .after = {.first = {100.0, 100.0}, .second = {120.0, 100.0}},
    };
    EXPECT_EQ(milt::runner::pinched(orbit, move, 400.0).fovy, orbit.fovy);
}
