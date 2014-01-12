void reshape_window(int w, int h)
{
    glViewport(0, 0, w, h);
    window_width = w;
    window_height = h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)w / (float)h, 1.0, 500.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void initRendering()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);

    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_BLEND_COLOR);

    glEnable(GL_SHADE_MODEL);
    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_DIFFUSE, WHITE);
    glLightfv(GL_LIGHT0, GL_SPECULAR, WHITE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, WHITE);
    glMaterialf(GL_FRONT, GL_SHININESS, 30);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    //glEnable(GL_NORMALIZE);

    glEnable(GL_CULL_FACE);
    glCullFace ( GL_BACK );

    glPointSize(8);
    glLineWidth(2);
    glClearColor(119.0/255,136.0/255,153.0/255,1);
    //glClearColor(1,1,1,1);
    srand (time(NULL));
}
