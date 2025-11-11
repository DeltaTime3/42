/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_raycasting.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: test                                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 16:00:00 by test              #+#    #+#             */
/*   Updated: 2025/10/31 16:00:00 by test             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3d.h"

// Simple key handler for testing
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
    
    // Create a simple test map
    game.map = malloc(sizeof(char*) * 4);
    game.map[0] = ft_strdup("111");
    game.map[1] = ft_strdup("1N1");
    game.map[2] = ft_strdup("111");
    game.map[3] = NULL;
    
    // Set player position and angle
    game.player.pos.x = 1.5;  // Center of the map
    game.player.pos.y = 1.5;
    game.player.angle = M_PI / 2.0;  // Facing North (90 degrees) - should show BLUE walls!
    
    // Initialize MLX
    game.mlx = mlx_init();
    if (!game.mlx)
    {
        printf("Error: mlx_init failed\n");
        return (1);
    }
    
    game.win = mlx_new_window(game.mlx, WIDTH, HEIGHT, "Test Raycasting");
    if (!game.win)
    {
        printf("Error: mlx_new_window failed\n");
        return (1);
    }
    
    printf("Testing raycasting function...\n");
    printf("Player position: (%.1f, %.1f)\n", game.player.pos.x, game.player.pos.y);
    printf("Player angle: %.2f radians (%.0f degrees)\n", game.player.angle, game.player.angle * 180.0 / M_PI);
    printf("Map:\n");
    for (int i = 0; game.map[i]; i++)
        printf("  %s\n", game.map[i]);
    
    // Test the render_walls function
    printf("Rendering with player facing North (should see BLUE wall)...\n");
    printf("Red = vertical grid crossings, Blue = horizontal grid crossings\n");
    printf("When facing North/South, rays hit horizontal boundaries -> BLUE\n");
    render_walls(&game);
    
    printf("Raycasting function executed successfully!\n");
    printf("Press ESC to exit.\n");
    
    // Set up event handlers
    mlx_hook(game.win, ON_KEYPRESS, KEYPRESS_MASK, keypress, &game);
    mlx_hook(game.win, ON_DESTROY, DESTROY_MASK, destroy_game, &game);
    
    // Start the MLX loop
    mlx_loop(game.mlx);
    
    return (0);
}
