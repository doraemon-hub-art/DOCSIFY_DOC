#!/usr/bin/env bash

# 有一条命令返回非0，就立即终止
set -e

DOCS_DIR="./docs"

# 1. 基本检查
if ! command -v node >/dev/null 2>&1; then
  echo "❌ Node.js 未安装，docsify 依赖 Node.js"
  echo "👉 请先安装 Node.js（建议使用 nvm）"
  exit 1
fi

# 2. 检查 docsify
if ! command -v docsify >/dev/null 2>&1; then
  echo "ℹ️  未检测到 docsify-cli，正在安装..."
  npm install -g docsify-cli
  echo "✅ docsify-cli 安装完成"
fi

# 3. 检查 docs 目录
if [ ! -d "$DOCS_DIR" ]; then
  echo "❌ 未找到 $DOCS_DIR 目录"
  exit 1
fi

# 4. 启动服务
echo "🚀 启动 Docsify 本地服务..."
docsify serve "$DOCS_DIR"
