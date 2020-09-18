#pragma once
#include "Core/mvAppItems.h"

namespace Marvel {

	void AddAppCommands(std::map<std::string, mvPythonParser>* parsers);

	PyObject* is_dearpygui_running(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_main_window_title(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_main_window_resizable(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_vsync(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* setup_dearpygui(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* render_dearpygui_frame(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* cleanup_dearpygui(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* start_dearpygui(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_start_callback(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_exit_callback(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* stop_dearpygui(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_global_font_scale(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_global_font_scale(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* run_async_function(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* select_directory_dialog(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* open_file_dialog(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_total_time(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_delta_time(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_main_window_size(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_thread_count(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* is_threadpool_high_performance(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_threadpool_timeout(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_active_window(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_dearpygui_version(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_threadpool_timeout(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_thread_count(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* add_data(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* get_data(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* delete_data(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_threadpool_high_performance(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_main_window_size(PyObject* self, PyObject* args, PyObject* kwargs);

	void AddLogCommands(std::map<std::string, mvPythonParser>* parsers);

	PyObject* get_log_level(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* set_log_level(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* log(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* log_debug(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* log_info(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* log_warning(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* log_error(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* clear_log(PyObject* self, PyObject* args, PyObject* kwargs);

	void AddStdWindowCommands(std::map<std::string, mvPythonParser>* parsers);

	PyObject* show_logger(PyObject* self, PyObject* args);
	PyObject* close_popup(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* show_style_editor(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* show_metrics(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* show_about(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* show_source(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* show_debug(PyObject* self, PyObject* args, PyObject* kwargs);
	PyObject* show_documentation(PyObject* self, PyObject* args, PyObject* kwargs);

}
