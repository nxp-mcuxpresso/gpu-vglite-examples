#ifndef STATIC_PATH_DEFINES_H
#define STATIC_PATH_DEFINES_H

#include "vg_lite.h"

typedef union data_mnemonic {
    int32_t cmd;
    int32_t data;
} data_mnemonic_t;

typedef struct path_info {
    uint32_t  path_length;
    int32_t  *path_data;
    float bounding_box[4];
    uint32_t *path_args;
    uint8_t *path_cmds;
    uint8_t end_path_flag;
} path_info_t;

typedef struct stroke_info {
    uint32_t dashPatternCnt;
    float dashPhase;
    float *dashPattern;
    float strokeWidth;
    float miterlimit;
    uint32_t strokeColor;
    vg_lite_cap_style_t linecap;
    vg_lite_join_style_t linejoin;
} stroke_info_t;

typedef struct image_info {
    char *image_name;
    int  image_size[2];
    vg_lite_format_t data_format;
    float *transform;
    int path_count;
    stroke_info_t *stroke_info;
    path_info_t paths_info[];
} image_info_t;

typedef struct stopValue {
    float offset;
    uint32_t stop_color;
} stopValue_t;

typedef struct linearGradient {
    uint32_t num_stop_points;
    vg_lite_linear_gradient_parameter_t linear_gradient;
    stopValue_t *stops;
} linearGradient_t;

typedef struct radialGradient {
    uint32_t num_stop_points;
    vg_lite_radial_gradient_parameter_t radial_gradient;
    stopValue_t *stops;
} radialGradient_t;

typedef struct hybridPath {
    fill_mode_t fillType;
    vg_lite_draw_path_type_t pathType;
} hybridPath_t;

typedef struct gradient_mode {
    linearGradient_t **linearGrads;
    radialGradient_t **radialGrads;
    hybridPath_t *hybridPath;
    vg_lite_fill_t *fillRule;
}gradient_mode_t;

#endif


/*path id=stroke-01*/
static int8_t svgt12_test_rect_1_cmds[] = {
    VLC_OP_MOVE,
    VLC_OP_LINE,
    VLC_OP_LINE,
    VLC_OP_LINE,
    VLC_OP_LINE,
    VLC_OP_END
};

static int32_t svgt12_test_rect_1_args[] = {
     90.00, 70.00,
     390.00, 70.00,
     390.00, 120.00,
     90.00, 120.00,
     90.00, 70.00,
};

/*path id=stroke-02*/
static int8_t svgt12_test_rect_2_cmds[] = {
    VLC_OP_MOVE,
    VLC_OP_LINE,
    VLC_OP_LINE,
    VLC_OP_LINE,
    VLC_OP_LINE,
    VLC_OP_END
};

static int32_t svgt12_test_rect_2_args[] = {
     90.00, 190.00,
     390.00, 190.00,
     390.00, 240.00,
     90.00, 240.00,
     90.00, 190.00,
};

/*path id=test-frame*/
static int8_t svgt12_test_rect_3_cmds[] = {
    VLC_OP_MOVE,
    VLC_OP_LINE,
    VLC_OP_LINE,
    VLC_OP_LINE,
    VLC_OP_LINE,
    VLC_OP_END
};

static int32_t svgt12_test_rect_3_args[] = {
     1.00, 1.00,
     479.00, 1.00,
     479.00, 359.00,
     1.00, 359.00,
     1.00, 1.00,
};

static stroke_info_t svgt12_test_stroke_info_data[] = {
    {
/*rect id=stroke-01*/
    },
    {
/*rect id=stroke-02*/
        .dashPatternCnt = 0,
        .dashPattern = NULL,
        .dashPhase = 0,
        .strokeWidth = 20,
        .miterlimit = 4,
        .strokeColor = 0xffff0000,
        .linecap = VG_LITE_CAP_BUTT,
        .linejoin = VG_LITE_JOIN_MITER
    },
    {
/*rect id=test-frame*/
        .dashPatternCnt = 0,
        .dashPattern = NULL,
        .dashPhase = 0,
        .strokeWidth = 1,
        .miterlimit = 4,
        .strokeColor = 0xff000000,
        .linecap = VG_LITE_CAP_BUTT,
        .linejoin = VG_LITE_JOIN_MITER
    },

};


hybridPath_t svgt12_test_hybrid_path[] = {
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = STROKE, .pathType = VG_LITE_DRAW_STROKE_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = STROKE, .pathType = VG_LITE_DRAW_STROKE_PATH },

};

static vg_lite_fill_t svgt12_test_fill_rule[] = {
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD
};

static gradient_mode_t svgt12_test_gradient_info = {
    .linearGrads = NULL,
    .radialGrads = NULL,
    .hybridPath = svgt12_test_hybrid_path,
    .fillRule = svgt12_test_fill_rule
};

static float svgt12_test_transform_matrix[] = {
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f
};

static image_info_t svgt12_test = {
    .image_name = "svgt12_test",
    .image_size = {480, 360},
    .data_format = VG_LITE_S32,
    .transform = svgt12_test_transform_matrix,
    .path_count = 3,
    .stroke_info = svgt12_test_stroke_info_data,
	.paths_info = {
        {.path_cmds = (uint8_t*)svgt12_test_rect_1_cmds, .path_args = (uint32_t*)svgt12_test_rect_1_args, .path_length = sizeof(svgt12_test_rect_1_cmds), .end_path_flag=0, .bounding_box = {90.00, 70.00, 390.00, 120.00} },
        {.path_cmds = (uint8_t*)svgt12_test_rect_2_cmds, .path_args = (uint32_t*)svgt12_test_rect_2_args, .path_length = sizeof(svgt12_test_rect_2_cmds), .end_path_flag=0, .bounding_box = {90.00, 190.00, 390.00, 240.00} },
        {.path_cmds = (uint8_t*)svgt12_test_rect_3_cmds, .path_args = (uint32_t*)svgt12_test_rect_3_args, .path_length = sizeof(svgt12_test_rect_3_cmds), .end_path_flag=0, .bounding_box = {1.00, 1.00, 479.00, 359.00} }
    },
};

uint32_t svgt12_test_color_data[] = {
    0xff0000ff, 0xff0000ff, 0xff000000
};

