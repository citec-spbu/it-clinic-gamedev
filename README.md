# Beat-Mark

Это ПО представляет собой простой инструмент для автоматического
анализа аудио и разметки музыкальных треков по битам.\
Изначально обработка была выполнена в Jupyter Notebook, но затем была
вынесена в отдельный Python-скрипт для удобства использования и
интеграции.

## Установка и запуск

### 1. Создание и активация виртуального окружения

    python -m venv venv

**Windows:**

    venv\Scripts\activate

**Linux/Mac:**

    source venv/bin/activate

### 2. Установка зависимостей

    pip install -r requirements.txt

### 3. Запуск скрипта

    python script/beatmark.pu <path/to/music.wav>

