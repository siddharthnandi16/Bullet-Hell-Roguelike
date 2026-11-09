#!/usr/bin/env python3
"""Generate bullet_types.h from Data/bullet_types.json.

Usage: python gen_bullet_enum.py
Paths are relative to the directory you run the script from.
"""
import json
import sys

INPUT_PATH = "Data/bullet_types.json"
OUTPUT_PATH = "bullet_types.h"


def main():
    with open(INPUT_PATH, "r", encoding="utf-8") as f:
        data = json.load(f)  # dict preserves file order (Python 3.7+)

    names = list(data.keys())
    if not names:
        print(f"error: no bullet types found in {INPUT_PATH}", file=sys.stderr)
        return 1

    lines = [
        "/* Generated from Data/bullet_types.json. Do not edit by hand. */",
        "#ifndef BULLET_TYPES_H",
        "#define BULLET_TYPES_H",
        "",
        "typedef enum {",
    ]
    for name in names:
        lines.append(f"    {name},")
    lines.append("    BULLET_TYPE_COUNT")
    lines.append("} ProjType;")
    lines.append("")
    lines.append("static const char *bullet_type_names[BULLET_TYPE_COUNT] = {")
    for name in names:
        lines.append(f'    "{name}",')
    lines.append("};")
    lines.append("")
    lines.append("#endif /* BULLET_TYPES_H */")
    lines.append("")

    with open(OUTPUT_PATH, "w", encoding="utf-8") as f:
        f.write("\n".join(lines))

    print(f"wrote {len(names)} bullet types to {OUTPUT_PATH}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
