```markdown
# 🌐 C++ HTTPS Web Server with Qt GUI

**Асинхронный HTTPS-сервер с графической панелью управления на Qt**  

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)  
![C++17](https://img.shields.io/badge/C++-17-blue.svg)
![Qt](https://img.shields.io/badge/Qt-5.15%2B-green.svg)
![Boost](https://img.shields.io/badge/Boost-1.75%2B-orange.svg)
![OpenSSL](https://img.shields.io/badge/OpenSSL-1.1.1%2B-yellow.svg)

## 🚀 Основные возможности
- **Полноценный HTTPS-сервер** (TLS 1.2/1.3 через OpenSSL)
- **Графический интерфейс управления** (Qt Widgets)
- **Асинхронная обработка** (Boost.Asio, 10К+ соединений)
- **Раздача статических файлов** (HTML/CSS/JS/изображения)
- **Защитные механизмы**:
  - Защита от directory traversal
  - Валидация SSL-сертификатов
  - Graceful shutdown

## ⚠️ Текущий статус
- **Ядро сервера**: полностью готово ✅
- **GUI-интерфейс**: в активной разработке 🚧

## 📦 Быстрый старт

### Зависимости
```bash
# Для Ubuntu/Debian
sudo apt install qtbase5-dev libboost-all-dev libssl-dev cmake build-essential

# Для Windows (vcpkg)
vcpkg install boost-asio openssl qt5
```

### Сборка проекта
```bash
git clone https://github.com/yourusername/cpp-https-server.git
cd cpp-https-server
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

### Запуск
**Режим без GUI** (готово):
```bash
./bin/https_server \
    --port 443 \
    --root ./html \
    --cert ./certs/server.pem \
    --key ./certs/key.pem
```

**Режим с GUI** (в разработке):
```bash
./bin/https_server_gui  # Скоро будет доступно!
```

## 🖥️ Интерфейс управления (Qt)
![GUI Preview](docs/gui-preview.png)

Функционал GUI:
- Старт/стоп сервера одной кнопкой
- Просмотр активных подключений
- Логирование в реальном времени
- Управление SSL-сертификатами

## 🛠 Технические детали

### Поддерживаемые MIME-типы
Сервер автоматически определяет:
- `text/html`
- `text/css` 
- `application/javascript`
- `image/*` (png/jpg/svg/gif)
- `application/json`

## 🌟 Особенности реализации
- **Потокобезопасность**: Все callback-и используют `shared_from_this()`
- **Производительность**: Асинхронный ввод-вывод через `io_context`
- **Безопасность**: Валидация всех входящих путей

## 📜 Лицензия
MIT License. Подробнее в файле [LICENSE](LICENSE).

---




```

### Что нужно сделать перед использованием:
1. Замените `yourusername` на ваш GitHub-аккаунт
2. Добавьте реальный скриншот GUI в `docs/gui-preview.png`
3. Обновите раздел "Текущий статус" по мере разработки

Этот файл полностью готов для размещения на GitHub и содержит все ключевые разделы для вашего проекта.
