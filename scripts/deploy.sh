#!/usr/bin/env bash
set -euo pipefail

APP_NAME="student_grade_service"
DEPLOY_DIR="/opt/student-grade-service"
BACKUP_ROOT="/opt/student-grade-service-backups"
SERVICE_NAME="student-grade-service"
PACKAGE_PATH="${1:-/tmp/student-grade-service.tar.gz}"
RELEASE_DIR="$DEPLOY_DIR/current"
TIMESTAMP=$(date +%Y%m%d%H%M%S)
BACKUP_DIR="$BACKUP_ROOT/$TIMESTAMP"
APP_USER="ubuntu"
APP_GROUP="ubuntu"

install -d -m 755 -o "$APP_USER" -g "$APP_GROUP" "$DEPLOY_DIR" "$BACKUP_ROOT"

if [[ -d "$RELEASE_DIR" ]]; then
  install -d -m 755 -o "$APP_USER" -g "$APP_GROUP" "$BACKUP_DIR"
  echo "Creating backup in $BACKUP_DIR"
  cp -a "$RELEASE_DIR/." "$BACKUP_DIR/"
  chown -R "$APP_USER:$APP_GROUP" "$BACKUP_DIR"
fi

TMP_EXTRACT_DIR=$(mktemp -d)
trap 'rm -rf "$TMP_EXTRACT_DIR"' EXIT

echo "Extracting package from $PACKAGE_PATH"
tar -xzf "$PACKAGE_PATH" -C "$TMP_EXTRACT_DIR"

if [[ ! -f "$TMP_EXTRACT_DIR/$APP_NAME" ]]; then
  echo "Binary $APP_NAME not found in package"
  exit 1
fi

rm -rf "$RELEASE_DIR"
install -d -m 755 -o "$APP_USER" -g "$APP_GROUP" "$RELEASE_DIR"
install -m 755 -o "$APP_USER" -g "$APP_GROUP" "$TMP_EXTRACT_DIR/$APP_NAME" "$RELEASE_DIR/$APP_NAME"

sudo systemctl daemon-reload
sudo systemctl restart "$SERVICE_NAME"
