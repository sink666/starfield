#include "starfield.h"

//
// random methods
//

int rnd_i(int min, int max)
{
    using Dist = std::uniform_int_distribution<int>;

    static std::random_device rd;
    static std::default_random_engine re(rd());
  	static Dist uid {};

  	return uid(re, Dist::param_type{min,max});
}

double rnd_d(float min, float max)
{
    using Dist = std::uniform_real_distribution<double>;

    static std::random_device rd_d;
    static std::default_random_engine re(rd_d());
    static Dist urd {};

    double ret;
    
    ret = urd(re, Dist::param_type{min,max});
    return ret;
}

//
// effect manager methods
//

void StarfieldEffect::run_effect()
{
    star_holder.update_stars();
    star_drawer.draw_stars(star_holder.get_stars_ref(), 100);
}

void StarfieldEffect::start_effect(int width, int height)
{
    // initialize meta stuff
    midp = { width / 2, height / 2 };
    frame_w = width;
    frame_h = height;

    // initialize the working parts, give references to their holder
    star_drawer.init(this);
    star_holder.init(this, 500);
}

//
// star holder methods
//

void Stars::init(StarfieldEffect *ref, int num_stars)
{
    inforef = ref;

    for(int i = num_stars; i > 0; --i)
    {
        // spawn point, a random xyz, -1 to 1
        // except for z, since 0 is where the camera is
        pWorld sp = { rnd_d(-1.0, 1.0), rnd_d(-1.0, 1.0), rnd_d(-1.0, 0.0000001) };
        // 'z' is the direction towards the camera
        double z_velocity = rnd_d(0.0003, 0.0066);

        stars.push_back(Star(sp, z_velocity));
    }
}

void Stars::update_stars()
{
    for(auto &s : stars)
    {
        s.update();
    }
}

//
// star methods
//

void Star::update()
{
    pos.z += z_vel;

    if(pos.z >= 0)
    {
        reset();
    }
}

void Star::reset()
{
    pos = { rnd_d(-1.0, 1.0), rnd_d(-1.0, 1.0), rnd_d(-1.0, 0.0000001) };
}

//
// star drawer methods
//

void DrawStars::init(StarfieldEffect *ref)
{
    inforef = ref;

    buf_pitch = inforef->frame_w * sizeof(uint32_t);
    buf_stride = inforef->frame_w;
    buf_size = (inforef->frame_w * inforef->frame_h);

    for(int i = 0; i < buf_size; ++i)
    {
        buffer.push_back(inforef->palette[0]);
    }
}

void DrawStars::clear_buffer(uint32_t color)
{
    std::fill(buffer.begin(), buffer.end(), color);
}

void DrawStars::change_pixel(int x, int y, uint32_t color)
{
    int index = (buf_stride * x) + y;
    buffer[index] = color;
}

void DrawStars::draw_square(pScreen pos, int lw, uint32_t color)
{
    int targ_y = pos.y + lw;
    int targ_x = pos.x + lw;

    for(int j = pos.y; j < targ_y; ++j)
    {   
        for(int k = pos.x; k < targ_x; ++k)
        {
            change_pixel(j, k, color);
        }
    }
}

void DrawStars::draw_stars(std::vector<Star> star_vec_ref, int fov)
{
    clear_buffer(inforef->palette[0]);

    for(auto& s : star_vec_ref)
    {
        pScreen sp;
        pWorld wp = s.get_pos();
        double halfFOV = std::tan((fov * (M_PI / 180)) / 2);

        Brightness b = OFF;

        sp.x = ((wp.x / (wp.z * halfFOV)) * inforef->midp.x) + inforef->midp.x;
        sp.y = ((wp.y / (wp.z * halfFOV)) * inforef->midp.y) + inforef->midp.y;
        // sp.x = ((wp.x / wp.z) * inforef->midp.x) + inforef->midp.x;
        // sp.y = ((wp.y / wp.z) * inforef->midp.y) + inforef->midp.y;

        if(wp.z > -0.3)
        {
            b = FULL;
        }
        else if(wp.z > -0.6)
        {
            b = HALF;
        }
        else if(wp.z > -0.9)
        {
            b = DIM;
        }

        if(sp.x < 0 || sp.x >= inforef->frame_w ||
            (sp.y < 0 || sp.y >= inforef->frame_h))
        {
            s.reset();
        }
        else 
        {
            draw_square(sp, 1, inforef->palette[b]);
        }
    }
}
