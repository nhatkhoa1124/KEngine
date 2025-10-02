@echo off
setlocal
"%FXC_PATH%\fxc.exe" "Base.hlsl" /Od /Zi /T vs_5_0 /E "VS" /Fo "Compiled/base_vs.cso" /Fc "Compiled/base_vs.asm"
"%FXC_PATH%\fxc.exe" "Base.hlsl" /Od /Zi /T ps_5_0 /E "PS" /Fo "Compiled/base_ps.cso" /Fc "Compiled/base_ps.asm"
pause