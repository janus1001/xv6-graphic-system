enum shape{pixel, line, circle, rect, box, skew};

typedef struct  {
    // Type of shape
    enum shape type;

    // Arguments
    int x0;
    int y0;
    int x1;
    int y1;
    int color;
} drawshape;