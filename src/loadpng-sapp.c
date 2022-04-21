//------------------------------------------------------------------------------
//  loadpng-sapp.c
//  Asynchronously load a png file via sokol_fetch.h, decode via stb_image.h
//  (this is non-perfect since it happens on the main thread)
//  and create a sokol-gfx texture from the decoded pixel data.
//
//  The CMakeLists.txt entry for loadpng-sapp.c also demonstrates the
//  sokol_file_copy() macro to copy assets into the fips deployment directory.
//
//  This is a modified version of texcube-sapp.c
//------------------------------------------------------------------------------
#define SOKOL_IMPL

#define HANDMADE_MATH_IMPLEMENTATION
#define HANDMADE_MATH_NO_SSE
#include "HandmadeMath.h"
#include "sokol_gfx.h"
#include "sokol_app.h"
#include "sokol_fetch.h"
#include "sokol_glue.h"
#include "sokol_time.h"
#include "stb/stb_image.h"
#include "dbgui/dbgui.h"
#include "util/fileutil.h"
#include "MainShader.glsl.h"

static struct {
    float rx, ry;
    sg_pass_action pass_action;
    sg_pipeline pip;
    sg_bindings bind;
  //  uint8_t file_buffer[256 * 1024];
    uint8_t file_buffer[1024 * 1024];
} state;

typedef struct {
    float x, y, z;
    int16_t u, v;
} vertex_t;

static void fetch_callback(const sfetch_response_t*);




static void load_png(char* path, size_t size) {

     int png_width, png_height, num_channels;
        const int desired_channels = 4;
        stbi_uc* pixels = stbi_load_from_memory(
            (const stbi_uc*)path,
            size,
            &png_width, &png_height,
            &num_channels, desired_channels);
        if (pixels) {
            /* ok, time to actually initialize the sokol-gfx texture */
            sg_init_image(state.bind.fs_images[SLOT_tex], &(sg_image_desc){
                .width = png_width,
                .height = png_height,
                .pixel_format = SG_PIXELFORMAT_RGBA8,
                .min_filter = SG_FILTER_LINEAR,
                .mag_filter = SG_FILTER_LINEAR,
                .data.subimage[0][0] = {
                    .ptr = pixels,
                    .size = (size_t)(png_width * png_height * 4),
                }
            });
            stbi_image_free(pixels);
        }
}


char* LoadFile(char* path, size_t* size) {

    size_t _nSize  = 0;

          FILE*  file = fopen( (char*)path, "rb");
        // file = fopen( "Rc/Tf.png", "rb");
            if (file != NULL){
          //  size_t size;
 
                fseek (file, 0, SEEK_END);
                _nSize=ftell(file);
                fseek(file, 0, SEEK_SET);
                
                char* _aData = (char*)malloc(_nSize * sizeof(char)); //TODO may be 0 sized
                fread(_aData, 1, _nSize, file);
              //  _oRc->fSetDynamicMemData(_aData, _nSize); //Will be auto free

                *size=_nSize;

                fclose(file);

                      printf("\nFile loaded\n");
                return _aData;
            }else{
                printf("\nError\n");
                //Debug.fError("Error, can't open file : " + sFullPath);
               return 0;
       
            
        }
            fclose(file);	
                  printf("\nError\n");
            return 0;
    }


void iniImGUI(void);
void rive_ini(void);
void frameImGUI(sg_pass_action* main_pass);
void rive_frame(sg_pass_action* main_pass);
void endframeImGUI(void);


static void init_cube(void) {
    
    /* pass action for clearing the framebuffer to some color */
    state.pass_action = (sg_pass_action) {
        .colors[0] = { .action = SG_ACTION_CLEAR, .value = { 0.125f, 0.25f, 0.35f, 1.0f } }
    };

    /* Allocate an image handle, but don't actually initialize the image yet,
       this happens later when the asynchronous file load has finished.
       Any draw calls containing such an "incomplete" image handle
       will be silently dropped.
    */
    state.bind.fs_images[SLOT_tex] = sg_alloc_image();

    /* cube vertex buffer with packed texcoords */
    const vertex_t vertices[] = {
        /* pos                  uvs */
        { -1.0f, -1.0f, -1.0f,      0,     0 },
        {  1.0f, -1.0f, -1.0f,  32767,     0 },
        {  1.0f,  1.0f, -1.0f,  32767, 32767 },
        { -1.0f,  1.0f, -1.0f,      0, 32767 },

        { -1.0f, -1.0f,  1.0f,      0,     0 },
        {  1.0f, -1.0f,  1.0f,  32767,     0 },
        {  1.0f,  1.0f,  1.0f,  32767, 32767 },
        { -1.0f,  1.0f,  1.0f,      0, 32767 },

        { -1.0f, -1.0f, -1.0f,      0,     0 },
        { -1.0f,  1.0f, -1.0f,  32767,     0 },
        { -1.0f,  1.0f,  1.0f,  32767, 32767 },
        { -1.0f, -1.0f,  1.0f,      0, 32767 },

        {  1.0f, -1.0f, -1.0f,      0,     0 },
        {  1.0f,  1.0f, -1.0f,  32767,     0 },
        {  1.0f,  1.0f,  1.0f,  32767, 32767 },
        {  1.0f, -1.0f,  1.0f,      0, 32767 },

        { -1.0f, -1.0f, -1.0f,      0,     0 },
        { -1.0f, -1.0f,  1.0f,  32767,     0 },
        {  1.0f, -1.0f,  1.0f,  32767, 32767 },
        {  1.0f, -1.0f, -1.0f,      0, 32767 },

        { -1.0f,  1.0f, -1.0f,      0,     0 },
        { -1.0f,  1.0f,  1.0f,  32767,     0 },
        {  1.0f,  1.0f,  1.0f,  32767, 32767 },
        {  1.0f,  1.0f, -1.0f,      0, 32767 },
    };
    state.bind.vertex_buffers[0] = sg_make_buffer(&(sg_buffer_desc){
        .data = SG_RANGE(vertices),
        .label = "cube-vertices"
    });

    /* create an index buffer for the cube */
    const uint16_t indices[] = {
        0, 1, 2,  0, 2, 3,
        6, 5, 4,  7, 6, 4,
        8, 9, 10,  8, 10, 11,
        14, 13, 12,  15, 14, 12,
        16, 17, 18,  16, 18, 19,
        22, 21, 20,  23, 22, 20
    };
    state.bind.index_buffer = sg_make_buffer(&(sg_buffer_desc){
        .type = SG_BUFFERTYPE_INDEXBUFFER,
        .data = SG_RANGE(indices),
        .label = "cube-indices"
    });

    /* a pipeline state object */
    state.pip = sg_make_pipeline(&(sg_pipeline_desc){
        .shader = sg_make_shader(loadpng_shader_desc(sg_query_backend())),
        .layout = {
            .attrs = {
                [ATTR_vs_pos].format = SG_VERTEXFORMAT_FLOAT3,
                [ATTR_vs_texcoord0].format = SG_VERTEXFORMAT_SHORT2N
            }
        },
        .index_type = SG_INDEXTYPE_UINT16,
        .cull_mode = SG_CULLMODE_BACK,
        .depth = {
            .compare = SG_COMPAREFUNC_LESS_EQUAL,
            .write_enabled = true
        },
        .label = "cube-pipeline",
         .colors[0] = {
        .write_mask = SG_COLORMASK_RGB,
        .blend = {
            .enabled = true,
            .src_factor_rgb = SG_BLENDFACTOR_SRC_ALPHA,
            .dst_factor_rgb = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
        }
        }

    });

    /* start loading the PNG file, we don't need the returned handle since
       we can also get that inside the fetch-callback from the response
       structure.
        - NOTE that we're not using the user_data member, since all required
          state is in a global variable anyway
    */


/*
size_t file_size = 0;
char* file = LoadFile("Rc/Ply.png", &file_size);
load_png(file, file_size);
*/

    char path_buf[512];
    sfetch_send(&(sfetch_request_t){
        .path = fileutil_get_path("Rc/Ply.png", path_buf, sizeof(path_buf)),
        .callback = fetch_callback,
        .buffer_ptr = state.file_buffer,
        .buffer_size = sizeof(state.file_buffer)
    });
    printf("%s\n", path_buf);
}


static void init(void) {
   
	 /* setup sokol-gfx and the optional debug-ui*/
    sg_setup(&(sg_desc){
        .context = sapp_sgcontext(),
        .buffer_pool_size    =  4096,
    });
    __dbgui_setup(sapp_sample_count());

    /* setup sokol-fetch with the minimal "resource limits" */
    sfetch_setup(&(sfetch_desc_t){
        .max_requests = 1,
        .num_channels = 1,
        .num_lanes = 1
    });

    state.pass_action = (sg_pass_action) {
        .colors[0] = {  .value = { 0.039f, 0.15f, 0.21f, 1.0f } }
    };

    //init_cube();

	iniImGUI();	


    rive_ini();
    
}





    

/* The fetch-callback is called by sokol_fetch.h when the data is loaded,
   or when an error has occurred.
*/
static void fetch_callback(const sfetch_response_t* response) {
    if (response->fetched) {
        /* the file data has been fetched, since we provided a big-enough
           buffer we can be sure that all data has been loaded here
        */
        load_png(response->buffer_ptr, response->fetched_size);
    }
    else if (response->failed) {
        // if loading the file failed, set clear color to red
        state.pass_action = (sg_pass_action) {
            .colors[0] = { .action = SG_ACTION_CLEAR, .value = { 1.0f, 0.0f, 0.0f, 1.0f } }
        };
    }
}



/* The frame-function is fairly boring, note that no special handling is
   needed for the case where the texture isn't loaded yet.
   Also note the sfetch_dowork() function, this is usually called once a
   frame to pump the sokol-fetch message queues.
*/


 void frame_scene(void) {

    /* pump the sokol-fetch message queues, and invoke response callbacks */
    sfetch_dowork();
/*
    state.pass_action = (sg_pass_action) {
        .colors[0] = { .action = SG_ACTION_CLEAR, .value = { 1.125f, 0.25f, 0.35f, 1.0f } }
    };*/



    /* compute model-view-projection matrix for vertex shader */
    const float t = (float)(sapp_frame_duration() * 60.0);
    hmm_mat4 proj = HMM_Perspective(60.0f, sapp_widthf()/sapp_heightf(), 0.01f, 10.0f);
    hmm_mat4 view = HMM_LookAt(HMM_Vec3(0.0f, 1.5f, 6.0f), HMM_Vec3(0.0f, 0.0f, 0.0f), HMM_Vec3(0.0f, 1.0f, 0.0f));
    hmm_mat4 view_proj = HMM_MultiplyMat4(proj, view);
    vs_params_t vs_params;
    state.rx += 0.78f * t; state.ry += 0.0823f * t;
    hmm_mat4 rxm = HMM_Rotate(state.rx, HMM_Vec3(1.0f, 0.0f, 0.0f));
    hmm_mat4 rym = HMM_Rotate(state.ry, HMM_Vec3(0.0f, 1.0f, 0.0f));
    hmm_mat4 model = HMM_MultiplyMat4(rxm, rym);
    vs_params.mvp = HMM_MultiplyMat4(view_proj, model);

    sg_begin_default_pass(&state.pass_action, sapp_width(), sapp_height());
    sg_apply_pipeline(state.pip);
    sg_apply_bindings(&state.bind);
    sg_apply_uniforms(SG_SHADERSTAGE_VS, SLOT_vs_params, &SG_RANGE(vs_params));
    sg_draw(0, 36, 1);
    __dbgui_draw();
	    sg_end_pass();
		
}

static void frame(void) {
	
	rive_frame(&state.pass_action);

	
 //   frame_scene();    
	frameImGUI(&state.pass_action);

    sg_commit();
	endframeImGUI();
}

static void cleanup(void) {
    __dbgui_shutdown();
    sfetch_shutdown();
    sg_shutdown();
}

void imgui_input(const sapp_event* event);
void input(const sapp_event* event) {
    __dbgui_event(event);
	imgui_input(event);
}

sapp_desc sokol_main(int argc, char* argv[]) {
    (void)argc; (void)argv;
    return (sapp_desc){
        .init_cb = init,
        .frame_cb = frame,
        .cleanup_cb = cleanup,
        .width = 1680,
        .height = 1080,
		.event_cb = input,
        .sample_count = 4,
		.swap_interval = 1,
        .gl_force_gles2 = true,
        .window_title = "ECS Ply",
        .icon.sokol_default = true,
    };
}



