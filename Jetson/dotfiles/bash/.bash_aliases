alias req='comm -23 <(apt-mark showmanual | sort -u) <(sed -n 's/^Package: //p' | sort -u) > ~/UoGACE2022/Jetson/dotfiles/requirements'
alias gst="git status"
alias ga= "git add"
alias gaa="git add ."
alias gcm="git commit -m"
alias gpl="git pull"
alias gps="git push"
