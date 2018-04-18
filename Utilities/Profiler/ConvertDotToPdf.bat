@echo off
dot -Tps2 profile.dot -o profile.ps | ps2pdf profile.ps && rm profile.dot && rm profile.ps