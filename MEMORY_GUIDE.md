# 🔍 Memory Leak Detection Guide for cub3D

## Quick Commands

### 1. Basic Leak Check
```bash
make leaks
```
This runs our custom script with a user-friendly output.

### 2. Comprehensive Analysis
```bash
make leaks-full
```
Runs Valgrind with all debugging options enabled.

### 3. Manual Valgrind
```bash
valgrind --leak-check=full --show-leak-kinds=all ./cub3d map.cub
```

### 4. Quick Script
```bash
./scripts/check_leaks.sh [map_file]
```

## Understanding Valgrind Output

### ✅ Good Signs
```
All heap blocks were freed -- no leaks are possible
ERROR SUMMARY: 0 errors from 0 contexts
```

### ❌ Warning Signs
```
definitely lost: X bytes in Y blocks
possibly lost: X bytes in Y blocks
```

### 🔍 What to Look For

1. **Heap Summary**
   - `total heap usage: X allocs, X frees` ← Should be equal!
   - `in use at exit: 0 bytes in 0 blocks` ← Should be 0!

2. **Leak Categories**
   - **Definitely lost**: Memory you allocated but forgot to free
   - **Indirectly lost**: Memory lost due to losing pointer to parent
   - **Possibly lost**: Memory that might be leaked (often false positives)
   - **Still reachable**: Memory still pointed to at exit (not necessarily bad)

## Common Leak Sources in cub3D

### 1. Texture Images
```c
// ❌ Missing cleanup
mlx_xpm_file_to_image(mlx, "texture.xpm", &w, &h);

// ✅ Proper cleanup in your cleanup function
for (int i = 0; i < 4; i++)
    if (game->textures[i].img)
        mlx_destroy_image(game->mlx, game->textures[i].img);
```

### 2. MLX Resources
```c
// ❌ Missing cleanup
mlx_new_window(mlx, width, height, "title");

// ✅ Proper cleanup
if (game->win)
    mlx_destroy_window(game->mlx, game->win);
if (game->mlx)
{
    mlx_destroy_display(game->mlx);
    free(game->mlx);
}
```

### 3. Dynamic Arrays
```c
// ❌ Missing cleanup
char **map = ft_split(line, '\n');

// ✅ Proper cleanup
if (game->map)
    ft_free_array(game->map);
```

## Your Current Status

✅ **EXCELLENT!** Your cub3D currently shows:
- 816 allocations, 816 frees (perfect balance!)
- All heap blocks freed
- No memory leaks detected

## Tips for Maintaining Leak-Free Code

1. **Every malloc needs a free**
2. **Every mlx_new_* needs corresponding mlx_destroy_***
3. **Test frequently with Valgrind**
4. **Use our scripts during development**
5. **Check after adding new features**

## Debug Build with Memory Tracking

To enable detailed memory tracking, compile with:
```bash
make CFLAGS="-DDEBUG_MEMORY -g"
```

This will show every malloc/free call in your program.
