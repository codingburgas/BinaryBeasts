<div align="center">

# ⚽ BinaryBeasts - Pitch & Score

![Version](https://img.shields.io/badge/version-1.0-blue)
![Language](https://img.shields.io/badge/language-C%2B%2B-00599C?logo=cplusplus)
![Graphics](https://img.shields.io/badge/framework-raylib-2d2d2d?logo=raylib)
![Status](https://img.shields.io/badge/status-active-success)

**A desktop football league manager with interactive screens for standings, scores, fixtures, and season insights**

</div>

---

<div align="center">

## 📋 Table of Contents

</div>

* [🎯 About the Project](#about-the-project)
* [🚀 Core Features](#core-features)
* [🛠️ Technologies](#technologies)
* [📊 System Overview](#system-overview)
* [📁 Project Structure](#project-structure)
* [📥 Installation](#installation)
* [▶️ Run the App](#run-the-app)

---

<div align="center">
<h2 id="about-the-project">🎯 About the Project</h2>
</div>

**BinaryBeasts (Pitch & Score)** is a C++ desktop application built with **raylib** that simulates a football league control desk.

It lets users manage teams, update table data, and navigate focused views for:

* 🏆 League standings
* 🔴 Live score previews (simulated)
* 📅 Match schedule
* 📈 Season statistics
* ℹ️ About section

Team data is persisted in a local binary file (`database.bin`) between runs.

---

<div align="center">
<h2 id="core-features">🚀 Core Features</h2>
</div>

### 🧭 Main Navigation (`football_menu.cpp`)

* Centered home menu with multiple screens
* Keyboard and mouse navigation support
* Shared top-bar chrome for subpages

---

### 🏆 League Table Management (`presentation.cpp`)

* Add new teams through modal input
* Edit team name, goals, and points
* Increment/decrement points directly from cards
* Delete last team or clear all teams
* Sort teams by points, goals, or name

---

### 🔴 Live Scores View (`football_menu.cpp`)

* Simulated match score cards
* "Live" indicator styling
* Auto-generated pairings based on current teams

---

### 📅 Match Schedule (`football_menu.cpp`)

* Fixture list with time slots
* Dynamic team-vs-team generation
* Matchday labeling

---

### 📈 Season Statistics (`football_menu.cpp`)

* Total clubs, goals, and points
* Average goals per club
* Leader detection and tier labeling

---

### 💾 Data Layer (`data.cpp`)

* Binary save/load system for `Team` records
* Reads from and writes to `database.bin`
* Restores previous league state on app start

---

### 🧠 Logic Utilities (`logic.cpp`)

* Team creation helpers
* Team list quick sort implementation
* Recursive total-goals calculator

---

<div align="center">
<h2 id="technologies">🛠️ Technologies</h2>

<br>

<a href="https://cplusplus.com/"><img src="https://img.shields.io/badge/C%2B%2B-00599C?logo=cplusplus&logoColor=white&style=for-the-badge" /></a>
<a href="https://www.raylib.com/"><img src="https://img.shields.io/badge/raylib-111111?logo=raylib&logoColor=white&style=for-the-badge" /></a>
<a href="https://visualstudio.microsoft.com/"><img src="https://img.shields.io/badge/Visual%20Studio%202022-5C2D91?logo=visualstudio&logoColor=white&style=for-the-badge" /></a>
<a href="https://www.nuget.org/"><img src="https://img.shields.io/badge/NuGet-004880?logo=nuget&logoColor=white&style=for-the-badge" /></a>
<a href="https://github.com/"><img src="https://img.shields.io/badge/GitHub-181717?logo=github&logoColor=white&style=for-the-badge" /></a>

</div>

---

<div align="center">
<h2 id="system-overview">📊 System Overview</h2>
</div>

| Component | Responsibility |
| --------- | -------------- |
| `main.cpp` | App entry point |
| `presentation.cpp` | Main app loop and league standings UI |
| `football_menu.cpp` | Main menu and sub-screen rendering |
| `logic.cpp` | Team operations and algorithm helpers |
| `data.cpp` | Binary persistence (load/save) |
| `team_ui.cpp` | Reusable team card drawing utilities |

---

<div align="center">
<h2 id="project-structure">📁 Project Structure</h2>
</div>

```text
BinaryBeasts/
|- BinaryBeasts.sln
|- README.md
`- BinaryBeasts/
   |- main.cpp
   |- presentation.cpp / presentation.h
   |- football_menu.cpp / football_menu.h
   |- logic.cpp / logic.h
   |- data.cpp / data.h
   |- team_ui.cpp / team_ui.h
   |- BinaryBeasts.vcxproj
   |- packages.config
   `- database.bin
```

---

<div align="center">
<h2 id="installation">📥 Installation</h2>
</div>

### ⚙️ Requirements

* Windows
* Visual Studio 2022 (MSVC v143)
* NuGet package restore enabled

---

### 📦 Setup

```bash
git clone https://github.com/<your-username>/BinaryBeasts.git
cd BinaryBeasts
```

1. Open `BinaryBeasts.sln` in Visual Studio.
2. Restore NuGet packages when prompted (raylib is declared in `packages.config`).
3. Select `Debug | x64` (or your preferred configuration).
4. Build the solution.

---

<div align="center">
<h2 id="run-the-app">▶️ Run the App</h2>
</div>

* Start from Visual Studio (`F5` / `Ctrl + F5`).
* The app opens in the main menu (`Pitch & Score`).
* Team data is auto-saved on exit to `database.bin`.
