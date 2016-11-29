﻿Все попытки смёрджить окончились фэйлом.
Слишком много кода было написано нами всеми, поэтому чтобы всё смерджить аккуратно нужно очень много времени.
![Картиночка](http://mtdata.ru/u23/photo9A17/20340466292-0/original.jpg)

# PowerSpace
1"Powerpoint for mindmaps."

__Общее описание структуры проекта__

В нашем проекте есть 3 компоненты: _editor_, _rendering engine_ (или _viewer_), _script engine_ (или _модуль python_).

_Editor_ — это первое окно, которое видит пользователь при запуске нашего приложения.
* Оно представляет из себя бесконечное полотно (канву), на котором можно размещать различные примитивные объекты (фигуры: квадраты, круги; текст).
* Для работы с объектами к окну с канвой необходимо прикрутить тулбар.
* После размещения на канве объекты можно двигать, увеличивать/уменьшать, менять цвет.
* К одному объекту на канве можно присоединить несколько внешних файлов с python-скриптами. Каждый из скриптов должен быть прикреплен к какому-то событию, на которое умеет реагировать объект.
* На тулбаре должна находиться кнопка "Play", которая открывает окошко _Viewer_.

_Viewer_ — окно, которое отображается после нажатия кнопки "Play" в _editor_.
* Оно представляет из себя то же полотно, но здесь объекты уже нельзя никак модифицировать.
* Можно осуществлять вызов любого из базового набора событий: _click_ (по объекту кликнули мышью) и _tick_ (истек таймер).
* При каком-то событии на объекте должны вызываться соответствующие этому событию скрипты (через _script engine_).
* Скрипты предназначены для изменения некоторых свойств (по умолчанию, цвета и положения) объекта, к которому они привязаны.

_Script engine_ — модуль исполнения python-скриптов.
* Принимает на вход сцену (список объектов) и список скриптов.
* Исполняет все скрипты по очереди, меняя свойства объектов.
* Возвращает обновленный список объектов.

__Детали реализации проекта__

* Пишем однопроцессное приложение, т.е. связь между компонентами осуществляется через include и обычные вызовы функций, ничего особенного.

* Для демонстрации работоспособности написанного кода на каждый модуль необходимо написать хотя бы один тест (вручную, Google Tests и т.п. излишни).

* Solution состоит из 4-х проектов:
    * Editor
    * RenderingEngine
    * ScriptEngine
    * PowerSpace

* В PowerSpace находятся объявления и реализация всех интерфейсов и классов, необходимых для работы всем компонентам:
    * `IDrawable` — интерфейс описывает функционал любого объекта, который может быть отрисован на канве;
        * `GetPosition()`
        * `SetPosition(position)`
        * `GetColor()`
        * `SetColor(color)`
        * `GetId()`
        * `Draw()`

    * `CDrawable` — абстрактный класс (реализация интерфейса `IDrawable`), от него наследуются все объекты
        * `color, position`
        * `unordered_map<EventType, vector<CScript>>` — для каждого объекта на каждый тип события можно задавать несколько скриптов
        * `id`

    * `CScript` — класс-обертка над путем до файла скрипта
        * `GetPath()`
        * `path` — путь до файла, в котором лежит скрипт

    * `enum EventType { EventTick, EventClick }` — поддерживаемые виды событий

    * `CStage` — struct для хранения содержимого канвы
        * `unordered_map<int, IDrawable> objects`

    * `CCanvas` — класс для отрисовки канвы (отрисовка с помощью WinAPI)
        * `DrawRectangle()`
        * `DrawCircle()`
        * `DrawText()`
        * `DrawObjects()`
        * `ClipObjects()` — возвращает список объектов, которые надо отрисовать (те, которые попадают в наш viewport)
        * `ConvertCoordinates()` — преобразует логические координаты объекта в реальные координаты в окошке

    * `CScriptEngine` — singleton класс для выполнения скриптов
        * `CScriptEngine(CStage&)` — конструктор, принимающий ссылку на сцену
        * `CStage& stage` — ссылка на сцену
        * `RunScripts(int objectId, const vector<CScript>& scripts)` — запускает все скрипты из `scripts`, возвращает список измененных объектов

- В editor'е заполняется сцена;
- по нажатию play: editor передает во viewer копию(!) сцены, т.к. viewer ее меняет.

- В объекте храним список скриптов;
- у каждого объекта будет свой `id` (целое число);
- в сцене будем хранить map из `id` в объект;
- в объекте храним `map` из типа скрипта в список скриптов.

- Viewer ловит event на объекте и пересылает список скриптов объекту `CScriptEngine`;
- ссылку на `stage` можно передавать в конструктор и сохранять в объекте синглтон-класса `CScriptEngine`.

- `CScriptEngine` передаем `id` объекта, на котором сработало событие, и список скриптов для выполнения;
- он меняет сцену и возвращают список тех объектов, в которых что-то поменялось;
- viewer после этого перерисовывает только те объекты, в которых были изменения.

- `CCanvas` служит для отрисовки всей логики в окошке (оборачивает функции WinAPI для отрисовки);
- `CCanvas` умеет отрисовывать примитивные фигуры и текст;
- отрисовка объекта происходит так: у объекта вызывается метод `Draw()`, который дергает методы из `CCanvas` (сам объект ничего не рисует).


- Все обсуждения по проекту надо записывать и оформлять на будущее.

__Code Style__  
1. Необходимо применить к своему коду Code Style, описанный в файле SourceFormatStyle.vssettings:  
    1.1. импортировать в Visual Studio настройки через Tools -> Import and Export Settings;  
    1.2. применить эти настройки ко всем файлам через Edit -> Advanced -> Format Document.  
2. Все комментарии и коммит-сообщения должны быть на английском языке.

__Инструкции по сборке__  
Перед сборкой проекта необходимо:  
1. Установить Python3;  
2. Выставить три переменные среды:  
* Python_Include — путь к папке \<Python3 Installation Path\>\\include;  
* Python_Libs — путь к папке \<Python3 Installation Path\>\\libs;
* PYTHONPATH — путь к папке PowerSpace\\PowerSpace\\scripts;
2a. Если ошибки линковки остались, попробовать поменять конфигурацию (мне помогло)