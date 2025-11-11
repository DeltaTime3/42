/* Debug version of raycasting to see what's happening */
#include "includes/cub3d.h"

void debug_render_walls(t_game *game)
{
    int screen_width = WIDTH;
    int screen_height = HEIGHT;
    double fov = M_PI / 3.0; // 60 degrees
    double angle_step = fov / (double)screen_width;
    
    double player_x = game->player.pos.x;
    double player_y = game->player.pos.y;
    double player_angle = game->player.angle;
    
    printf("\n=== DEBUG RAYCASTING ===\n");
    printf("Screen: %dx%d, FOV: %.2f rad, Angle step: %.6f\n", 
           screen_width, screen_height, fov, angle_step);
    printf("Player: (%.2f, %.2f) facing %.2f rad\n", player_x, player_y, player_angle);
    
    // Test just a few rays in the center
    for (int test_x = screen_width/2 - 2; test_x <= screen_width/2 + 2; test_x++) {
        double ray_angle = player_angle - (fov / 2.0) + (test_x * angle_step);
        ray_angle = fmod(ray_angle + 2 * M_PI, 2 * M_PI);
        
        double dir_x = cos(ray_angle);
        double dir_y = sin(ray_angle);
        
        printf("\nRay %d: angle=%.3f, dir=(%.3f,%.3f)\n", test_x, ray_angle, dir_x, dir_y);
        
        int map_x = (int)player_x;
        int map_y = (int)player_y;
        
        double delta_dist_x = (fabs(dir_x) < 1e-30) ? 1e30 : fabs(1.0 / dir_x);
        double delta_dist_y = (fabs(dir_y) < 1e-30) ? 1e30 : fabs(1.0 / dir_y);
        
        int step_x, step_y;
        double side_dist_x, side_dist_y;
        
        if (dir_x < 0) {
            step_x = -1;
            side_dist_x = (player_x - map_x) * delta_dist_x;
        } else {
            step_x = 1;
            side_dist_x = (map_x + 1.0 - player_x) * delta_dist_x;
        }
        
        if (dir_y < 0) {
            step_y = -1;
            side_dist_y = (player_y - map_y) * delta_dist_y;
        } else {
            step_y = 1;
            side_dist_y = (map_y + 1.0 - player_y) * delta_dist_y;
        }
        
        printf("  Initial: map=(%d,%d), step=(%d,%d), side_dist=(%.3f,%.3f)\n",
               map_x, map_y, step_x, step_y, side_dist_x, side_dist_y);
        
        int side = 0;
        int steps = 0;
        
        // DDA
        while (steps < 10) { // Safety limit
            steps++;
            if (side_dist_x < side_dist_y) {
                side_dist_x += delta_dist_x;
                map_x += step_x;
                side = 0;
                printf("  Step %d: X-step to (%d,%d), side=0\n", steps, map_x, map_y);
            } else {
                side_dist_y += delta_dist_y;
                map_y += step_y;
                side = 1;
                printf("  Step %d: Y-step to (%d,%d), side=1\n", steps, map_x, map_y);
            }
            
            // Check bounds and wall
            if (map_y < 0 || map_x < 0 || !game->map[map_y] || !game->map[map_y][map_x]) {
                printf("  Out of bounds, breaking\n");
                break;
            }
            
            if (game->map[map_y][map_x] == '1') {
                printf("  Hit wall at (%d,%d), side=%d -> Color: %s\n", 
                       map_x, map_y, side, (side == 0) ? "RED" : "BLUE");
                break;
            }
        }
    }
    
    printf("\nExpected: Since facing East, should hit East wall with side=1 (BLUE)\n");
}

// Simple test
int main(void)
{
    t_game game;
    ft_bzero(&game, sizeof(t_game));
    
    // Create test map
    game.map = malloc(sizeof(char*) * 4);
    game.map[0] = ft_strdup("111");
    game.map[1] = ft_strdup("1N1");
    game.map[2] = ft_strdup("111");
    game.map[3] = NULL;
    
    // Set player
    game.player.pos.x = 1.5;
    game.player.pos.y = 1.5;
    game.player.angle = 0.0; // Facing East
    
    debug_render_walls(&game);
    
    return 0;
}
