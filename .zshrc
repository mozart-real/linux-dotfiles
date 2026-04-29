# ==========================================================================
# 🚀 ZSH ULTRA RICE - MOZART EDITION (Gray & Deep Blue)
# ==========================================================================
export ZSH="$HOME/.oh-my-zsh"
ZSH_THEME="agnoster" 
# --- PLUGINS ---
plugins=(git docker sudo web-search node python zsh-autosuggestions zsh-syntax-highlighting)
source $ZSH/oh-my-zsh.sh
# --- CUSTOMIZAÇÃO DE CORES (O Rice Verde Banco) ---
# 1. Sugestões (Verde Escuro / Sombra)
ZSH_AUTOSUGGEST_HIGHLIGHT_STYLE="fg=#1a4a1a"
# 2. Sintaxe (Verde Banco Clássico)
# Comandos principais em Verde Fosforescente
ZSH_HIGHLIGHT_STYLES[command]='fg=#00ff00,bold'
# Argumentos e caminhos em Verde Médio
ZSH_HIGHLIGHT_STYLES[arg0]='fg=#39d353'
ZSH_HIGHLIGHT_STYLES[path]='fg=#57cc57,underline'
# Erros em um tom mais sóbrio
ZSH_HIGHLIGHT_STYLES[error]='fg=#f7768e,bold'
# --- ATALHOS DE TECLADO ---
bindkey '^[[C' forward-word      # Seta Direita aceita palavra
bindkey '^e' forward-word        # Ctrl + E aceita palavra
bindkey '^i' autosuggest-accept  # Tab aceita a sugestão completa
# --- ALIASES DE PRODUTIVIDADE ---
# Docker (Azul por natureza)
alias d="docker"
alias dps="docker ps --format 'table {{.Names}}\t{{.Status}}\t{{.Ports}}'"
alias dstop="docker stop \$(docker ps -q)"
alias dlogs="docker logs -f"
# Sistema e Dev
alias cls="clear && figlet -f slant 'MOZART'"
alias update="sudo apt update && sudo apt upgrade -y"
alias pinguim="neofetch"
alias bot="cd ~/Documentos/thatbot && code ."
alias bat="upower -i /org/freedesktop/UPower/devices/battery_BAT0 | grep percentage"
alias m='matrix -c black -l k -s 98'
# --- CONFIGURAÇÕES DE AMBIENTE (NVM, Angular, Spicetify) ---
export NVM_DIR="$HOME/.nvm"
[ -s "$NVM_DIR/nvm.sh" ] && \. "$NVM_DIR/nvm.sh"
[ -s "$NVM_DIR/bash_completion" ] && \. "$NVM_DIR/bash_completion"
source <(ng completion script)
export PATH=$PATH:/home/mozart/.spicetify
# --- STARTUP ---
clear
figlet -f slant "MOZART"
export PATH="$HOME/.rbenv/bin:$PATH"
eval "$(rbenv init - zsh)"
# O corretor que nunca desiste de você
alias fuck='echo "Never Gonna Give You Up!" | lolcat && xdg-open "https://www.youtube.com/watch?v=dQw4w9WgXcQ"'
alias browser='(chromium > /dev/null 2>&1 & disown)'
#THIS MUST BE AT THE END OF THE FILE FOR SDKMAN TO WORK!!!
export SDKMAN_DIR="$HOME/.sdkman"
[[ -s "$HOME/.sdkman/bin/sdkman-init.sh" ]] && source "$HOME/.sdkman/bin/sdkman-init.sh"
