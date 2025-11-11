/* Memory leak test - calls render_walls multiple times */
#include "includes/cub3d.h"

int main(void)
{
    t_game game;
    
    // Initialize game structure
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
    game.player.angle = 0.0;
    
    // Initialize MLX
    game.mlx = mlx_init();
    if (!game.mlx)
    {
        printf("Error: mlx_init failed\n");
        return (1);
    }
    
    game.win = mlx_new_window(game.mlx, WIDTH, HEIGHT, "Leak Test");
    if (!game.win)
    {
        printf("Error: mlx_new_window failed\n");
        return (1);
    }
    
    printf("Testing for memory leaks...\n");
    printf("Calling render_walls 100 times to test for leaks...\n");
    
    // Call render_walls multiple times to test for leaks
    for (int i = 0; i < 100; i++)
    {
        render_walls(&game);
        if (i % 20 == 0)
            printf("Rendered %d frames...\n", i);
    }
    
    printf("Completed 100 renders successfully!\n");
    printf("If no memory issues occurred, the leak is fixed.\n");
    
    // Cleanup
    if (game.win)
        mlx_destroy_window(game.mlx, game.win);
    if (game.mlx)
    {
        mlx_destroy_display(game.mlx);
        free(game.mlx);
    }
    
    // Free map
    for (int i = 0; game.map[i]; i++)
        free(game.map[i]);
    free(game.map);
    
    printf("Memory cleanup completed.\n");
    return (0);
}
