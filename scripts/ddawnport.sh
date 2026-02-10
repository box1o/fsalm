#!/usr/bin/env bash
set -euo pipefail

TAG="${1:-v20260209.194954}"

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
PORTS_DIR="${HOME}/dawn_ports"
PKG_DIR="${PORTS_DIR}/emdawnwebgpu_pkg"
PORT_PY="${PKG_DIR}/emdawnwebgpu.port.py"
ZIP_NAME="emdawnwebgpu_pkg-${TAG}.zip"
ZIP_PATH="${PORTS_DIR}/${ZIP_NAME}"
ENV_FILE="${REPO_ROOT}/scripts/emdawnwebgpu.env"

need_cmd() {
  command -v "$1" >/dev/null 2>&1 || {
    echo "Missing command: $1" >&2
    exit 1
  }
}

need_cmd wget
need_cmd unzip

mkdir -p "${PORTS_DIR}"

echo "Installing emdawnwebgpu (${TAG})"

URL="https://github.com/google/dawn/releases/download/${TAG}/${ZIP_NAME}"

[[ -f "${ZIP_PATH}" ]] || wget -O "${ZIP_PATH}" "${URL}"

rm -rf "${PKG_DIR}"
unzip -q "${ZIP_PATH}" -d "${PORTS_DIR}"

if [[ ! -f "${PORT_PY}" ]]; then
  echo "Port file not found: ${PORT_PY}" >&2
  ls -la "${PKG_DIR}" >&2 || true
  exit 1
fi

cat > "${ENV_FILE}" <<EOF
export EMDAWNWEBGPU_TAG="${TAG}"
export EMDAWNWEBGPU_PORT="${PORT_PY}"
EOF

echo "Env written: ${ENV_FILE}"
echo "Next:"
echo "  source ${ENV_FILE}"
echo "  rm -rf build-web && make web EMDAWNWEBGPU_PORT=\"${PORT_PY}\""
