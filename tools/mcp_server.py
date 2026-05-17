import mcp.types as types
from mcp.server import Server
import mcp.server.stdio
import json
import subprocess
import os
from pathlib import Path
import asyncio
from typing import Any

server = Server("yalk-mcp-server")


def _get_yalk_path() -> str:
    env_path = os.environ.get("YALK_PATH")
    if env_path:
        return env_path
    script_dir = Path(__file__).resolve().parent
    default_path = script_dir / ".." / "build-mingw" / "bin" / "yalk.exe"
    return str(default_path.resolve())


def _run_yalk(args: list[str]) -> dict[str, Any]:
    yalk_path = _get_yalk_path()
    try:
        result = subprocess.run(
            [yalk_path] + args,
            capture_output=True,
            text=True,
            check=True,
        )
        return json.loads(result.stdout)
    except subprocess.CalledProcessError as e:
        raise RuntimeError(
            f"yalk 命令执行失败 (返回码 {e.returncode}):\n"
            f"stderr: {e.stderr.strip()}"
        )
    except json.JSONDecodeError as e:
        raise RuntimeError(f"无法解析 yalk 输出的 JSON: {e}")


TOOL_DEFINITIONS = [
    types.Tool(
        name="earth_distance",
        description="计算地球表面两点之间的距离（支持 haversine 等多种算法）",
        inputSchema={
            "type": "object",
            "properties": {
                "from_lon": {"type": "number", "description": "起点经度"},
                "from_lat": {"type": "number", "description": "起点纬度"},
                "to_lon": {"type": "number", "description": "终点经度"},
                "to_lat": {"type": "number", "description": "终点纬度"},
                "type": {
                    "type": "string",
                    "description": "距离算法类型（默认 haversine）",
                    "default": "haversine",
                },
            },
            "required": ["from_lon", "from_lat", "to_lon", "to_lat"],
        },
    ),
    types.Tool(
        name="earth_convert",
        description="坐标系统转换：WGS84 转 ECEF / UTM / Mercator",
        inputSchema={
            "type": "object",
            "properties": {
                "lon": {"type": "number", "description": "经度"},
                "lat": {"type": "number", "description": "纬度"},
                "alt": {"type": "number", "description": "海拔高度（米），默认 0"},
                "to": {
                    "type": "string",
                    "description": "目标坐标系：ecef | utm | mercator",
                    "enum": ["ecef", "utm", "mercator"],
                },
            },
            "required": ["lon", "lat", "to"],
        },
    ),
    types.Tool(
        name="earth_bearing",
        description="计算从起点到终点的方位角（0-360°）",
        inputSchema={
            "type": "object",
            "properties": {
                "from_lon": {"type": "number", "description": "起点经度"},
                "from_lat": {"type": "number", "description": "起点纬度"},
                "to_lon": {"type": "number", "description": "终点经度"},
                "to_lat": {"type": "number", "description": "终点纬度"},
            },
            "required": ["from_lon", "from_lat", "to_lon", "to_lat"],
        },
    ),
    types.Tool(
        name="earth_is_visible",
        description="判断两个点之间是否有视线通视（考虑地球曲率）",
        inputSchema={
            "type": "object",
            "properties": {
                "from_lon": {"type": "number", "description": "起点经度"},
                "from_lat": {"type": "number", "description": "起点纬度"},
                "from_alt": {
                    "type": "number",
                    "description": "起点海拔高度（米），默认 0",
                },
                "to_lon": {"type": "number", "description": "终点经度"},
                "to_lat": {"type": "number", "description": "终点纬度"},
                "to_alt": {
                    "type": "number",
                    "description": "终点海拔高度（米），默认 0",
                },
            },
            "required": ["from_lon", "from_lat", "to_lon", "to_lat"],
        },
    ),
    types.Tool(
        name="earth_is_point_in_polygon",
        description="判断一个点是否在地图多边形（Polygon）内部",
        inputSchema={
            "type": "object",
            "properties": {
                "point_lon": {"type": "number", "description": "待判断点的经度"},
                "point_lat": {"type": "number", "description": "待判断点的纬度"},
                "polygon": {
                    "type": "string",
                    "description": "多边形顶点 JSON 数组，格式如 [[lon1,lat1],[lon2,lat2],...]",
                },
                "projection": {
                    "type": "string",
                    "description": "投影方式（默认 utm）",
                    "default": "utm",
                },
            },
            "required": ["point_lon", "point_lat", "polygon"],
        },
    ),
    types.Tool(
        name="math_angle_convert",
        description="角度单位转换：度（deg）与弧度（rad）互转",
        inputSchema={
            "type": "object",
            "properties": {
                "value": {"type": "number", "description": "待转换的数值"},
                "from": {
                    "type": "string",
                    "description": "源单位：deg | rad",
                    "enum": ["deg", "rad"],
                },
                "to": {
                    "type": "string",
                    "description": "目标单位：deg | rad",
                    "enum": ["deg", "rad"],
                },
            },
            "required": ["value", "from", "to"],
        },
    ),
    types.Tool(
        name="math_power_convert",
        description="功率单位转换：W / kW / hp / dBW / dBm 互转",
        inputSchema={
            "type": "object",
            "properties": {
                "value": {"type": "number", "description": "待转换的数值"},
                "from": {
                    "type": "string",
                    "description": "源单位：w | kw | hp | dbw | dbm",
                    "enum": ["w", "kw", "hp", "dbw", "dbm"],
                },
                "to": {
                    "type": "string",
                    "description": "目标单位：w | kw | hp | dbw | dbm",
                    "enum": ["w", "kw", "hp", "dbw", "dbm"],
                },
            },
            "required": ["value", "from", "to"],
        },
    ),
    types.Tool(
        name="utils_exec_path",
        description="获取 yalk 可执行文件路径及所在目录",
        inputSchema={
            "type": "object",
            "properties": {},
        },
    ),
    types.Tool(
        name="utils_system_type",
        description="获取当前操作系统类型信息",
        inputSchema={
            "type": "object",
            "properties": {},
        },
    ),
]


@server.list_tools()
async def handle_list_tools() -> list[types.Tool]:
    return TOOL_DEFINITIONS


@server.call_tool()
async def handle_call_tool(
    name: str, arguments: dict | None
) -> list[types.TextContent]:
    if arguments is None:
        arguments = {}

    try:
        if name == "earth_distance":
            result = _run_yalk([
                "earth", "distance",
                "--from", f"{arguments['from_lon']},{arguments['from_lat']}",
                "--to", f"{arguments['to_lon']},{arguments['to_lat']}",
                "--type", arguments.get("type", "haversine"),
            ])
            return [types.TextContent(
                type="text",
                text=str(result.get("distance", result)),
            )]

        elif name == "earth_convert":
            alt = arguments.get("alt", 0)
            result = _run_yalk([
                "earth", "convert",
                "--input", f"{arguments['lon']},{arguments['lat']},{alt}",
                "--from", "wgs84",
                "--to", arguments["to"],
            ])
            return [types.TextContent(
                type="text",
                text=json.dumps(result, ensure_ascii=False),
            )]

        elif name == "earth_bearing":
            result = _run_yalk([
                "earth", "bearing",
                "--from", f"{arguments['from_lon']},{arguments['from_lat']}",
                "--to", f"{arguments['to_lon']},{arguments['to_lat']}",
            ])
            return [types.TextContent(
                type="text",
                text=str(result.get("bearing", result)),
            )]

        elif name == "earth_is_visible":
            from_alt = arguments.get("from_alt", 0)
            to_alt = arguments.get("to_alt", 0)
            result = _run_yalk([
                "earth", "is-visible",
                "--from", f"{arguments['from_lon']},{arguments['from_lat']},{from_alt}",
                "--to", f"{arguments['to_lon']},{arguments['to_lat']},{to_alt}",
            ])
            return [types.TextContent(
                type="text",
                text=str(result.get("is_visible", result)),
            )]

        elif name == "earth_is_point_in_polygon":
            projection = arguments.get("projection", "utm")
            result = _run_yalk([
                "earth", "is-point-in-polygon",
                "--point", f"{arguments['point_lon']},{arguments['point_lat']}",
                "--polygon", arguments["polygon"],
                "--projection", projection,
            ])
            is_inside = result.get("is_inside")
            return [types.TextContent(
                type="text",
                text=json.dumps(is_inside, ensure_ascii=False),
            )]

        elif name == "math_angle_convert":
            result = _run_yalk([
                "math", "angle",
                "--value", str(arguments["value"]),
                "--from", arguments["from"],
                "--to", arguments["to"],
            ])
            return [types.TextContent(
                type="text",
                text=str(result.get("result", result)),
            )]

        elif name == "math_power_convert":
            result = _run_yalk([
                "math", "power",
                "--value", str(arguments["value"]),
                "--from", arguments["from"],
                "--to", arguments["to"],
            ])
            return [types.TextContent(
                type="text",
                text=str(result.get("result", result)),
            )]

        elif name == "utils_exec_path":
            result = _run_yalk(["utils", "exec-path"])
            return [types.TextContent(
                type="text",
                text=json.dumps({
                    "executable_path": result.get("executable_path"),
                    "executable_dir": result.get("executable_dir"),
                }, ensure_ascii=False),
            )]

        elif name == "utils_system_type":
            result = _run_yalk(["utils", "system-type"])
            return [types.TextContent(
                type="text",
                text=str(result.get("system_type", result)),
            )]

        else:
            raise ValueError(f"未知的工具名称: {name}")

    except Exception as e:
        return [types.TextContent(
            type="text",
            text=f"错误: {e}",
        )]


async def main():
    async with mcp.server.stdio.stdio_server() as (read_stream, write_stream):
        await server.run(
            read_stream,
            write_stream,
            server.create_initialization_options(),
        )


if __name__ == "__main__":
    asyncio.run(main())