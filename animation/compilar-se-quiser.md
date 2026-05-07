# Instalação da minha animação

## Pré-requisitos

Certifique-se de ter um gerenciador de pacotes compatível: **pacman** (Arch), **apt** (Debian/Ubuntu) ou **dnf** (Fedora).

---

## Passo 1 — Baixe o ui.c

Faça o download do arquivo `ui.c` e salve-o no seu computador.

---

## Passo 2 — Crie a pasta e mova o arquivo

```bash
mkdir ~/meus_apps
```

Mova o arquivo `ui.c` para a pasta recém-criada:

```bash
mv ~/Downloads/ui.c ~/meus_apps/
cd ~/meus_apps
```

---

## Passo 3 — Instale as dependências

Execute **apenas o comando correspondente** à sua distro:

**Arch Linux (pacman):**
```bash
sudo pacman -S --needed base-devel gtk4 cairo pkgconf
```

**Debian / Ubuntu (apt):**
```bash
sudo apt update && sudo apt install -y build-essential libgtk-4-dev libcairo2-dev pkg-config
```

**Fedora (dnf):**
```bash
sudo dnf install -y gcc gtk4-devel cairo-devel pkgconf-pkg-config
```

---

## Passo 4 — Compile

Dentro da pasta `~/meus_apps`, execute:

```bash
gcc ui.c -o ui $(pkg-config --cflags --libs gtk4 cairo)
```

---

## Passo 5 — Execute

```bash
./ui
```

Pronto! Aproveite 🎉
