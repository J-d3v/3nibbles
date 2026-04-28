#!/usr/bin/env bash
set -euo pipefail

APP_ROOT="/opt/student-grade-service"
CURRENT_DIR="${APP_ROOT}/current"
BACKUP_LINK="/opt/student-grade-service-backups/latest"

if [ ! -L "$BACKUP_LINK" ] && [ ! -d "$BACKUP_LINK" ]; then
  echo "No backup found for rollback. This is expected on first deployment."
  exit 0
fi

TARGET_BACKUP="$(readlink -f "$BACKUP_LINK")"
if [ -z "$TARGET_BACKUP" ] || [ ! -d "$TARGET_BACKUP" ]; then
  echo "Resolved backup path is invalid"
  exit 1
fi

rm -rf "$CURRENT_DIR"
mkdir -p "$CURRENT_DIR"
cp -R "${TARGET_BACKUP}/." "$CURRENT_DIR/"
chmod +x "$CURRENT_DIR/student_grade_service"

sudo systemctl restart student-grade-service
echo "Rollback completed successfully"