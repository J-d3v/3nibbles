# Student Grade Service

- CMake
- GoogleTest
- pruebas unitarias
- pruebas de integración
- cobertura mínima
- SonarQube
- escaneo de contraseñas con Gitleaks
- SAST con CodeQL
- DAST con OWASP ZAP
- package
- deploy por SSH
- health check post-deploy
- rollback automático

## 1. Requisitos locales

En Ubuntu/Debian:

```bash
sudo apt-get update
sudo apt-get install -y cmake ninja-build g++ lcov curl git
```

## 2. Compilar localmente

```bash
cmake --preset default
cmake --build --preset default --parallel
```

## 3. Ejecutar pruebas unitarias

```bash
ctest --test-dir build -R '^unit\.' --output-on-failure
```

## 4. Ejecutar la aplicación

```bash
./build/student_grade_service
```

Endpoints disponibles:

- `GET /health`
- `POST /api/v1/grades/final`

Ejemplo de request:

```bash
curl -X POST http://localhost:8080/api/v1/grades/final \
  -H "Content-Type: application/json" \
  -d '{"grades":[90,80,70]}'
```

## 5. Ejecutar pruebas de integración

En una terminal:

```bash
./build/student_grade_service
```

En otra terminal:

```bash
ctest --test-dir build -R '^integration\.' --output-on-failure
```

## 6. Generar cobertura local

```bash
cmake --preset default -DCMAKE_CXX_FLAGS="--coverage" -DCMAKE_EXE_LINKER_FLAGS="--coverage"
cmake --build --preset default --parallel
ctest --test-dir build --output-on-failure
mkdir -p build/coverage
lcov --capture --directory build --output-file build/coverage/coverage.info
lcov --remove build/coverage/coverage.info '/usr/*' '*/_deps/*' '*/tests/*' --output-file build/coverage/coverage.filtered.info
./scripts/check_coverage.sh build/coverage/coverage.filtered.info
```

## 7. Secrets requeridos en GitHub Actions

Configurar en `Settings > Secrets and variables > Actions`:

- `SONAR_HOST_URL`
- `SONAR_TOKEN`
- `DEPLOY_HOST`
- `DEPLOY_USER`
- `DEPLOY_SSH_KEY`

## 8. ¿Qué hace el pipeline?

### secret-scan
Busca contraseñas, tokens y llaves filtradas.

### codeql
Realiza análisis estático de seguridad y calidad sobre C++.

### build-test
Compila, corre unit tests, integration tests, genera cobertura y exige un mínimo de cobertura.

### sonarqube
Envía el análisis a SonarQube.

### dast-zap
Lanza OWASP ZAP sobre el servicio HTTP para una revisión básica dinámica.

### package
Empaqueta binario y scripts de despliegue.

### deploy
Despliega por SSH, verifica salud y hace rollback si el servicio falla.

## 9. Instalación del servicio en Linux

Copiar el archivo `deploy/student-grade-service.service` a:

```bash
/etc/systemd/system/student-grade-service.service
```

Después:

```bash
sudo systemctl daemon-reload
sudo systemctl enable student-grade-service
sudo systemctl start student-grade-service
sudo systemctl status student-grade-service
```

## 10. Explicación del rollback

El script `deploy.sh` guarda la versión anterior en:

```bash
/opt/student-grade-service-backups/<timestamp>
```

Si el health check falla, el workflow ejecuta `rollback.sh`, restaura el último respaldo y vuelve a verificar `/health`.
