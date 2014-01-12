void detect_boundary_collision(Humanoid humanoid,World world)
{
    if(falling)
        return;
    if(humanoid.loc_x < 0.0)
    {
        falling = 1;
        fall_game_over(0);
    }
    if(humanoid.loc_z < 0.0)
    {
        falling = 1;
        fall_game_over(0);
    }
    if(humanoid.loc_x > world.width*world.get_num_blocks_width()*2.0)
    {
        falling = 1;
        fall_game_over(0);
    }
    if(humanoid.loc_z > world.depth*world.get_num_blocks_depth()*2.0)
    {
        falling = 1;
        fall_game_over(0);
    }
}

vector2d get_humanoid_block(Humanoid humanoid,World world)
{
    vector2d block_num;
    float width = world.width*2.0;
    float depth = world.depth*2.0;
    block_num.x = int(humanoid.loc_x/width);
    block_num.z = int(humanoid.loc_z/depth);
    return block_num;
}

bool detect_is_on_moving_block(Humanoid humanoid,World world)
{
    vector2d block_num = get_humanoid_block(humanoid,world);
    if(world.moving[block_num.x][block_num.z] && !world.missing[block_num.x][block_num.z])
        return 1;
    return 0;
}

void detect_missing_blocks_collision(Humanoid humanoid,World world)
{
    if(falling)
        return;
    if(robot_jump)
        return;
    vector2d block_num = get_humanoid_block(humanoid,world);
    if(world.missing[block_num.x][block_num.z])
    {
        falling = 1;
        fall_game_over(0);
    }
}

bool detect_moving_blocks_collision(Humanoid humanoid,World &world)
{
    if(world.moving[prev_block.x][prev_block.z])
        return 1;

    vector2d block_num;
    float width = world.width*2.0;
    float depth = world.depth*2.0;
    block_num.x = int((r_x+jump_translate_x)/width);
    block_num.z = int((r_z+jump_translate_z)/depth);

    float humanoid_height = 26.5;
    if(world.moving[block_num.x][block_num.z])
    {
        float y = world.block_motion_value[block_num.x][block_num.z]*2.0 + world.height;
        if(y>world.height)
        {
            if(y - world.height*2.0 > humanoid.loc_y + humanoid_height)
            {
                falling = 1;
                fall_game_over(0);
            }
            else
            {
                r_x = prev_r.x;
                r_y = prev_r.y;
                r_z = prev_r.z;
            }
            return 0;
        }
        else
        {
            fall_height = humanoid.loc_y - y ;
            //fall_on_block(0);
            return 1;
        }
    }
}

bool detect_on_moving_block_collision()
{
    if(!world.moving[prev_block.x][prev_block.z])
        return 1;

    vector2d block_num;
    float width = world.width*2.0;
    float depth = world.depth*2.0;
    block_num.x = int((r_x+jump_translate_x)/width);
    block_num.z = int((r_z+jump_translate_z)/depth);

    if( (block_num.x == prev_block.x) && ( block_num.z == prev_block.z))
        return 1;

    float humanoid_height = 26.5;
    float y = world.block_motion_value[prev_block.x][prev_block.z]*2.0 + world.height;
    if(y>world.height)
        return 1;
    else
    {
        if(y < -2.0*world.height)
        {
            falling = 1;
            fall_game_over(0);
        }
        else
        {
            r_x = prev_r.x;
            r_y = prev_r.y;
            r_z = prev_r.z;
        }
        return 0;
    }
}

void collect_coins(Humanoid humanoid , World &world)
{
    vector2d block_num = get_humanoid_block(humanoid,world);
    if(world.coins[block_num.x][block_num.z])
    {
        if(!world.moving[block_num.x][block_num.z])
        {
            if(abs(humanoid.loc_x - world.get_tile_center_x(block_num.x+1)*2.0)<=5.0
                    && abs(humanoid.loc_z - world.get_tile_center_z(block_num.z+1)*2.0) <= 5.0 )
            {
                world.coins[block_num.x][block_num.z] = 0;
                coins_collected++;
            }
        }
        else
        {
            if(abs(humanoid.loc_x - world.get_tile_center_x(block_num.x+1)*2.0)<=5.0
                    && abs(humanoid.loc_z - world.get_tile_center_z(block_num.z+1)*2.0) <= 5.0 &&
                    abs(humanoid.loc_y - world.block_motion_value[block_num.x][block_num.z]*2.0 - world.height) < EPS )
            {
                world.coins[block_num.x][block_num.z] = 0;
                coins_collected++;
            }
        }
    }
}

