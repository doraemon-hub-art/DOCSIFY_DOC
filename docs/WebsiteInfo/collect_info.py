#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
collect_info.py — 统计 docs 下所有 markdown 文章的信息。

功能：
  1. 遍历 docs/ 下所有 .md，统计每个文件的字数、行数、git 创建时间、git 最近更新时间
  2. 生成中间数据文件 WebsiteInfo/document_info.json
  3. 渲染 WebsiteInfo/README.md（全局总览）与 WebsiteInfo/over_view.md（文件信息）

统计口径：
  - 排除 _ 开头的配置文件（_sidebar.md / _navbar.md 等）
  - 排除根目录 README.md（主站首页）
  - 排除 WebsiteInfo 目录下所有 md（本站信息区自身）
  - 字数 = 剥掉 markdown 语法符号后，中文字符逐个计数 + 英文单词逐个计数（代码块内容计入）

用法：python3 collect_info.py
纯标准库，无第三方依赖。
"""

import json
import re
import subprocess
from datetime import datetime
from pathlib import Path

DOCS_ROOT = Path(__file__).resolve().parent.parent   # docs/
INFO_DIR = Path(__file__).resolve().parent           # docs/WebsiteInfo/
OUT_JSON = INFO_DIR / "document_info.json"
OUT_README = INFO_DIR / "README.md"
OUT_OVERVIEW = INFO_DIR / "over_view.md"

GIT_DATE_FMT = "%Y-%m-%d %H:%M:%S"


def is_article(rel_path: Path) -> bool:
    """判断文件是否属于统计范围内的文章。"""
    if rel_path.suffix != ".md":
        return False
    if rel_path.name.startswith("_"):               # _sidebar.md / _navbar.md 等配置文件
        return False
    if str(rel_path) == "README.md":                # 根目录主站首页不计
        return False
    if rel_path.parts[0] == "WebsiteInfo":          # 网站信息区自身不计
        return False
    return True


def git_log_date(rel_path: Path, *, first: bool):
    """取文件在 git 中首次提交(first=True)或最近提交(first=False)的日期。

    返回 "YYYY-MM-DD HH:MM:SS" 字符串；文件未提交过则返回 None。
    """
    args = ["git", "log", "--format=%cd", "--date=format:" + GIT_DATE_FMT]
    if first:
        args.append("--diff-filter=A")
    args += ["--", str(rel_path)]
    proc = subprocess.run(args, cwd=DOCS_ROOT, capture_output=True, text=True)
    if proc.returncode != 0:
        return None
    lines = [ln for ln in proc.stdout.splitlines() if ln.strip()]
    if not lines:
        return None
    return lines[0] if not first else lines[-1]


def strip_markdown(text: str) -> str:
    """剥掉 markdown 语法符号，保留正文文字（代码块内容保留）。"""
    t = text
    t = re.sub(r'^\s*```[^\n]*$', '', t, flags=re.M)          # 代码块围栏行
    t = re.sub(r'!\[([^\]]*)\]\([^)]*\)', r'\1', t)           # 图片 → 保留 alt
    t = re.sub(r'\[([^\]]*)\]\([^)]*\)', r'\1', t)            # 链接 → 保留文字
    t = re.sub(r'<[^>]+>', '', t)                             # HTML 标签
    t = re.sub(r'`([^`]*)`', r'\1', t)                        # 行内代码 → 保留内容
    t = re.sub(r'\*\*([^*]+)\*\*', r'\1', t)                  # 粗体
    t = re.sub(r'\*([^*]+)\*', r'\1', t)                      # 斜体
    t = re.sub(r'__([^_]+)__', r'\1', t)                      # 粗体（下划线式）
    t = re.sub(r'^\s*#{1,6}\s*', '', t, flags=re.M)           # 标题符号
    t = re.sub(r'^\s*[-*+]\s+', '', t, flags=re.M)            # 无序列表符号
    t = re.sub(r'^\s*\d+\.\s+', '', t, flags=re.M)            # 有序列表符号
    t = re.sub(r'^\s*>\s?', '', t, flags=re.M)                # 引用符号
    t = re.sub(r'^\s*[-*_]{3,}\s*$', '', t, flags=re.M)       # 水平分隔线
    t = re.sub(r'^\s*\|[\s\-|:]+\|\s*$', '', t, flags=re.M)   # 表格分隔行
    return t


def count_words(text: str) -> int:
    """中文字符逐个计数 + 英文单词逐个计数。"""
    t = strip_markdown(text)
    cjk = len(re.findall(
        r'[\u4e00-\u9fff\u3400-\u4dbf\uf900-\ufaff\u3040-\u30ff'
        r'\uac00-\ud7af\u3000-\u303f\uff00-\uffef]', t))
    en = len(re.findall(r'[A-Za-z0-9]+(?:[-_][A-Za-z0-9]+)*', t))
    return cjk + en


def collect() -> list:
    """收集所有文章信息，按最后更新时间倒序。"""
    files = []
    for p in sorted(DOCS_ROOT.rglob("*.md")):
        rel = p.relative_to(DOCS_ROOT)
        if not is_article(rel):
            continue
        text = p.read_text(encoding="utf-8", errors="ignore")
        files.append({
            "path": str(rel),
            "words": count_words(text),
            "lines": len(text.splitlines()),
            "created": git_log_date(rel, first=True),
            "updated": git_log_date(rel, first=False),
        })
    files.sort(key=lambda f: (f["updated"] or "", f["path"]), reverse=True)
    return files


def fmt(n: int) -> str:
    return f"{n:,}"


def render_readme(files: list) -> str:
    total_words = sum(f["words"] for f in files)
    total_lines = sum(f["lines"] for f in files)
    now = datetime.now().strftime(GIT_DATE_FMT)
    return f"""# 网站信息

## 全局总览

- 文章总数：{len(files)} 篇
- 总字数：{fmt(total_words)} 字
- 总行数：{fmt(total_lines)} 行

> 数据由 collect_info.py 自动生成（{now}），请勿手写编辑，修改会在下次运行时被覆盖。
"""


def render_overview(files: list) -> str:
    rows = []
    for f in files:
        rows.append(
            f"| [{f['path']}](../{f['path']}) | {fmt(f['words'])} | "
            f"{f['created'] or '-'} | {f['updated'] or '-'} |"
        )
    now = datetime.now().strftime(GIT_DATE_FMT)
    return f"""# 文件信息

> 按最后更新时间倒序，共 {len(files)} 篇。数据由 collect_info.py 自动生成（{now}），请勿手写编辑。

| 文件 | 字数 | 创建时间 | 最后更新 |
|---|---|---|---|
{chr(10).join(rows)}
"""


def main():
    files = collect()
    data = {
        "generated_at": datetime.now().strftime(GIT_DATE_FMT),
        "summary": {
            "total_files": len(files),
            "total_words": sum(f["words"] for f in files),
            "total_lines": sum(f["lines"] for f in files),
        },
        "files": files,
    }
    OUT_JSON.write_text(json.dumps(data, ensure_ascii=False, indent=2), encoding="utf-8")
    OUT_README.write_text(render_readme(files), encoding="utf-8")
    OUT_OVERVIEW.write_text(render_overview(files), encoding="utf-8")
    print(f"OK: 共 {len(files)} 篇文章")
    print(f"  总字数 {fmt(data['summary']['total_words'])}，总行数 {fmt(data['summary']['total_lines'])}")
    print(f"  生成 {OUT_JSON}")
    print(f"  生成 {OUT_README}")
    print(f"  生成 {OUT_OVERVIEW}")


if __name__ == "__main__":
    main()
