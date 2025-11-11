/* Minimal memory test - test just the core algorithms without MLX */
#include "includes/cub3d.h"

// Mock MLX functions for testing
void* mlx_new_image(void *mlx, int width, int height) {
    (void)mlx; (void)width; (void)height;
    return malloc(1920 * 1080 * 4); // Simulate image allocation
}

char* mlx_get_data_addr(void *img, int *bpp, int *size_line, int *endian) {
    *bpp = 32;           // 32 bits per pixel
    *size_line = 1920 * 4; // 1920 pixels * 4 bytes per pixel
    *endian = 0;         // Little endian
    return (char*)img;   // Return the allocated memory as data pointer
}

int mlx_put_image_to_window(void *mlx, void *win, void *img, int x, int y) {
    (void)mlx; (void)win; (void)img; (void)x; (void)y;
    return 0; // Do nothing
}

int mlx_destroy_image(void *mlx, void *img) {
    (void)mlx;
    free(img); // Free the allocated memory
    return 0;
}

// Test function that isolates the raycasting memory usage
void test_render_memory(t_game *game) {
    printf("Testing render_walls memory usage...\n");
    
    // Call render_walls multiple times
    for (int i = 0; i < 10; i++) {
        render_walls(game);
        printf("Render %d completed\n", i + 1);
    }
    
    printf("All renders completed successfully!\n");
}

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
    
    // Mock MLX pointers
    game.mlx = (void*)0x1;  // Dummy pointer
    game.win = (void*)0x2;  // Dummy pointer
    
    printf("=== MEMORY LEAK TEST ===\n");
    printf("Testing raycasting function memory usage...\n");
    
    test_render_memory(&game);
    
    printf("\n=== CLEANUP ===\n");
    // Free map
    for (int i = 0; game.map[i]; i++) {
        free(game.map[i]);
        printf("Freed map[%d]\n", i);
    }
    free(game.map);
    printf("Freed map array\n");
    
    printf("\nTest completed. Check valgrind output for leaks.\n");
    return (0);
}
