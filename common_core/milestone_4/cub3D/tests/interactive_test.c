/* Test multiple angles to see both red and blue walls */
#include "includes/cub3d.h"

// Global game variable for the hook functions
t_game *g_game;

// Enhanced key handler for testing different angles
int keypress(int key, t_game *game)
{
    if (key == ESC)
    {
        if (game->win)
            mlx_destroy_window(game->mlx, game->win);
        if (game->mlx)
        {
            mlx_destroy_display(game->mlx);
            free(game->mlx);
        }
        exit(0);
    }
    else if (key == W) // W key - face North (90°) - BLUE
    {
        game->player.angle = M_PI / 2.0;
        printf("Facing North (90°) - Should see BLUE walls\n");
        render_walls(game);
    }
    else if (key == S) // S key - face South (270°) - BLUE  
    {
        game->player.angle = 3.0 * M_PI / 2.0;
        printf("Facing South (270°) - Should see BLUE walls\n");
        render_walls(game);
    }
    else if (key == D) // D key - face East (0°) - RED
    {
        game->player.angle = 0.0;
        printf("Facing East (0°) - Should see RED walls\n");
        render_walls(game);
    }
    else if (key == A) // A key - face West (180°) - RED
    {
        game->player.angle = M_PI;
        printf("Facing West (180°) - Should see RED walls\n");
        render_walls(game);
    }
    
    return (0);
}

// Simple destroy handler
int destroy_game(t_game *game)
{
    if (game->win)
        mlx_destroy_window(game->mlx, game->win);
    if (game->mlx)
    {
        mlx_destroy_display(game->mlx);
        free(game->mlx);
    }
    exit(0);
}

int main(void)
{
    t_game game;
    
    // Initialize game structure
    ft_bzero(&game, sizeof(t_game));
    g_game = &game;
    
    // Create a simple test map
    game.map = malloc(sizeof(char*) * 4);
    game.map[0] = ft_strdup("111");
    game.map[1] = ft_strdup("1N1");
    game.map[2] = ft_strdup("111");
    game.map[3] = NULL;
    
    // Set player position and initial angle
    game.player.pos.x = 1.5;
    game.player.pos.y = 1.5;
    game.player.angle = 0.0;  // Start facing East
    
    // Initialize MLX
    game.mlx = mlx_init();
    if (!game.mlx)
    {
        printf("Error: mlx_init failed\n");
        return (1);
    }
    
    game.win = mlx_new_window(game.mlx, WIDTH, HEIGHT, "Multi-Angle Raycasting Test");
    if (!game.win)
    {
        printf("Error: mlx_new_window failed\n");
        return (1);
    }
    
    printf("=== RAYCASTING COLOR TEST ===\n");
    printf("Player at (1.5, 1.5) in 3x3 room\n");
    printf("Map:\n  111\n  1N1\n  111\n\n");
    
    printf("CONTROLS:\n");
    printf("  W = Face North (90°)  -> BLUE walls (horizontal grid crossings)\n");
    printf("  S = Face South (270°) -> BLUE walls (horizontal grid crossings)\n");
    printf("  A = Face West (180°)  -> RED walls (vertical grid crossings)\n");
    printf("  D = Face East (0°)    -> RED walls (vertical grid crossings)\n");
    printf("  ESC = Exit\n\n");
    
    printf("Starting with East view (RED walls)...\n");
    render_walls(&game);
    
    printf("Use WASD keys to change direction and see different colors!\n");
    
    // Set up event handlers
    mlx_hook(game.win, ON_KEYPRESS, KEYPRESS_MASK, keypress, &game);
    mlx_hook(game.win, ON_DESTROY, DESTROY_MASK, destroy_game, &game);
    
    // Start the MLX loop
    mlx_loop(game.mlx);
    
    return (0);
}
