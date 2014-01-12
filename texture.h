GLuint LoadTexture1( const char * filename, int width, int height )
{
    GLuint texture;
    unsigned char * data;
    FILE * file;
    file = fopen( filename, "rb" );
    if ( file == NULL ) return 0;
    data = (unsigned char *)malloc( width * height * 3 );
    fread( data, width * height * 3, 1, file );
    fclose( file );

    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    free( data );
    return texture;
}

GLuint LoadTexture(const char *filename, int width, int height)
{
    GLuint texture;
    unsigned char *data;
    FILE *file;
    file = fopen(filename, "rb");
    if (file == NULL) return 0;
    data = (unsigned char*) malloc(width * height * 4);
    fread(data, width * height * 4, 1, file);
    fclose(file);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPEAT);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_DECAL);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_DECAL);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR );

    /*if (strstr((char*)glGetString(GL_EXTENSIONS),"GL_EXT_texture_filter_anisotropic"))
    {
        float f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT,&f);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, f);
    }*/
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    //glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);

    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_BGR, GL_UNSIGNED_BYTE, data);
    free(data);

    return texture;
}

void load_textures()
{
    texture = LoadTexture("./textures/1.bmp",400,300);
    texture1 = LoadTexture("./textures/2.bmp",300,200);
    wall_texture1 = LoadTexture("./textures/floor1.bmp",512,512);
    wall_texture2 = LoadTexture("./textures/floor.bmp",1024,1024);
    coin_texture = LoadTexture("./textures/gold.bmp",512,512);
    background_texture = LoadTexture("./textures/8.bmp",1024,1024);
}
