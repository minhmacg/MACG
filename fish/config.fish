if status is-interactive
    # Commands to run in interactive sessions can go here
end

set -q GHCUP_INSTALL_BASE_PREFIX[1]; or set GHCUP_INSTALL_BASE_PREFIX $HOME ; set -gx PATH $HOME/.cabal/bin /home/minhmacg/.ghcup/bin $PATH # ghcup-env

alias drb="cd ~/Documents/rust_beat_2023"



