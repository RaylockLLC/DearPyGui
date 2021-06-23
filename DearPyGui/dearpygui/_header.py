from typing import List, Any, Callable, Union
from contextlib import contextmanager
import dearpygui.core as internal_dpg

########################################################################################################################
# User API
#
#    * Sections
#      - Helper Commands
#      - Container Context Managers
#      - Core Wrappings
#      - Constants
#
########################################################################################################################

########################################################################################################################
# Helper Commands
########################################################################################################################

def configure_item(item : int, **kwargs) -> None:
	"""Undocumented"""
	internal_dpg.configure_item(item, **kwargs)

def start_dearpygui():
    vp = internal_dpg.create_viewport()
    internal_dpg.setup_dearpygui(viewport=vp)
    internal_dpg.show_viewport(vp)
    while(internal_dpg.is_dearpygui_running()):
        internal_dpg.render_dearpygui_frame()   
    internal_dpg.cleanup_dearpygui()

@contextmanager
def mutex():
   
   try:
        yield internal_dpg.lock_mutex()
   finally:
        internal_dpg.unlock_mutex()

@contextmanager
def popup(parent: int, mousebutton: int = internal_dpg.mvMouseButton_Right, modal: bool = False) -> int:
    
    try:
        _internal_popup_id = internal_dpg.generate_uuid()
        internal_dpg.add_clicked_handler(parent, mousebutton, callback=lambda: internal_dpg.configure_item(_internal_popup_id, show=True))
        if modal:
            internal_dpg.add_window(modal=True, show=False, id=_internal_popup_id, autosize=True)
        else:
            internal_dpg.add_window(popup=True, show=False, id=_internal_popup_id, autosize=True)
        internal_dpg.push_container_stack(internal_dpg.last_container())
        yield _internal_popup_id

    finally:
        internal_dpg.pop_container_stack()

# window configure
def set_window_pos(window: str, x: int, y: int):
    """Sets the top left corner of the window to the specified position.

    Args:
        window: window to position
        x: The x position.
        y: The y position.

    Returns:
        None
    """
    internal_dpg.configure_item(window, x_pos=x, y_pos=y)


def get_window_pos(window: str) -> Union[List[int], None]:
    """Gets the top left corner of the window to the specified position.

    Args:
        window: window to retrieve position
        x: The x position.
        y: The y position.

    Returns:
        list as [x,y] or None
    """
    config = internal_dpg.get_item_configuration(window)
    return [config["x_pos"], config["y_pos"]]


def show_item(item: str):
    """Shows the item.

    Args:
        item: Item to show.

    Returns:
        None
    """
    internal_dpg.configure_item(item, show=True)


def hide_item(item: str, *, children_only: bool = False):
    """Hides the item.

    Args:
        **item: Item to hide.

    Returns:
        None
    """
    if children_only:
        children = get_item_children(item)
        for child in children:
            internal_dpg.configure_item(child, show=False)
    else:
        internal_dpg.configure_item(item, show=False)

def enable_item(item: str):
    """Enables the item.

    Args:
        **item: Item to enable.

    Returns:
        None
    """
    internal_dpg.configure_item(item, enabled=True)


def disable_item(item: str):
    """Disables the item.

    Args:
        **item: Item to disable.

    Returns:
        None
    """
    internal_dpg.configure_item(item, enable=False)


def is_item_shown(item: str) -> Union[bool, None]:
    """Checks if item is shown.

    Returns:
        status as a bool
    """
    return internal_dpg.get_item_configuration(item)["show"]

def is_item_enabled(item: str) -> Union[bool, None]:
    """Checks if item is enabled.

    Returns:
        status as a bool
    """
    return internal_dpg.get_item_configuration(item)["enabled"]


def is_item_container(item: str) -> Union[bool, None]:
    """Checks if item is a container.

    Returns:
        status as a bool
    """
    return internal_dpg.get_item_info(item)["container"]

def is_item_hovered(item: str) -> Union[bool, None]:
    """Checks if item is hovered.

    Returns:
        status as a bool
    """
    return internal_dpg.get_item_state(item)["hovered"]


def is_item_active(item: str) -> Union[bool, None]:
    """Checks if item is active.

    Returns:
        status as a bool
    """
    return internal_dpg.get_item_state(item)["active"]

def is_item_focused(item: str) -> Union[bool, None]:
    """Checks if item is focused.

    Returns:
        status as a bool
    """
    return internal_dpg.get_item_state(item)["focused"]

def is_item_clicked(item: str) -> Union[bool, None]:
    """Checks if item is clicked.

    Returns:
        status as a bool
    """
    return internal_dpg.get_item_state(item)["clicked"]

def is_item_visible(item: str) -> Union[bool, None]:
    """Checks if item is visible.

    Returns:
        status as a bool
    """
    return internal_dpg.get_item_state(item)["visible"]


def is_item_edited(item: str) -> Union[bool, None]:
    """Checks if item is edited.

    Returns:
        status as a bool
    """
    return internal_dpg.get_item_state(item)["edited"]


def is_item_activated(item: str) -> Union[bool, None]:
    """Checks if item is activated.

    Returns:
        status as a bool
    """
    return internal_dpg.get_item_state(item)["activated"]


def is_item_deactivated(item: str) -> Union[bool, None]:
    """Checks if item is deactivated.

    Returns:
        status as a bool
    """
    return internal_dpg.get_item_state(item)["deactivated"]


def is_item_deactivated_after_edit(item: str) -> Union[bool, None]:
    """Checks if item is deactivated_after_edit.

    Returns:
        status as a bool
    """
    return internal_dpg.get_item_state(item)["deactivated_after_edit"]


def is_item_toggled_open(item: str) -> Union[bool, None]:
    """Checks if item is toggled_open.

    Returns:
        status as a bool
    """
    return internal_dpg.get_item_state(item)["toggled_open"]


def is_item_ok(item: str) -> Union[bool, None]:
    """Checks if item is a ok.

    Returns:
        status as a bool
    """
    return internal_dpg.get_item_state(item)["ok"]


def set_item_label(item: str, label: str):
    """Sets the item's displayed label, anything after the characters "##" in the name will not be shown.

    Args:
        item: Item label will be applied to.
        label: Displayed name to be applied.

    Returns:
        None
    """
    internal_dpg.configure_item(item, label=label)


def set_item_source(item: str, source: str):
    """Sets the item's value, to the source's value. Widget's value will now be "linked" to source's value.

    Args:
        item: Item to me linked.
        source: Source to link to.

    Returns:
        None
    """
    internal_dpg.configure_item(item, source=source)


def set_item_width(item: str, width: int):
    """Sets the item's width.

    Args:
        item: Item the Width will be applied to.
        width: Width to be applied.

    Returns:
        None
    """
    internal_dpg.configure_item(item, width=width)


def set_item_height(item: str, height: int):
    """Sets the item's height.

    Args:
        item: Item the Height will be applied to.
        height: Height to be applied.

    Returns:
        None
    """
    internal_dpg.configure_item(item, height=height)

def set_item_callback(item: str, callback: Callable):
    """Sets the item's callack.

    Args:
        item: Item the callback will be applied to.
        callback: Callback to be applied.

    Returns:
        None
    """
    internal_dpg.configure_item(item, callback=callback)


def set_item_user_data(item: str, user_data: Any):
    """Sets the item's callack_data to any python object.

    Args:
        item: Item the callback will be applied to.
        user_data: Callback_data to be applied.

    Returns:
        None
    """
    internal_dpg.configure_item(item, user_data=user_data)


def get_item_label(item: str) -> Union[str, None]:
    """Gets the item's label.

    Returns:
        label as a string or None
    """
    return internal_dpg.get_item_configuration(item)["label"]


def get_item_width(item: str) -> Union[int, None]:
    """Gets the item's width.

    Returns:
        width as a int or None
    """
    return internal_dpg.get_item_configuration(item)["width"]


def get_item_height(item: str) -> Union[int, None]:
    """Gets the item's height.

    Returns:
        height as a int or None
    """
    return internal_dpg.get_item_configuration(item)["height"]

def get_item_callback(item: str) -> Union[str, None]:
    """Gets the item's callback.

    Returns:
        callback as a string or None
    """
    return internal_dpg.get_item_configuration(item)["callback"]

def get_item_user_data(item: str) -> Union[Any, None]:
    """Gets the item's callback data.

    Returns:
        callback data as a python object or None
    """
    return internal_dpg.get_item_configuration(item)["user_data"]

def get_item_source(item: str) -> Union[str, None]:
    """Gets the item's source.

    Returns:
        source as a string or None
    """
    return internal_dpg.get_item_configuration(item)["source"]


def get_item_parent(item: str) -> Union[str, None]:
    """Gets the item's parent.

    Returns:
        parent as a string or None
    """
    return internal_dpg.get_item_info(item)["parent"]


def get_item_children(item: str) -> Union[List[str], None]:
    """Provides access to the item's children slots.

    Returns:
        A 2-D tuple of children slots ex. ((child_slot_1),(child_slot_2),(child_slot_3),...)
    """
    return internal_dpg.get_item_info(item)["children"]


def get_item_type(item: str) -> Union[str]:
    """Gets the item's type.

    Returns:
        type as a string or None
    """
    return internal_dpg.get_item_info(item)["type"]


def get_item_rect_size(item: str) -> Union[List[int]]:
    """Gets the item's current size.

    Returns:
        size as a float list
    """
    return internal_dpg.get_item_state(item)["rect_size"]


def get_item_rect_min(item: str) -> Union[List[int]]:
    """Gets the item's current rect_min.

    Returns:
        rect_min as a float
    """
    return internal_dpg.get_item_state(item)["rect_min"]


def get_item_rect_max(item: str) -> Union[List[int]]:
    """Gets the item's current rect_max.

    Returns:
        rect_max as a float
    """
    return internal_dpg.get_item_state(item)["rect_max"]


def get_item_pos(item: str) -> Union[List[int]]:
    """Gets the item's current pos.

    Returns:
        pos as a float
    """
    return internal_dpg.get_item_state(item)["pos"]


def show_style_editor(sender: str="", data: Any=None) -> None:
    """Shows the standard style editor window

    Returns:
        None
    """
    internal_dpg.show_tool(internal_dpg.mvTool_Style)

def show_metrics(sender: str="", data: Any=None) -> None:
    """Shows the standard metrics window

    Returns:
        None
    """
    internal_dpg.show_tool(internal_dpg.mvTool_Metrics)

def show_about(sender: str="", data: Any=None) -> None:
    """Shows the standard about window

    Returns:
        None
    """
    internal_dpg.show_tool(internal_dpg.mvTool_About)

def show_debug(sender: str="", data: Any=None) -> None:
    """Shows the standard debug window

    Returns:
        None
    """
    internal_dpg.show_tool(internal_dpg.mvTool_Debug)

def show_documentation(sender: str="", data: Any=None) -> None:
    """Shows the standard documentation window

    Returns:
        None
    """
    internal_dpg.show_tool(internal_dpg.mvTool_Doc)

def show_font_manager(sender: str="", data: Any=None) -> None:
    """Shows the standard documentation window

    Returns:
        None
    """
    internal_dpg.show_tool(internal_dpg.mvTool_Font)

def show_item_registry(sender: str="", data: Any=None) -> None:
    """Shows the standard documentation window

    Returns:
        None
    """
    internal_dpg.show_tool(internal_dpg.mvTool_ItemRegistry)
