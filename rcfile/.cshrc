alias scheme 'echo This command is a alias, the origin is \"petite\" && petite \!*'
alias ll 'ls -al'
alias lsl 'ls -l'
alias bs2 'screen -X encoding big5; telnet -8 bs2.to; screen -X encoding utf8'

if ($?prompt) then
  if(! $?WINDOW ) then
    set prompt="%{^[[1;36m%}%T%{^[[m%}%{^[[1;33m%}%n%{^[[m%}@%{^[[1;37m%}%m%{^[[1;32m%}[%~]%{^[[m%}> "
  else
    set prompt="%{^[[1;36m%}%t%{^[[m%}%{^[[1;33m%}%n%{^[[m%}@%{^[[1;37m%}%m%{^[[1;32m%}[%~]%{^[[m%}%{^[[1;35m%}[W$WINDOW]%{^[[m%}> "
  endif
  if ($?tcsh) then
    set mail = (/var/mail/$USER)
    bindkey "^W" backward-delete-word
    bindkey -k up history-search-backward
    bindkey -k down history-search-forward
  endif
endif

#echo remember to run screen -r
setenv LANG zh_TW.UTF-8
