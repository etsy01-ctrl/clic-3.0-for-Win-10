# Clic Player para Windows 10

**[Descargar ClicPlayer.exe](https://github.com/etsy01-ctrl/clic-3.0-for-Win-10/raw/refs/heads/main/dist/ClicPlayer.exe)** · **[Ver carpeta dist](dist/)**

Reproductor nativo Win32 de actividades Clic 3.0, con portada e interfaz propias. Motor original conservado; no incluye editor.

## Portada y apariencia

- Portada Clic 3.0 creada con ImageGen: logo azul con ratón y fila Windows 7, Windows 10 y Windows 11 debajo.
- Paleta azul Clic, blanco y gris perla. Arte integrado en el EXE.
- Diseño compacto predeterminado; alternativa moderna en **Herramientas → Apariencia**.
- Controles compactos: flechas anterior/siguiente, bandera reiniciar, `?` ayuda, `i` información y **Clic** abrir juego. Cada icono tiene ayuda emergente.

## Idiomas

**Herramientas → Idioma** ofrece **Español**, **English**, **Français** y **Català**.

La selección cambia inmediatamente menús, controles, ayudas emergentes, diálogos y avisos del reproductor. Para conservarla al reiniciar, pulsa **Usar idioma actual por defecto** dentro del mismo menú. La línea inferior muestra el idioma predeterminado guardado.

## Abrir juegos

- **Archivo → Abrir juego** o Ctrl+O abre paquetes `.pcc` y `.pac`.
- También puedes arrastrar un paquete sobre `dist\ClicPlayer.exe`.
- Barra de estado: paquete, actividad, aciertos, intentos y tiempo.
- Menú **Actividad**: reinicio, ayuda, opciones de puzzle, información e impresión, según disponibilidad.
- Ventana ajustable, maximizar y minimizar; varias ventanas independientes.

Mantén audio y video externos junto al paquete, con sus nombres originales. Se admiten también ejecutables Clic que usan el formato del runtime original.

## Vincular archivos .pcc

**Archivo → Vincular archivos .pcc** registra el reproductor para el usuario actual y abre directamente Configuración de Windows. No hay modal intermedio.

En aplicaciones predeterminadas por tipo de archivo, busca `.pcc` y elige **Clic Player**. Windows exige elección del usuario para cambiar la aplicación predeterminada. [Documentación de Microsoft](https://learn.microsoft.com/en-us/windows/win32/shell/vista-managing-defaults).

Si mueves el ejecutable, repite el registro desde la nueva carpeta.

## Atajos

| Acción | Atajo |
| --- | --- |
| Abrir juego | Ctrl+O |
| Reiniciar | F5 o Ctrl+R |
| Ayuda de actividad | F1 |
| Anterior / siguiente | Ctrl+Izquierda / Ctrl+Derecha |
| Cerrar | Alt+F4 |

## Compilar

Requiere Borland C++ 4 para Win32 y `rc.exe` del Windows SDK.

```powershell
.\tools\build.ps1 -BorlandRoot 'C:\ruta\a\tools\bc4' -WindowsSdkRc 'C:\ruta\a\rc.exe'
```

Resultado: `dist\ClicPlayer.exe`. El build convierte el PNG con alpha a recurso BGRA para el dibujo nativo. Arte original y prompts: `src/resources/assets/`.
## Código base y licencia

Basado en [frncesc/clic](https://github.com/frncesc/clic), de Francesc Busquets. Licencia original conservada en [LICENSE](LICENSE).

## Estructura del repositorio

- `dist/`: player compilado y ajustes predeterminados.
- `src/*.cpp`, `src/runtime-*`, `src/resources/`: port actual del player Win32.
- `tools/build.ps1`: compilación del player actual.
- `src/clic/`, `docs/`, `media/` y demás carpetas históricas: código y material original conservados.