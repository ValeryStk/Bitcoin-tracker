[![Linux & Windows](https://img.shields.io/badge/Build-Linux%20%26%20Windows-brightgreen)](https://github.com/ValeryStk/Bitcoin-tracker/actions)

# ![BTC](https://raw.githubusercontent.com/spothq/cryptocurrency-icons/master/32/color/btc.png) Bitcoin Tracker 


**Десктопное приложение для отслеживания торговых операций с Bitcoin и мониторинга цены BTC/USDT в реальном времени.**

## ✨ Возможности

- 📊 **Торговая таблица** — учет сделок (цена закупки, BTC, USDT)
- 💰 **Автоматическое обновление цены BTC** с Binance API
- 🎯 **Горячие клавиши** (Ctrl+N — добавить сделку)
- 📈 **Автоматический расчет summary** (средняя цена, общий BTC/USDT)
- ⚡ **Оптимизированная модель** `QAbstractTableModel`


## 📱 Модель данных

```
┌─────────────────────┬──────────┬──────────┐
│ Entry Price         │ BTC      │ USDT     │
├─────────────────────┼──────────┼──────────┤
│ 64'500.00           │ 0.0155   │ 1'000.00 │
│ 65'200.00           │ 0.0300   │ 1'956.00 │
├─────────────────────┼──────────┼──────────┤
│ 65'012.34           │ 0.0455   │ 2'956.00 │ ← Summary
└─────────────────────┴──────────┴──────────┘

```


## 🚀 Быстрый старт

### Предварительные требования

- Qt 5.x
- C++17
- qmake


### Сборка

```bash
git clone https://github.com/ValeryStk/Bitcoin-tracker.git
cd Bitcoin-tracker
mkdir build && cd build
cmake ..
make -j$(nproc)
./BitcoinTracker
```


### Qt Creator

1. Откройте `bitcoin-tracker.pro`
2. Настройте Kit (Qt5, C++17)
3. `Ctrl+R` → Run

## 🛠 Основные компоненты

| Компонент | Описание |
| :-- | :-- |
| `TradingTableModel` | Модель данных сделок с summary |
| `BTCPriceFetcher` | Получение цены BTC/USDT |
| `MainWindow` | Главное окно с горячими клавишами |

## ⌨️ Горячие клавиши

| Клавиша | Действие |
| :-- | :-- |
| `Ctrl+N` | Добавить новую сделку |

## 🔗 API

- **Binance**: `https://api.binance.com/api/v3/ticker/price?symbol=BTCUSDT`
- Обновление: каждые 10 сек + вручную (F5)


## 📁 Структура проекта

```
Bitcoin-tracker/
├── src/
│   ├── models/
│   │   └── trading_table_model.*
│   ├── fetchers/
│   │   └── btc_price_fetcher.*
│   ├── main.cpp
│   └── mainwindow.*
├── bitcoin-tracker.pro
└── README.md
```


## ⚙️ Настройки

Все настройки в коде:

- `QTimer::start(10'000)` — автообновление цены (10 сек)
- `QString::number(value, 'f', 6)` — форматирование чисел
- `request.setTransferTimeout(10'000)` — таймаут API (10 сек)


## 🐛 Известные проблемы
- Summary строка не редактируется


## 🤝 Вклад в проект

1. Форкните репозиторий
2. Создайте ветку `feature/ваша-фича`
3. Commit'ы: `git commit -m "feat: добавлено X"`
4. Push и Pull Request

## 📄 Лицензия

MIT — используйте на здоровье! 🚀

***

**Автор**: ValeryStk
**Статус**: Production Ready
**Язык**: C++17 / Qt5



