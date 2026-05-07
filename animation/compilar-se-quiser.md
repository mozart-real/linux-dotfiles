# Baixe ui.c
## de o comando
` mkdir ~/meus_apps `
### Mova ui.c para está pasta
#### Baixe as dependencias
` sudo pacman -S --needed base-devel gtk4 cairo pkgconf || sudo apt update && sudo apt install -y build-essential libgtk-4-dev libcairo2-dev pkg-config || sudo dnf install -y gcc gtk4-devel cairo-devel pkgconf-pkg-config `
#### rode o comando inteiro acima 
rode
` gcc ui.c -o ui \$(pkg-config --cflags --libs gtk4 cairo)`
#### e aproveite



