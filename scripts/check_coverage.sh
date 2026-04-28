#!/usr/bin/env bash
set -euo pipefail

COVERAGE_FILE="${1:-build/coverage/coverage.filtered.info}"
MIN_COVERAGE="${MIN_COVERAGE:-80}"

if [[ ! -f "$COVERAGE_FILE" ]]; then
  echo "Coverage file not found: $COVERAGE_FILE"
  exit 1
fi

line=$(lcov --summary "$COVERAGE_FILE" 2>/dev/null | grep -E 'lines\.*:')
if [[ -z "$line" ]]; then
  echo "Unable to parse coverage summary"
  exit 1
fi

coverage=$(echo "$line" | sed -E 's/.*: *([0-9]+\.[0-9]+|[0-9]+)%.*/\1/')
coverage_int=$(printf '%.0f' "$coverage")

echo "Detected line coverage: ${coverage}%"
echo "Minimum required coverage: ${MIN_COVERAGE}%"

if (( coverage_int < MIN_COVERAGE )); then
  echo "Coverage gate failed"
  exit 1
fi

echo "Coverage gate passed"
