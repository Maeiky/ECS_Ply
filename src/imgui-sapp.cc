//------------------------------------------------------------------------------
//  imgui-sapp.c
//
//  Demonstrates Dear ImGui UI rendering via sokol_gfx.h and
//  the utility header sokol_imgui.h.
//
//  Also tests default window size by keeping sapp_desc.width/height
//  zero-initialized.
//------------------------------------------------------------------------------
#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "imgui.h"
#define SOKOL_IMGUI_IMPL
#include "sokol_imgui.h"

static bool show_test_window = true;
static bool show_another_window = false;


extern  void ShowExampleAppConsole(bool* p_open);


static sg_pass_action pass_action;

void init(void) {
    // setup sokol-gfx, sokol-time and sokol-imgui
    sg_desc desc = { };
    desc.context = sapp_sgcontext();
    sg_setup(&desc);
/*
    // use sokol-imgui with all default-options (we're not doing
    // multi-sampled rendering or using non-default pixel formats)
    simgui_desc_t simgui_desc = { };
    simgui_setup(&simgui_desc);

    // initial clear color
    pass_action.colors[0].action = SG_ACTION_CLEAR;
    pass_action.colors[0].value = { 0.0f, 0.5f, 0.7f, 1.0f };*/
}


extern "C" void iniImGUI(void) {
	
    // use sokol-imgui with all default-options (we're not doing
    // multi-sampled rendering or using non-default pixel formats)
    simgui_desc_t simgui_desc = { };
    simgui_setup(&simgui_desc);
	
	//ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;


    // initial clear color
    pass_action.colors[0].action = SG_ACTION_LOAD;
    pass_action.colors[0].value = { 0.0f, 0.5f, 0.7f, 0.0f };
}




void CreateMenuBar(){

    static ImGuiDockNodeFlags dockspace_flags = 0;
    bool p_open_ = true;
    bool* p_open = &p_open_;

    // Variables to configure the Dockspace example.
    static bool opt_fullscreen = true; // Is the Dockspace full-screen?
    static bool opt_padding = false; // Is there padding (a blank space) between the window edge and the Dockspace?

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Options"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
            ImGui::MenuItem("Padding", NULL, &opt_padding);
            ImGui::Separator();

            // Display a menu item for each Dockspace flag, clicking on one will toggle its assigned flag.
            if (ImGui::MenuItem("Flag: NoSplit",                "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))                 { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
            if (ImGui::MenuItem("Flag: NoResize",               "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))                { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
            if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))  { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
            if (ImGui::MenuItem("Flag: AutoHideTabBar",         "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))          { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
            if (ImGui::MenuItem("Flag: PassthruCentralNode",    "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
            ImGui::Separator();

            // Display a menu item to close this example.
            if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
                if (p_open != NULL) // Remove MSVC warning C6011 (NULL dereference) - the `p_open != NULL` in MenuItem() does prevent NULL derefs, but IntelliSense doesn't analyze that deep so we need to add this in ourselves.
                    *p_open = false; // Changing this variable to false will close the parent window, therefore closing the Dockspace as well.
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

}


extern "C" void frame_scene();
 #include "imgui_internal.h" 


 // ImGuiID dock_id_right ;
   ImGuiID dockspace_id ;
void CreateDockingSpace(){

 




    ImGui::GetStyle().Colors[ImGuiCol_DockingEmptyBg]  =ImVec4(0.20f, 0.20f, 0.20f, 0.50f);
    static ImGuiDockNodeFlags dockspace_flags = 0; //ImGuiDockNodeFlags_PassthruCentralNode
    //static ImGuiDockNodeFlags dockspace_flags =ImGuiDockNodeFlags_PassthruCentralNode; //ImGuiDockNodeFlags_PassthruCentralNode
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    //ImGuiWindowFlags window_flags =  ImGuiWindowFlags_NoDocking;

    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    //  if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace", nullptr, window_flags);
    ImGui::PopStyleVar();
    ImGui::PopStyleVar(2);



    // DockSpace
    ImGuiIO &io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
         dockspace_id = ImGui::GetID("MyDockSpace");
       //ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

        static auto first_time = true;
        if (first_time)
        {
            first_time = false;

            ImGui::DockBuilderRemoveNode(dockspace_id); // clear any previous layout
            ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags | ImGuiDockNodeFlags_DockSpace ) ;
            ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

           //  dock_id_right = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.25f, nullptr, &dockspace_id);
            //ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.25f, nullptr, &dockspace_id);
            //ImGui::DockBuilderDockWindow("Control Panel", dock_id_right);

// split the dockspace into 2 nodes --
	// DockBuilderSplitNode takes in the following args in the following order
	//   window ID to split, direction, fraction (between 0 and 1),
	// the final two setting let's us choose which id we want (which ever one we DON'T set as NULL,
	// will be returned by the function)
	// out_id_at_dir is the id of the node in the direction we specified earlier,
	// out_id_at_opposite_dir is in the opposite direction


            ImGui::DockBuilderDockWindow("Debug",         ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.25f, nullptr, &dockspace_id));

            ImGui::DockBuilderDockWindow("Metrics",         ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.25f, nullptr, &dockspace_id));
            ImGui::DockBuilderDockWindow("Stack",         ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.25f, nullptr, &dockspace_id));

            ImGui::DockBuilderDockWindow("Style Editor",    ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.2f,  nullptr, &dockspace_id));
            ImGui::DockBuilderDockWindow("Console",         ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.25f, nullptr, &dockspace_id));
            ImGui::DockBuilderDockWindow("###Scene",        dockspace_id);




            ImGui::DockBuilderFinish(dockspace_id);
        }
    }


/*
 dockspace_id = ImGui::GetID("MyDockSpace");
ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

// split the dockspace into 2 nodes --
	// DockBuilderSplitNode takes in the following args in the following order
	//   window ID to split, direction, fraction (between 0 and 1),
	// the final two setting let's us choose which id we want (which ever one we DON'T set as NULL,
	// will be returned by the function)
	// out_id_at_dir is the id of the node in the direction we specified earlier,
	// out_id_at_opposite_dir is in the opposite direction
	auto dock_id_top = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Up, 0.2f, nullptr, &dockspace_id);
	auto dock_id_down = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.25f, nullptr, &dockspace_id);
	auto dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.2f, nullptr, &dockspace_id);
	auto dock_id_right = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.15f, nullptr, &dockspace_id);
	//auto dock_id_left2 = ImGui::DockBuilderSplitNode(dock_id_left, ImGuiDir_Down, 0.2f, nullptr, &dock_id_left);
	//auto dock_id_down2 = ImGui::DockBuilderSplitNode(dock_id_down, ImGuiDir_Right, 0.15f, nullptr, &dock_id_down);

	// we now dock our windows into the docking node we made above

	ImGui::DockBuilderDockWindow("Window 1", dock_id_top);
	ImGui::DockBuilderDockWindow("Scene", dock_id_right);
	ImGui::DockBuilderDockWindow("Window 3", dock_id_left);
	ImGui::DockBuilderDockWindow("Window 4", dock_id_down);
	ImGui::DockBuilderDockWindow("Window 0", dock_id_top);

	ImGui::DockBuilderFinish(dockspace_id);
*/

    CreateMenuBar();
    ImGui::End();


}



void CreateContextWindow(){


    //ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_Once);


   // ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    char buf[128];
    sprintf(buf, "Scene, Fps:%.2f %c Frame:%d  ###Scene", ImGui::GetIO().Framerate, "|/-\\"[(int)(ImGui::GetTime() / 0.25f) & 3], ImGui::GetFrameCount());

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 1.0f, 0.0f));
    ImGui::Begin(buf);
    ImGui::PopStyleColor();
    //ImGuiDockNodeFlags_PassthruCentralNode


//ImGui::DockBuilderDockWindow("Scene", dock_id_right);
//ImGui::DockBuilderFinish(dockspace_id);
/*
     ImVec2 availableSize = ImGui::GetContentRegionAvail();
   // ImGuiID dockspace_id2 = ImGui::GetID("RelightDockSpace2");
    ImGuiID dockspace_id2 = ImGui::GetID("MyDockSpace2");

    //ImGui::DockSpace(dockspace_id2, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
    ImGui::DockSpace(dockspace_id2, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
    
    static auto first_time2 = true;
    if (first_time2)
    {
        first_time2 = false;

        ImGui::DockBuilderRemoveNode(dockspace_id2); // clear any previous layout
        //  ImGui::DockBuilderAddNode(dockspace_id2, ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_DockSpace);
            ImGui::DockBuilderAddNode(dockspace_id2, ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspace_id2, availableSize);

        auto dock_id_right = ImGui::DockBuilderSplitNode(dockspace_id2, ImGuiDir_Right, 0.25f, nullptr, &dockspace_id2);

        ImGui::DockBuilderDockWindow("Debug window", dock_id_right);
        ImGui::DockBuilderFinish(dockspace_id2);
    }
*/

    /*
    if ( ImGui::IsMouseReleased( ImGuiMouseButton_Left) || m_width == 0 ){
    // std::cout << availableSize.x << " " << availableSize.y << std::endl;
    // should I resize my FBO here
    if (availableSize.x != m_width || availableSize.y != m_height)
    {
    m_width = availableSize.x;
    m_height = availableSize.y;
    //m_isFrameBufferDirty = true;
    //onResize(m_width, m_height);
    }
    }
    */

    //draw FBO
   // ImGui::Image((void *)m_frameBufferTextureID, availableSize, ImVec2(0, 1),ImVec2(1, 0));
    ImGui::End();



//https://github.com/ocornut/imgui/issues/4430
}




extern "C" void frameImGUI(sg_pass_action* main_pass) {
	
    const int width = sapp_width();
    const int height = sapp_height();
    simgui_new_frame({ width, height, sapp_frame_duration(), sapp_dpi_scale() });

    CreateDockingSpace();



    // 1. Show a simple window
    // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"

  
    ImGui::Begin("Debug");
        ImGui::ColorEdit3("clear color", &main_pass->colors[0].value.r);

        if (ImGui::Button("Test Window")) show_test_window ^= 1;
        ImGui::Text("w: %d, h: %d, dpi_scale: %.1f", sapp_width(), sapp_height(), sapp_dpi_scale());

        if (ImGui::Button(sapp_is_fullscreen() ? "Switch to windowed" : "Switch to fullscreen")) {
        sapp_toggle_fullscreen();
        }
    ImGui::End();



//ImGui::GetStyle().Colors[ImGuiCol_ChildBg]  = ImVec4(0.0f, 0.0f, 0.0f, 0.00f);
//ImGui::GetStyle().Colors[ImGuiCol_WindowBg]  = ImVec4(0.0f, 0.0f, 1.0f, 0.0f);
//ImGui::GetStyle().Colors[ImGuiCol_FrameBg]  = ImVec4(0.0f, 0.0f, 0.0f, 0.00f);


    CreateContextWindow();


bool popen = true;
 ShowExampleAppConsole(&popen);


    ImGui::Begin("Style Editor", &popen);
    ImGui::ShowStyleEditor();
    ImGui::End();



     ImGui::ShowMetricsWindow(&popen); 
     ImGui::ShowStackToolWindow(&popen); 


    // the sokol_gfx draw pass
    sg_begin_default_pass(&pass_action, width, height);
    simgui_render();
    sg_end_pass();
   // sg_commit();
}


extern "C" void endframeImGUI(void){
	/*
	 // Multi Viewports : Update and Render additional Platform Windows
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}*/

}

void cleanup(void) {
    simgui_shutdown();
    sg_shutdown();
}

extern "C" void imgui_input(const sapp_event* event) {
    simgui_handle_event(event);
}

sapp_desc sokol_main_second(int argc, char* argv[]) {
    (void)argc; (void)argv;
    sapp_desc desc = { };
    desc.init_cb = init;
    //desc.frame_cb = frame;
    desc.cleanup_cb = cleanup;
    //desc.event_cb = input;
    desc.gl_force_gles2 = true;
    desc.window_title = "Dear ImGui (sokol-app)";
    desc.ios_keyboard_resizes_canvas = false;
    desc.icon.sokol_default = true;
    return desc;
}
