static void window_resize(GLFWwindow *, int w, int h) {
    window_width = w, window_height = h;
    glViewport(0, 0, w, h);
    glUniform2f(glGetUniformLocation(s_shader, "window_size"), static_cast<float>(w), static_cast<float>(h));
}

static void mouse_move(GLFWwindow *, double x, double y) {
    mouse.x = static_cast<float>(x), mouse.y = static_cast<float>(window_height) - static_cast<float>(y);
    grid_mouse = {to_grid(mouse.x - cam_pos.x) + cam_pos.x,
                  to_grid(mouse.y - cam_pos.y) + cam_pos.y};

    if (tool_id == 1) {
        hover_lines();
        hover_rects();
    }

    drag_line_handle();
    drag_line();

    if (place_mode) {
        switch (tool_id) {
        case 0: break;
        case 1: break;
        case 2: drag_line_point2(); break;
        default: break;
        }
    }

    drag_camera();
}

static void mouse_press(GLFWwindow *, int button, int action, int) {
    switch (action) {
    case GLFW_PRESS:
        switch (button) {
        case GLFW_MOUSE_BUTTON_1:
            if (mouse.x > 200) {
                switch (tool_id) {
                case 0: break;
                case 1:
                    grab_line_handle();
                    grab_line();
                    break;
                case 2: set_line_point1(); break;
                default: break;
                }
            }
            break;
        case GLFW_MOUSE_BUTTON_3: grab_camera(); break;
        default: break;
        }
        break;
    case GLFW_RELEASE:
        switch (button) {
        case GLFW_MOUSE_BUTTON_1:
            if (tool_id == 1) {
                place_line_handle();
                place_line();
            }
            if (place_mode) {
                switch (tool_id) {
                case 0: break;
                case 1: break;
                case 2: set_line_point2(); break;
                default: break;
                }
            }
            break;
        case GLFW_MOUSE_BUTTON_3: place_camera(); break;
        default: break;
        }
        break;
    default: break;
    }
}

static void key_press(GLFWwindow *glfw_window, int key, int, int action, int) {
    switch (key) {
    case GLFW_KEY_D:
        switch (action) {
        case GLFW_PRESS:
            glfwSetCursor(glfw_window, select_cursor);
            tool_id = 2;
            break;
        default: break;
        }
        break;
    case GLFW_KEY_R: set_cam_pos({0, 0}); break;
    case GLFW_KEY_LEFT_CONTROL:
        switch (action) {
        case GLFW_PRESS: snap_to_grid = true; break;
        case GLFW_RELEASE: snap_to_grid = false; break;
        default: break;
        }
        break;
    default: break;
    }
}
