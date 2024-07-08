#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <random>

// random stuff

int rnd_i(int min, int max);
double rnd_d(float min, float max);

struct pWorld { double x; double y; double z; };
struct pScreen { int x; int y; };
enum Brightness { OFF, DIM, HALF, FULL };

class Stars;
class DrawStars;

// definitions
class Star
{
    pWorld pos;
    double z_vel;

public:
    Star(pWorld xyz, double zv): pos(xyz), z_vel(zv) {};

    void update();
    void reset();

    pWorld get_pos() { return pos; };
};

class EffectInfo
{
public:
    std::vector<uint32_t> palette = { 0x00000000, 0x00666666, 0x00A8A8A8, 0x00FFFFFF };
    int frame_w, frame_h; 
    pScreen midp;

    void init(int width, int height);
};

class Stars
{
    std::vector<Star> stars;
    EffectInfo *inforef;

public:
    Stars(){};

    void init(EffectInfo *ref, int num_stars);
    void update_stars();

    std::vector<Star> &get_stars_ref() { return stars; };
};

class DrawStars
{
    std::vector<uint32_t> buffer;
    int buf_pitch, buf_stride, buf_size;
    EffectInfo *inforef;

    void change_pixel(int x, int y, uint32_t color);
    void draw_square(pScreen pos, int lw, uint32_t color);
    void clear_buffer(uint32_t color);

public:
    DrawStars(){};

    void init(EffectInfo *ref);
    void draw_stars(std::vector<Star> star_vec_ref, int fov);

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