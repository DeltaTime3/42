
#include <math.h>
#include "cub3d.h"

void rendering_alg(t_game *game) {
    int screen_width = WIDTH;
    int screen_height = HEIGHT;
    double fov = M_PI / 3.0;
    double angle_step = fov / (double)screen_width;

    double player_x = game->player.pos.x;
    double player_y = game->player.pos.y;
    double player_angle = game->player.angle;

    int bpp, size_line, endian;
    void *img = mlx_new_image(game->mlx, screen_width, screen_height);
    char *data = mlx_get_data_addr(img, &bpp, &size_line, &endian);

    for (int x = 0; x < screen_width; x++) {
        double ray_angle = player_angle - (fov / 2.0) + (x * angle_step);
        ray_angle = fmod(ray_angle + 2 * M_PI, 2 * M_PI); // Normalize angle

        double ray_x = player_x;
        double ray_y = player_y;

        double dir_x = cos(ray_angle);
        double dir_y = sin(ray_angle);

        int map_x = (int)ray_x;
        int map_y = (int)ray_y;

        double side_dist_x;
        double side_dist_y;

        double delta_dist_x;
        double delta_dist_y;

        if (fabs(dir_x) < 1e-30)
            delta_dist_x = 1e30;
        else
            delta_dist_x = fabs(1.0 / dir_x);

        if (fabs(dir_y) < 1e-30)
            delta_dist_y = 1e30;
        else
            delta_dist_y = fabs(1.0 / dir_y);

        int step_x, step_y;
        int side; // 0 = X, 1 = Y

        // Calculate initial steps and distances
        if (dir_x < 0) {
            step_x = -1;
            side_dist_x = (ray_x - map_x) * delta_dist_x;
        } else {
            step_x = 1;
            side_dist_x = (map_x + 1.0 - ray_x) * delta_dist_x;
        }
        if (dir_y < 0) {
            step_y = -1;
            side_dist_y = (ray_y - map_y) * delta_dist_y;
        } else {
            step_y = 1;
            side_dist_y = (map_y + 1.0 - ray_y) * delta_dist_y;
        }

        // DDA loop
        while (1) {
            if (side_dist_x < side_dist_y) {
                side_dist_x += delta_dist_x;
                map_x += step_x;
                side = 0;
            } else {
                side_dist_y += delta_dist_y;
                map_y += step_y;
                side = 1;
            }
            // Out of bounds check (avoid segfaults!)
            if (map_y < 0 || map_x < 0 || !game->map[map_y] || !game->map[map_y][map_x])
                break;
            // Break if hit wall
            if (game->map[map_y][map_x] == '1')
                break;
        }

        // Calculate distance to wall
        double distance;
        if (side == 0)
            distance = (map_x - ray_x + (1 - step_x) / 2.0) / dir_x;
        else
            distance = (map_y - ray_y + (1 - step_y) / 2.0) / dir_y;

        // Fix fisheye
        double perp_distance = distance * cos(ray_angle - player_angle);

        // Wall height / draw range
        int wall_height = (int)(screen_height / (perp_distance + 1e-6));  // avoid div by zero
        int draw_start = (screen_height - wall_height) / 2;
        int draw_end = draw_start + wall_height;

        if (draw_start < 0)
            draw_start = 0;
        if (draw_end >= screen_height)
            draw_end = screen_height - 1;

        int color;
        if (side == 0)
            color = 0xFF0000;
        else
            color = 0x0000FF;

        for (int y = draw_start; y < draw_end; y++) {
            int pixel_index = (y * size_line) + (x * (bpp / 8));
            *((int *)(data + pixel_index)) = color;
        }
    }
    mlx_put_image_to_window(game->mlx, game->win, img, 0, 0);
}
