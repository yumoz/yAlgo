# yUtils 路径处理工具脚本
# 提供各种路径操作的实用函数

# 函数: 获取当前目录的绝对路径
function(get_current_dir_path)
    get_filename_component(current_dir_path "${CMAKE_CURRENT_SOURCE_DIR}" ABSOLUTE)
    set(current_dir_path ${current_dir_path} PARENT_SCOPE)
endfunction()

# 函数: 获取当前目录的名称
function(get_current_dir_name)
    get_current_dir_path()
    get_filename_component(current_dir_name "${current_dir_path}" NAME)
    set(current_dir_name ${current_dir_name} PARENT_SCOPE)
endfunction()

# 函数: 获取父目录的绝对路径
function(get_parent_dir_path)
    get_current_dir_path()
    get_filename_component(parent_dir_path "${current_dir_path}/.." ABSOLUTE)
    set(parent_dir_path ${parent_dir_path} PARENT_SCOPE)
endfunction()

# 函数: 获取父目录的名称
function(get_parent_dir_name)
    get_parent_dir_path()
    get_filename_component(parent_dir_name "${parent_dir_path}" NAME)
    set(parent_dir_name ${parent_dir_name} PARENT_SCOPE)
endfunction()

# 函数: 获取祖父目录的绝对路径
function(get_grand_parent_dir_path)
    get_parent_dir_path()
    get_filename_component(grand_parent_dir_path "${parent_dir_path}/.." ABSOLUTE)
    set(grand_parent_dir_path ${grand_parent_dir_path} PARENT_SCOPE)
endfunction()

# 函数: 获取祖父目录的名称
function(get_grand_parent_dir_name)
    get_grand_parent_dir_path()
    get_filename_component(grand_parent_dir_name "${grand_parent_dir_path}" NAME)
    set(grand_parent_dir_name ${grand_parent_dir_name} PARENT_SCOPE)
endfunction()

# 函数: 规范化路径（处理路径分隔符和相对路径）
function(normalize_path path)
    file(TO_CMAKE_PATH "${path}" normalized_path)
    set(normalized_path ${normalized_path} PARENT_SCOPE)
endfunction()

# 函数: 确保目录存在
function(ensure_directory_exists dir_path)
    if(NOT EXISTS "${dir_path}")
        file(MAKE_DIRECTORY "${dir_path}")
    endif()
endfunction()

# 函数: 连接两个路径
function(join_paths path1 path2)
    normalize_path("${path1}/${path2}")
    set(joined_path ${normalized_path} PARENT_SCOPE)
endfunction()

# 函数: 获取文件扩展名
function(get_file_extension file_path)
    get_filename_component(extension "${file_path}" EXT)
    string(REGEX REPLACE "^\\." "" extension "${extension}")  # 移除前导点
    set(file_extension "${extension}" PARENT_SCOPE)
endfunction()

# 函数: 获取相对路径
function(get_relative_path from_path to_path)
    file(RELATIVE_PATH relative_path "${from_path}" "${to_path}")
    set(relative_path "${relative_path}" PARENT_SCOPE)
endfunction()

# 保持向后兼容性，设置传统变量
# 这些变量将在脚本包含时自动设置
get_current_dir_path()
get_current_dir_name()
get_parent_dir_path()
get_parent_dir_name()
get_grand_parent_dir_path()
get_grand_parent_dir_name()
