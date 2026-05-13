# PC Monitor

Приложение для отображения загрузки ресурсов PC в Linux.

Проект состоит из двух частей:

- **Backend** на C++ — собирает данные о системе.
- **Frontend** на HTML/CSS/TypeScript — отображает данные в браузере.

## Возможности

- отображение загрузки CPU;
- отображение использования RAM;
- отображение load average;
- динамическое обновление данных;
- HTTP API для связи frontend и backend.


### Backend

- C++
- CMake
- cpp-httplib

### Frontend

- HTML
- CSS
- TypeScript
- Vite

## Структура проекта

```text
pc-monitor/
├── backend/
│   ├── CMakeLists.txt
│   ├── main.cpp
│   └── httplib.h
│
├── frontend/
│   ├── index.html
│   ├── package.json
│   ├── package-lock.json
│   ├── tsconfig.json
│   └── src/
│       ├── main.ts
│       └── style.css
│
├── README.md
├── .gitignore
└── pc-monitor.zip
