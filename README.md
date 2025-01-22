<div align="center">

<img src="https://github.com/user-attachments/assets/4427f4a8-709e-45d8-b62f-d8adae4f3657" alt="RaytRazor_Logo" width="256"/>

# RaytRazor

![Release_Version_Badge](https://img.shields.io/github/v/release/lgndluke/RaytRazor?include_prereleases&sort=date&display_name=release&style=for-the-badge&label=Release%3A)
![Release_Status_Badge](https://img.shields.io/github/actions/workflow/status/lgndluke/RaytRazor/build.yml?style=for-the-badge&label=Build%20Status%3A)
![Top_Language_Badge](https://img.shields.io/github/languages/top/lgndluke/RaytRazor?style=for-the-badge)

</div>

## 🖼️ Beispiel Bilder:

<div align="center" style="display: flex; justify-content: space-between;">
  <img src="https://github.com/user-attachments/assets/7866cbe5-4d05-42e1-ab96-32e19fb7836d" alt="Audis" style="width: 33%;">
  <img src="https://github.com/user-attachments/assets/aac029a2-1b72-41f5-9aac-92ddd1212d4e" alt="Teapot-and-Chess" style="width: 33%;">
  <img src="https://github.com/user-attachments/assets/169f562b-1adb-4123-9aca-ad4b883b355e" alt="Spheres" style="width: 33%;">
</div>

## ⚡️ Quick start:

Neuesten Release herunterladen, Archiv entpacken und direkt unter Windows durchstarten. <br>
Zum aktuellsten Release: [RaytRazor-Latest](https://github.com/lgndluke/RaytRazor/releases/latest/)

## 📖 Project Wiki:

Die Projekt-Dokumentation kann hier gefunden werden: [Wiki](https://github.com/lgndluke/RaytRazor/wiki/)

## 🔩 Commit Syntax:

Die Commit-Richtlinie des Projekts sieht wie folgt aus:

```
Allgemein:
---
<filename>: <subject>
    	
<body>
    	
<optional:footer>
```

```
Beispiel Add-Commit:
---
Shader.cpp: Neu: Implementierung eines einfachen Shader-Programms.
    	
Shader.cpp enthält die Implementierung eines einfachen Shader-Programms, das in der Raytracer Anwendung verwendet werden kann.
Der Shader unterstützt grundlegende Lichtbrechungen und Texturierungen.

Schließt Issue #22
```

```
Beispiel Change-Commit:
---
Raytracer.cpp: Verbesserung des Ray-Sphäre-Algorithmus
    	
Der Algorithmus zur Berechnung der Schnittpunkte zwischen Ray und Sphäre wurde optimiert.
Zusätzlich wurden neue Testfälle hinzugefügt.

Schließt Issue #15
BREAKING CHANGE: Die Funktion `intersectRaySphere` hat jetzt eine andere Signatur und erwartet einen zusätzlichen Parameter.
```

## 🔧 Versioning Syntax:

Versionen werden wie folgt definiert: ```1.5.0.143 (MAJOR.MINOR.PATCH.BUILD)```
