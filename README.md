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
... (остава(т) 97 реда)
