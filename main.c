#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<time.h>
#include<GL/glx.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>
#include<X11/X.h>
#include<X11/keysym.h>

#include "functions.h"
#include "variable.h"
#include "init.h"
#include "texture.h"
#include "humanoid.h"
#include "world.h"

Humanoid humanoid;
World world(10,10,25,25);

#include "mouse.h"
#include "collision.h"
#include "camera.h"



Tile_Camera tile_camera;
Tower_Camera tower_camera;
World_Camera world_camera;
First_Person_Camera first_person_camera;
Third_Person_Camera third_person_camera;
Helicoptor_Camera helicoptor_camera;
Center_Tower_Camera center_tower_camera;


float lta = 0.0;
float rta = 0.0;
float lta_x = 0.0;
float rta_x = 0.0;
float lsa = 0.0;
float rsa = 0.0;
float lea = 0.0;
float rea = 0.0;
float lka = 0.0;
float rka = 0.0;
float ba = 0.0;
float px,pz,py;

void print_collected_coins(float x, float y, void *font, const char* string)
{
    glRasterPos2f(x, y);
    int len = strlen(string);
    for(int i=0; i<len; i++)
        glutBitmapCharacter(font, string[i]);
    char coin_string[10];
    coin_string[0] = '0';
    int k = 0,num = coins_collected;
    while(num)
    {
        coin_string[k++] = num%10 + '0';
        num /= 10;
    }
    if(k)
        while(k-->0)
            glutBitmapCharacter(font, coin_string[k]);
    else
        glutBitmapCharacter(font, '0');
}

void make_humanoid()
{
    glPushMatrix();

    glScalef(0.5,0.5,0.5);

    prev_block = get_humanoid_block(humanoid,world);

    humanoid.loc_x = r_x + jump_translate_x;
    humanoid.loc_y = world.height + r_y + jump_translate_y;
    humanoid.loc_z = r_z + jump_translate_z;

    detect_boundary_collision(humanoid,world);
    detect_missing_blocks_collision(humanoid,world);
    if(detect_is_on_moving_block(humanoid,world))
    {
        vector2d block_num = get_humanoid_block(humanoid,world);
        humanoid.loc_y += world.block_motion_value[block_num.x][block_num.z]*2.0;
    }
    collect_coins(humanoid,world);

    if(!robot_jump)
        px = r_x,pz = r_z,py=r_y;
    humanoid.body(humanoid.loc_x,humanoid.loc_y + 22.5,humanoid.loc_z,ba,lsa,lea,rsa,rea,lta,lka,rta,rka,lta_x,rta_x);

    glPopMatrix();
}

void select_camera()
{
    if(view_mode == THIRD_PERSON)
    {
        third_person_camera.init_camera(ba);
    }
    else if(view_mode == FIRST_PERSON)
    {
        glTranslatef(0,0,85);
        first_person_camera.init_camera(ba);
        glTranslatef(0,0,-85);
    }
    else if(view_mode == WORLD_VIEW)
    {
        glTranslatef(0,0,85);
        world_camera.init_camera();
        glTranslatef(0,0,-85);
    }
    else if(view_mode == TOWER_VIEW)
    {
        tower_camera.init_camera(world);
    }
    else if(view_mode == CENTER_TOWER_VIEW)
    {
        center_tower_camera.init_camera(world);
    }
    else if(view_mode == TILE_VIEW)
    {
        glTranslatef(0,0,85);
        tile_camera.init_camera(tile_num_width,tile_num_depth,world);
        glTranslatef(0,0,-85);
    }
    else if(view_mode == HELICOPTOR_VIEW)
    {
        glTranslatef(0,0,-zoom_camera);
        helicoptor_camera.init_camera();
    }
}

void background()
{
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glBindTexture( GL_TEXTURE_2D, background_texture );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,1,1,0,-1,1);

    glBegin(GL_QUADS);
    glTexCoord2f(0,0);
    glVertex2f(0, 0);
    glTexCoord2f(0,1);
    glVertex2f(0,1);
    glTexCoord2f(1,1);
    glVertex2f(1, 1);
    glTexCoord2f(1,0);
    glVertex2f(1, 0);
    glEnd();

    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_2D);
}

void draw_scene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    background();
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)window_width / (GLfloat)window_height, 1.0f, 15500.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    glPushMatrix();
    glTranslatef(0,0,-5);
    print_collected_coins(2.7,1.9,GLUT_BITMAP_HELVETICA_18,"Coins Collected:");
    glPopMatrix();

    glPushMatrix();
    glRotatef(rot_x,1,0,0);
    glRotatef(rot_y,0,1,0);
    glRotatef(rot_z,0,0,1);

    glTranslatef(0,0,-85);
    select_camera();

    glPushMatrix();

    world.design(0,0,0);
    glPushMatrix();
    make_humanoid();
    glPopMatrix();

    glPopMatrix();

    glPopMatrix();
    glFlush();
    glutSwapBuffers();
}

void update_world(int value)
{
    int x,z;
    for(x=0;x<world.get_num_blocks_width();x++)
    {
        for(z=0;z<world.get_num_blocks_depth();z++)
        {
            if(world.moving[x][z])
            {
                if( (world.block_motion_value[x][z]>=world.max_block_motion_value[x][z] && world.block_motion_direction[x][z]==0) ||
                   (world.block_motion_value[x][z]<=-world.max_block_motion_value[x][z] && world.block_motion_direction[x][z]==1) )
                        world.block_motion_direction[x][z] = !world.block_motion_direction[x][z];
                world.block_motion_value[x][z] += world.block_motion_direction[x][z]?-1.0:1.0;
            }
        }
    }
    coin_rotate += 5.0;
    glutPostRedisplay();
    glutTimerFunc(60,update_world,0);
}

#include "robot_movement.h"
#include "keyboard.h"

int main(int argc ,char **argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    screen_width = glutGet(GLUT_SCREEN_WIDTH);
    screen_height = glutGet(GLUT_SCREEN_HEIGHT);

    glutInitWindowPosition(0,0);
    glutInitWindowSize(screen_width,screen_height);
    glutCreateWindow("Graphics.... :)");

    initRendering();
    world.gen_missing();
    world.gen_moving();
    world.gen_coins();
    load_textures();

    glutDisplayFunc(draw_scene);
    glutReshapeFunc(reshape_window);
    glutKeyboardFunc(handle_keyboard_keys);
    glutSpecialFunc(handle_special_keyboard_keys);
    glutSpecialUpFunc(release_keyboard_keys);
    glutMouseFunc(click_action);
    glutMotionFunc(click_hold_action);
    //glutPassiveMotionFunc(mouse_action);
    glutTimerFunc(60,update_world,0);
    glutTimerFunc(10,robot_move_forward,0);
    glutMainLoop();
    return 0;
}
