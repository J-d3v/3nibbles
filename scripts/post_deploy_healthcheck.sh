#!/usr/bin/env bash
set -euo pipefail

TARGET_URL="${1:-http://localhost:8080/health}"
RETRIES="${RETRIES:-10}"
SLEEP_SECONDS="${SLEEP_SECONDS:-3}"

for ((i=1; i<=RETRIES; i++)); do
  echo "Health check attempt $i/$RETRIES -> $TARGET_URL"
  if curl -fsS "$TARGET_URL" >/dev/null; then
    echo "Health check passed"
    exit 0
  fi
  sleep "$SLEEP_SECONDS"
done

echo "Health check failed"
exit 1
