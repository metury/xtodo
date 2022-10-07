# xtodo

**xtodo** je desktopová todo aplikace, která pracuje s úkoly uloženýma v textovém souboru, pomocí definovaného standardu [todo.txt](https://github.com/todotxt/todo.txt). Aplikace lze spustit na příkazové řádce a také ve svém grafickém prostředí.

Na zařízeních s Linuxem lze projekt skompilovat pomocí přidaného skriptu `autobuild.sh`. Tento skript má i možné flagy, přesněji `run` pro spuštění programu, `install` pro zkopírování buildu do složky `/opt/xtodo` a poslední flag je `clean`, který smaže předchozí build. Instalace pomocí skriptu nevytvoří návaznost a je nutné ručně tento program spouštět. Doporučeně přímo ze složky `/opt/xtodo/`. Nebo-li lze si vytvořit přímo zkratky anebo vytvořit alias do terminálu: `alias xtodo='cd /opt/xtodo && ./xtodo'`.
