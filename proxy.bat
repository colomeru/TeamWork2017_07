@echo off
echo "0:�v���L�V�[���� 1:�v���L�V�[�Z�b�g"
set /P I="����:"
if %I% == 0 (
setx HTTP_Proxy ""
setx HTTPS_Proxy ""
) else if %I% == 1 (
setx HTTP_Proxy "cache9.hac.neec.ac.jp:8080"
setx HTTPS_Proxy "cache9.hac.neec.ac.jp:8080"
)
pause