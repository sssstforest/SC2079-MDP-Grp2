# MDP-Android

> Android app for MDP

## 🎤 Communication Protocol

> Replace paremeters inside [ ]

Update Display

```
COMMAND,[command]
```

Update Robot Position

```
ROBOT,<[x]>,<[y]>,<[direction]>
```

Update Status (Send "STOPPED" to indicate end of IR)

```
STATUS,[Message]
```

Update Obstacle ID

```
TARGET,[obstacleNo],[obstacleID]
```

## 📂 Project Folder Structure

#### Top Level Directory Layout

```terminal
.
├── .gradle
├── .idea
├── app
├── gradle
├── .gitignore
├── build.gradle
├── gradle.properties
├── gradlew
├── gradlew.bat
├── README.md
└── settings.gradle
```
