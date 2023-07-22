# xtodo WIP

*!!! momentálně se mění celý kód, tak není funkční !!!*

**xtodo** je desktopová todo aplikace, která pracuje s úkoly uloženýma v textovém souboru, pomocí definovaného standardu [todo.txt](https://github.com/todotxt/todo.txt). Aplikace lze spustit na příkazové řádce a také ve svém grafickém prostředí.

Na zařízeních s Linuxem lze projekt zkompilovat pomocí meson buildu. Konkrétně:

1. `meson setup build/`
2. `meson compile -C build/`

A potom i nainstalovat pomocí:

3. `meson install -C build/`
