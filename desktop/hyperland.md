# Guia de Migração para Hyprland: Otimizando seu Ambiente de Trabalho

Este documento serve como um guia para usuários que desejam migrar para o **Hyprland**, um gerenciador de janelas dinâmico (Tiling Window Manager), e otimizar suas configurações (`dotfiles`). Diferente de ambientes de desktop completos como o GNOME, o Hyprland foca na eficiência e personalização através de configurações diretas e programas independentes.

## Tabela de Equivalência de Funcionalidades

Abaixo, apresentamos uma tabela que compara funcionalidades comuns do GNOME com suas alternativas no Hyprland, juntamente com as ferramentas recomendadas para replicar ou aprimorar essas funcionalidades.

| Função GNOME         | Alternativa Hyprland        | Ferramenta Recomendada |
| :------------------- | :-------------------------- | :--------------------- |
| Blur my Shell        | Nativo (Decoration)         | Ajustes de `blur` no `hyprland.conf` |
| Zorin Tiling Shell   | Nativo (Dwindle)            | Comportamento padrão do WM |
| Desktop Cube         | Plugins                     | `hyprland-plugins`     |
| Zorin Connect        | KDE Connect                 | `kdeconnect`           |
| Emoji Copy           | Rofi / Wofi                 | `rofi-wayland`         |
| Drive Menu           | Udiskie                     | `udiskie`              |
| Barra Superior       | Waybar                      | `waybar`               |

## Instalação das Ferramentas (Base Arch Linux)

Para configurar o ambiente Hyprland, execute os comandos abaixo para instalar o ecossistema base e as ferramentas que substituem as funcionalidades das extensões do GNOME. Estes comandos são específicos para distribuições baseadas em Arch Linux.

```bash
# Core do sistema e utilitários visuais
sudo pacman -S hyprland waybar kitty rofi-wayland swww

# Ferramentas de suporte (substitutos das extensões)
sudo pacman -S udiskie kdeconnect pavucontrol wl-clipboard nm-connection-editor

# Plugins para efeitos extras (como o Cubo)
# Requer um AUR helper como o 'yay'
yay -S hyprland-plugins
```

## Exemplo de Configuração (`hyprland.conf`)

As seções a seguir demonstram como adicionar configurações ao seu arquivo `hyprland.conf` para replicar o comportamento visual e funcional que você pode ter tido em ambientes como o Zorin OS.

### 1. Visual e Efeito Blur (Substituindo o Blur my Shell)

Para ativar e configurar o efeito de desfoque (blur) e arredondamento das janelas, adicione as seguintes linhas à seção `decoration` do seu `hyprland.conf`:

```conf
decoration {
    rounding = 10
    
    blur {
        enabled = true
        size = 5
        passes = 2
        new_optimizations = true
    }
}
```

### 2. Gestos de Touchpad (Substituindo o X11 Gestures)

Para habilitar gestos de touchpad, como o deslizamento de três dedos para alternar entre áreas de trabalho, inclua a seção `gestures`:

```conf
gestures {
    workspace_swipe = true
    workspace_swipe_fingers = 3
}
```

### 3. Autostart (Início Automático de Programas)

Para iniciar automaticamente a barra de status (`waybar`), o gerenciador de wallpapers (`swww`), o montador de discos (`udiskie`) e o gerenciador de conexões de rede (`nm-applet`), adicione as seguintes linhas:

```conf
# Inicializa a barra de status, o gestor de wallpapers e a montagem de discos
exec-once = waybar
exec-once = swww init
exec-once = udiskie &
exec-once = nm-applet --indicator &
```

Este guia foi gerado para auxiliar na documentação de configuração e personalização (`Ricing`) do seu ambiente Hyprland.
