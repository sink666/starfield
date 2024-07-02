#include "starfield.h"

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
    star_drawer.draw_stars(star_holder.get_stars_ref());
}

void StarfieldEffect::start_effect(int width, int height)
{
    // initialize the metainfo
    stars_info.init(width, height, 320, 240, 100);
    // initialize the working parts, give them references to metainfo
    star_drawer.init(&stars_info);
    star_holder.init(&stars_info);
}

//
// effect info methods
//

void EffectInfo::init(int width, int height, int cw, int ch, int cd)
{
    bound_w = width;
    bound_h = height;

    cam_dist = cd;
    cam_width = cw;
    cam_height = ch;
    
    ScreenPos midp = { (0.0f + width) / 2.0f, (0.0f + height) / 2.0f };
}

bool EffectInfo::check_in_bounds(ScreenPos star_pos)
{
    bool ret = true;

    if(star_pos.x > bound_w || star_pos.x < 0)
        ret = false;
    
    if(ret != false && (star_pos.y > bound_h || star_pos.y < 0))
        ret = false;

    return ret;
}

//
// star holder methods
//

void Stars::init(EffectInfo *ref)
{
    inforef = ref;
    new_star_batch(100);
}

void Stars::new_star_batch(int num_stars)
{
    int cd = inforef->get_cam_dist();
    int cw = inforef->get_cam_w();
    int ch = inforef-> get_cam_h();
    int vw = inforef->get_w(); 
    int vh = inforef->get_h();

    for(int i = num_stars; i > 0; --i)
    {
        // spawn point, a random point between -1 and 1
        Point sp = { rnd_d(0.55, 0.75), rnd_d(0.4, 0.6), rnd_d(-1, 1) };
        // 'z' is the direction towards the camera
        double z_velocity = rnd_d(0.09, 0.75);

        // first screen point
        ScreenPos fsp = { ((((sp.x * cd) / sp.z) * cw) / vw), ((((sp.y * cd) / sp.z) * ch) / vh) };

        int size;
        Brightness b;

        if(sp.z < -0.5)
        {
            size = 1;
            b = Brightness::HALF;
        }

        if(sp.z > -0.5)
        {
            size = 2;
            b = Brightness::HALF;
        }

        if(sp.z > -0.25)
        {
            size = 3;
            b = Brightness::HALF;
        }

        if(sp.z > 0)
        {
            size = 5;
            b = Brightness::HALF;
        }
        
        if(sp.z > 0.25)
        {
            size = 8;
            b = Brightness::FULL;
        }

        if(sp.z > 0.5)
        {
            size = 13;
            b = Brightness::FULL;
        }

        if(sp.z > 0.75)
        {
            size = 21;
            b = Brightness::FULL;
        }

        if(sp.z > 0.85)
        {
            size = 34;
            b = Brightness::FULL;
        }

        stars.push_back(Star(sp, fsp, b, z_velocity, size));
    }
}

void Stars::update_stars()
{
    for(auto &s : stars)
    {

    }
}

// 
// star methods
//

void Star::update(ScreenPos &midp)
{

}

void Star::reset()
{

}

//
// star drawer methods
//

void DrawStars::init(EffectInfo *ref)
{
    inforef = ref;

    buf_pitch = inforef->get_w() * sizeof(uint32_t);
    buf_stride = inforef->get_w();
    buf_size = (inforef->get_w() * inforef->get_h());

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

void DrawStars::draw_square(ScreenPos& pos, int lw, uint32_t color)
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

void DrawStars::draw_stars(std::vector<Star> &star_vec_ref)
{
    for(auto &s : star_vec_ref)
    {
        ScreenPos sp = s.get_pos();

        if(inforef->check_in_bounds(sp))
        {
            draw_square(sp, s.get_size(), inforef->palette[s.get_br()]);
        }
    }
}
