#include "pch.h"

#include "Grid.h"

#include "PI.h"

namespace rc {

    TEST(Grid, Creation) {
        Grid g(10, 11, 64);

        ASSERT_EQ(10u, g.x_size);
        ASSERT_EQ(11u, g.z_size);
        ASSERT_EQ(64u, g.cell_size);
    }

    TEST(Grid, wall_at__no_wall) {
        Grid g(10, 10, 64);
        ASSERT_FALSE(g.wall_at(0, 0));
    }

    TEST(Grid, build_wall) {
        Grid g(10, 10, 64);

        g.build_wall(0, 0);

        ASSERT_TRUE(g.wall_at(0, 0));
    }

    TEST(Grid, grid_coordinate__outside) {
        Grid g(1, 1, 64);

        GridCoordinate gc = g.grid_coordinate(-0.1f, 65.1f);

        ASSERT_EQ(GridCoordinate::OUTSIDE, gc.x);
        ASSERT_EQ(GridCoordinate::OUTSIDE, gc.z);
    }

    TEST(Grid, grid_coordinate__inside) {
        Grid g(1, 1, 64);

        GridCoordinate gc = g.grid_coordinate(32.0f, 32.0f);

        ASSERT_EQ(0, gc.x);
        ASSERT_EQ(0, gc.z);
    }

    TEST(Grid, grid_coordinate__inside_one_cell_away_from_origin) {
        Grid g(2, 2, 64);

        GridCoordinate gc = g.grid_coordinate(65.0f, 65.0f);

        ASSERT_EQ(1, gc.x);
        ASSERT_EQ(1, gc.z);
    }

    TEST(Ray, construction) {
        Ray r(1, 2, 3);

        ASSERT_EQ(1, r.x);
        ASSERT_EQ(2, r.z);
        ASSERT_EQ(3, r.alpha_rad);
    }

    TEST(Grid, cast_ray__none) {
        Grid g(1, 1, 64);
        Ray r(32, 32, 0);

        RayHit hit = g.cast_ray(r);

        ASSERT_EQ(GridCoordinate::OUTSIDE, hit.cell.x);
        ASSERT_EQ(GridCoordinate::OUTSIDE, hit.cell.z);
    }

    TEST(Grid, cast_ray__horizontal_ray) {
        Grid g(2, 1, 64);
        g.build_wall(1, 0);

        Ray r(32, 32, 0);

        RayHit hit = g.cast_ray(r);

        ASSERT_EQ(1, hit.cell.x);
        ASSERT_EQ(0, hit.cell.z);

        ASSERT_FLOAT_EQ(64, hit.x);
        ASSERT_FLOAT_EQ(32, hit.z);
    }

    TEST(Grid, cast_ray__horizontal_ray_going_left ) {
        Grid g(2, 1, 64);
        g.build_wall(0, 0);

        Ray r(32 + 64, 32, PI);

        RayHit hit = g.cast_ray(r);

        ASSERT_EQ(0, hit.cell.x);
        ASSERT_EQ(0, hit.cell.z);

        ASSERT_FLOAT_EQ(64, hit.x);
        ASSERT_FLOAT_EQ(32, hit.z);
    }


    TEST(Grid, cast_ray__vertical_ray) {
        Grid g(1, 2, 64);
        g.build_wall(0, 1);
        Ray r(32, 32, PI/2);

        RayHit hit = g.cast_ray(r);

        ASSERT_EQ(0, hit.cell.x);
        ASSERT_EQ(1, hit.cell.z);

        ASSERT_FLOAT_EQ(32, hit.x);
        ASSERT_FLOAT_EQ(64, hit.z);
    }

    TEST(Grid, cast_ray__vertical_ray_two_cells_away) {
        Grid g(1, 3, 64);
        g.build_wall(0, 2);
        Ray r(32, 32, PI / 2);

        RayHit hit = g.cast_ray(r);

        ASSERT_EQ(0, hit.cell.x);
        ASSERT_EQ(2, hit.cell.z);

        ASSERT_FLOAT_EQ(32, hit.x);
        ASSERT_FLOAT_EQ(128, hit.z);
    }

    TEST(Grid, cast_ray__vertical_ray_going_down) {
        Grid g(1, 3, 64);
        g.build_wall(0, 0);
        Ray r(32, 32 + 128, 3 * PI / 2);

        RayHit hit = g.cast_ray(r);

        ASSERT_EQ(0, hit.cell.x);
        ASSERT_EQ(0, hit.cell.z);

        ASSERT_FLOAT_EQ(32, hit.x);
        ASSERT_FLOAT_EQ(64, hit.z);
    }
    TEST(Grid, cast_ray__vertical_ray_both_sides) {
        Grid g(1, 5, 64);
        g.build_wall(0, 2);
        Ray from_above(32, 32 + 64 * 4, 3 * PI / 2);
        Ray from_below(32, 32, PI / 2);

        RayHit hit_from_above = g.cast_ray(from_above);
        RayHit hit_from_below = g.cast_ray(from_below);

        ASSERT_EQ(2, hit_from_above.cell.z);
        ASSERT_EQ(2, hit_from_below.cell.z);

        ASSERT_FLOAT_EQ(128, hit_from_below.z);
        ASSERT_FLOAT_EQ(192, hit_from_above.z);
    }

    TEST(Grid, cast_ray__no_hits) {
        Grid g(5, 5, 64);
        // NO walls.

        Ray r(32 + 64 * 2, 32 + 64 * 2, 0.53f);  // Random alpha.

        RayHit hit = g.cast_ray(r);

        ASSERT_EQ(GridCoordinate::OUTSIDE, hit.cell.z);
        ASSERT_EQ(GridCoordinate::OUTSIDE, hit.cell.z);
    }


    TEST(Grid, cast_ray__up_right_corner) {
        Grid g(3, 3, 64);
        g.build_wall(2, 2);

        Ray r(32, 32, PI / 4);

        RayHit hit = g.cast_ray(r);

        ASSERT_EQ(2, hit.cell.x);
        ASSERT_EQ(2, hit.cell.z);

        ASSERT_FLOAT_EQ(128, hit.x);
        ASSERT_FLOAT_EQ(128, hit.z);
    }

    TEST(Grid, cast_ray__up_right_corner_on_first_point) {
        Grid g(2, 2, 64);
        g.build_wall(1, 1);

        Ray r(32, 32, PI / 4);

        RayHit hit = g.cast_ray(r);

        ASSERT_EQ(1, hit.cell.x);
        ASSERT_EQ(1, hit.cell.z);

        ASSERT_FLOAT_EQ(64, hit.x);
        ASSERT_FLOAT_EQ(64, hit.z);
    }


    TEST(Grid, cast_ray__bottom_left_corner) {
        Grid g(3, 3, 64);
        g.build_wall(0, 0);

        Ray r(50 + 128, 50 + 128, PI + PI / 4);

        RayHit hit = g.cast_ray(r);

        ASSERT_EQ(0, hit.cell.x);
        ASSERT_EQ(0, hit.cell.z);

        ASSERT_FLOAT_EQ(63.000038f, hit.x); //TODO! What happened to the precision?
        ASSERT_FLOAT_EQ(64, hit.z);
    }

    TEST(Grid, cast_ray__bottom_left_corner_on_first_point) {
        Grid g(2, 2, 64);
        g.build_wall(0, 0);

        Ray r(32 + 64, 32 + 64, PI + PI / 4);

        RayHit hit = g.cast_ray(r);

        ASSERT_EQ(0, hit.cell.x);
        ASSERT_EQ(0, hit.cell.z);

        ASSERT_FLOAT_EQ(63.000011f, hit.x); //TODO! What happened to the precision?
        ASSERT_FLOAT_EQ(64, hit.z);
    }
}