#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <random>
#include <cmath>

struct Point { double x; double y; double z; };
struct ScreenPos { double x; double y; };
enum Brightness { OFF, HALF, FULL };

int rnd_i(int min, int max);
double rnd_d(char side);

class Stars;
class DrawStars;

// definitions
class Star
{
    Point pos;
    ScreenPos screen_pos; // we only use x/y from this
    Brightness brightness;
    double z_vel;
    int size;

public:
    Star(Point xyz, ScreenPos xy, Brightness br, double zv, int sz): pos(xyz), screen_pos(xy), brightness(br), z_vel(zv), size(sz) {};

    void update(ScreenPos &midp);
    void reset();

    ScreenPos &get_pos() { return screen_pos; };
    Brightness get_br() { return brightness; };
    int get_size() { return size; };
};

class EffectInfo
{
    int bound_w, bound_h, max_z, cam_dist, cam_width, cam_height;
    ScreenPos midp;

public:
    std::vector<uint32_t> palette = { 0x00000000, 0x00A8A8A8, 0x00FFFFFF };

    void init(int width, int height, int cw, int ch, int cd);
    bool check_in_bounds(ScreenPos p);

    ScreenPos &get_midp() { return midp; };

    int get_w() { return bound_w; };
    int get_h() { return bound_h; };

    int get_cam_dist() { return cam_dist; };
    int get_cam_w() { return cam_width; };
    int get_cam_h() { return cam_height; };
};

class Stars
{
    std::vector<Star> stars;
    EffectInfo *inforef;

public:
    Stars(){};

    void init(EffectInfo *ref);
    void new_star_batch(int num_stars);
    void update_stars();

    std::vector<Star> &get_stars_ref() { return stars; };
};

class DrawStars
{
    std::vector<uint32_t> buffer;
    int buf_pitch, buf_stride, buf_size;
    EffectInfo *inforef;

    void change_pixel(int x, int y, uint32_t color);
    void draw_square(ScreenPos& pos, int lw, uint32_t color);
    void clear_buffer(uint32_t color);

public:
    DrawStars(){};

    void init(EffectInfo *ref);
    void draw_stars(std::vector<Star> &star_vec_ref);

    uint32_t *get_buf_data() { return buffer.data(); };
    int get_buf_pitch() { return buf_pitch; };
};

class StarfieldEffect
{
    Stars star_holder;
    DrawStars star_drawer;
    EffectInfo stars_info;

public:
    StarfieldEffect(){};
    ~StarfieldEffect(){};

    void start_effect(int width, int height);
    void run_effect();

    uint32_t *get_data() { return star_drawer.get_buf_data(); };
    int get_pitch() { return star_drawer.get_buf_pitch(); };
};